#!/usr/bin/env python
"""Test the specialization worklist without a MIR or C-emitter dependency."""
import argparse

from build_support import BuildCache, check_sirius_driver, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = check_sirius_driver(cache, 'hir-worklist', (
		'src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius',
		'src/hir.sirius', 'src/hir_resolve.sirius', 'src/hir_constants.sirius', 'src/hir_declarations.sirius', 'src/hir_analyze.sirius', 'src/hir_specialize.sirius',
		'tests/types/worklist_driver.sirius',
	), 'PASS HIR worklist without backend\n')
	print(output, end='')


if __name__ == '__main__':
	main()
