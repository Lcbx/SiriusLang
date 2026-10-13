#!/usr/bin/env python
"""Test managed-register liveness over branches, loops and nested values."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-lifetime', MIR_DRIVER_SOURCES + ('tests/mir/lifetime_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(5)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR CFG lifetimes\n':
			raise AssertionError(result.stdout)
		for source, expected in zip(outputs, (4, 4, 4, 1, 7)):
			check_native(cache, source, code=expected)
	print('PASS MIR CFG lifetime insertion, idempotency and interpreter/native execution')


if __name__ == '__main__':
	main()
