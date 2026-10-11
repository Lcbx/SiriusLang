#!/usr/bin/env python
"""Execute the same managed MIR in the interpreter and native C backend."""
import tempfile
from pathlib import Path

from test_build import BuildCache, check_native, execute, require_cc
from sirius_sources import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-managed', MIR_DRIVER_SOURCES + ('tests/mir/managed_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / 'managed.c', folder / 'invalid.c']
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'hello!\nPASS managed MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, fragment in ((generated[0], b'hello!\n', None), (generated[1], b'', b'uninitialized register')):
			check_native(cache, source, stdout=expected, code=6 if fragment is None else None, stderr_contains=fragment)
		print('PASS managed MIR: interpreter, native C, and use-after-move detection')


if __name__ == '__main__':
	main()
