#!/usr/bin/env python
"""Test last-use managed call transfers in the MIR interpreter and native C."""
import argparse
import shutil
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, link_flags, check_native, execute, require_cc
from project_paths import executable_path, ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='reuse a built native MIR compiler')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		if args.compiler is None:
			driver = cache.native_driver('mir-call-move', ROOT / 'tests/mir/call_move_driver_native.sirius')
		else:
			modules = work / 'modules'
			modules.mkdir()
			for source in (ROOT / 'src').glob('*.sirius'):
				shutil.copyfile(source, modules / source.name)
			shutil.copyfile(ROOT / 'tests/mir/call_move_driver_native.sirius', modules / 'Main.sirius')
			generated = work / 'driver.c'
			execute([str(args.compiler.resolve()), '--module-root', str(modules), 'Main', str(generated)])
			driver = executable_path(work / 'driver')
			cc = shutil.which('clang') or require_cc()
			flags = ['-Wno-unused-function'] if 'clang' in Path(cc).name else []
			execute([cc, *C_FLAGS, *flags, '-I', str(Path(__file__).resolve().parent.parent / 'runtime'), str(generated), str(cache.runtime()), *link_flags(cc), '-o', str(driver)])
		outputs = [work / f'case{i}.c' for i in range(5)]
		result = execute([str(driver), *(str(path) for path in outputs)])
		if result.stdout != 'PASS MIR call ownership transfers\n':
			raise AssertionError(result.stdout)
		for i, source in enumerate(outputs):
			check_native(cache, source, code=3 if i == 0 else 6)
	print('PASS MIR call transfers: ownership, duplicates, interpreter and native C')


if __name__ == '__main__':
	main()
