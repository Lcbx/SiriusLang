#!/usr/bin/env python3
"""Array stride/ownership descriptor: emitted ABI, native CoW/ARC and MIR parity."""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT


EXPECTED = 67


def check(command: list[str], code: int) -> None:
    result = subprocess.run(command, capture_output=True, text=True, timeout=120)
    if (result.returncode, result.stdout, result.stderr) != (code, '', ''):
        raise AssertionError((command, result.returncode, result.stdout, result.stderr))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    source = ROOT / 'tests/mir/source_array_stride_ownership.sirius'
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp)
        c = work / 'arrays.c'
        check([str(compiler), str(source), str(c)], 0)
        emitted = c.read_text(encoding='utf-8')
        # Noninline elements still use the full S_MirField ABI; eligible
        # scalar-record arrays instead use their own inline element widths.
        layouts = re.findall(r'case \d+: return \(S_MirArrayLayout\)\{\d+, sizeof\(S_MirField\), (S_MIR_ELEMENT_\w+)\};', emitted)
        for expected in ('S_MIR_ELEMENT_POD', 'S_MIR_ELEMENT_STRING', 'S_MIR_ELEMENT_OBJECT'):
            assert expected in layouts, (expected, layouts)
        for helper in ('s_mir_array_element_copy(', 's_mir_array_element_drop(',
                       's_mir_array_at(', 's_mir_record_stride('):
            assert helper in emitted, helper
        for label, defs in (('default', []), ('hybrid', ['-DSIRIUS_EXPERIMENTAL_UNION_HYBRID'])):
            output = work / label
            build = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                *defs, '-I', str(ROOT / 'runtime'), str(c), str(ROOT / 'runtime/sirius_runtime.c'),
                *link_flags(cache.cc), '-o', str(output)], capture_output=True, text=True, timeout=120)
            if build.returncode:
                raise AssertionError(build.stderr)
            check([str(output)], EXPECTED)
        check([str(runner), str(source)], EXPECTED)
    print('PASS array descriptor: scalar, string, boxed record, union, enum, nested CoW, ARC; native/MIR')


if __name__ == '__main__':
    main()
