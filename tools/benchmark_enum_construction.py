#!/usr/bin/env python3
"""Compare two Sirius compiler builds on an enum-heavy generated-native workload.

Provide the baseline and updated native compiler binaries. Measures resulting
program execution, not compiler throughput, across alternating warm runs.
"""
import argparse
from pathlib import Path
import statistics
import subprocess
import tempfile
import time

from build_support import link_flags, require_cc
from project_paths import ROOT, executable_path


def checked(command: list[str]) -> None:
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f'{command[0]}: {result.stdout}{result.stderr}')


def generate(count: int) -> str:
    return f'''enum State
\tRed
\tGreen
\tBlue

def main() -> i64
\tvar values = Array[State]()
\tfor i in 0..{count}
\t\tif i % 3 == 0
\t\t\tvalues = values.append(Red())
\t\telif i % 3 == 1
\t\t\tvalues = values.append(Green())
\t\telse
\t\t\tvalues = values.append(Blue())
\tvar total = 0
\tfor state in values
\t\tmatch state
\t\t\tRed()
\t\t\t\ttotal += 1
\t\t\tGreen()
\t\t\t\ttotal += 2
\t\t\tBlue()
\t\t\t\ttotal += 3
\treturn total % 251
'''


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('baseline', type=Path, help='native compiler before the MIR optimization')
    parser.add_argument('updated', type=Path, help='native compiler with the MIR optimization')
    parser.add_argument('--cc', default=require_cc())
    parser.add_argument('--count', type=int, default=2400000)
    parser.add_argument('--runs', type=int, default=9)
    args = parser.parse_args()
    if args.count < 3 or args.count % 3 != 0 or args.runs < 3:
        parser.error('--count must be a positive multiple of 3; --runs must be >= 3')
    expected = (args.count // 3 * 6) % 251
    with tempfile.TemporaryDirectory(prefix='sirius-enum-construction-') as directory:
        folder = Path(directory)
        source = folder / 'bench.sirius'
        source.write_text(generate(args.count), encoding='utf-8')
        binaries = []
        for label, compiler in (('baseline', args.baseline), ('updated', args.updated)):
            c_file, binary = folder / (label + '.c'), executable_path(folder / label)
            checked([str(compiler.resolve()), str(source), str(c_file)])
            checked([args.cc, '-std=c11', '-O2', '-w', '-I', str(ROOT / 'runtime'),
                     str(c_file), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(args.cc), '-o', str(binary)])
            binaries.append(binary)
        samples = [[], []]
        for repeat in range(args.runs + 2):
            for index in (repeat % 2, (repeat + 1) % 2):
                start = time.perf_counter()
                result = subprocess.run([str(binaries[index])], capture_output=True)
                elapsed = time.perf_counter() - start
                if result.returncode != expected or result.stdout or result.stderr:
                    raise AssertionError(f'{binaries[index].name} produced {result!r}; expected exit {expected}')
                if repeat >= 2:
                    samples[index].append(elapsed)
        for label, sample in zip(('baseline', 'updated'), samples):
            print(f'{label}: {statistics.median(sample) * 1000:.3f} ms median ({len(sample)} runs)')
        print(f'updated / baseline: {statistics.median(samples[1]) / statistics.median(samples[0]):.3f}x')
        print('Includes process startup and execution, excludes code generation and compilation.')


if __name__ == '__main__':
    main()
