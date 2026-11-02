#!/usr/bin/env python3
"""Separator join regressions: native and ARC-audited."""
import subprocess
import sys
import tempfile
import textwrap
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def source(code):
    return textwrap.dedent(code).replace('    ', '\t').strip() + '\n'


def run(args):
    return subprocess.run([str(x) for x in args], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('rewrite-join', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('normal', '''def main() -> i64
    print("-".join(["alpha", "beta", "gamma"]))
    return 0''', b'alpha-beta-gamma\n'),
        ('empty / singleton', '''def main() -> i64
    print("!".join(Array[string]()))
    print("!".join(["one"]))
    return 0''', b'\none\n'),
        ('utf8 and NUL', '''def main() -> i64
    let joined = "\\0é".join(["á", "b\\0"])
    print(len(joined))
    print(joined.byte_at(2) or 99)
    print(joined.byte_at(4) or 99)
    return 0''', b'7\n0\n169\n'),
        ('snapshot / call / concat', '''def merge(parts: Array[string], sep: string) -> string
    return sep.join(parts)
def main() -> i64
    let original = ["one", "two"]
    var changed = original
    changed[!0] = "changed"
    print(merge(original, "/") + "!")
    print("".join(changed))
    return 0''', b'one/two!\nchangedtwo\n'),
        ('nested and chained', '''def main() -> i64
    print("/".join(["+".join(["a", "b"]), "c"]).ends_with("c"))
    return 0''', b'true\n'),
    ]
    invalid = [
        ('separator not string', 'def main() -> i64\n\tprint(1.join(["a"]))\n\treturn 0\n'),
        ('not an array', 'def main() -> i64\n\tprint("-".join("x"))\n\treturn 0\n'),
        ('wrong element', 'def main() -> i64\n\tprint("-".join([1, 2]))\n\treturn 0\n'),
        ('no argument', 'def main() -> i64\n\tprint("-".join())\n\treturn 0\n'),
        ('extra argument', 'def main() -> i64\n\tprint("-".join(["a"], ["b"]))\n\treturn 0\n'),
    ]
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        audit = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        src, generated, binary = (root / x for x in ('source.sirius', 'generated.c', 'program'))
        for name, program, expected in valid:
            src.write_text(source(program), encoding='utf-8')
            result = run([compiler, src, generated])
            assert result.returncode == 0, (name, 'compile', result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, result)
        print(f'PASS joins: {len(valid)} ARC-audited programs', flush=True)
        for name, program in invalid:
            src.write_text(source(program), encoding='utf-8')
            result = run([compiler, src, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, (name, result)
        print(f'PASS joins: {len(invalid)} invalid programs', flush=True)

if __name__ == '__main__':
    main()
