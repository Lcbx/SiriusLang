#!/usr/bin/env python3
"""Check scalar enum storage alongside managed heterogeneous unions."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc
from project_paths import ROOT


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    source = ROOT / 'tests/mir/source_union_scalar_enum.sirius'
    with tempfile.TemporaryDirectory(prefix='sirius-scalar-enum-') as directory:
        folder = Path(directory)
        generated = folder / 'scalar_enum.c'
        result = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
        assert result.returncode == 0, result.stdout + result.stderr
        check_native(cache, generated, code=10)
        interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
        assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (10, b'', b''), interpreted
        executable = folder / 'arc_audit'
        built = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                                '-I', str(ROOT / 'runtime'), str(generated),
                                str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc),
                                '-o', str(executable)], capture_output=True, text=True)
        assert built.returncode == 0, built.stderr
        audit = subprocess.run([str(executable)], capture_output=True)
        assert (audit.returncode, audit.stdout, audit.stderr) == (10, b'', b''), audit
    print('PASS scalar enum tags: arrays, Dict, CoW, nested values, mixed unions, ARC, native/MIR')


if __name__ == '__main__':
    main()
