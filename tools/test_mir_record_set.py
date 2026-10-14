#!/usr/bin/env python
"""Check record-update MIR verification, interpreter, CoW, and native C."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-record-set', MIR_DRIVER_SOURCES + ('tests/mir/record_update_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		generated = Path(directory) / 'updated.c'
		result = execute([str(driver), str(generated)])
		if result.stdout != 'PASS MIR record update\n':
			raise AssertionError(result.stdout)
		check_native(cache, generated, code=10)
		print('PASS MIR record update: verifier, interpreter, snapshot CoW, ownership, and native C')


if __name__ == '__main__':
	main()
