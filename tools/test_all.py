#!/usr/bin/env python3
"""Run active regression scripts, including native execution and MIR self-hosting.

Scripts run in name order. Failures are collected unless --fail-fast is used.
"""
import argparse
from fnmatch import fnmatchcase
from pathlib import Path
import os
import signal
import subprocess
import sys
from time import perf_counter


def select_scripts(root: Path, include: tuple[str, ...] = (), exclude: tuple[str, ...] = ()) -> list[Path]:
	"""Select test drivers by filename or stem; patterns use shell-style globs."""
	def matches(path: Path, patterns: tuple[str, ...]) -> bool:
		return any(fnmatchcase(path.name, pattern) or fnmatchcase(path.stem, pattern) for pattern in patterns)

	return [path for path in sorted(root.glob('test_*.py'))
		if path.name != Path(__file__).name
		and (not include or matches(path, include)) and not matches(path, exclude)]


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
			os.killpg(process.pid, signal.SIGKILL)
		process.wait()
		return None, True


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--list', action='store_true', help='list selected scripts without running them')
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
	scripts = select_scripts(root, tuple(args.match), tuple(args.exclude))
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
