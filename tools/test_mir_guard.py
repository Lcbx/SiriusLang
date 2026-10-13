#!/usr/bin/env python
"""Validate MIR trap guards, cleanup, and interpreter/native parity."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-guard', MIR_DRIVER_SOURCES + ('tests/mir/guard_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'guard{i}.c' for i in range(10)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR explicit runtime guards\n':
			raise AssertionError(result)
		for path, code, error in zip(outputs, (26, None, None, 7, 12, None, 15, None, 17, None),
			(None, b'division by zero', b'integer overflow', None, None, b'array index out of bounds', None, b'array index out of bounds', None, b'missing optional value')):
			check_native(cache, path, code=code, stderr_contains=error)
	print('PASS MIR trap guards and native/interpreter parity')


if __name__ == '__main__':
	main()
