#!/usr/bin/env python
"""Check record-update MIR verification, interpreter, CoW, and native C."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	print('Building native MIR record-update driver...', flush=True)
	driver = cache.native_driver('mir-record-set', ROOT / 'tests/mir/record_update_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		generated = Path(directory) / 'updated.c'
		print('Running record-update verifier/interpreter...', flush=True)
		result = execute([str(driver), str(generated)])
		if result.stdout != 'PASS MIR record update\n':
			raise AssertionError(result.stdout)
		print('Compiling and running generated C...', flush=True)
		check_native(cache, generated, code=10)
		print('PASS MIR record update: verifier, interpreter, snapshot CoW, ownership, and native C')


if __name__ == '__main__':
	main()
