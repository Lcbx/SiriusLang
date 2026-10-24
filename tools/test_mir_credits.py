#!/usr/bin/env python3
"""Verify returned heap ownership credits and native CFG cleanup parity."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-credits', ROOT / 'tests/mir/credit_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(8)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		assert result.stdout == 'PASS MIR return heap credits\n', result.stdout
		for i, source in enumerate(outputs):
			check_native(cache, source, code=23 if i >= 5 else (3 if i == 2 else (2 if i >= 3 else 19)))
	print('PASS MIR ownership credits, CFG cleanup and native parity')


if __name__ == '__main__':
	main()
