#!/usr/bin/env python3
"""Validate and measure one-allocation union wrappers with escaped record payloads.

An isolated C11 model, NOT the Sirius C emitter. Each model preserves two
independent ARC lifetimes (wrapper and record) and includes managed payloads,
CoW copies and extracted records surviving their wrappers. This models the
ownership mechanism that a production compiler would need, not its lowering.
"""
import argparse
from pathlib import Path
import shutil
import statistics
import subprocess
import tempfile


SOURCE = Path(__file__).with_name('bench_union_coallocation.c')
FIELDS = ('construction', 'traversal', 'cow', 'cleanup')


def execute(args: list[str]) -> str:
    result = subprocess.run(args, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f'{" ".join(args)} exited {result.returncode}\n'
                           f'{result.stdout}\n{result.stderr}')
    return result.stdout.strip()


def build(cc: str, binary: Path, *, sanitize: bool = False) -> None:
    flags = ['-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic',
             '-O1' if sanitize else '-O2']
    if sanitize:
        flags += ['-g', '-fno-omit-frame-pointer', '-fsanitize=address,undefined']
    execute([cc, *flags, str(SOURCE), '-o', str(binary)])


def measure(binary: Path, model: str, count: int, case: str,
            scans: int, clones: int) -> dict[str, int | float]:
    parts = execute([str(binary), model, str(count), case, str(scans), str(clones)]).split(',')
    if len(parts) != 10 or parts[:2] != [model, case]:
        raise AssertionError(f'unexpected benchmark output: {parts!r}')
    times = [float(v) for v in parts[3:7]]
    result: dict[str, int | float] = {
        'checksum': int(parts[2]), 'total': sum(times),
        'allocations': int(parts[7]), 'requested_bytes': int(parts[8]),
        'peak_bytes': int(parts[9]),
    }
    result.update(zip(FIELDS, times))
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='cc')
    parser.add_argument('--count', type=int, default=80000)
    parser.add_argument('--scans', type=int, default=48)
    parser.add_argument('--clones', type=int, default=16)
    parser.add_argument('--runs', type=int, default=7)
    parser.add_argument('--case', choices=('all', 'scalar', 'managed'), default='all')
    parser.add_argument('--sanitize', action='store_true',
                        help='also build and execute an ASan/UBSan correctness run')
    args = parser.parse_args()
    if min(args.count, args.scans, args.clones, args.runs) < 1:
        parser.error('all numeric arguments must be positive')
    cc = shutil.which(args.cc)
    if cc is None:
        parser.error(f'C compiler not found: {args.cc}')
    cases = ('scalar', 'managed') if args.case == 'all' else (args.case,)
    with tempfile.TemporaryDirectory(prefix='sirius-union-coallocation-') as temp:
        exe = Path(temp) / 'prototype'
        build(cc, exe)
        print(execute([str(exe), '--check']))
        if args.sanitize:
            sanitized = Path(temp) / 'prototype_sanitized'
            build(cc, sanitized, sanitize=True)
            print('ASan/UBSan: ' + execute([str(sanitized), '--check']))
            # Include escaped payloads and CoW under sanitizers, but cap cost.
            for case in cases:
                for model in ('current', 'fused'):
                    measure(sanitized, model, min(args.count, 256), case, 3, 2)
        print(f'C11 model: {cc} -O2, {args.count:,} elements, {args.scans} scans, '
              f'{args.clones} detached CoW copies, {args.runs} runs')
        for case in cases:
            results = {'current': [], 'fused': []}
            for iteration in range(args.runs):
                for model in ('current', 'fused') if iteration % 2 == 0 else ('fused', 'current'):
                    results[model].append(measure(exe, model, args.count, case,
                                                  args.scans, args.clones))
            checksums = {r['checksum'] for xs in results.values() for r in xs}
            if len(checksums) != 1:
                raise AssertionError(f'{case}: checksum mismatch: {checksums}')
            med = {name: {key: statistics.median(v[key] for v in samples)
                          for key in ('total', *FIELDS)}
                   for name, samples in results.items()}
            baseline = med['current']['total']
            print(f'\n{case}: checksum={checksums.pop()}')
            print('  model      total ms   build ms   scan ms    CoW ms  free ms   '
                  'ratio   allocations   peak MiB')
            for name, samples in results.items():
                data, sample = med[name], samples[0]
                print(f"  {name:8} {data['total'] * 1000:10.2f} "
                      f"{data['construction'] * 1000:10.2f} "
                      f"{data['traversal'] * 1000:9.2f} "
                      f"{data['cow'] * 1000:9.2f} "
                      f"{data['cleanup'] * 1000:8.2f} "
                      f"{data['total'] / baseline:7.3f}x "
                      f"{sample['allocations']:12,} "
                      f"{sample['peak_bytes'] / 1048576:10.2f}")
    print('\nIndependent C model only. NOT a generated Sirius performance result.')


if __name__ == '__main__':
    main()
