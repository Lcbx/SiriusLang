#!/usr/bin/env python3
"""Verify managed Optional returns on both paths through borrowed/moved calls."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, execute, require_cc, link_flags
from project_paths import executable_path, ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-inband-ownership', ROOT / 'tests/mir/inband_ownership_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		sources = [Path(directory) / f'case{i}.c' for i in range(4)]
		result = execute([str(driver), *(str(path) for path in sources)])
		assert result.stdout == 'PASS in-band nested ownership transfers\n', result.stdout
		for i, source in enumerate(sources):
			binary = executable_path(source.with_suffix(''))
			execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
				'-I', str(ROOT / 'runtime'), str(source), str(ROOT / 'runtime/sirius_runtime.c'),
				*link_flags(cache.cc), '-o', str(binary)])
			run = subprocess.run([str(binary)], capture_output=True, text=True)
			expected = 2 if i % 2 == 0 else 0
			assert run.returncode == expected and run.stdout == '' and run.stderr == '', (i, run)
	print('PASS in-band nested returns, borrowed/moved calls and native ARC')


if __name__ == '__main__':
	main()
