#!/usr/bin/env python
"""Test optimized MIR copy ownership and verified-C cleanup semantics."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-opt', ROOT / 'tests/mir/opt_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(5)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR owned-copy elimination\n':
			raise AssertionError(result.stdout)
		for source in outputs:
			check_native(cache, source, code=42 if source == outputs[3] else (43 if source == outputs[4] else 3))
	print('PASS optimized MIR ownership, interpreter and native C')


if __name__ == '__main__':
	main()
