#!/usr/bin/env python
"""Verify MIR definite initialization and consumption across the CFG."""

from build_support import BuildCache, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	output = execute([str(cache.native_driver('mir-ownership', ROOT / 'tests/mir/ownership_driver_native.sirius'))]).stdout
	assert output == 'PASS MIR forward ownership checks\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
