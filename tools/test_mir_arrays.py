#!/usr/bin/env python
"""Check MIR arrays, recursive ownership and COW through Sirius and C."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-array', MIR_DRIVER_SOURCES + ('tests/mir/array_driver.sirius',))
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
			check_native(cache, source, stdout=expected, code=code, stderr_contains=error)
	print('PASS MIR arrays: CoW, bounds, nested ownership (interpreter/native)')


if __name__ == '__main__':
	main()
