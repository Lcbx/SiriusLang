#!/usr/bin/env python
"""Audit explicit CFG trap guards and report non-unwinding operations."""
from project_paths import executable_path, ROOT
from build_support import BuildCache, C_FLAGS, execute, require_cc, link_flags
from pathlib import Path
import subprocess
import tempfile


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-failure-contract', ROOT / 'tests/mir/failure_contract_driver_native.sirius')
	with tempfile.TemporaryDirectory() as tmp:
		folder = Path(tmp)
		sources = [folder / 'optional.c', folder / 'ieee.c']
		result = execute([str(driver), *(str(source) for source in sources)])
		for source in sources:
			binary = executable_path(source.with_suffix(''))
			execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
				str(source), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)])
			native = subprocess.run([str(binary)], capture_output=True, text=True)
			assert native.returncode == 0 and native.stdout == '' and native.stderr == '', (source.name, native)

	if result.stdout.replace('\r\n', '\n') != 'PASS MIR failure contract\n':
		raise AssertionError(result)
	print('PASS MIR failure contract audit')


if __name__ == '__main__':
	main()
