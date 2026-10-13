#!/usr/bin/env python
"""Verify MIR definite initialization and consumption across the CFG."""

from build_support import BuildCache, check_sirius_driver, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	output = check_sirius_driver(cache, 'mir-ownership', MIR_DRIVER_SOURCES + (
		'tests/mir/ownership_driver.sirius',
	), 'PASS MIR forward ownership checks\n')
	print(output, end='')


if __name__ == '__main__':
	main()
