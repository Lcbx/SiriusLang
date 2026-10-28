#!/usr/bin/env python3
"""Measure three native C storage models for scalar tagged-union arrays.

This is an isolated C11 prototype, not a Sirius compiler backend. Measurements
include allocation, traversal, copying a CoW-detached array, updating one
alternative, and ARC-like retain/release in each model. 'register-boxed'
models a register-inline union whose array representation remains pointer-
boxed: it should match 'current' allocation counts, unlike an inline array.
The standalone 'fused' model has one RC identity and is NOT the compiler's
coallocation experiment, which preserves separately owned payload records. Managed values, the
MIR interpreter, package layouts, and generated code are deliberately excluded.
"""
import argparse
from pathlib import Path
import shutil
import statistics
import subprocess
import tempfile

from build_support import require_cc


CASES = {
    'enums': (0, 2),
    'small-balanced': (6, 2),
    'small-sparse': (6, 64),
    'wide-sparse': (100, 64),
}
MODELS = ('current', 'fused', 'inline', 'register-boxed')


def build(cc: str, binary: Path, opt: int) -> None:
    source = Path(__file__).with_name('bench_union_storage.c')
    process = subprocess.run([cc, '-std=c11', f'-O{opt}', '-Wall', '-Wextra', '-Werror',
                              '-pedantic', str(source), '-o', str(binary)],
                             capture_output=True, text=True)
    if process.returncode:
        raise RuntimeError(process.stderr)


def measure(binary: Path, model: str, n: int, maximum: int, period: int,
            scans: int, clones: int) -> dict[str, float | int]:
    args = [str(binary), model, str(n), str(maximum), str(period), str(scans), str(clones)]
    result = subprocess.run(args, capture_output=True, text=True, check=True)
    parts = result.stdout.strip().split(',')
    if len(parts) != 10 or parts[0] != model:
        raise RuntimeError(f'unexpected result: {result.stdout!r}')
    checksum = int(parts[1])
    build_s, scan_s, cow_s, drop_s = (float(x) for x in parts[2:6])
    allocs, reallocs, bytes_requested, peak_bytes = (int(x) for x in parts[6:])
    return dict(checksum=checksum, construction=build_s, traversal=scan_s,
                cow=cow_s, cleanup=drop_s, total=build_s + scan_s + cow_s + drop_s,
                allocations=allocs, reallocations=reallocs,
                requested_bytes=bytes_requested, peak_bytes=peak_bytes)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default=require_cc())
    parser.add_argument('--opt', type=int, default=2, choices=(0, 2, 3))
    parser.add_argument('--count', type=int, default=80000)
    parser.add_argument('--runs', type=int, default=5)
    parser.add_argument('--scans', type=int, default=48)
    parser.add_argument('--clones', type=int, default=16)
    parser.add_argument('--case', choices=(*CASES, 'all'), default='all')
    args = parser.parse_args()
    if min(args.count, args.runs, args.scans, args.clones) < 1:
        parser.error('--count, --runs, --scans and --clones must be positive')
    cc = shutil.which(args.cc)
    if cc is None:
        parser.error(f'C compiler not found: {args.cc}')
    cases = CASES if args.case == 'all' else {args.case: CASES[args.case]}
    with tempfile.TemporaryDirectory(prefix='sirius-union-storage-') as folder:
        binary = Path(folder) / 'union_storage'
        build(cc, binary, args.opt)
        print(f'C11 model experiment: {cc} -O{args.opt}, {args.count:,} values, '
              f'{args.scans} traversals, {args.clones} detached CoW updates; '
              f'{args.runs} measured runs', flush=True)
        for case_name, (max_words, period) in cases.items():
            results = {model: [] for model in MODELS}
            # Rotate order across runs rather than consistently giving one
            # representation the cold-start or hot-cache advantage.
            for repeat in range(args.runs):
                for offset in range(len(MODELS)):
                    model = MODELS[(repeat + offset) % len(MODELS)]
                    results[model].append(measure(binary, model, args.count, max_words,
                                                  period, args.scans, args.clones))
            checksums = {r['checksum'] for runs in results.values() for r in runs}
            if len(checksums) != 1:
                raise AssertionError(f'{case_name}: output mismatch: {checksums}')
            expected_allocations = {
                'current': 2 * (args.count + args.clones),
                'register-boxed': 2 * (args.count + args.clones),
                'fused': args.count + args.clones,
                'inline': 0,
            }
            for model, runs in results.items():
                for sample in runs:
                    if sample['allocations'] != expected_allocations[model]:
                        raise AssertionError(f'{case_name} {model}: unexpected allocations '
                                             f'{sample["allocations"]}')
            if results['current'][0]['peak_bytes'] != results['register-boxed'][0]['peak_bytes']:
                raise AssertionError(f'{case_name}: boxing altered container memory footprint')
            med = {model: {field: statistics.median(r[field] for r in results[model])
                           for field in ('construction', 'traversal', 'cow', 'cleanup', 'total')}
                   for model in MODELS}
            baseline = med['current']['total']
            print(f'\n{case_name} (maximum payload {max_words} x i64, '
                  f'1/{period} wide): checksum={checksums.pop()}')
            print('  model           total ms   build ms   scan ms    CoW ms  free ms   '
                  'vs current    allocs   peak MiB')
            for model in MODELS:
                entry = med[model]
                sample = results[model][0]
                print(f"  {model:14} {entry['total'] * 1000:8.2f} "
                      f"{entry['construction'] * 1000:10.2f} "
                      f"{entry['traversal'] * 1000:9.2f} "
                      f"{entry['cow'] * 1000:9.2f} "
                      f"{entry['cleanup'] * 1000:8.2f} "
                      f"{entry['total'] / baseline:10.2f}x "
                      f"{sample['allocations']:9,} "
                      f"{sample['peak_bytes'] / 1048576:10.1f}")
    print('\nMeasured C model only; not a Sirius native/MIR performance claim.')
    print('The enums row is a generic C-layout control: actual Sirius payload-free enums already use i64.')


if __name__ == '__main__':
    main()
