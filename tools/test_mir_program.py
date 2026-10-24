#!/usr/bin/env python
"""Test HIR specialization worklist, call-aware MIR lowering and execution."""
import argparse

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = execute([str(cache.native_driver('mir-program', ROOT / 'tests/mir/program_driver_native.sirius'))]).stdout
	assert output == 'PASS HIR specialization worklist and MIR calls' + '\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
