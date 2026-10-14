#!/usr/bin/env python
"""Test last-use managed call transfers in the MIR interpreter and native C."""
import argparse
import shutil
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, LINK_FLAGS, check_native, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES, combined_sources


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='reuse a built native MIR compiler')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		if args.compiler is None:
			driver = cache.sirius('mir-call-move', MIR_DRIVER_SOURCES + ('tests/mir/call_move_driver.sirius',))
		else:
			combined = work / 'driver.sirius'
			combined.write_bytes(combined_sources(MIR_DRIVER_SOURCES + ('tests/mir/call_move_driver.sirius',)))
			generated = work / 'driver.c'
			execute([str(args.compiler.resolve()), str(combined), str(generated)])
			driver = work / 'driver'
			cc = shutil.which('clang') or require_cc()
			flags = ['-Wno-unused-function'] if 'clang' in Path(cc).name else []
			execute([cc, *C_FLAGS, *flags, *LINK_FLAGS, '-I', str(Path(__file__).resolve().parent.parent / 'runtime'), str(generated), str(cache.runtime()), '-lm', '-o', str(driver)])
		outputs = [work / f'case{i}.c' for i in range(5)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR call ownership transfers\n':
			raise AssertionError(result.stdout)
		for i, source in enumerate(outputs):
			check_native(cache, source, code=3 if i == 0 else 6)
	print('PASS MIR call transfers: ownership, duplicates, interpreter and native C')


if __name__ == '__main__':
	main()
