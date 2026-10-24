#!/usr/bin/env python
"""Compare joint must/may propagation with the standalone ownership solvers."""
from project_paths import ROOT
from build_support import BuildCache, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	output = execute([str(cache.native_driver('mir-ownership-combined', ROOT / 'tests/mir/ownership_combined_driver_native.sirius'))]).stdout
	assert output == 'PASS MIR combined forward ownership\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
