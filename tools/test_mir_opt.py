#!/usr/bin/env python
"""Test optimized MIR copy ownership and verified-C cleanup semantics."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-opt', MIR_DRIVER_SOURCES + ('tests/mir/opt_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(3)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR owned-copy elimination\n':
			raise AssertionError(result.stdout)
		for source in outputs:
			check_native(cache, source, code=3)
	print('PASS optimized MIR ownership, interpreter and native C')


if __name__ == '__main__':
	main()
