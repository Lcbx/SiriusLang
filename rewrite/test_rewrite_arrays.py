#!/usr/bin/env python3
"""Check the rewrite's first Array[i64] runtime path and CoW ownership."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def invoke(*command: object) -> subprocess.CompletedProcess[bytes]:
    return subprocess.run([str(x) for x in command], capture_output=True, timeout=60)


def main() -> None:
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    verifier = cache.native_driver('rewrite-array-ir', ROOT / 'rewrite/array_ir_test_driver.sirius', extra_modules=modules)
    checked = invoke(verifier)
    assert (checked.returncode, normalize_newlines(checked.stdout), checked.stderr) == (0, b'PASS arrays IR\n', b''), checked
    cases = [
        ('''def main() -> i64
\tlet values = [3, 0, -6]
\tprint(len(values))
\tprint(values[!0])
\tprint(values[!-1])
\tprint(values[!-3])
\treturn 0
''', b'3\n3\n-6\n3\n'),
        ('''def main() -> i64
\tlet empty: Array[i64] = Array[i64]()
\tlet next = empty.append(5).append(7)
\tprint(len(empty))
\tprint(len(next))
\tprint(next[!-1])
\treturn 0
''', b'0\n2\n7\n'),
        ('''def changed(source: Array[i64]) -> Array[i64]
\tvar result = source
\tresult[!0] = 41
\treturn result.append(99)
def main() -> i64
\tvar a = [1, 2, 3]
\tlet snapshot = a
\ta[!0] = 9
\ta[!-1] += 5
\ta = changed(a)
\tprint(snapshot[!0])
\tprint(snapshot[!-1])
\tprint(a[!0])
\tprint(a[!-2])
\tprint(a[!-1])
\treturn 0
''', b'1\n3\n41\n8\n99\n'),
        ('''def total(values: Array[i64]) -> i64
\tvar sum = 0
\tfor item in values
\t\tsum += item
\treturn sum
def main() -> i64
\tlet values = [1, 2, 3, 4]
\tprint(total(values))
\tfor item in values
\t\tif item == 2
\t\t\tcontinue
\t\tprint(item)
\tprint(total(Array[i64]()))
\treturn 0
''', b'10\n1\n3\n4\n0\n'),
        ('''def main() -> i64
\tvar data = [0, 1, 2]
\tlet saved = data
\tfor i in 0..5
\t\tdata = data.append(i)
\tfor item in data
\t\tif item == 3
\t\t\tbreak
\t\tprint(item)
\tprint(len(data))
\tprint(len(saved))
\treturn 0
''', b'0\n1\n2\n0\n1\n2\n8\n3\n'),
        ('''def choose_index() -> i64
\tprint(99)
\treturn 1
def main() -> i64
\tvar a = [7, 8]
\ta[!choose_index()] += 4
\tprint(a[!1])
\tvar total = 0
\tfor item in a
\t\ttotal += item
\t\ta = a.append(item)
\tprint(total)
\tprint(len(a))
\treturn 0
''', b'99\n12\n19\n4\n'),
        ('''def mirror(xs: Array[i64]) -> Array[i64]
\treturn xs
def main() -> i64
\tvar xs = mirror([10, 20])
\tlet a = xs
\txs[!0] = xs[!1]
\txs[!1] = xs[!0]
\tprint(a[!0])
\tprint(xs[!0])
\tprint(xs[!1])
\treturn 0
''', b'10\n20\n20\n'),
    ]
    # String arrays are valid now; managed ownership is also checked in
    # test_rewrite_managed_arrays.py. Keep this suite for integer regressions.
    invalid = [
        'let x = []\n\treturn 0',
        'let x = [1, true]\n\treturn 0',
        'let x: Array[i64] = 3\n\treturn 0',
        'let x = [1]\n\tprint(str(x))\n\treturn 0',
        'let x = [1]\n\tprint(x)\n\treturn 0',
        'let x = [1]\n\tprint(x[!true])\n\treturn 0',
        'let x = 3\n\tprint(x[!0])\n\treturn 0',
        'let x = [1]\n\tx[!0] = 2\n\treturn 0',
        'var x = [1]\n\tx[!0] = false\n\treturn 0',
        'var x = [1]\n\tx[!true] = 2\n\treturn 0',
        'var x = [1]\n\tx[!0] += "bad"\n\treturn 0',
        'let x = [1]\n\tprint(x.append("bad")[!0])\n\treturn 0',
        'for x in 3\n\t\tprint(x)\n\treturn 0',
        'for x in [[1]]\n\t\tprint(x)\n\treturn 0',
        'let x = [1,]\n\treturn 0',
        'let x: Array[string] = Array[i64]()\n\treturn 0',
    ]
    traps = [
        'print([1, 2][!2])',
        'print([1, 2][!-3])',
        'var a = [1, 2]\n\ta[!2] = 8',
        'var a = [1, 2]\n\ta[!-3] += 8',
        'print(Array[i64]()[!-1])',
    ]
    with tempfile.TemporaryDirectory() as directory:
        base = Path(directory)
        audit = base / 'audit-runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source = base / 'input.sirius'
        output = base / 'output.c'
        binary = base / 'output'
        for index, (text, expected) in enumerate(cases):
            source.write_text(text)
            emitted = invoke(compiler, source, output)
            assert emitted.returncode == 0, (index, 'emission', emitted)
            compile_c(cache.cc, output, binary, audit)
            native = invoke(binary)
            assert (native.returncode, normalize_newlines(native.stdout), native.stderr) == (0, expected, b''), (index, 'native', native)
        print(f'PASS Array[i64]: {len(cases)} audited native cases', flush=True)
        for index, body in enumerate(invalid):
            source.write_text('def main() -> i64\n\t' + body + '\n')
            result = invoke(compiler, source, output)
            assert result.returncode == 1 and b'ERROR ' in result.stderr, (index, result)
        print(f'PASS Array[i64]: {len(invalid)} invalid programs rejected', flush=True)
        for index, body in enumerate(traps):
            source.write_text('def main() -> i64\n\t' + body + '\n\treturn 0\n')
            emitted = invoke(compiler, source, output)
            assert emitted.returncode == 0, (index, 'emission', emitted)
            compile_c(cache.cc, output, binary, audit)
            native = invoke(binary)
            assert native.returncode == 1 and b'index out of bounds' in native.stderr, (index, 'native', native)
        print(f'PASS Array[i64]: {len(traps)} runtime bounds traps', flush=True)


if __name__ == '__main__':
    main()
