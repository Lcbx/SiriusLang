#!/usr/bin/env python3
"""Test direct MIR-to-C emission without the retired source-code emitter."""
import argparse
from pathlib import Path
import tempfile

from build_support import BuildCache, assert_native, execute, options, require_cc, run_native
from source_manifest import COMPILER_SOURCES


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	driver = cache.sirius('mir-direct-c', COMPILER_SOURCES[:-1] + ('tests/mir/c_driver.sirius',))
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		generated = [work / f'{name}.c' for name in ('print', 'panic', 'uninitialized')]
		execute([str(driver), *(str(path) for path in generated)])
		for source, expected, fragment in (
			(generated[0], b'42\ntrue\n', None),
			(generated[1], b'', b'quoted "panic" and backslash \\ end'),
			(generated[2], b'', b'uninitialized register'),
		):
			result = run_native(cache, source)
			assert_native(result, stdout=expected, code=0 if fragment is None else None,
				stderr_contains=fragment)
	print('PASS direct MIR-C print, panic escaping and uninitialized register')


if __name__ == '__main__':
	main()
