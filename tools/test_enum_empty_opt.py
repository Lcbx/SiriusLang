#!/usr/bin/env python3
"""Check last-use empty enum record elimination and both backends."""
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, execute, link_flags, require_cc
from project_paths import ROOT


def main() -> None:
    cache = BuildCache(require_cc())
    driver = cache.native_driver('enum-empty-opt', ROOT / 'tests/mir/enum_empty_opt_driver_native.sirius')
    with tempfile.TemporaryDirectory(prefix='sirius-enum-empty-') as directory:
        source = Path(directory) / 'enum_empty.c'
        result = execute([str(driver), str(source)])
        assert result.stdout == 'PASS empty enum payload elimination, safe fallback, serialization, native/MIR\n', result.stdout
        check_native(cache, source, code=0)
        audited = Path(directory) / 'enum_empty_arc'
        extra = ('-Wno-unused-function',) if 'clang' in Path(cache.cc).name else ()
        execute([cache.cc, *C_FLAGS, *extra, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 str(source), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc),
                 '-o', str(audited)])
        result = subprocess.run([str(audited)], capture_output=True)
        assert (result.returncode, result.stdout, result.stderr) == (0, b'', b''), result
    print('PASS enum empty-record elimination: MIR verifier, interpreter, native, ARC')


if __name__ == '__main__':
    main()
