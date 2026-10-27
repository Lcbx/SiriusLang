#!/usr/bin/env python3
"""Benchmark compiled Std.Numeric against equivalent flat-array Sirius loops.

The flat variants use the same append-based storage, checked indexing, and
arithmetic order. Times include allocations and CoW; source generation and C
compilation are reported separately. Neither variant is a vectorized kernel.
"""
import argparse
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, require_cc, link_flags
from project_paths import ROOT, executable_path


# Each pair uses the same numeric inputs. Large cases amortize process startup;
# tiny cases expose per-call checks and record/tag handling.
CASES = {
    'add-tiny': (4, 4, 120000),
    'add-large': (128, 128, 512),
    'add-broadcast': (128, 128, 512),
    'matmul': (64, 64, 64),
    'cow-snapshot': (256, 256, 512),
}


def program(case: str, matrix: bool, rows: int, cols: int, iterations: int) -> str:
    prelude = '''import Std.Numeric alias N

def filled(count: i64, value: f64) -> Array[f64]
    var data = Array[f64]()
    for _ in 0..count
        data = data.append(value)
    return data

def main() -> i64
'''
    # Avoid implicit numeric conversions and use a source-level checksum so
    # native code must produce and inspect each result.
    if case in ('add-tiny', 'add-large', 'add-broadcast'):
        other_len = cols if case == 'add-broadcast' else rows * cols
        initial = f'''    let left_data = filled({rows * cols}, 1.0)
    let right_data = filled({other_len}, 2.0)
    var checksum = 0.0
'''
        if matrix:
            right_rows = 1 if case == 'add-broadcast' else rows
            body = f'''    let left = N.Matrix({rows}, {cols}, left_data)
    let right = N.Matrix({right_rows}, {cols}, right_data)
    for _ in 0..{iterations}
        match N.add(left, right)
            N.ShapeError(message)
                return 1
            N.MatrixOk(result)
                checksum += result.data[!{rows * cols - 1}]
'''
        else:
            # Mirror the original row/column traversal and broadcast
            # selection; unlike Std.Numeric this flat version trusts its
            # caller's known-valid shape.
            right_index = 'col' if case == 'add-broadcast' else f'row * {cols} + col'
            body = f'''    for _ in 0..{iterations}
        var result = Array[f64]()
        for row in 0..{rows}
            for col in 0..{cols}
                result = result.append(left_data[!(row * {cols} + col)] + right_data[!({right_index})])
        checksum += result[!{rows * cols - 1}]
'''
        expected = iterations * 3.0
    elif case == 'matmul':
        initial = f'''    let left_data = filled({rows * cols}, 1.0)
    let right_data = filled({rows * cols}, 1.0)
    var checksum = 0.0
'''
        if matrix:
            body = f'''    let left = N.Matrix({rows}, {cols}, left_data)
    let right = N.Matrix({rows}, {cols}, right_data)
    for _ in 0..{iterations}
        match N.matmul(left, right)
            N.ShapeError(message)
                return 1
            N.MatrixOk(result)
                checksum += result.data[!{rows * cols - 1}]
'''
        else:
            body = f'''    for _ in 0..{iterations}
        var result = Array[f64]()
        for row in 0..{rows}
            for col in 0..{cols}
                var sum = 0.0
                for k in 0..{cols}
                    sum += left_data[!(row * {cols} + k)] * right_data[!(k * {cols} + col)]
                result = result.append(sum)
        checksum += result[!{rows * cols - 1}]
'''
        expected = iterations * cols * 1.0
    elif case == 'cow-snapshot':
        initial = f'''    let original = filled({rows * cols}, 1.0)
    var checksum = 0.0
'''
        if matrix:
            body = f'''    let matrix = N.Matrix({rows}, {cols}, original)
    for _ in 0..{iterations}
        var changed = matrix.data
        changed[!{rows * cols - 1}] = 2.0
        checksum += changed[!{rows * cols - 1}] + matrix.data[!{rows * cols - 1}]
'''
        else:
            body = f'''    for _ in 0..{iterations}
        var changed = original
        changed[!{rows * cols - 1}] = 2.0
        checksum += changed[!{rows * cols - 1}] + original[!{rows * cols - 1}]
'''
        expected = iterations * 3.0
    else:
        raise ValueError(case)
    # Exit nonzero if either version returns an incorrect checksum.
    source = prelude + initial + body + f'''    if checksum != {expected:.1f}
        return 2
    print(checksum)
    return 0
'''
    return re.sub(r'(?m)^(    )+', lambda match: '\t' * (len(match.group()) // 4), source)


def run(command: list[str], timeout: int) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, capture_output=True, text=True, timeout=timeout)
    if result.returncode:
        raise RuntimeError(f"{command} failed {result.returncode}: {result.stdout} {result.stderr}")
    return result


def timed(command: list[str], timeout: int) -> tuple[float, subprocess.CompletedProcess[str]]:
    start = perf_counter()
    result = run(command, timeout)
    return perf_counter() - start, result


def build(cc: str, seed: Path, runtime: Path, modules: Path, output: Path,
          case: str, matrix: bool, rows: int, cols: int, iterations: int,
          optimize: int, timeout: int) -> tuple[float, float]:
    source = modules / 'App.sirius'
    source.write_text(program(case, matrix, rows, cols, iterations), encoding='utf-8')
    generated = output.with_suffix('.c')
    t_source, _ = timed([str(seed), '--module-root', str(modules), 'App', str(generated)], timeout)
    start = perf_counter()
    run([cc, '-std=c11', f'-O{optimize}', '-Wno-unused-function', '-I', str(ROOT / 'runtime'),
         str(generated), str(runtime), *link_flags(cc), '-o', str(output)], timeout)
    return t_source, perf_counter() - start


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runs', type=int, default=5, help='timed invocations per executable')
    parser.add_argument('--cc', default=require_cc(), help='C11 compiler')
    parser.add_argument('--baseline-numeric', type=Path,
                        help='previous Numeric.sirius to compare against the current module')
    parser.add_argument('--opt', type=int, choices=(0, 2), default=2, help='C optimizer level')
    parser.add_argument('--timeout', type=int, default=180)
    parser.add_argument('--case', choices=(*CASES, 'all'), default='all')
    args = parser.parse_args()
    if args.runs < 1 or args.timeout < 1:
        parser.error('--runs and --timeout must be positive')
    cc_path = shutil.which(args.cc)
    if cc_path is None:
        parser.error(f"C compiler not found: {args.cc}")
    # The seed emits identical C irrespective of the benchmark C compiler.
    seed = BuildCache(require_cc()).seed()
    cc = cc_path
    previous = args.baseline_numeric.resolve() if args.baseline_numeric else None
    if previous is not None and not previous.is_file():
        parser.error(f"missing baseline: {previous}")
    selected = CASES if args.case == 'all' else {args.case: CASES[args.case]}
    with tempfile.TemporaryDirectory(prefix='sirius-numeric-bench-') as directory:
        root = Path(directory)
        modes = ('flat', 'before', 'matrix') if previous else ('flat', 'matrix')
        modules = {}
        for mode in modes:
            directory = root / f'modules-{mode}'
            (directory / 'Std').mkdir(parents=True)
            shutil.copyfile(previous if mode == 'before' else ROOT / 'stdlib/Std/Numeric.sirius',
                            directory / 'Std/Numeric.sirius')
            modules[mode] = directory
        print(f'compiler={run([cc, "--version"], args.timeout).stdout.splitlines()[0]}; '
              f'C=-O{args.opt}; runs={args.runs}', flush=True)
        for case, (rows, cols, iterations) in selected.items():
            binary = {}
            build_times = {}
            for mode in modes:
                target = executable_path(root / f'{case}-{mode}')
                build_times[mode] = build(cc, seed, ROOT / 'runtime/sirius_runtime.c', modules[mode],
                    target, case, mode != 'flat', rows, cols, iterations, args.opt, args.timeout)
                binary[mode] = target
            samples: dict[str, list[float]] = {mode: [] for mode in modes}
            checksums = {}
            # Rotate evaluation order to reduce thermal/frequency effects.
            for i in range(args.runs + 1):
                for mode in modes[i % len(modes):] + modes[:i % len(modes)]:
                    seconds, result = timed([str(binary[mode])], args.timeout)
                    checksums[mode] = result.stdout.strip()
                    if i:  # discard one warm-up per case/mode
                        samples[mode].append(seconds)
            if len(set(checksums.values())) != 1:
                raise RuntimeError(f'{case}: outputs disagree: {checksums}')
            medians = {mode: statistics.median(samples[mode]) for mode in modes}
            measurements = ' '.join(f'{mode}={medians[mode]:.4f}s' for mode in modes)
            comparisons = f'matrix/flat={medians["matrix"] / medians["flat"]:.2f}x'
            if previous:
                comparisons += f' after/before={medians["matrix"] / medians["before"]:.2f}x'
            print(f'{case:15s} {rows}x{cols} x{iterations}: {measurements} '
                  f'{comparisons} checksum={checksums["flat"]} '
                  f'generate(flat/matrix)={build_times["flat"][0]:.2f}/'
                  f'{build_times["matrix"][0]:.2f}s '
                  f'C(flat/matrix)={build_times["flat"][1]:.2f}/'
                  f'{build_times["matrix"][1]:.2f}s', flush=True)


if __name__ == '__main__':
    main()
