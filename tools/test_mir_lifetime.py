#!/usr/bin/env python
"""Test managed-register liveness over branches, loops and nested values."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-lifetime', ROOT / 'tests/mir/lifetime_driver_native.sirius')
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
