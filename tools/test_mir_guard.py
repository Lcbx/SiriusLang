#!/usr/bin/env python
"""Validate MIR trap guards, cleanup, and interpreter/native parity."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-guard', ROOT / 'tests/mir/guard_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'guard{i}.c' for i in range(16)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR explicit runtime guards\n':
			raise AssertionError(result)
		for path, code, error in zip(outputs, (26, None, None, 7, 12, None, 15, None, 17, None, 69, 9, None, None, None, None),
			(None, b'division by zero', b'integer overflow', None, None, b'array index out of bounds', None, b'array index out of bounds', None, b'missing optional value', None, None, b'shift count out of range', b'shift count out of range', b'shift count out of range', b'shift count out of range')):
			check_native(cache, path, code=code, stderr_contains=error)
	print('PASS MIR trap guards and native/interpreter parity')


if __name__ == '__main__':
	main()
