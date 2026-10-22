#!/usr/bin/env python3
"""Filesystem builtins: deterministic enumeration and native/MIR parity."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, compile_native, require_cc
from source_manifest import ROOT, MIR_RUNNER_SOURCES


def execute(args: list[str], *, code: int = 0) -> subprocess.CompletedProcess[bytes]:
	result = subprocess.run(args, capture_output=True, timeout=60)
	assert result.returncode == code, (args, result)
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='Sirius C compiler')
	parser.add_argument('--runner', type=Path, help='prebuilt MIR runner for execution parity')
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.seed()
	with tempfile.TemporaryDirectory(prefix='sirius-fs-') as tmp:
		work = Path(tmp)
		root = work / 'root'
		(root / 'B').mkdir(parents=True)
		(root / 'a').mkdir()
		(root / 'hello.txt').write_text('hello', encoding='utf-8')
		(root / 'a' / 'français.txt').write_text('unicode', encoding='utf-8')
		(root / 'B' / 'zeta.txt').write_text('z', encoding='utf-8')
		(root / 'empty').mkdir()
		# A symlink to a directory must not cause recursive cycles.
		try:
			(root / 'B' / 'loop').symlink_to(root, target_is_directory=True)
		except (OSError, NotImplementedError):
			pass
		binary_source = ROOT / 'examples' / 'filesystem_walk.sirius'
		c_file = work / 'filesystem.c'
		execute([str(compiler), str(binary_source), str(c_file)])
		binary = compile_native(cache, c_file)
		for directory in (root, root / 'empty'):
			native = execute([str(binary), str(directory)])
			entries = sorted(path.name for path in directory.iterdir())
			files = []
			pending = [directory]
			for folder in pending:
				for child in sorted(folder.iterdir()):
					if child.is_dir() and not child.is_symlink():
						pending.append(child)
					else:
						files.append(child)
			expected = ''.join(f'{entry}\n' for entry in entries)
			expected += ''.join(f'file: {item.as_posix()}\n' for item in files)
			assert native.stdout.decode('utf-8') == expected, (native.stdout, expected)
			if args.runner:
				dynamic = execute([str(args.runner.resolve()), str(binary_source), str(directory)])
				assert (dynamic.stdout, dynamic.stderr, dynamic.returncode) == (native.stdout, native.stderr, native.returncode)
		print('PASS filesystem: sorted entries, recursive walk, empty dir, unicode, symlink policy')
		missing = execute([str(binary), str(work / 'not-found')], code=2)
		assert missing.stdout == b''
		if args.runner:
			dynamic = execute([str(args.runner.resolve()), str(binary_source), str(work / 'not-found')], code=2)
			assert dynamic.stdout == missing.stdout
		print('PASS filesystem: failed listing is distinct from empty directory')
		for index, bad_call in enumerate(('list_dir(123)', 'is_dir(false)')):
			bad = work / f'bad{index}.sirius'
			bad.write_text(f'def main() -> i64\n\tlet value = {bad_call}\n\treturn 0\n')
			failure = subprocess.run([str(compiler), str(bad), str(work / 'bad.c')], capture_output=True, timeout=60)
			assert failure.returncode == 1 and b'expected string' in failure.stdout, failure
		print('PASS filesystem: invalid argument types rejected')


if __name__ == '__main__':
	main()
