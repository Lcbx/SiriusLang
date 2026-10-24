#!/usr/bin/env python3
"""Build the MIR runner through the native module-root compiler."""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, copy_binary, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius-run-modular.exe' if sys.platform == 'win32' else 'sirius-run-modular'))
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	parser.add_argument('--rebuild', action='store_true')
	args = parser.parse_args()
	cache = BuildCache(shutil.which(args.cc) or args.cc, rebuild=args.rebuild)
	binary = cache.native_tool('run_cli')
	copy_binary(binary, args.output)
	print(f'Built native modular MIR runner: {args.output}')


if __name__ == '__main__':
	main()
