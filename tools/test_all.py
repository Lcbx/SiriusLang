#!/usr/bin/env python3
"""Run test drivers in deterministic order (full suite by default).

Groups are overlapping; --group restricts the suite and --match/--exclude refine it.
Failures are collected unless --fail-fast is requested.
"""
import argparse
import ast
from fnmatch import fnmatchcase
from pathlib import Path
import os
import signal
import subprocess
import sys
from tempfile import TemporaryDirectory
from time import perf_counter

from build_support import (BuildCache, MIR_RUNNER_ENV, MIR_RUNNER_TOOLCHAIN_ENV,
	copy_binary, require_cc)
from project_paths import executable_path


# Filename-based groups keep drivers independently executable from tools/.
# New scripts should follow an existing prefix or be added to the appropriate group.
GROUP_PATTERNS = {
	'smoke': ('test_tool_utils', 'test_fixture_layout', 'test_docs', 'test_compiler_layout'),
	'frontend': ('test_tokenizer', 'test_parser', 'test_types', 'test_hir_*', 'test_compiler*'),
	'mir': ('test_mir', 'test_mir_*', 'test_enum_empty_opt',
		'test_negative_array_index', 'test_integer_edges',
		'test_numeric_conversions', 'test_structural_equality',
		'test_union_hybrid_coallocation'),
	'modules': ('test_module_*', 'test_source_modules', 'test_long_module_symbols'),
	'language': ('test_callback_*', 'test_constructor_*', 'test_contextual_*',
		'test_match_*',
		'test_function_*', 'test_generic_*', 'test_multistage_*',
		'test_union_*', 'test_stdlib_callbacks', 'test_stdlib_numeric',
		'test_negative_array_index', 'test_integer_edges'),
	'runtime': ('test_filesystem*', 'test_runtime_*', 'test_reference_c',
		'test_stdlib_callbacks', 'test_stdlib_numeric'),
	'tooling': ('test_bootstrap*', 'test_build_cache', 'test_docs',
		'test_examples', 'test_link_flags', 'test_tool_utils',
		'test_compiler_layout', 'test_fixture_layout'),
}


def groups_for(path: Path) -> tuple[str, ...]:
	return tuple(name for name, patterns in GROUP_PATTERNS.items()
		if any(fnmatchcase(path.stem, pattern) for pattern in patterns))


def select_scripts(root: Path, include: tuple[str, ...] = (), exclude: tuple[str, ...] = (),
	groups: tuple[str, ...] = ()) -> list[Path]:
	"""Select test drivers by filename or stem; patterns use shell-style globs."""
	def matches(path: Path, patterns: tuple[str, ...]) -> bool:
		return any(fnmatchcase(path.name, pattern) or fnmatchcase(path.stem, pattern) for pattern in patterns)

	return [path for path in sorted(root.glob('test_*.py'))
		if path.name != Path(__file__).name
		and (not include or matches(path, include)) and not matches(path, exclude)
		and (not groups or any(group in groups_for(path) for group in groups))]


def needs_mir_runner(path: Path) -> bool:
	"""Recognize standard runner requests without importing or executing tests."""
	try:
		tree = ast.parse(path.read_text(encoding='utf-8'))
	except SyntaxError:
		return False  # Let the normal test invocation report its syntax error.
	for statement in tree.body:
		if (isinstance(statement, ast.Assign) and any(isinstance(name, ast.Name)
			and name.id == 'SUITE_NEEDS_MIR_RUNNER' for name in statement.targets)
			and isinstance(statement.value, ast.Constant) and isinstance(statement.value.value, bool)):
			return statement.value.value
	for node in ast.walk(tree):
		if (isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute)
			and node.func.attr == 'mir_runner'):
			return True
		if not (isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute)
			and node.func.attr == 'native_tool'):
			continue
		entry = node.args[0] if node.args else next(
			(keyword.value for keyword in node.keywords if keyword.arg == 'entry'), None)
		# Dynamic arguments may request run_cli; prepare rather than silently
		# letting that test build another runner.
		if entry is None or not isinstance(entry, ast.Constant) or entry.value == 'run_cli':
			return True
	return False


def deduplicate_fixture_suite(scripts: list[Path]) -> list[Path]:
	"""Bootstrap runs the same compiler fixtures as test_compiler."""
	if any(path.name == 'test_bootstrap.py' for path in scripts):
		return [path for path in scripts if path.name != 'test_compiler.py']
	return scripts


def run_script(path: Path, root: Path, timeout: float | None, *,
	arguments: tuple[str, ...] = (), env: dict[str, str] | None = None) -> tuple[int | None, bool]:
	"""Run a test with a bounded lifetime, including its compiler subprocesses."""
	kwargs = {'cwd': root}
	if env is not None:
		kwargs['env'] = env
	if os.name == 'nt':
		kwargs['creationflags'] = subprocess.CREATE_NEW_PROCESS_GROUP
	else:
		kwargs['start_new_session'] = True
	process = subprocess.Popen([sys.executable, str(path), *arguments], **kwargs)
	try:
		return process.wait(timeout=timeout), False
	except (subprocess.TimeoutExpired, KeyboardInterrupt) as error:
		if os.name == 'nt':
			# Killing only Python can leave its C compiler/linker running.
			result = subprocess.run(['taskkill', '/F', '/T', '/PID', str(process.pid)],
				capture_output=True, check=False)
			if result.returncode and process.poll() is None:
				process.kill()
		else:
			try:
				os.killpg(process.pid, signal.SIGKILL)
			except ProcessLookupError:
				pass
		process.wait()
		if isinstance(error, KeyboardInterrupt):
			raise
		return None, True


def prepare_mir_runner(root: Path, destination: Path, timeout: float | None,
	env: dict[str, str], supplied: Path | None) -> dict[str, str]:
	"""Pin one suite-local executable; cache deletion cannot remove this copy."""
	print('Preparing shared MIR runner...', flush=True)
	start = perf_counter()
	cache = BuildCache(require_cc())
	if supplied is not None:
		if not supplied.is_file():
			raise SystemExit(f'MIR runner does not exist: {supplied}')
		copy_binary(supplied, destination)
	else:
		code, timed_out = run_script(root / 'build_mir_runner.py', root.parent, timeout,
			arguments=('--cc', cache.cc, '--output', str(destination)), env=env)
		if timed_out:
			raise SystemExit('Shared MIR runner setup timed out')
		if code:
			raise SystemExit(f'Shared MIR runner setup failed: exit {code}')
	if not destination.is_file():
		raise SystemExit('Shared MIR runner setup produced no executable')
	shared_env = dict(env)
	shared_env[MIR_RUNNER_ENV] = str(destination)
	shared_env[MIR_RUNNER_TOOLCHAIN_ENV] = cache.toolchain
	print(f'Shared MIR runner ready in {perf_counter() - start:.1f}s: {destination}', flush=True)
	return shared_env


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--list', action='store_true', help='list selected scripts without running them')
	parser.add_argument('--group', action='append', choices=tuple(GROUP_PATTERNS), default=[],
		help='select a named group (repeatable; overlapping groups are combined)')
	parser.add_argument('--match', action='append', default=[], metavar='GLOB',
		help='include test scripts matching a filename or stem glob (repeatable)')
	parser.add_argument('--exclude', action='append', default=[], metavar='GLOB',
		help='exclude test scripts matching a filename or stem glob (repeatable)')
	parser.add_argument('--fail-fast', action='store_true', help='stop at the first failing script')
	parser.add_argument('--runner', type=Path, help='reuse an existing MIR runner instead of building one')
	parser.add_argument('--timeout', type=float, default=600, metavar='SECONDS',
		help='per-test timeout including native builds (default 600; 0 disables)')
	args = parser.parse_args()
	if args.timeout < 0:
		parser.error('--timeout must be >= 0')
	root = Path(__file__).resolve().parent
	scripts = deduplicate_fixture_suite(select_scripts(root, tuple(args.match), tuple(args.exclude), tuple(args.group)))
	if not scripts:
		parser.error('no test scripts match the given filters')
	if args.list:
		for path in scripts:
			print(path.name)
		return
	failures = []
	env = dict(os.environ)
	for name in (MIR_RUNNER_ENV, MIR_RUNNER_TOOLCHAIN_ENV):
		env.pop(name, None)
	with TemporaryDirectory(prefix='sirius-test-suite-') as temporary:
		shared_runner = executable_path(Path(temporary) / 'sirius-run')
		fixture_marker = Path(temporary) / 'bootstrap-shared-fixtures'
		prepared = False
		fixtures_passed = False
		for path in scripts:
			if not prepared and needs_mir_runner(path):
				supplied = args.runner.resolve() if args.runner else None
				env = prepare_mir_runner(root, shared_runner, args.timeout or None, env, supplied)
				prepared = True
			print(f'\n========== {path.name} ==========', flush=True)
			start = perf_counter()
			arguments = ()
			if path.name == 'test_bootstrap.py':
				arguments = ('--shared-fixtures-marker', str(fixture_marker))
			if path.name == 'test_mir_compiler.py' and fixtures_passed:
				arguments = ('--skip-shared-cases',)
				print('Shared compiler fixtures already passed; running MIR-specific checks.', flush=True)
			code, timed_out = run_script(path, root.parent, args.timeout or None, arguments=arguments, env=env)
			elapsed = perf_counter() - start
			if timed_out:
				print(f'TIMEOUT {path.name} after {elapsed:.1f}s', flush=True)
				failures.append((path.name, 'timeout'))
			elif code:
				print(f'FAIL {path.name}: exit {code} after {elapsed:.1f}s', flush=True)
				failures.append((path.name, code))
			else:
				print(f'PASS {path.name} in {elapsed:.1f}s', flush=True)
				if path.name == 'test_compiler.py' or (path.name == 'test_bootstrap.py' and fixture_marker.is_file()):
					fixtures_passed = True
			if failures and args.fail_fast:
				break
	if failures:
		print('\nFAILED:', ', '.join(f'{name} ({code})' for name, code in failures))
		raise SystemExit(1)
	print(f'\nPASS all {len(scripts)} selected test scripts')


if __name__ == '__main__':
	main()
