"""End-to-end conformance runner for the Python Sirius->C bootstrap compiler.

Contract: <compiler-command> compile <source.sirius> -o <output.c>
Example: python tools/test_compiler.py --command 'python bootstrap_python/sirius.py'

Reference C is deliberately not used to judge emitted C syntax; the compiler's
C is compiled and run and must match expected stdout instead.
"""
import argparse
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--command', required=True, help='Compiler executable and flags, as a shell-like string')
    parser.add_argument('--provisional', action='store_true', help='Also require proposed syntax/API cases')
    parser.add_argument('--negatives', action='store_true', help='Also test expected compile errors')
    parser.add_argument('--runtime-errors', action='store_true', help='Also test checked runtime panics')
    parser.add_argument('--strict-c', action='store_true', help='Compile generated C with strict warnings as errors')
    ns = parser.parse_args()
    compiler = shlex.split(ns.command)
    cc = shutil.which('cc') or shutil.which('gcc')
    if cc is None:
        raise SystemExit('Missing cc (C11 compiler)')
    fixtures = sorted((ROOT / 'tests' / 'fixtures').glob('*.sirius'))
    if not ns.provisional:
        fixtures = [x for x in fixtures if '_PROVISIONAL' not in x.name]
    with tempfile.TemporaryDirectory() as td:
        work = Path(td)
        for src in fixtures:
            c_path = work / (src.stem + '.c')
            result = subprocess.run([*compiler, 'compile', str(src), '-o', str(c_path)], capture_output=True, text=True)
            if result.returncode:
                raise SystemExit(f'FAIL {src.stem} Sirius compiler: {result.stderr}')
            binary = work / src.stem
            flags = ['-Wall', '-Wextra', '-Werror', '-pedantic'] if ns.strict_c else []
            result = subprocess.run([cc, '-std=c11', *flags, '-I', str(ROOT / 'include'), str(c_path),
                                     str(ROOT / 'include' / 'sirius_runtime.c'), '-lm', '-o', str(binary)],
                                    capture_output=True, text=True)
            if result.returncode:
                raise SystemExit(f'FAIL {src.stem} C compiler: {result.stderr}')
            result = subprocess.run([str(binary)], cwd=work, capture_output=True, text=True)
            expected = (ROOT / 'tests' / 'expected_output' / (src.stem + '.txt')).read_text()
            if result.returncode or result.stdout != expected:
                raise SystemExit(f'FAIL {src.stem}: actual={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
            print(f'PASS {src.stem}')
        if ns.negatives:
            failures = json.loads((ROOT / 'tests' / 'negative' / 'expectations.json').read_text())
            for stem, code in failures.items():
                if 'DEFERRED' in stem and not ns.provisional:
                    continue
                src = ROOT / 'tests' / 'negative' / (stem + '.sirius')
                result = subprocess.run([*compiler, 'compile', str(src), '-o', str(work / 'negative.c')],
                                        capture_output=True, text=True)
                diagnostics = result.stdout + result.stderr
                if result.returncode == 0 or code not in diagnostics:
                    raise SystemExit(f'FAIL {stem}: expected diagnostic {code}, got {diagnostics!r}')
                print(f'PASS {stem} -> {code}')
        if ns.runtime_errors:
            errors = json.loads((ROOT / 'tests' / 'runtime_errors' / 'expectations.json').read_text())
            for stem, expected in errors.items():
                source = ROOT / 'tests' / 'runtime_errors' / (stem + '.sirius')
                c_path = work / (stem + '.c')
                binary = work / stem
                result = subprocess.run([*compiler, 'compile', str(source), '-o', str(c_path)],
                                        capture_output=True, text=True)
                if result.returncode:
                    raise SystemExit(f'FAIL {stem} compile: {result.stderr}')
                result = subprocess.run([cc, '-std=c11', '-I', str(ROOT / 'include'),
                                         str(c_path), str(ROOT / 'include' / 'sirius_runtime.c'),
                                         '-lm', '-o', str(binary)], capture_output=True, text=True)
                if result.returncode:
                    raise SystemExit(f'FAIL {stem} C: {result.stderr}')
                result = subprocess.run([str(binary)], cwd=work, capture_output=True, text=True)
                if result.returncode != expected['exit'] or expected['stderr_contains'] not in result.stderr:
                    raise SystemExit(f'FAIL {stem} panic: {result}')
                print(f'PASS {stem} (expected runtime panic)')


if __name__ == '__main__':
    main()
