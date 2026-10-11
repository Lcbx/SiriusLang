#!/usr/bin/env python
"""Validate canonical MIR dictionaries through the Sirius interpreter and C."""
import subprocess
import tempfile
from pathlib import Path

from test_build import BuildCache, C_FLAGS, LINK_FLAGS, execute, require_cc
from sirius_sources import COMPILER_SOURCES, ROOT


def main() -> None:
	cc = require_cc()
	cache = BuildCache(cc)
	driver = cache.sirius('mir-dict', COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius', 'tests/mir/dict_driver.sirius'))
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / f'{name}.c' for name in ('strings', 'nested', 'invalid')]
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'old\nnew\nfallback\nPASS dictionary MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, status in ((generated[0], b'old\nnew\nfallback\n', 1), (generated[1], b'', 3), (generated[2], b'', 1)):
			program = source.with_suffix('')
			execute([cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source), str(cache.runtime()), '-lm', '-o', str(program)])
			run = subprocess.run([str(program)], capture_output=True)
			if run.stdout != expected or run.returncode != status:
				raise AssertionError((source.name, run))
	print('PASS MIR dictionaries: CoW, string keys, nested values (interpreter/native)')


if __name__ == '__main__':
	main()
