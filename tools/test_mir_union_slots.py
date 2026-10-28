#!/usr/bin/env python3
"""Direct scalar union array construction/matching: allocations, snapshots and ARC."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, executable_path


SOURCE = '''rec One(value: i64)
rec Alias(value: i64)
rec Mixed(value: i64, weight: f64, valid: bool)
rec Empty()
rec Managed(text: string)
let Choice = One | Alias | Mixed | Empty | Managed

def main() -> i64
    var xs = Array[Choice]()
    for i in 0..COUNT
        if i % 3 == 0
            xs = xs.append(One(i))
        elif i % 3 == 1
            xs = xs.append(Mixed(i, 2.5, true))
        else
            xs = xs.append(Alias(i))
    var total = 0
    for value in xs
        match value
            One(n)
                total += n
            Alias(n)
                total += n
            Mixed(n, weight, valid)
                if not valid or weight != 2.5
                    return 100
                total += n
            Empty()
                return 101
            Managed(text)
                return 102
    return total % 251
'''

PROBE = '''static size_t slot_allocations;
static void *slot_calloc(size_t count, size_t size) {
    ++slot_allocations;
    return calloc(count, size);
}
#define calloc slot_calloc
'''


def run(command, code=0, stderr=b''):
    result = subprocess.run([str(arg) for arg in command], capture_output=True, timeout=180)
    assert (result.returncode, result.stdout, result.stderr.replace(b'\r\n', b'\n')) == (code, b'', stderr), (command, result)


def build(cache, generated, binary, defines):
    run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', *defines,
         '-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
         *link_flags(cache.cc), '-o', binary])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    checked = cache.native_driver('union-slots-checked', ROOT / 'tests/mir/union_slots_checked_native.sirius')
    modes = [('default', []), ('hybrid', ['-DSIRIUS_EXPERIMENTAL_UNION_HYBRID']),
             ('coalloc', ['-DSIRIUS_EXPERIMENTAL_UNION_COALLOC'])]
    with tempfile.TemporaryDirectory() as temp:
        work = Path(temp)
        for emitter in (compiler, checked):
            fixture = ROOT / 'tests/mir/source_union_array_slots.sirius'
            generated = work / 'snapshots.c'
            run([emitter, fixture, generated])
            for label, defines in modes:
                binary = executable_path(work / label)
                build(cache, generated, binary, defines)
                run([binary])
            for count in (8, 256):
                source = work / 'scalar.sirius'
                source.write_text(SOURCE.replace('COUNT', str(count)).replace('    ', '\t'))
                run([emitter, source, generated])
                text = generated.read_text()
                anchor = 'static inline S_MirRecord *s_mir_record_new(int64_t id, size_t fields) {'
                assert text.count(anchor) == 1
                text = text.replace(anchor, PROBE + anchor)
                entry = 'int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_mir_f0(); }'
                assert text.count(entry) == 1
                text = text.replace(entry, 'int main(int argc, char **argv) { s_set_arguments(argc, argv); '
                                    'int result = (int)s_mir_f0(); fprintf(stderr, "%zu\\n", slot_allocations); return result; }')
                generated.write_text(text)
                for label, defines in modes:
                    binary = executable_path(work / label)
                    build(cache, generated, binary, defines)
                    # Only the array buffer allocates: zero union or payload
                    # allocations for either construction or matching.
                    run([binary], count * (count - 1) // 2 % 251, b'1\n')
                run([runner, source], count * (count - 1) // 2 % 251)
            invalid = work / 'bounds.sirius'
            invalid.write_text(fixture.read_text().replace('let first = xs[!-1]', 'let first = xs[!99]'))
            run([emitter, invalid, generated])
            binary = executable_path(work / 'bounds')
            build(cache, generated, binary, [])
            failed = subprocess.run([str(binary)], capture_output=True, timeout=180)
            assert failed.returncode != 0 and b'array index out of bounds' in failed.stderr, failed
        run([runner, fixture])
    print('PASS union slots: one buffer allocation at 8/256 elements, snapshots, boxed ARC, checked/verified, native/MIR')


if __name__ == '__main__':
    main()
