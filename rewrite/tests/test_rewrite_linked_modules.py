#!/usr/bin/env python3
"""Cross-module exported calls/types, ownership, visibility and source diagnostics."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from project_paths import ROOT, executable_path, normalize_newlines
from rewrite_paths import ENTRY, MODULES


def run(args):
    return subprocess.run(args, capture_output=True, timeout=75)


def main() -> None:
    cache = BuildCache(require_cc())
    compiler = cache.native_driver('rewrite-linked-modules-v3', ENTRY, extra_modules=MODULES)
    with tempfile.TemporaryDirectory(prefix='rewrite-linked-') as tmp:
        root = Path(tmp)
        out = root / 'generated.c'
        runtime = ROOT / 'runtime' / 'sirius_runtime.c'
        cases = [
            ({
                'First': 'export def score(x: i64) -> i64\n\treturn x + 1\n',
                'Second': 'export def score(x: i64) -> i64\n\treturn x * 3\n',
                'Main': 'import First alias A\nimport Second alias B\ndef main() -> i64\n\tprint(A.score(8))\n\tprint(B.score(8))\n\treturn 0\n',
            }, b'9\n24\n'),
            ({
                'Leaf': 'export def decorate(x: string) -> string\n\treturn "<" + x + ">"\n',
                'Middle': 'import Leaf alias L\nexport def forward(x: string) -> string\n\treturn L.decorate(x)\n',
                'Main': 'import Middle alias M\nimport Leaf alias Z\ndef main() -> i64\n\tprint(M.forward("ok"))\n\tprint(Z.decorate("bye"))\n\treturn 0\n',
            }, b'<ok>\n<bye>\n'),
            ({
                'Types': 'export rec Label(text: string, id: i64)\nrec Private(value: i64)\nexport def make(s: string) -> Label\n\treturn Label(s, 9)\n',
                'Main': 'import Types alias T\ndef main() -> i64\n\tlet label: T.Label = T.make("hello")\n\tlet copy = T.Label(label.text, label.id + 1)\n\tprint(label.text)\n\tprint(copy.id)\n\tlet copies: Array[T.Label] = [copy, label]\n\tprint(len(copies))\n\treturn 0\n',
            }, b'hello\n10\n2\n'),
            ({
                'Empty': 'export rec Box(values: Array[string])\n',
                'Lib': 'import Empty alias E\nexport def make() -> E.Box\n\treturn E.Box(["a", "b"])\n',
                'Main': 'import Lib alias L\ndef main() -> i64\n\tlet b = L.make()\n\tvar values = b.values\n\tvalues = values.append("c")\n\tprint(len(b.values))\n\tprint(len(values))\n\treturn 0\n',
            }, b'2\n3\n'),
            ({
                'A': 'export rec Value(n: i64)\nexport def make() -> Value\n\treturn Value(9)\n',
                'B': 'export rec Value(n: i64)\nexport def read(v: Value) -> i64\n\treturn v.n + 1\n',
                'Main': 'import A alias Left\nimport B alias Right\ndef main() -> i64\n\tlet v: Left.Value = Left.Value(9)\n\tprint(Right.read(v))\n\tprint(Right.read(Left.make()))\n\treturn 0\n',
            }, b'10\n10\n'),
            ({
                'Types': 'rec A(value: i64)\nrec B(value: string)\nexport let U = A | B\nexport enum Color\n\tRed\n\tBlue\nexport def union() -> U\n\treturn B("word")\nexport def color() -> Color\n\treturn Blue()\n',
                'Main': 'import Types alias T\ndef main() -> i64\n\tlet x: T.U = T.union()\n\tlet y: T.Color = T.color()\n\tprint(x == T.union())\n\tprint(y == T.color())\n\treturn 0\n',
            }, b'true\ntrue\n'),
        ]
        for n, (sources, expected) in enumerate(cases):
            for name, source in sources.items():
                (root / f'{name}.sirius').write_text(source)
            result = run([str(compiler), '--compile-modules', str(root), 'Main', str(out)])
            assert result.returncode == 0, (n, result)
            original = out.read_bytes()
            second = run([str(compiler), '--compile-modules', str(root), 'Main', str(out)])
            assert second.returncode == 0 and original == out.read_bytes(), (n, second)
            binary = executable_path(root / f'case{n}')
            execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                '-I', str(ROOT / 'runtime'), str(out), str(runtime),
                *link_flags(cache.cc), '-o', str(binary)])
            native = run([str(binary)])
            assert native.returncode == 0 and normalize_newlines(native.stdout) == expected and not native.stderr, (n, native)
            out.unlink()
        invalid = [
            ({'Lib': 'def hidden() -> i64\n\treturn 3\n',
              'Main': 'import Lib alias L\ndef main() -> i64\n\treturn L.hidden()\n'}, 'unknown or ambiguous function'),
            ({'Lib': 'export def show(x: string) -> i64\n\treturn len(x)\n',
              'Main': 'import Lib alias L\ndef main() -> i64\n\treturn L.show(1)\n'}, 'argument type mismatch'),
            ({'Lib': 'export def show(x: i64) -> i64\n\treturn x\n',
              'Main': 'import Lib alias L\ndef main() -> i64\n\treturn show(1)\n'}, 'unknown or ambiguous function'),
            ({'Lib': 'export rec Public(value: i64)\nrec Private(value: i64)\nexport def make() -> Private\n\treturn Private(1)\n',
              'Main': 'import Lib alias L\ndef main() -> i64\n\tlet x: L.Private = L.make()\n\treturn 0\n'}, 'unknown type'),
            ({'Lib': 'export def show() -> i64\n\treturn 1\n',
              'Main': 'import Lib alias L\ndef main() -> i64\n\treturn L.missing()\n'}, 'unknown or ambiguous function'),
            ({'Lib': 'export def show() -> i64\n\treturn 1\n',
              'Main': 'import Lib alias L\ndef worker() -> i64\n\treturn L.show()\n'}, 'missing main'),
            ({'Main': 'import Missing alias L\ndef main() -> i64\n\treturn 0\n'}, 'cannot read module'),
            ({'Other': 'import Main alias Main\n',
              'Main': 'import Other alias O\ndef main() -> i64\n\treturn 0\n'}, 'cyclic source import'),
        ]
        for n, (sources, expected) in enumerate(invalid):
            for name, source in sources.items():
                (root / f'{name}.sirius').write_text(source)
            if out.exists():
                out.unlink()
            failure = run([str(compiler), '--compile-modules', str(root), 'Main', str(out)])
            assert failure.returncode == 1 and expected.encode() in failure.stderr and not out.exists(), (n, failure)
        # Legacy single-file compiler rejects imports; its contract is unchanged.
        single = run([str(compiler), str(root / 'Main.sirius'), str(out)])
        assert single.returncode == 1 and b'source imports require module linking' in single.stderr and not out.exists(), single
    print(f'PASS linked rewrite modules: {len(cases)} native ARC-audited cases; {len(invalid)} invalid; deterministic C')


if __name__ == '__main__':
    main()
