#!/usr/bin/env python3
"""Array-valued record fields: value snapshots, CoW and ARC/native parity."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(command):
    return subprocess.run([str(item) for item in command], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('''rec Bag(nums: Array[i64], words: Array[string], label: string)
def changed(input: Bag) -> Bag
\tvar next = input
\tnext.words = next.words.append("extra")
\tnext.nums = [10, 20]
\treturn next
def main() -> i64
\tlet saved = Bag([1, 2], ["a", "b"], "old")
\tlet newer = changed(saved)
\tprint(len(saved.words))
\tprint(len(newer.words))
\tprint(saved.nums[!0])
\tprint(newer.nums[!0])
\tlet rows = [saved, newer]
\tprint(rows[!0].words[!0])
\tprint(rows[!1].words[!-1])
\treturn 0
''', b'2\n3\n1\n10\na\nextra\n'),
        ('''rec A(numbers: Array[i64], title: string)
rec B(title: string, numbers: Array[i64])
def bump(x: B) -> A
\tvar y = x
\ty.numbers = y.numbers.append(30)
\treturn y
def main() -> i64
\tlet start: A = B("hello", [10])
\tlet final = bump(start)
\tprint(start.numbers[!0])
\tprint(len(start.numbers))
\tprint(len(final.numbers))
\tprint(final.numbers[!-1])
\treturn 0
''', b'10\n1\n2\n30\n'),
        ('''rec R(words: Array[string])
def main() -> i64
\tvar a = R(["a", "b"])
\tlet snapshot = a
\ta.words = a.words
\ta.words = ["new"]
\tprint(snapshot.words[!-1])
\tprint(a.words[!0])
\tprint(len(a.words))
\treturn 0
''', b'b\nnew\n1\n'),
        ('''rec R(nums: Array[i64], words: Array[string])
def main() -> i64
\tvar rows = [R([1], ["first"]), R([2], ["second"])]
\tlet snapshot = rows
\trows[!0].nums = [3, 4]
\trows[!1].words = ["updated"]
\tprint(snapshot[!0].nums[!0])
\tprint(rows[!0].nums[!-1])
\tprint(snapshot[!1].words[!0])
\tprint(rows[!1].words[!0])
\treturn 0
''', b'1\n4\nsecond\nupdated\n'),
        ('''rec R(x: Array[string])
def main() -> i64
\tvar list = [R(["x"])]
\tlet keep = list
\tfor i in 0..8
\t\tlist[!0].x = list[!0].x.append(str(i))
\tprint(len(keep[!0].x))
\tprint(len(list[!0].x))
\tprint(list[!0].x[!-1])
\treturn 0
''', b'1\n9\n7\n'),
        ('''rec R(a: Array[i64], b: Array[string])
def identity(x: R) -> R
\treturn x
def main() -> i64
\tlet x = R(Array[i64](), Array[string]())
\tlet y = identity(x)
\tprint(len(x.a))
\tprint(len(y.b))
\treturn 0
''', b'0\n0\n'),
        ('''rec R(values: Array[i64])
def main() -> i64
\tlet source = [1, 2]
\tvar p = R(source)
\tlet snapshot = p
\tp.values = p.values.append(9)
\tprint(len(source))
\tprint(len(snapshot.values))
\tprint(len(p.values))
\treturn 0
''', b'2\n2\n3\n'),
        ('''rec R(a: Array[string], b: string)
def main() -> i64
\tlet value = R(["a\\0é"], "s")
\tvar rows = [value]
\tlet snapshot = rows
\trows[!0] = R(["replacement"], "new")
\tprint(snapshot[!0].a[!0])
\tprint(rows[!0].a[!0])
\tprint(snapshot[!0].b)
\treturn 0
''', b'a\x00\xc3\xa9\nreplacement\ns\n'),
    ]
    invalid = [
        'rec R(a: Array[Missing])\ndef main() -> i64\n\treturn 0\n',
        'rec R(a: Array[Optional[i64]])\ndef main() -> i64\n\treturn 0\n',
        'rec R(a: Optional[i64])\ndef main() -> i64\n\treturn 0\n',
        'rec R(a: Array[i64])\ndef main() -> i64\n\tlet r = R(["bad"])\n\treturn 0\n',
        'rec R(a: Array[string])\ndef main() -> i64\n\tvar r = R(["a"])\n\tr.a = [1]\n\treturn 0\n',
        'rec R(a: Array[i64])\ndef main() -> i64\n\tlet r = R([1])\n\tr.a = [2]\n\treturn 0\n',
        'rec R(a: Array[string])\ndef main() -> i64\n\tvar rows = [R(["a"])]\n\trows[!0].a = [1]\n\treturn 0\n',
        'rec R(a: Array[i64])\ndef main() -> i64\n\tlet r = R([1])\n\tprint(r.a[!0] == "x")\n\treturn 0\n',
    ]
    bounds = [
        'rec R(a: Array[i64])\ndef main() -> i64\n\tlet r = R([1])\n\tprint(r.a[!2])\n\treturn 0\n',
        'rec R(a: Array[string])\ndef main() -> i64\n\tvar r = R(["a"])\n\tprint(r.a[!-2])\n\treturn 0\n',
        'rec R(a: Array[i64])\ndef main() -> i64\n\tvar a = [R([1])]\n\tprint(a[!0].a[!2])\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        audit = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = root / 'test.sirius', root / 'generated.c', root / 'test'
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS composed record fields: {len(valid)} valid audited native C', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS composed record fields: {len(invalid)} invalid programs', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS composed record fields: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
