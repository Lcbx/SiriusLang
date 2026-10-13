#!/usr/bin/env python
"""Check optional MIR values, CFG behavior, ownership and native execution."""
import tempfile
from pathlib import Path

from build_support import BuildCache, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-optional', MIR_DRIVER_SOURCES + ('tests/mir/optional_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		generated = [work / f'{name}.c' for name in ('present', 'missing', 'nested')]
		output = execute([str(driver), *(str(p) for p in generated)]).stdout
		if output != 'PASS optional MIR\n':
			raise AssertionError(output)
		for source, code, error in (
			(generated[0], 10, None),
			(generated[1], None, b'missing optional value'),
			(generated[2], 1, None),
		):
			check_native(cache, source, code=code, stderr_contains=error)
	print('PASS MIR optionals: present/missing, managed payload and early panic (interpreter/native)')


if __name__ == '__main__':
	main()
