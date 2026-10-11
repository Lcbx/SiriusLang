#!/usr/bin/env python
"""Test the specialization worklist without a MIR or C-emitter dependency."""
import argparse

from test_build import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	binary = cache.sirius('hir-worklist', (
		'src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius',
		'src/hir.sirius', 'src/hir_specialize.sirius',
		'tests/types/worklist_driver.sirius',
	))
	output = execute([str(binary)]).stdout
	if output != 'PASS HIR worklist without backend\n':
		raise AssertionError(f'Unexpected worklist output: {output!r}')
	print(output, end='')


if __name__ == '__main__':
	main()
