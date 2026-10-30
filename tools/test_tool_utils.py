#!/usr/bin/env python
"""Unit tests for shared process and newline comparison utilities."""
import subprocess
from contextlib import redirect_stdout
from io import StringIO
import os
import sys
from pathlib import Path, PureWindowsPath
from tempfile import TemporaryDirectory
from types import SimpleNamespace
from unittest.mock import patch

import test_all
from test_all import GROUP_PATTERNS, deduplicate_fixture_suite, groups_for, needs_mir_runner, prepare_mir_runner, run_script, select_scripts
from test_fixture_layout import check_pairs, check_json_cases
from profile_mir_allocations import parse_report, same_generated_c
from profile_mir import instrument

from project_paths import executable_path, normalize_newlines
from build_support import MIR_RUNNER_ENV, MIR_RUNNER_TOOLCHAIN_ENV, assert_native, copy_binary


def check_suite_runner(root: Path) -> None:
	bootstrap = root / 'test_bootstrap.py'
	compiler = root / 'test_compiler.py'
	assert deduplicate_fixture_suite([bootstrap, compiler]) == [bootstrap]
	assert deduplicate_fixture_suite([compiler]) == [compiler]
	first = root / 'test_first.py'
	second = root / 'test_second.py'
	for script in (first, second):
		script.write_text("cache.native_tool('run_cli')\n", encoding='utf-8')
	assert needs_mir_runner(first)
	other = root / 'not_a_runner.py'
	for source in ("cache.native_tool(entry)\n", "cache.native_tool(entry='run_cli')\n", "cache.mir_runner()\n"):
		other.write_text(source, encoding='utf-8')
		assert needs_mir_runner(other)
	for source in ("cache.native_tool('modular_cli')\n", "# cache.native_tool('run_cli')\n", 'broken ('):
		other.write_text(source, encoding='utf-8')
		assert not needs_mir_runner(other)
	other.write_text('SUITE_NEEDS_MIR_RUNNER = False\ncache.mir_runner()\n', encoding='utf-8')
	assert not needs_mir_runner(other)
	prepared = []
	seen = []
	def prepare(tools, destination, timeout, env, supplied):
		destination.write_bytes(b'runner')
		prepared.append(destination)
		return {**env, MIR_RUNNER_ENV: str(destination), MIR_RUNNER_TOOLCHAIN_ENV: 'test'}
	def run(path, project, timeout, *, env, arguments=()):
		binary = Path(env[MIR_RUNNER_ENV])
		assert binary.is_file()
		seen.append(binary)
		return 0, False
	with patch.object(sys, 'argv', ['test_all.py']), \
		patch.object(test_all, 'select_scripts', return_value=[first, second]), \
		patch.object(test_all, 'prepare_mir_runner', side_effect=prepare), \
		patch.object(test_all, 'run_script', side_effect=run):
		test_all.main()
	assert len(prepared) == 1 and seen == [prepared[0], prepared[0]]
	assert not prepared[0].exists(), 'Suite runner survived its session'
	with patch.object(sys, 'argv', ['test_all.py', '--list']), \
		patch.object(test_all, 'select_scripts', return_value=[first]), \
		patch.object(test_all, 'prepare_mir_runner', side_effect=AssertionError('unexpected runner setup')):
		test_all.main()

	# Skip the repeated fixture battery only after a successful earlier run.
	mir = root / 'test_mir_compiler.py'
	for first_status, same_compiler, expected in ((0, True, ('--skip-shared-cases',)),
		(0, False, ()), (1, False, ())):
		calls = []
		def run_fixtures(path, project, timeout, *, arguments=(), env):
			if path == bootstrap:
				assert arguments[0] == '--shared-fixtures-marker'
				if same_compiler:
					Path(arguments[1]).write_text('passed')
			calls.append((path.name, arguments if path == mir else ()))
			return (first_status if path == bootstrap else 0), False
		with patch.object(sys, 'argv', ['test_all.py']), \
			patch.object(test_all, 'select_scripts', return_value=[bootstrap, mir]), \
			patch.object(test_all, 'needs_mir_runner', return_value=False), \
			patch.object(test_all, 'run_script', side_effect=run_fixtures):
			try:
				test_all.main()
			except SystemExit as error:
				assert first_status and error.code == 1
		assert calls == [(bootstrap.name, ()), (mir.name, expected)]


def check_runner_setup(root: Path) -> None:
	cache = SimpleNamespace(cc='cc', toolchain='test toolchain')
	destination = root / 'prepared-runner'
	base_env = {'EXAMPLE': 'unchanged'}
	def build(path, project, timeout, *, arguments, env):
		assert arguments == ('--cc', 'cc', '--output', str(destination)) and timeout == 5
		assert env == base_env
		destination.write_bytes(b'built runner')
		return 0, False
	with patch.object(test_all, 'require_cc', return_value='cc'), \
		patch.object(test_all, 'BuildCache', return_value=cache), \
		patch.object(test_all, 'run_script', side_effect=build) as process:
		env = prepare_mir_runner(root, destination, 5, base_env, None)
		assert process.call_count == 1 and Path(env[MIR_RUNNER_ENV]) == destination
		assert env[MIR_RUNNER_TOOLCHAIN_ENV] == cache.toolchain and base_env == {'EXAMPLE': 'unchanged'}
		for result in ((None, True), (7, False)):
			process.side_effect = None
			process.return_value = result
			try:
				prepare_mir_runner(root, destination, 5, base_env, None)
			except SystemExit:
				pass
			else:
				raise AssertionError('Runner setup failure was ignored')
		process.side_effect = AssertionError('Supplied runner triggered a build')
		supplied = root / 'prebuilt'
		supplied.write_bytes(b'prebuilt runner')
		prepare_mir_runner(root, destination, 5, base_env, supplied)
		assert destination.read_bytes() == b'prebuilt runner'


def check_runner_children(root: Path) -> None:
	binary = root / 'shared-runner'
	binary.write_bytes(b'runner')
	report = root / 'runner-paths'
	child = root / 'test_runner_child.py'
	child.write_text('''import os
from pathlib import Path
from build_support import BuildCache, MIR_RUNNER_ENV
cache = BuildCache.__new__(BuildCache)
cache.toolchain = 'child toolchain'
cache.rebuild = False
def unexpected_build():
    raise AssertionError('Child rebuilt the runner')
cache.seed = unexpected_build
entry = 'run_cli'
runner = cache.native_tool(entry)
assert str(runner) == os.environ[MIR_RUNNER_ENV]
with open(os.environ['RUNNER_REPORT'], 'a', encoding='utf-8') as output:
    output.write(str(runner) + '\\n')
''', encoding='utf-8')
	env = {**os.environ, MIR_RUNNER_ENV: str(binary), MIR_RUNNER_TOOLCHAIN_ENV: 'child toolchain',
		'PYTHONPATH': str(Path(__file__).resolve().parent), 'RUNNER_REPORT': str(report)}
	for i in range(2):
		assert run_script(child, root, 5, env=env) == (0, False)
	assert report.read_text(encoding='utf-8').splitlines() == [str(binary), str(binary)]


def main() -> None:
	for references in ('', '(void)&s_mir_f0; '):
		source = ('static int64_t s_mir_f0(void)\n{\n\treturn returned;\n}\n'
			'int main(int argc, char **argv) { s_set_arguments(argc, argv); '
			+ references + 'return (int)s_mir_f0(); }\n')
		profiled = instrument(source)
		assert references in profiled and 'p_leave(0);' in profiled and 'p_report(); return result;' in profiled
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
		lf = root / 'generated.c'; crlf = root / 'seed.c'
		lf.write_bytes(b'int main(void) {\n return 0;\n}\n')
		crlf.write_bytes(b'int main(void) {\r\n return 0;\r\n}\r\n')
		assert same_generated_c(lf, crlf) and same_generated_c(crlf, lf)
		crlf.write_bytes(b'int main(void) {\r\n return 1;\r\n}\r\n')
		assert not same_generated_c(lf, crlf)
		for name in ('test_z.py', 'test_a.py', 'test_mir_ownership.py', 'not_a_test.py'):
			(root / name).touch()
		assert [path.name for path in select_scripts(root)] == ['test_a.py', 'test_mir_ownership.py', 'test_z.py']
		assert [path.name for path in select_scripts(root, ('test_mir_*',))] == ['test_mir_ownership.py']
		assert [path.name for path in select_scripts(root, ('test_*',), ('*ownership*',))] == ['test_a.py', 'test_z.py']
		assert select_scripts(root, ('missing_*',)) == []
		assert [path.name for path in select_scripts(root, groups=('mir',))] == ['test_mir_ownership.py']
		assert select_scripts(root, ('test_a',), groups=('mir',)) == []
		assert groups_for(root / 'test_mir_ownership.py') == ('mir',)
		assert groups_for(root / 'test_enum_empty_opt.py') == ('mir',)
		assert groups_for(root / 'test_a.py') == ()
		assert 'smoke' in GROUP_PATTERNS
		tools = root / 'project' / 'tools'
		rewrite = tools.parent / 'rewrite'
		tools.mkdir(parents=True)
		rewrite.mkdir()
		(tools / 'test_a.py').touch()
		(rewrite / 'test_rewrite_lexer.py').touch()
		assert [p.name for p in select_scripts(tools)] == ['test_a.py', 'test_rewrite_lexer.py']
		assert select_scripts(tools, groups=('rewrite',)) == [rewrite / 'test_rewrite_lexer.py']
		fixtures = root / 'fixtures'
		output = root / 'expected_output'
		fixtures.mkdir(); output.mkdir()
		(fixtures / 'a.sirius').write_text('')
		(output / 'a.txt').write_text('')
		assert check_pairs(root) == 1
		(output / 'extra.txt').write_text('')
		try:
			check_pairs(root)
		except AssertionError:
			pass
		else:
			raise AssertionError('Orphan output fixture was accepted')
		cases = root / 'cases'; cases.mkdir()
		(cases / 'a.c').write_text('')
		(cases / 'expectations.json').write_text('{"a": {}}')
		assert check_json_cases(cases, '.c') == 1
		(cases / 'orphan.c').write_text('')
		try:
			check_json_cases(cases, '.c')
		except AssertionError:
			pass
		else:
			raise AssertionError('Missing case expectation was accepted')
		# Every registered driver must be discoverable through at least one group.
		unclassified = [p.name for p in select_scripts(Path(__file__).resolve().parent)
			if not groups_for(p)]
		assert not unclassified, f'Unclassified test drivers: {unclassified}'
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
		with redirect_stdout(StringIO()):
			check_suite_runner(root)
			check_runner_setup(root)
			check_runner_children(root)
	print('PASS tool helpers: CRLF, output, exit status, diagnostics')


if __name__ == '__main__':
	main()
