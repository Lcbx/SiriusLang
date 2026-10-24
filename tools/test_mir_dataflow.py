#!/usr/bin/env python
"""Check dense CFG liveness/ownership through a branching, cyclic program."""
from project_paths import ROOT
from build_support import BuildCache, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	output = execute([str(cache.native_driver('mir-dataflow', ROOT / 'tests/mir/dataflow_driver_native.sirius'))]).stdout
	assert output == 'PASS MIR dense worklist CFG\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
