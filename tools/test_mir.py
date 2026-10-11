#!/usr/bin/env python
"""Compile and run the MIR interpreter written in Sirius against hand-built MIR."""
import argparse

from test_build import BuildCache, check_sirius_driver, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = check_sirius_driver(cache, 'mir', (
		'src/types.sirius',
		'src/mir.sirius',
		'src/mir_interpreter.sirius',
		'tests/mir/driver.sirius',
	), '42\nPASS Sirius MIR interpreter\n')
	print(output, end='')


if __name__ == '__main__':
	main()
