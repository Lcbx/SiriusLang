#!/usr/bin/env python3
"""Run the actual HIR core via native imports of parser and types."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from project_paths import ROOT, normalize_newlines


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=60)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='reuse a previously built MIR runner')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for name in ('tokenizer', 'parser', 'types', 'hir'):
			shutil.copy2(ROOT / f'src/{name}.sirius', root / f'{name}.sirius')
		shutil.copy2(ROOT / 'tests/hir/module_driver.sirius', root / 'Main.sirius')
		library = root / 'hir.smirl'
		result = run(runner, '--build-modules', root, 'Main', library)
		assert result.returncode == 0, result.stderr
		first = library.read_bytes()
		result = run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', library)
		assert result.returncode == 0 and normalize_newlines(result.stdout) == b'HIR module OK\n', result
		result = run(runner, '--build-modules', root, 'Main', library)
		assert result.returncode == 0 and library.read_bytes() == first, 'non-deterministic linkage'
		# The public function can be imported, and the internal type-key helper cannot.
		(root / 'Private.sirius').write_text('import types alias T\ndef main() -> i64\n\tlet key = T.type_descriptor_key(1, 0, 0, Array[T.RecordField]())\n\treturn len(key)\n', encoding='utf-8')
		bad = run(runner, '--build-modules', root, 'Private', library)
		assert bad.returncode != 0 and b'not exported' in bad.stderr, bad.stderr
		print('PASS HIR core imports parser and types, deterministic linking and private symbols')


if __name__ == '__main__':
	main()
