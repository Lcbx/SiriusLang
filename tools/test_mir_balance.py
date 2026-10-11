#!/usr/bin/env python
"""Check CFG ownership balance and conditional releases in both MIR backends."""
import tempfile
from pathlib import Path

from test_build import BuildCache, check_native, execute, require_cc
from sirius_sources import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-balance', MIR_DRIVER_SOURCES + (
		'tests/mir/balance_driver.sirius',
	))
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(6)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR CFG ownership balance\n':
			raise AssertionError(result)
		for i, source in enumerate(outputs):
			check_native(cache, source, code=7 if i < 2 else (11 if i < 4 else 13))
	print('PASS MIR may-ownership, CFG cleanup, and native/interpreter parity')


if __name__ == '__main__':
	main()
