#!/usr/bin/env python3
"""Benchmark opt-in MIR-driven native union coallocation against the same emitted C.

Production representation remains the default. The C flag enables the experiment.
The program is compiled by the Sirius compiler, with no C text rewriting.
"""
import argparse
import shutil
import statistics
import subprocess
import tempfile
import time
from pathlib import Path

from benchmark_union_fusion import source, cow_source
from build_support import BuildCache, link_flags
from project_paths import ROOT, executable_path


def execute(args: list[str], expected: int = 0) -> None:
    result = subprocess.run(args, capture_output=True, text=True)
    if result.returncode != expected or result.stdout or result.stderr:
        raise RuntimeError(f'{args!r}: expected {expected}, exit={result.returncode}, '
                           f'stdout={result.stdout!r}, stderr={result.stderr!r}')


def compile_native(cc: str, code: Path, path: Path, *, fused: bool, audit: bool = False,
                   sanitize: bool = False) -> None:
    flags = ['-std=c11', '-O1' if sanitize else '-O2', '-w']
    if fused:
        flags.append('-DSIRIUS_EXPERIMENTAL_UNION_COALLOC')
    if audit:
        flags.append('-DSIRIUS_ARC_AUDIT')
    if sanitize:
        flags.extend(['-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-g'])
    execute([cc, *flags, '-I', str(ROOT / 'runtime'), str(code),
             str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cc), '-o', str(path)])


def check_case(compiler: Path, cc: str, code: str, expected: int, label: str,
               *, runs: int, sanitize: bool = False) -> None:
    with tempfile.TemporaryDirectory(prefix='sirius-coalloc-compiler-') as tmp:
        folder = Path(tmp)
        src = folder / 'main.sirius'
        src.write_text(code, encoding='utf-8')
        emitted = folder / 'main.c'
        execute([str(compiler), str(src), str(emitted)])
        text = emitted.read_text(encoding='utf-8')
        sites = text.count('record = s_mir_record_new_fused(')
        if sites == 0 and 'memmove(variant->fields + 1' not in text and 'value = {0}; value.tag = ' not in text:
            raise AssertionError('expected coallocation or default scalar union allocation reuse')
        if sites == 0:
            print(f'{label}: scalar unions already inline; coallocation flag has no effect')
        binaries = {}
        for fused in (False, True):
            key = 'coallocated' if fused else 'current'
            exe = executable_path(folder / key)
            compile_native(cc, emitted, exe, fused=fused, audit=(runs == 0), sanitize=sanitize and runs == 0)
            execute([str(exe)], expected)
            if runs:
                # Keep ARC auditing and sanitizers out of performance samples.
                compile_native(cc, emitted, exe, fused=fused)
                execute([str(exe)], expected)
                binaries[key] = exe
        if runs:
            timings = {'current': [], 'coallocated': []}
            for iteration in range(runs + 2):
                order = ('current', 'coallocated') if iteration % 2 else ('coallocated', 'current')
                for key in order:
                    start = time.perf_counter()
                    execute([str(binaries[key])], expected)
                    if iteration >= 2:
                        timings[key].append((time.perf_counter() - start) * 1000)
            a = statistics.median(timings['current'])
            b = statistics.median(timings['coallocated'])
            print(f'{label}: sites={sites}, current={a:.3f} ms, coallocated={b:.3f} ms, ratio={b/a:.3f}x')
        else:
            print(f'{label}: sites={sites}, native normal/coallocated ARC + sanitizers PASS')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='clang')
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--count', type=int, default=80000)
    parser.add_argument('--runs', type=int, default=7)
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    if not shutil.which(args.cc):
        parser.error('requested C compiler unavailable')
    if args.count < 1 or args.runs < 1:
        parser.error('count and runs must be positive')
    args.cc = shutil.which(args.cc)
    compiler = args.compiler.resolve() if args.compiler else BuildCache(args.cc).seed()
    check_case(compiler, args.cc, cow_source(), 18, 'CoW/managed', runs=0, sanitize=args.sanitize)
    for managed in (False, True):
        count = min(128, args.count)
        check_case(compiler, args.cc, source(count, managed), count * (count - 1) // 2 % 251,
                   'scalar ARC' if not managed else 'managed ARC', runs=0, sanitize=args.sanitize)
        n = args.count
        check_case(compiler, args.cc, source(n, managed), n * (n - 1) // 2 % 251,
                   'scalar' if not managed else 'managed', runs=args.runs)


if __name__ == '__main__':
    main()
