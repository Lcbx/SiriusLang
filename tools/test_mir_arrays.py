#!/usr/bin/env python
"""Check MIR arrays, recursive ownership and COW through Sirius and C."""
import subprocess
import tempfile
from pathlib import Path

from test_build import BuildCache, C_FLAGS, LINK_FLAGS, execute, require_cc
from sirius_sources import COMPILER_SOURCES, ROOT


def main() -> None:
	cc = require_cc()
	cache = BuildCache(cc)
	driver = cache.sirius('mir-array', COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius', 'tests/mir/array_driver.sirius'))
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		generated = [work / f'{name}.c' for name in ('cow', 'bounds', 'nested', 'unique', 'move')]
		output = execute([str(driver), *(str(p) for p in generated)]).stdout
		if output != 'old\nnew\nPASS array MIR\n':
			raise AssertionError(output)
		for source, expected, code, error in (
			(generated[0], b'old\nnew\n', 6, None),
			(generated[1], b'', None, b'array index out of bounds'),
			(generated[2], b'', 3, None),
			(generated[3], b'', 7, None),
			(generated[4], b'', None, b'uninitialized register'),
		):
			program = source.with_suffix('')
			execute([cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source), str(cache.runtime()), '-lm', '-o', str(program)])
			result = subprocess.run([str(program)], capture_output=True)
			if result.stdout != expected or (code is not None and result.returncode != code) or (error is not None and (result.returncode == 0 or error not in result.stderr)):
				raise AssertionError((source.name, result))
	print('PASS MIR arrays: CoW, bounds, nested ownership (interpreter/native)')


if __name__ == '__main__':
	main()
