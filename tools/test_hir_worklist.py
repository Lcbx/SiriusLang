#!/usr/bin/env python
"""Test the specialization worklist without a MIR or C-emitter dependency."""
import argparse

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = execute([str(cache.native_driver('hir-worklist', ROOT / 'tests/types/worklist_driver_native.sirius'))]).stdout
	assert output == 'PASS HIR worklist without backend' + '\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
