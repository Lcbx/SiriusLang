#!/usr/bin/env python
"""Verify fused record-array append in MIR, interpreter, and native C."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-record-array-append', MIR_DRIVER_SOURCES + ('tests/mir/record_array_append_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		generated = Path(directory) / 'appended.c'
		result = execute([str(driver), str(generated)])
		if result.stdout != 'PASS MIR fused record array append\n':
			raise AssertionError(result.stdout)
		check_native(cache, generated, code=1)
		print('PASS MIR record array append: verifier, interpreter, snapshot CoW, ownership and native C')


if __name__ == '__main__':
	main()
