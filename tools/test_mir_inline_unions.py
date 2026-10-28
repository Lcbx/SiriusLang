#!/usr/bin/env python3
"""Hybrid scalar unions in arrays and other value contexts: CoW, hashing and ARC."""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT


def run(command, expected=0, timeout=120):
    result = subprocess.run(command, capture_output=True, timeout=timeout)
    assert result.returncode == expected and result.stdout == b'' and result.stderr == b'', (
        command, result.returncode, result.stdout, result.stderr.decode(errors='replace'))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    parser.add_argument('--native-only', action='store_true')
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = None if args.native_only else (args.runner.resolve() if args.runner else cache.native_tool('run_cli'))
    sources = [
        ROOT / 'tests/mir/source_inline_union_array.sirius',
        ROOT / 'tests/mir/source_inline_union_values.sirius',
        ROOT / 'tests/mir/source_union_labeled_collections.sirius',
        ROOT / 'tests/mir/source_union_labeled_collections_managed.sirius',
        ROOT / 'tests/mir/source_union_explicit_construction.sirius',
    ]
    expected = [0, 0, 107, 52, 58]
    with tempfile.TemporaryDirectory() as tmp:
        for source, code in zip(sources, expected):
            output = Path(tmp) / (source.stem + '.c')
            run([str(compiler), str(source), str(output)])
            generated = output.read_text()
            if source.stem == 'source_inline_union_array':
                assert re.search(r'S_MirArrayLayout\)\{\d+, sizeof\(S_MirArrayUnion\d+\), S_MIR_ELEMENT_UNION\}', generated)
                assert 's_mir_array_union_pack' in generated
                assert 's_mir_array_union_read' in generated
                assert 'slot.tag = -1; slot.payload.boxed = owned' in generated
                assert 's_mir_array_element_drop' in generated
            for variant, defines in [('default', []), ('hybrid', ['-DSIRIUS_EXPERIMENTAL_UNION_HYBRID']), ('coalloc', ['-DSIRIUS_EXPERIMENTAL_UNION_COALLOC'])]:
                binary = Path(tmp) / (source.stem + '-' + variant)
                cmd = [cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', *defines]
                if args.sanitize:
                    cmd += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
                cmd += ['-I', str(ROOT / 'runtime'), str(output), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)]
                run(cmd)
                run([str(binary)], expected=code)
            if runner is not None:
                run([str(runner), str(source)], expected=code)
    print('PASS hybrid union values and arrays: scalar tags, boxed fallback, CoW, hashing, ARC, optional' +
          (', native/MIR parity' if runner is not None else ', native only'))


if __name__ == '__main__':
    main()
