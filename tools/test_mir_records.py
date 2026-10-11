#!/usr/bin/env python
"""Exercise nested canonical records through Sirius MIR and native C."""
import tempfile
from pathlib import Path

from test_build import BuildCache, check_native, execute, require_cc
from sirius_sources import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-record', MIR_DRIVER_SOURCES + ('tests/mir/record_driver.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / 'records.c', folder / 'invalid.c', folder / 'returned.c']
		result = execute([str(driver), *(str(path) for path in generated)])
		if result.stdout != 'Ada\nPASS record MIR\n':
			raise AssertionError(result.stdout)
		for source, expected, fragment in ((generated[0], b'Ada\n', None), (generated[1], b'', b'uninitialized register'), (generated[2], b'', None)):
			check_native(cache, source, stdout=expected, code=None if fragment is not None else (8 if source == generated[0] else 4), stderr_contains=fragment)
		print('PASS nested records: interpreter, native C, recursive ownership, and use-after-release')


if __name__ == '__main__':
	main()
