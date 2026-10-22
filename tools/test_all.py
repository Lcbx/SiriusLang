#!/usr/bin/env python3
"""Run active regression tests, including MIR self-hosting and native execution.

The full suite includes compiler self-hosting and large native builds. Failures do
not stop later tests, and the final exit code is nonzero if any test fails.
"""
import argparse
from pathlib import Path
import subprocess
import sys


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--list', action='store_true', help='list selected scripts without running them')
	args = parser.parse_args()
	root = Path(__file__).resolve().parent
	scripts = sorted(path for path in root.glob('test_*.py')
		if path.name != Path(__file__).name)
	if args.list:
		for path in scripts:
			print(path.name)
		return
	failures = []
	for path in scripts:
		print(f'\n========== {path.name} ==========', flush=True)
		command = [sys.executable, str(path)]
		result = subprocess.run(command, cwd=root.parent)
		if result.returncode:
			failures.append((path.name, result.returncode))
	if failures:
		print('\nFAILED:', ', '.join(f'{name} ({code})' for name, code in failures))
		raise SystemExit(1)
	print(f'\nPASS all {len(scripts)} selected test scripts')


if __name__ == '__main__':
	main()
