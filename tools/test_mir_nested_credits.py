#!/usr/bin/env python3
"""Check type-directed nested credit contracts and borrowed/moved call frames."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-nested-credits', ROOT / 'tests/mir/nested_credit_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(4)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		assert result.stdout == 'PASS nested credit contracts and frame transfers\n', result.stdout
		for i, source in enumerate(outputs):
			check_native(cache, source, code=2 if i < 2 else 1)
	print('PASS nested heap ownership and borrowed/moved call frames')


if __name__ == '__main__':
	main()
