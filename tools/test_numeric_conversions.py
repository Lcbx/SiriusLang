#!/usr/bin/env python3
"""Exercise checked scalar numeric conversions in native code and optionally MIR."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, executable_path


VALID = '''def mean(xs: Array[i64]) -> f64
    var total = 0.0
    for x in xs
        total += f64(x)
    return total / f64(len(xs))

def main() -> i64
    if mean([1, 2, 3, 4]) != 2.5
        return 1
    if f64(9007199254740993) != f64(9007199254740992)
        return 2
    if f64(-9007199254740993) != -f64(9007199254740992)
        return 3
    if f64(9223372036854775807) != -f64(-9223372036854775807 - 1)
        return 4
    if f64(-9223372036854775807 - 1) != -f64(9223372036854775807)
        return 5
    if i64(-0.0) != 0 or i64(0.5) != 0 or i64(-0.5) != 0
        return 6
    if i64(3.9) != 3 or i64(-3.9) != -3
        return 7
    if i64(f64(-9223372036854775807 - 1)) != (-9223372036854775807 - 1)
        return 8
    if i64(f64(9223372036854773760)) != 9223372036854773760
        return 9
    return 0
'''

TRAPS = (
    'i64(0.0 / 0.0)',  # NaN
    'i64(1.0 / 0.0)',  # +inf
    'i64(-1.0 / 0.0)',  # -inf
    'i64(f64(9223372036854775807))',  # exclusive upper endpoint
    'i64(f64(-9223372036854775807 - 1) - 2048.0)',  # below lower endpoint
)

INVALID = (
    ('f64(true)', 'expected i64'),
    ('i64(true)', 'expected f64'),
    ('f64(1.0)', 'expected i64'),
    ('i64(1)', 'expected f64'),
    ('f64(1, 2)', 'f64 expects one i64 argument'),
    ('i64()', 'i64 expects one f64 argument'),
)


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
    return subprocess.run([str(x) for x in args], capture_output=True, timeout=60)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='prebuilt native Sirius compiler')
    parser.add_argument('--runner', type=Path, help='prebuilt MIR runner for source parity')
    parser.add_argument('--skip-mir-driver', action='store_true', help='skip MIR interpreter/guard driver')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler or cache.seed()
    with tempfile.TemporaryDirectory(prefix='sirius-numeric-conversions-') as directory:
        root = Path(directory)
        cases = [('valid', VALID, True)]
        cases += [(f'trap-{i}', f'def main() -> i64\n    return {expr}\n', False)
                  for i, expr in enumerate(TRAPS)]
        # Test trap cases by evaluating the conversion as an expression,
        # rather than depending on a special result or return type.
        for label, source, success in cases:
            sirius = root / f'{label}.sirius'
            generated = root / f'{label}.c'
            binary = executable_path(root / label)
            sirius.write_text(source.replace('    ', '\t'))
            compiled = run(compiler, sirius, generated)
            assert compiled.returncode == 0, (label, compiled.stdout, compiled.stderr)
            emitted = generated.read_text()
            if not success:
                assert '>= -0x1p63 &&' in emitted and 's_panic(' in emitted, label
            built = run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-I', ROOT / 'runtime',
                        generated, ROOT / 'runtime/sirius_runtime.c', *link_flags(cache.cc), '-o', binary)
            assert built.returncode == 0, (label, built.stderr)
            native = run(binary)
            if success:
                assert native.returncode == 0 and native.stdout == b'' and native.stderr == b'', (label, native)
            else:
                assert native.returncode != 0 and b'floating-point value cannot be represented as i64' in native.stderr, (label, native)
            if args.runner:
                interpreted = run(args.runner, sirius)
                if success:
                    assert interpreted.returncode == 0 and interpreted.stdout == native.stdout, (label, interpreted, native)
                else:
                    assert interpreted.returncode != 0 and b'floating-point value cannot be represented as i64' in interpreted.stderr + interpreted.stdout, (label, interpreted)
        for i, (expression, message) in enumerate(INVALID):
            sirius = root / f'invalid-{i}.sirius'
            generated = root / f'invalid-{i}.c'
            sirius.write_text(f'def main() -> i64\n\tlet unused = {expression}\n\treturn 0\n')
            rejected = run(compiler, sirius, generated)
            assert rejected.returncode != 0 and message.encode() in rejected.stdout + rejected.stderr, (expression, rejected)
    if not args.skip_mir_driver:
        driver = cache.native_driver('numeric-conversions', ROOT / 'tests/mir/numeric_conversions_driver_native.sirius')
        checked = run(driver)
        assert checked.returncode == 0 and checked.stdout == b'' and checked.stderr == b'', checked
    print(f'PASS numeric conversions: {len(cases)} native cases, {len(INVALID)} typing rejections' +
          (', MIR parity' if args.runner else ', MIR driver' if not args.skip_mir_driver else ' (MIR driver skipped)'))


if __name__ == '__main__':
    main()
