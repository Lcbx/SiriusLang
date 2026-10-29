#!/usr/bin/env python3
"""Inline scalar-record array slots: native, ARC, CoW, and MIR parity."""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


def run(command, *, expected=0, output=b'', timeout=120):
    result = subprocess.run(command, capture_output=True, timeout=timeout)
    assert result.returncode == expected, (command, result.returncode, result.stderr.decode(errors='replace'))
    assert normalize_newlines(result.stdout) == output, (command, result.stdout, output)
    assert result.stderr == b'', (command, result.stderr)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--native-only', action='store_true', help='skip MIR parity when the runner has not yet been built')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = None if args.native_only else (args.runner.resolve() if args.runner else cache.mir_runner())
    sources = (
        ('tests/mir/source_inline_array_storage.sirius', b''),
        ('tests/mir/source_inline_array_allocations.sirius', b''),
        ('tests/language/fixtures/40_record_value_slots.sirius', (ROOT / 'tests/language/expected_output/40_record_value_slots.txt').read_bytes()),
        ('tests/language/fixtures/41_inline_scalar_record.sirius', (ROOT / 'tests/language/expected_output/41_inline_scalar_record.txt').read_bytes()),
    )
    with tempfile.TemporaryDirectory() as tmp:
        for source_path, expected_output in sources:
            source = ROOT / source_path
            name = source.stem
            c = Path(tmp) / (name + '.c')
            run([str(compiler), str(source), str(c)])
            emitted = c.read_text(encoding='utf-8')
            if name == 'source_inline_array_storage':
                assert re.search(r'return \(S_MirArrayLayout\)\{\d+, sizeof\(S_MirArrayInline\d+\), S_MIR_ELEMENT_INLINE\}', emitted)
                assert 'S_MIR_ELEMENT_OBJECT' in emitted
                assert 'memcpy(s_mir_array_at(array,' in emitted
                assert 's_mir_array_read' in emitted
                assert 's_mir_array_set_owned' in emitted
                assert 's_mir_array_append_owned' in emitted
            if name == 'source_inline_array_allocations':
                # The array element constructor must be copied inline; an
                # optional projection may still box, but this fixture has none.
                assert re.search(r'S_MirArrayInline\d+ added = \{0\}; added.value =', emitted)
                assert not re.search(r'added\.record = s_mir_inline_box', emitted)
            for variant, flags in (('default', []), ('hybrid', ['-DSIRIUS_EXPERIMENTAL_UNION_HYBRID'])):
                binary = Path(tmp) / (name + variant)
                cmd = [cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', *flags]
                if args.sanitize:
                    cmd += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
                cmd += ['-I', str(ROOT / 'runtime'), str(c), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)]
                run(cmd)
                run([str(binary)], output=normalize_newlines(expected_output))
            if runner is not None:
                run([str(runner), str(source)], output=normalize_newlines(expected_output))
    print('PASS inline arrays: widths 8/16/24/32, C ABI, ARC, CoW, optional, equality, hybrid' + (', native/MIR' if runner is not None else ', native only'))


if __name__ == '__main__':
    main()
