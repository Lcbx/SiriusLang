#!/usr/bin/env python
"""Check dense CFG liveness/ownership through a branching, cyclic program."""
from build_support import BuildCache, check_sirius_driver, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	output = check_sirius_driver(cache, 'mir-dataflow', MIR_DRIVER_SOURCES + (
		'tests/mir/dataflow_driver.sirius',
	), 'PASS MIR dense worklist CFG\n')
	print(output, end='')


if __name__ == '__main__':
	main()
