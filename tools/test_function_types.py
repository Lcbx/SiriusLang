#!/usr/bin/env python3
"""Validate canonical first-class function signatures before executable values."""

from build_support import BuildCache, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	binary = cache.native_driver('type-function', ROOT / 'tests/types/function_driver_native.sirius')
	assert execute([str(binary)]).stdout == 'PASS canonical function signatures and HIR annotations\n'
	print('PASS function type metadata, structural remapping, and HIR annotations')


if __name__ == '__main__':
	main()
