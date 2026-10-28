#!/usr/bin/env python3
"""Verify default scalar union storage and optional boxed fallback coallocation.

Scalar payloads inline in wrappers and array slots without an experimental flag.
Large and managed payloads retain independent ownership and can coallocate under
SIRIUS_EXPERIMENTAL_UNION_COALLOC. Count generated-runtime allocations with ARC.
"""
import argparse
import re
import shutil
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, link_flags
from project_paths import ROOT, executable_path


SOURCE = '''rec Small(value: i64)
rec Big(value: i64, a: i64, b: i64, c: i64, d: i64, e: i64)
rec Managed(value: i64, title: string, values: Array[i64])
let Choice = Small | Big | Managed

def main() -> i64
    var xs = Array[Choice]()
    for i in 0..256
        if i % 128 == 0
            xs = xs.append(Managed(i, "title", [i, i + 1]))
        elif i % 64 == 0
            xs = xs.append(Big(i, i+1, i+2, i+3, i+4, i+5))
        else
            xs = xs.append(Small(i))
    let snapshot = xs
    xs = xs.append(Small(27))
    var total = 0
    for x in snapshot
        match x
            Small(value)
                total += value
            Big(value, a, b, c, d, e)
                total += value
            Managed(value, title, values)
                total += value + len(values) - 2
    return total % 251
'''

# Instrument generated record and fused slab allocations, without altering
# the runtime library's allocations. No source-to-C semantic rewriting occurs.
PROBE = '''static size_t hybrid_probe_allocations = 0;
static void *hybrid_probe_calloc(size_t n, size_t size) {
    ++hybrid_probe_allocations;
    return calloc(n, size);
}
#define calloc hybrid_probe_calloc
'''


def execute(args: list[str], expected: int | None = 0) -> subprocess.CompletedProcess[str]:
    run = subprocess.run(args, capture_output=True, text=True)
    if expected is not None and run.returncode != expected:
        raise AssertionError(f'{args!r}: exit {run.returncode}, stdout={run.stdout!r}, stderr={run.stderr!r}')
    return run


def build(cc: str, c_file: Path, output: Path, flag: str, sanitize: bool) -> None:
    flags = ['-std=c11', '-O1' if sanitize else '-O2', '-w', '-DSIRIUS_ARC_AUDIT']
    if flag:
        flags.append(flag)
    if sanitize:
        flags += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-g']
    execute([cc, *flags, '-I', str(ROOT / 'runtime'), str(c_file),
             str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cc), '-o', str(output)])


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--compiler', type=Path)
    ap.add_argument('--cc', default=shutil.which('clang') or shutil.which('cc') or 'cc')
    ap.add_argument('--sanitize', action='store_true')
    args = ap.parse_args()
    compiler = args.compiler.resolve() if args.compiler else BuildCache(args.cc).native_tool('modular_cli')
    with tempfile.TemporaryDirectory(prefix='sirius-hybrid-') as tmp:
        folder = Path(tmp)
        src, c_file = folder / 'test.sirius', folder / 'test.c'
        src.write_text(SOURCE.replace('    ', '\t'), encoding='utf-8')
        execute([str(compiler), str(src), str(c_file)])
        emitted = c_file.read_text(encoding='utf-8')
        # Scalar alternatives now inline in the wrapper by default. Only
        # the two boxed fallback sites remain eligible for full coallocation.
        branches = re.findall(r'(#(?:if|ifdef)[^\n]+)\n\s*record = s_mir_record_new_fused\(', emitted)
        assert len(branches) == 2 and all('HYBRID' not in b for b in branches), branches
        anchor = 'static inline S_MirRecord *s_mir_record_new(int64_t id, size_t fields) {'
        assert emitted.count(anchor) == 1
        emitted = emitted.replace(anchor, PROBE + anchor)
        entry = 'int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_mir_f0(); }'
        assert emitted.count(entry) == 1
        emitted = emitted.replace(entry, 'int main(int argc, char **argv) { '
                                  's_set_arguments(argc, argv); int result=(int)s_mir_f0(); '
                                  'fprintf(stderr,"alloc=%zu\\n",hybrid_probe_allocations); return result; }')
        c_file.write_text(emitted, encoding='utf-8')
        counts = {}
        for name, macro in (('default', ''), ('hybrid', '-DSIRIUS_EXPERIMENTAL_UNION_HYBRID'),
                            ('all', '-DSIRIUS_EXPERIMENTAL_UNION_COALLOC')):
            exe = executable_path(folder / name)
            build(args.cc, c_file, exe, macro, args.sanitize)
            run = execute([str(exe)], 256 * 255 // 2 % 251)
            match = re.fullmatch(r'alloc=(\d+)\n', run.stderr)
            if not match or run.stdout:
                raise AssertionError((name, run.stdout, run.stderr))
            counts[name] = int(match.group(1))
        # Small payloads have no separate live allocation in any mode.
        # Full coallocation still saves the four boxed fallback allocations.
        # Two array buffers (including CoW), two managed payload/array/wrapper
        # triples, and two large payload/wrapper pairs. Scalar construction
        # and matching allocate nothing, down from the previous 769 calls.
        assert counts['default'] == 12, counts
        if counts['default'] != counts['hybrid'] or counts['hybrid'] - counts['all'] != 4:
            raise AssertionError(counts)
        print('PASS hybrid union: allocation selection, large/managed fallback, '
              f'CoW, native ARC{", ASan/UBSan" if args.sanitize else ""}: {counts}')


if __name__ == '__main__':
    main()
