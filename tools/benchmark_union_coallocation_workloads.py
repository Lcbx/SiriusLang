#!/usr/bin/env python3
"""Compare opt-in MIR union coallocation on non-construction-dominated Sirius programs.

Compile each Sirius source once, then compile the same generated C with and
without SIRIUS_EXPERIMENTAL_UNION_COALLOC. Source-to-C, C compilation, and
native execution are reported separately. Small cases use ARC auditing.
The interpreter and compiler fixtures are tested by their existing drivers.
"""
import argparse
from pathlib import Path
import shutil
import statistics
import subprocess
import tempfile
import time

from benchmark_union_compiler_coallocation import compile_native
from build_support import BuildCache
from project_paths import executable_path


TYPES = '''rec Small(value: i64)
rec Large(value: i64, label: string, samples: Array[i64])
let Variant = Small | Large
'''


def source(case: str, count: int, repeats: int) -> tuple[str, int]:
    if case == 'traverse':
        body = f'''\tvar values = Array[Variant]()
\tfor i in 0..{count}
\t\tif i % 2 == 0
\t\t\tvalues = values.append(Small(i))
\t\telse
\t\t\tvalues = values.append(Large(i, "abc", [i, i + 1]))
\tvar total = 0
\tfor iteration in 0..{repeats}
\t\tfor item in values
\t\t\tmatch item
\t\t\t\tSmall(value)
\t\t\t\t\ttotal += value
\t\t\t\tLarge(value, label, samples)
\t\t\t\t\ttotal += value
\treturn total % 251
'''
        expected = (count * (count - 1) // 2 * repeats) % 251
    elif case == 'cow':
        body = f'''\tvar values = Array[Variant]()
\tfor i in 0..{count}
\t\tif i % 2 == 0
\t\t\tvalues = values.append(Small(i))
\t\telse
\t\t\tvalues = values.append(Large(i, "abc", [i]))
\tvar total = 0
\tfor k in 0..{repeats}
\t\tlet snapshot = values
\t\tvalues = values.append(Small(k))
\t\ttotal += len(snapshot)
\treturn total % 251
'''
        expected = (repeats * count + repeats * (repeats - 1) // 2) % 251
    elif case == 'escaped':
        body = f'''\tvar saved = Array[Array[i64]]()
\tfor i in 0..{count}
\t\tlet value: Variant = Large(i, "abc", [i, i + 1])
\t\tlet extracted = get_samples(value)
\t\tsaved = saved.append(extracted)
\tvar total = 0
\tfor iteration in 0..{repeats}
\t\tfor samples in saved
\t\t\ttotal += samples[!1]
\treturn total % 251
'''
        prefix = '''def get_samples(item: Variant) -> Array[i64]
\tmatch item
\t\tSmall(value)
\t\t\treturn [value]
\t\tLarge(value, label, samples)
\t\t\treturn samples
'''
        expected = ((count * (count + 1) // 2) * repeats) % 251
        return TYPES + prefix + '\ndef main() -> i64\n' + body, expected
    elif case == 'control':
        # No tagged unions anywhere in this program. Measures the cost of
        # enlarging the record header for *all* records under the flag.
        program = f'''rec Entry(value: i64, label: string, samples: Array[i64])

def main() -> i64
\tvar values = Array[Entry]()
\tfor i in 0..{count}
\t\tvalues = values.append(Entry(i, "abc", [i, i + 1]))
\tvar total = 0
\tfor iteration in 0..{repeats}
\t\tfor item in values
\t\t\ttotal += item.value
\treturn total % 251
'''
        return program, (count * (count - 1) // 2 * repeats) % 251
    else:
        raise ValueError(case)
    return TYPES + '\ndef main() -> i64\n' + body, expected


def run(cmd: list[str], expected: int = 0) -> float:
    start = time.perf_counter()
    result = subprocess.run(cmd, capture_output=True)
    duration = (time.perf_counter() - start) * 1000
    if (result.returncode, result.stdout, result.stderr) != (expected, b'', b''):
        raise AssertionError(f'command {cmd!r}: expected {expected}, got '
                             f'{result.returncode}, {result.stdout!r}, {result.stderr!r}')
    return duration


def evaluate(compiler: Path, cc: str, case: str, count: int, repeats: int,
             runs: int, audit_count: int, sanitize: bool = False) -> dict[str, float]:
    with tempfile.TemporaryDirectory(prefix='sirius-coalloc-matrix-') as tmp:
        folder = Path(tmp)
        text, expected = source(case, count, repeats)
        sirius = folder / 'program.sirius'
        generated = folder / 'program.c'
        sirius.write_text(text, encoding='utf-8')
        src_ms = run([str(compiler), str(sirius), str(generated)])
        sites = generated.read_text(encoding='utf-8').count('record = s_mir_record_new_fused(')
        if (case != 'control' and sites < 1) or (case == 'control' and sites != 0):
            raise AssertionError(f'{case}: unexpected fused constructor sites: {sites}')

        # Independently validate ownership on a smaller, bounded program.
        # Audit overhead is quadratic for large graphs, so never benchmark it.
        small, small_expected = source(case, audit_count, min(3, repeats))
        audit_src = folder / 'audit.sirius'
        audit_c = folder / 'audit.c'
        audit_src.write_text(small, encoding='utf-8')
        run([str(compiler), str(audit_src), str(audit_c)])
        for fused in (False, True):
            binary = executable_path(folder / ('audit_fused' if fused else 'audit_original'))
            compile_native(cc, audit_c, binary, fused=fused, audit=True, sanitize=sanitize)
            run([str(binary)], small_expected)

        binaries = {}
        compile_ms = {}
        for fused in (False, True):
            key = 'fused' if fused else 'original'
            binary = executable_path(folder / key)
            t = time.perf_counter()
            compile_native(cc, generated, binary, fused=fused)
            compile_ms[key] = (time.perf_counter() - t) * 1000
            run([str(binary)], expected)
            binaries[key] = binary
        times = {'original': [], 'fused': []}
        for iteration in range(runs + 2):
            for key in (('original', 'fused') if iteration % 2 else ('fused', 'original')):
                duration = run([str(binaries[key])], expected)
                if iteration >= 2:
                    times[key].append(duration)
        original = statistics.median(times['original'])
        fused = statistics.median(times['fused'])
        variation = {k: (max(v) - min(v)) / statistics.median(v) for k, v in times.items()}
        result = {'source_ms': src_ms, 'c_original_ms': compile_ms['original'],
                  'c_fused_ms': compile_ms['fused'], 'original_ms': original,
                  'fused_ms': fused, 'ratio': fused / original, 'sites': sites, 'spread_original': variation['original'],
                  'spread_fused': variation['fused']}
        print(f'{case:9s} sites={sites} source->C={src_ms:.1f}ms C build='
              f'{compile_ms["original"]:.1f}/{compile_ms["fused"]:.1f}ms '
              f'native={original:.3f}/{fused:.3f}ms ratio={fused/original:.3f}x '
              f' spread={variation["original"]:.1%}/{variation["fused"]:.1%}'
              ' (baseline/fused)', flush=True)
        return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='clang')
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--count', type=int, default=60000)
    parser.add_argument('--repeats', type=int, default=64)
    parser.add_argument('--runs', type=int, default=9)
    parser.add_argument('--audit-count', type=int, default=32)
    parser.add_argument('--sanitize', action='store_true',
                        help='run Clang/GCC ASan+UBSan during small ARC correctness checks')
    parser.add_argument('--case', choices=('all', 'traverse', 'cow', 'escaped', 'control'), default='all')
    args = parser.parse_args()
    cc = shutil.which(args.cc)
    if not cc:
        parser.error(f'compiler not found: {args.cc}')
    if min(args.count, args.repeats, args.runs, args.audit_count) < 1:
        parser.error('all counts must be positive')
    compiler = args.compiler.resolve() if args.compiler else BuildCache(cc).seed()
    if not compiler.exists():
        parser.error('Sirius compiler not found')
    cases = ('traverse', 'cow', 'escaped', 'control') if args.case == 'all' else (args.case,)
    for case in cases:
        evaluate(compiler, cc, case, args.count, args.repeats, args.runs,
                 args.audit_count, sanitize=args.sanitize)


if __name__ == '__main__':
    main()
