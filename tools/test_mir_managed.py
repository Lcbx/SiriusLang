#!/usr/bin/env python
"""Execute the same managed MIR in the interpreter and native C backend."""
import subprocess
import tempfile
from pathlib import Path

from test_build import BuildCache, C_FLAGS, LINK_FLAGS, execute, require_cc
from sirius_sources import COMPILER_SOURCES, ROOT


def main() -> None:
	cc = require_cc()
	cache = BuildCache(cc)
	driver = cache.sirius('mir-managed', COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius', 'tests/mir/managed_driver.sirius'))
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / 'managed.c', folder / 'invalid.c']
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'hello!\nPASS managed MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, fragment in ((generated[0], b'hello!\n', None), (generated[1], b'', b'uninitialized register')):
			program = source.with_suffix('')
			execute([cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source), str(cache.runtime()), '-lm', '-o', str(program)])
			run = subprocess.run([str(program)], capture_output=True)
			if run.stdout != expected or (fragment is None and run.returncode != 6) or (fragment is not None and (run.returncode == 0 or fragment not in run.stderr)):
				raise AssertionError((source.name, run))
		print('PASS managed MIR: interpreter, native C, and use-after-move detection')


if __name__ == '__main__':
	main()
