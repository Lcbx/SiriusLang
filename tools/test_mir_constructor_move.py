#!/usr/bin/env python
"""Test last-use constructor ownership transfers in MIR and generated C."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from mir_test_support import ownership_transfers_driver


def main() -> None:
	cache = BuildCache(require_cc())
	driver = ownership_transfers_driver(cache)
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(10)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR constructor transfers\n':
			raise AssertionError(result.stdout)
		for i, source in enumerate(outputs):
			check_native(cache, source, code=2 if i % 5 == 1 else 5)
	print('PASS MIR constructor transfers: verification, ownership, interpreter and native C')


if __name__ == '__main__':
	main()
