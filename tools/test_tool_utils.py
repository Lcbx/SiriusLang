#!/usr/bin/env python
"""Unit tests for shared process and newline comparison utilities."""
import subprocess
from pathlib import Path, PureWindowsPath
from tempfile import TemporaryDirectory

from test_all import run_script, select_scripts
from profile_mir_allocations import parse_report

from project_paths import executable_path, normalize_newlines
from build_support import assert_native, copy_binary


def main() -> None:
	assert normalize_newlines(b'a\r\nb\n') == b'a\nb\n'
	windows_children = [PureWindowsPath('a'), PureWindowsPath('B')]
	assert [path.name for path in sorted(windows_children, key=lambda path: path.name.encode('utf-8'))] == ['B', 'a']
	assert executable_path(Path('runner'), 'linux') == Path('runner')
	assert executable_path(Path('runner'), 'win32') == Path('runner.exe')
	assert executable_path(Path('runner.exe'), 'win32') == Path('runner.exe')
	assert executable_path(Path('runner.EXE'), 'win32') == Path('runner.EXE')
	assert executable_path(Path('runner.exe'), 'linux') == Path('runner.exe')
	assert_native(subprocess.CompletedProcess(['test'], 0, b'42\r\ntrue\r\n', b''), stdout=b'42\ntrue\n')
	assert_native(subprocess.CompletedProcess(['test'], 7, b'', b'error\r\n'), code=7, stderr_contains=b'error\n')
	assert_native(subprocess.CompletedProcess(['test'], 2, b'', b'error'), code=None, stderr_contains=b'error')
	for result, options in (
		(subprocess.CompletedProcess(['test'], 0, b'42\r\nextra\r\n', b''), {'stdout': b'42\n'}),
		(subprocess.CompletedProcess(['test'], 0, b'', b''), {'code': None}),
		(subprocess.CompletedProcess(['test'], 1, b'', b''), {'code': 0}),
		(subprocess.CompletedProcess(['test'], 1, b'', b'other'), {'code': None, 'stderr_contains': b'error'}),
	):
		try:
			assert_native(result, **options)
		except AssertionError:
			pass
		else:
			raise AssertionError(f'Expected rejection for {result!r}')
	profile = ('S_MIR_ALLOC_PROFILE new=3 empty=0 one=1 many=2 clones=2 unique=1 growth=0 '
		'dict_new=0 dict_insert=0 array_append=2 append_clones=2 array_set=0 optional=0\n'
		'S_MIR_ALLOC_TYPE id=6 new=2 empty=0 bytes=40 clones=2\n'
		'S_MIR_ALLOC_TYPE id=7 new=1 empty=0 bytes=24 clones=0\n')
	for line_ending in ('\n', '\r\n'):
		counts, by_type = parse_report(profile.replace('\n', line_ending))
		assert counts['clones'] == sum(row['clones'] for row in by_type.values()) == 2
		assert len(by_type) == 2
	with TemporaryDirectory() as directory:
		root = Path(directory)
		for name in ('test_z.py', 'test_a.py', 'test_mir_ownership.py', 'not_a_test.py'):
			(root / name).touch()
		assert [path.name for path in select_scripts(root)] == ['test_a.py', 'test_mir_ownership.py', 'test_z.py']
		assert [path.name for path in select_scripts(root, ('test_mir_*',))] == ['test_mir_ownership.py']
		assert [path.name for path in select_scripts(root, ('test_*',), ('*ownership*',))] == ['test_a.py', 'test_z.py']
		assert select_scripts(root, ('missing_*',)) == []
		source = root / 'cached'; source.write_bytes(b'binary')
		output = root / 'new' / 'binary'
		assert copy_binary(source, output) == output and output.read_bytes() == b'binary'
		assert copy_binary(output, output) == output
		quick = root / 'test_quick.py'
		quick.write_text('pass\n', encoding='utf-8')
		assert run_script(quick, root, 5) == (0, False)
		slow = root / 'test_slow.py'
		slow.write_text('import time\ntime.sleep(2)\n', encoding='utf-8')
		assert run_script(slow, root, 0.05) == (None, True)
	print('PASS tool helpers: CRLF, output, exit status, diagnostics')


if __name__ == '__main__':
	main()
