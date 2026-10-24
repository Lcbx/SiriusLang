#!/usr/bin/env python
"""Verify shared MIR semantics independently of the two execution backends."""
import argparse

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = execute([str(cache.native_driver('mir-common', ROOT / 'tests/mir/common_driver_native.sirius'))]).stdout
	assert output == 'PASS MIR backend common semantics' + '\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
