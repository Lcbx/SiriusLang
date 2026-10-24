#!/usr/bin/env python3
"""Test direct MIR-to-C emission without the retired source-code emitter."""
import argparse
from pathlib import Path
import tempfile

from build_support import BuildCache, assert_native, execute, options, require_cc, run_native
from project_paths import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	driver = cache.native_driver('mir-direct-c', ROOT / 'tests/mir/c_driver_native.sirius')
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		generated = [work / f'{name}.c' for name in ('print', 'panic', 'uninitialized', 'verified')]
		execute([str(driver), *(str(path) for path in generated)])
		for source, expected, fragment in (
			(generated[0], b'42\ntrue\n', None),
			(generated[1], b'', b'quoted "panic" and backslash \\ end'),
			(generated[2], b'', b'uninitialized register'),
			(generated[3], b'42\ntrue\n', None),
		):
			result = run_native(cache, source)
			assert_native(result, stdout=expected, code=0 if fragment is None else None,
				stderr_contains=fragment)
		assert b'bool init' in generated[0].read_bytes(), 'defensive emission must keep init tracking'
		assert b'bool init' not in generated[3].read_bytes(), 'verified emission must omit init tracking'
	print('PASS direct MIR-C checked/verified modes, panic escaping and uninitialized register')


if __name__ == '__main__':
	main()
