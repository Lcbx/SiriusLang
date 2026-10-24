#!/usr/bin/env python
"""Exercise packed MIR ownership and i64 bitwise operations."""
from project_paths import ROOT
from build_support import BuildCache, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	output = execute([str(cache.native_driver('mir-ownership-packed', ROOT / 'tests/mir/packed_ownership_driver_native.sirius'))]).stdout
	assert output == 'PASS packed MIR ownership\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
