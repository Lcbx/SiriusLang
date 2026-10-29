#!/usr/bin/env python3
"""Compile real compiler utility sources as typed Sirius modules.

The flat source manifest remains the bootstrap path; this checks a first
standalone compiler-source dependency through native module imports/linking.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_modules import ModuleError, build, symbol
from build_support import BuildCache, require_cc
from project_paths import ROOT


def run(runner: Path, entry: str, library: Path) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), '--entry', entry, '--load-lib', str(library)], capture_output=True, timeout=30)


def main() -> None:
	runner = BuildCache(shutil.which('clang') or require_cc()).mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		# Use the actual compiler source, not a test reimplementation.
		for name in ('types', 'module'):
			shutil.copy2(ROOT / f'src/{name}.sirius', root / f'{name}.sirius')
		(root / 'Main.sirius').write_text(
			'import module alias Names\n'
			'def main() -> i64\n'
			'\tif Names.hir_module_symbol("Compiler.HIR", "lower") != "M_8_Compiler_3_HIR_5_lower"\n'
			'\t\treturn 1\n'
			'\tif Names.module_scan_filename("src", "Compiler.HIR") != "src/Compiler/HIR.sirius"\n'
			'\t\treturn 2\n'
			'\treturn 0\n', encoding='utf-8')
		library = root / 'linked.smirl'
		entry = build(root, 'Main', library, runner)
		assert entry == symbol('Main', 'main')
		result = run(runner, entry, library)
		assert (result.returncode, result.stdout, result.stderr) == (0, b'', b''), result
		first = library.read_bytes()
		build(root, 'Main', library, runner)
		assert library.read_bytes() == first, 'module build must be deterministic'
		listing = subprocess.run([str(runner), '--list-lib', str(library)], capture_output=True, timeout=30)
		assert listing.returncode == 0, listing
		for name in ('hir_module_symbol', 'module_scan_filename'):
			assert symbol('module', name).encode() in listing.stdout, listing.stdout
		print('PASS real compiler module: qualified imports, typed functions, linkage, deterministic build')

		(root / 'Private.sirius').write_text(
			'import module alias Names\n'
			'def main() -> i64\n\treturn Names.unknown("x", "y")\n', encoding='utf-8')
		try:
			build(root, 'Private', root / 'bad.smirl', runner)
		except ModuleError as error:
			assert 'not exported' in str(error), error
		else:
			raise AssertionError('unknown compiler module symbol accepted')
		print('PASS compiler module interface: unknown export rejected')


if __name__ == '__main__':
	main()
