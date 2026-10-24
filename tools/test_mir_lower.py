#!/usr/bin/env python
"""Lower checked HIR into scalar MIR and execute it with the Sirius interpreter."""
import argparse

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = execute([str(cache.native_driver('mir-lower', ROOT / 'tests/mir/lowering_driver_native.sirius'))]).stdout
	assert output == 'PASS typed HIR scalar CFG lowering' + '\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
