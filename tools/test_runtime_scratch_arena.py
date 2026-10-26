#!/usr/bin/env python3
"""Test an isolated C11 region allocator; optionally compare with malloc/free.

This is an experiment, not an allocator used by Sirius values. Its pointers
must not escape the reset boundary or hold managed references.
"""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from project_paths import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=os.environ.get('CC') or shutil.which('cc') or 'cc')
	parser.add_argument('--benchmark', action='store_true', help='print illustrative CPU timings (not a compiler benchmark)')
	parser.add_argument('--sanitize', action='store_true', help='enable ASan/UBSan if supported by the C toolchain')
	args = parser.parse_args()
	with tempfile.TemporaryDirectory(prefix='sirius_scratch_') as directory:
		binary = Path(directory) / ('scratch.exe' if sys.platform == 'win32' else 'scratch')
		command = [args.cc, '-std=c11', '-O1' if args.sanitize else '-O2', '-Wall', '-Wextra',
			'-pedantic', str(ROOT / 'tests/runtime/scratch_arena.c'), '-o', str(binary)]
		if args.sanitize:
			command += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
		subprocess.run(command, check=True)
		result = subprocess.run([str(binary), *(['--benchmark'] if args.benchmark else [])],
			check=True, capture_output=True, text=True)
		print(result.stdout, end='')


if __name__ == '__main__':
	main()
