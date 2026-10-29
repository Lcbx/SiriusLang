#!/usr/bin/env python3
"""Checked recursive walk: error propagation, empty roots and MIR/C parity."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

from build_support import BuildCache, compile_native, require_cc
from project_paths import ROOT


PROGRAM = '''import Std.Filesystem alias FS

def main() -> i64
	let args = arguments()
	let root = args[!1]
	let empty_dir = args[!2]
	let missing = args[!3]
	let regular_file = args[!4]
	let locked_root = args[!5]
	let locked_child = args[!6]
	let test_permissions = args[!7]
	let symlink = args[!8]
	let test_symlink = args[!9]
	let dir_kind: FS.PathKindResult = FS.path_kind_result(root)
	match dir_kind
		FS.PathKindSuccess(kind)
			if kind != FS.FS_PATH_DIRECTORY
				return 10
		FS.FilesystemFailure(operation, path, kind, code)
			return 11
	let file_kind: FS.PathKindResult = FS.path_kind_result(regular_file)
	match file_kind
		FS.PathKindSuccess(kind)
			if kind != FS.FS_PATH_FILE
				return 12
		FS.FilesystemFailure(operation, path, kind, code)
			return 13
	let missing_kind: FS.PathKindResult = FS.path_kind_result(missing)
	match missing_kind
		FS.PathKindSuccess(kind)
			return 14
		FS.FilesystemFailure(operation, path, kind, code)
			if operation != "path_kind" or path != missing or kind != FS.FS_ERROR_NOT_FOUND or code == 0
				return 15
	if test_symlink == "yes"
		let link_kind: FS.PathKindResult = FS.path_kind_result(symlink)
		match link_kind
			FS.PathKindSuccess(kind)
				if kind != FS.FS_PATH_OTHER
					return 16
			FS.FilesystemFailure(operation, path, kind, code)
				return 17
	# Successful metadata lookups must clear the previous error snapshot.
	if path_kind(root) != FS.FS_PATH_DIRECTORY or filesystem_error_kind() != 0 or filesystem_error_code() != 0
		return 18
	let walked: FS.WalkResult = FS.walk_files_result(root)
	match walked
		FS.WalkSuccess(files)
			for path in files
				print(path)
		FS.FilesystemFailure(operation, path, kind, code)
			return 1
	let empty: FS.WalkResult = FS.walk_files_result(empty_dir)
	match empty
		FS.WalkSuccess(files)
			if len(files) != 0
				return 2
		FS.FilesystemFailure(operation, path, kind, code)
			return 3
	let absent: FS.WalkResult = FS.walk_files_result(missing)
	match absent
		FS.WalkSuccess(files)
			return 4
		FS.FilesystemFailure(operation, path, kind, code)
			if operation != "list_dir" or path != missing or kind != FS.FS_ERROR_NOT_FOUND or code == 0
				return 5
	let not_directory: FS.WalkResult = FS.walk_files_result(regular_file)
	match not_directory
		FS.WalkSuccess(files)
			return 6
		FS.FilesystemFailure(operation, path, kind, code)
			if operation != "list_dir" or path != regular_file or kind == 0 or code == 0
				return 7
	if test_permissions == "yes"
		let blocked: FS.WalkResult = FS.walk_files_result(locked_root)
		match blocked
			FS.WalkSuccess(files)
				return 8
			FS.FilesystemFailure(operation, path, kind, code)
				if operation != "list_dir" or path != locked_child or kind != FS.FS_ERROR_PERMISSION or code == 0
					return 9
	print("checked walk: ok")
	return 0
'''


def run(*command: str) -> subprocess.CompletedProcess[bytes]:
	result = subprocess.run(command, capture_output=True, timeout=90)
	assert result.returncode == 0, (command, result.returncode, result.stdout, result.stderr)
	return result


def main() -> None:
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-fs-walk-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Filesystem.sirius', root / 'Std/Filesystem.sirius')
		(root / 'App.sirius').write_text(PROGRAM, encoding='utf-8')
		walk = root / 'tree'
		(walk / 'a').mkdir(parents=True)
		(walk / 'b').mkdir()
		(walk / 'top.txt').write_text('top', encoding='utf-8')
		(walk / 'a' / 'français.txt').write_text('a', encoding='utf-8')
		(walk / 'b' / 'zeta.txt').write_text('b', encoding='utf-8')
		expected = [f'{walk}/top.txt', f'{walk}/a/français.txt', f'{walk}/b/zeta.txt']
		test_symlink = 'no'
		link = walk / 'b' / 'loop'
		try:
			link.symlink_to(walk, target_is_directory=True)
			expected.insert(2, f'{walk}/b/loop')
			test_symlink = 'yes'
		except (OSError, NotImplementedError):
			pass
		(root / 'empty').mkdir()
		ordinary = root / 'regular.txt'
		ordinary.write_text('not a folder', encoding='utf-8')
		locked_root = root / 'locked-root'
		locked_child = locked_root / 'locked'
		locked_child.mkdir(parents=True)
		(locked_child / 'secret').write_text('hidden', encoding='utf-8')
		permission_test = 'no'
		if os.name == 'posix':
			locked_child.chmod(0)
			try:
				with os.scandir(locked_child) as entries:
					list(entries)
			except PermissionError:
				permission_test = 'yes'
		arguments = tuple(map(str, (walk, root / 'empty', root / 'missing', ordinary,
			locked_root, locked_child, permission_test, link, test_symlink)))
		try:
			c_file = root / 'app.c'
			run(str(cache.seed()), '--module-root', str(root), 'App', str(c_file))
			binary = compile_native(cache, c_file)
			native = run(str(binary), *arguments)
			expected_output = [path.encode('utf-8') for path in expected] + [b'checked walk: ok']
			assert native.stdout.splitlines() == expected_output, (native.stdout, expected_output)
			runner = cache.mir_runner()
			library = root / 'linked.smirl'
			run(str(runner), '--build-modules', str(root), 'App', str(library))
			interpreted = run(str(runner), '--entry', 'M_3_App_4_main', '--load-lib', str(library), *arguments)
			assert (native.stdout.splitlines(), native.stderr.splitlines()) == (interpreted.stdout.splitlines(), interpreted.stderr.splitlines())
		finally:
			if os.name == 'posix':
				locked_child.chmod(0o700)
		print('PASS checked walk: path-kind, BFS, empty/missing/non-directory, symlink policy, native/MIR parity'
			+ (', nested permission errors' if permission_test == 'yes' else ' (permission test skipped)'))


if __name__ == '__main__':
	main()
