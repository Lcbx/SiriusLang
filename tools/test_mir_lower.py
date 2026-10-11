#!/usr/bin/env python
"""Lower checked HIR into scalar MIR and execute it with the Sirius interpreter."""
import argparse

from test_build import BuildCache, check_sirius_driver, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = check_sirius_driver(cache, 'mir-lower', (
		'src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius',
		'src/hir.sirius', 'src/mir.sirius', 'src/mir_interpreter.sirius',
		'src/mir_lower.sirius', 'tests/mir/lowering_driver.sirius',
	), 'PASS typed HIR scalar CFG lowering\n')
	print(output, end='')


if __name__ == '__main__':
	main()
