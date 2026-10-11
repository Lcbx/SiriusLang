#!/usr/bin/env python
"""Test HIR specialization worklist, call-aware MIR lowering and execution."""
import argparse

from test_build import BuildCache, check_sirius_driver, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = check_sirius_driver(cache, 'mir-program', (
		'src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius',
		'src/hir.sirius', 'src/hir_specialize.sirius', 'src/mir.sirius', 'src/mir_interpreter.sirius',
		'src/mir_lower.sirius', 'src/mir_lifetime.sirius', 'src/mir_program.sirius',
		'tests/mir/program_driver.sirius',
	), 'PASS HIR specialization worklist and MIR calls\n')
	print(output, end='')


if __name__ == '__main__':
	main()
