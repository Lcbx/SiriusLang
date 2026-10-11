#!/usr/bin/env python
"""Exercise nested canonical records through Sirius MIR and native C."""
import subprocess
import tempfile
from pathlib import Path

from test_build import BuildCache, C_FLAGS, LINK_FLAGS, execute, require_cc
from sirius_sources import COMPILER_SOURCES, ROOT


def main() -> None:
	cc = require_cc()
	cache = BuildCache(cc)
	driver = cache.sirius('mir-record', COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius', 'tests/mir/record_driver.sirius'))
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / 'records.c', folder / 'invalid.c', folder / 'returned.c']
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'Ada\nPASS record MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, fragment in ((generated[0], b'Ada\n', None), (generated[1], b'', b'uninitialized register'), (generated[2], b'', None)):
			program = source.with_suffix('')
			execute([cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source), str(cache.runtime()), '-lm', '-o', str(program)])
			run = subprocess.run([str(program)], capture_output=True)
			if run.stdout != expected or (fragment is None and run.returncode != (8 if source == generated[0] else 4)) or (fragment is not None and (run.returncode == 0 or fragment not in run.stderr)):
				raise AssertionError((source.name, run))
		print('PASS nested records: interpreter, native C, recursive ownership, and use-after-release')


if __name__ == '__main__':
	main()
