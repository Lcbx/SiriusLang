#!/usr/bin/env python
"""Check conservative verified-C scalar slot reuse with branches and loops."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-scalar-slots', ROOT / 'tests/mir/scalar_slots_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		outputs = [Path(directory) / f'case{i}.c' for i in range(3)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS scalar MIR slot reuse\n':
			raise AssertionError(result.stdout)
		for index, source in enumerate(outputs):
			check_native(cache, source, code=(5, 42, 47)[index])
	print('PASS scalar register-slot reuse, CFG branches, loops and cross-block lifetimes')


if __name__ == '__main__':
	main()
