#!/usr/bin/env python
"""Compile and run the MIR interpreter written in Sirius against hand-built MIR."""
import argparse

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = execute([str(cache.native_driver('mir', ROOT / 'tests/mir/driver_native.sirius'))]).stdout
	assert output == '42\nPASS Sirius MIR interpreter' + '\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
