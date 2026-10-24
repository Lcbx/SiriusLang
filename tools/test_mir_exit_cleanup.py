#!/usr/bin/env python
"""Check path-sensitive MIR cleanup and native/interpreter parity."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-exit-cleanup', ROOT / 'tests/mir/exit_cleanup_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(4)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.returncode != 0 or result.stdout != 'PASS MIR path-sensitive exit cleanup\n':
			raise AssertionError(result)
		for source, expected in zip(outputs, (3, 9, 4, 7)):
			check_native(cache, source, code=expected)
	print('PASS MIR CFG exit cleanup, idempotency and interpreter/native execution')


if __name__ == '__main__':
	main()
