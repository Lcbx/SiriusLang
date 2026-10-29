#!/usr/bin/env python3
"""Recoverable filesystem Result wrappers through native modules and MIR."""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, compile_native, require_cc
from project_paths import ROOT, normalize_newlines


PROGRAM = '''import Std.Filesystem alias FS

def main() -> i64
	let args = arguments()
	let path = args[!1] + "/français.txt"
	let missing = args[!2]
	let empty_file = args[!3]
	let directory = args[!4]
	let empty_dir = args[!5]
	let invalid_write = args[!6]
	let wrote: FS.WriteResult = FS.write_text_result(path, "hello\\né")
	match wrote
		FS.WriteSuccess(bytes)
			if bytes != len("hello\\né")
				return 1
		FS.FilesystemFailure(operation, target, kind, code)
			return 2
	let read: FS.ReadResult = FS.read_text_result(path)
	match read
		FS.ReadSuccess(content)
			if content != "hello\\né"
				return 3
		FS.FilesystemFailure(operation, target, kind, code)
			return 4
	let empty: FS.ReadResult = FS.read_text_result(empty_file)
	match empty
		FS.ReadSuccess(content)
			if content != ""
				return 5
		FS.FilesystemFailure(operation, target, kind, code)
			return 6
	let absent: FS.ReadResult = FS.read_text_result(missing)
	match absent
		FS.ReadSuccess(content)
			return 7
		FS.FilesystemFailure(operation, target, kind, code)
			if operation != "read_text" or target != missing or kind != FS.FS_ERROR_NOT_FOUND or code == 0
				return 8
	let failed_write: FS.WriteResult = FS.write_text_result(invalid_write, "x")
	match failed_write
		FS.WriteSuccess(bytes)
			return 9
		FS.FilesystemFailure(operation, target, kind, code)
			if operation != "write_text" or target != invalid_write or kind != FS.FS_ERROR_NOT_FOUND or code == 0
				return 10
	let listed: FS.ListResult = FS.list_dir_result(directory)
	match listed
		FS.ListSuccess(entries)
			if len(entries) == 0
				return 11
		FS.FilesystemFailure(operation, target, kind, code)
			return 12
	let listed_empty: FS.ListResult = FS.list_dir_result(empty_dir)
	match listed_empty
		FS.ListSuccess(entries)
			if len(entries) != 0
				return 13
		FS.FilesystemFailure(operation, target, kind, code)
			return 14
	let absent_dir: FS.ListResult = FS.list_dir_result(missing)
	match absent_dir
		FS.ListSuccess(entries)
			return 15
		FS.FilesystemFailure(operation, target, kind, code)
			if operation != "list_dir" or target != missing or kind != FS.FS_ERROR_NOT_FOUND or code == 0
				return 16
	# Success resets the snapshot (not merely leaving a previous error).
	if filesystem_error_kind() != FS.FS_ERROR_NOT_FOUND
		return 17
	let again: FS.ReadResult = FS.read_text_result(path)
	match again
		FS.ReadSuccess(content)
			if content != "hello\\né"
				return 18
		FS.FilesystemFailure(operation, target, kind, code)
			return 19
	if filesystem_error_kind() != 0 or filesystem_error_code() != 0
		return 20
	# Malformed paths must be classified consistently.
	print("filesystem Result: ok")
	return 0
'''


def run(*command: str) -> subprocess.CompletedProcess[bytes]:
	result = subprocess.run(command, capture_output=True, timeout=90)
	assert result.returncode == 0, (command, result.returncode, result.stdout, result.stderr)
	return result


def main() -> None:
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-fs-result-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Filesystem.sirius', root / 'Std/Filesystem.sirius')
		(root / 'App.sirius').write_text(PROGRAM, encoding='utf-8')
		(root / 'empty-file').write_bytes(b'')
		(root / 'directory').mkdir()
		(root / 'directory' / 'entry').write_text('data', encoding='utf-8')
		(root / 'empty-dir').mkdir()
		(root / 'unicode').mkdir()
		# Narrow Windows argv is not UTF-8. Construct the Unicode filename in
		# Sirius so this checks wide-path filesystem I/O independently of argv.
		arguments = tuple(map(str, (root / 'unicode', root / 'missing', root / 'empty-file',
			root / 'directory', root / 'empty-dir', root / 'missing' / 'cannot-write')))
		generated = root / 'app.c'
		run(str(cache.seed()), '--module-root', str(root), 'App', str(generated))
		native = compile_native(cache, generated)
		native_result = run(str(native), *arguments)
		assert normalize_newlines(native_result.stdout) == b'filesystem Result: ok\n', native_result
		runner = cache.native_tool('run_cli')
		library = root / 'linked.smirl'
		run(str(runner), '--build-modules', str(root), 'App', str(library))
		interpreted = run(str(runner), '--entry', 'M_3_App_4_main', '--load-lib', str(library), *arguments)
		assert (normalize_newlines(native_result.stdout), normalize_newlines(native_result.stderr)) == (
			normalize_newlines(interpreted.stdout), normalize_newlines(interpreted.stderr))
		assert (root / 'unicode' / 'français.txt').read_bytes() == 'hello\né'.encode('utf-8')
		print('PASS filesystem Result: success, absence, write failure, empty directory, UTF-8, native/MIR parity')


if __name__ == '__main__':
	main()
