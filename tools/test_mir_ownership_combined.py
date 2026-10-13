#!/usr/bin/env python
"""Compare joint must/may propagation with the standalone ownership solvers."""
from test_build import BuildCache, check_sirius_driver, require_cc
from sirius_sources import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	output = check_sirius_driver(cache, 'mir-ownership-combined', MIR_DRIVER_SOURCES + (
		'tests/mir/ownership_combined_driver.sirius',
	), 'PASS MIR combined forward ownership\n')
	print(output, end='')


if __name__ == '__main__':
	main()
