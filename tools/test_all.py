#!/usr/bin/env python3
"""Run test drivers in deterministic order (full suite by default).

Groups are overlapping; --group restricts the suite and --match/--exclude refine it.
Failures are collected unless --fail-fast is requested.
"""
import argparse
from fnmatch import fnmatchcase
from pathlib import Path
import os
import signal
import subprocess
import sys
from time import perf_counter


# Filename-based groups keep drivers independently executable from tools/.
# New scripts should follow an existing prefix or be added to the appropriate group.
GROUP_PATTERNS = {
	'smoke': ('test_tool_utils', 'test_fixture_layout', 'test_docs', 'test_compiler_layout'),
	'frontend': ('test_tokenizer', 'test_parser', 'test_types', 'test_hir_*', 'test_compiler*'),
	'mir': ('test_mir', 'test_mir_*', 'test_negative_array_index', 'test_integer_edges'),
	'modules': ('test_module_*', 'test_source_modules', 'test_long_module_symbols'),
	'language': ('test_callback_*', 'test_constructor_*', 'test_contextual_*',
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


def run_script(path: Path, root: Path, timeout: float | None) -> tuple[int | None, bool]:
	"""Run a test with a bounded lifetime, including its compiler subprocesses."""
	kwargs = {'cwd': root}
	if os.name == 'nt':
		kwargs['creationflags'] = subprocess.CREATE_NEW_PROCESS_GROUP
	else:
		kwargs['start_new_session'] = True
	process = subprocess.Popen([sys.executable, str(path)], **kwargs)
	try:
		return process.wait(timeout=timeout), False
	except subprocess.TimeoutExpired:
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
		return None, True


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
	parser.add_argument('--timeout', type=float, default=600, metavar='SECONDS',
		help='per-test timeout including native builds (default 600; 0 disables)')
	args = parser.parse_args()
	if args.timeout < 0:
		parser.error('--timeout must be >= 0')
	root = Path(__file__).resolve().parent
	scripts = select_scripts(root, tuple(args.match), tuple(args.exclude), tuple(args.group))
	if not scripts:
		parser.error('no test scripts match the given filters')
	if args.list:
		for path in scripts:
			print(path.name)
		return
	failures = []
	for path in scripts:
		print(f'\n========== {path.name} ==========', flush=True)
		start = perf_counter()
		code, timed_out = run_script(path, root.parent, args.timeout or None)
		elapsed = perf_counter() - start
		if timed_out:
			print(f'TIMEOUT {path.name} after {elapsed:.1f}s', flush=True)
			failures.append((path.name, 'timeout'))
		elif code:
			print(f'FAIL {path.name}: exit {code} after {elapsed:.1f}s', flush=True)
			failures.append((path.name, code))
		else:
			print(f'PASS {path.name} in {elapsed:.1f}s', flush=True)
		if failures and args.fail_fast:
			break
	if failures:
		print('\nFAILED:', ', '.join(f'{name} ({code})' for name, code in failures))
		raise SystemExit(1)
	print(f'\nPASS all {len(scripts)} selected test scripts')


if __name__ == '__main__':
	main()
