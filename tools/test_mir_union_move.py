#!/usr/bin/env python
"""Check MIR union payload transfer, ownership and native/interpreter parity."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
    cache = BuildCache(require_cc())
    driver = cache.native_driver('mir-union-move', ROOT / 'tests/mir/union_move_driver_native.sirius')
    with tempfile.TemporaryDirectory() as tmp:
        paths = [Path(tmp) / name for name in ('moved.c', 'shared.c', 'baseline.c')]
        result = execute([str(driver), *(str(path) for path in paths)])
        if result.stdout != 'PASS union last-use transfer\n':
            raise AssertionError(result.stdout)
        for source in paths:
            check_native(cache, source, code=5)
    print('PASS union payload moves: optimized MIR, interpreter, native C')


if __name__ == '__main__':
    main()
