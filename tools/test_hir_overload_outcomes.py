#!/usr/bin/env python3
"""Verify HIR overload outcome discrimination with compiler-native imports."""
from build_support import BuildCache, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	binary = cache.native_driver('hir-overload-outcomes', ROOT / 'tests/hir/overload_outcome_driver.sirius')
	result = execute([str(binary)])
	assert result.stdout == 'PASS HIR overload outcome states\n', result.stdout
	print(result.stdout, end='')


if __name__ == '__main__':
	main()
