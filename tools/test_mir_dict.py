#!/usr/bin/env python
"""Validate canonical MIR dictionaries through the Sirius interpreter and C."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-dict', ROOT / 'tests/mir/dict_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / f'{name}.c' for name in ('strings', 'nested', 'invalid')]
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'old\nnew\nfallback\nPASS dictionary MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, status in ((generated[0], b'old\nnew\nfallback\n', 1), (generated[1], b'', 3), (generated[2], b'', 1)):
			check_native(cache, source, stdout=expected, code=status)
	print('PASS MIR dictionaries: CoW, string keys, nested values (interpreter/native)')


if __name__ == '__main__':
	main()
