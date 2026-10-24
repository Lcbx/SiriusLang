#!/usr/bin/env python
"""Check CFG-aware managed slot reuse, cleanup and native parity."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-managed-slots', ROOT / 'tests/mir/managed_slots_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(12)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS managed MIR slot reuse\n':
			raise AssertionError(result.stdout)
		for i, source in enumerate(outputs):
			check_native(cache, source, code=(5, 5, 7, 5, 2, 4, 9, 9, 6, 6, 3, 7)[i])
	print('PASS managed slots: CFG reuse, conditional cleanup, move hazards, joins, loops and interpreter/native parity')


if __name__ == '__main__':
	main()
