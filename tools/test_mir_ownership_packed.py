#!/usr/bin/env python
"""Exercise packed MIR ownership and i64 bitwise operations."""
from build_support import BuildCache, check_sirius_driver, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	output = check_sirius_driver(cache, 'mir-ownership-packed', MIR_DRIVER_SOURCES + (
		'tests/mir/packed_ownership_driver.sirius',
	), 'PASS packed MIR ownership\n')
	print(output, end='')


if __name__ == '__main__':
	main()
