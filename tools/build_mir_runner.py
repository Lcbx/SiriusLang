#!/usr/bin/env python
"""Build the native executable that dynamically runs source-lowered MIR."""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, copy_binary, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius-run.exe' if sys.platform == 'win32' else 'sirius-run'))
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc(), help='C11 compiler; Clang is preferred for the large generated translation unit')
	parser.add_argument('--rebuild', action='store_true')
	args = parser.parse_args()
	binary = BuildCache(shutil.which(args.cc) or args.cc, rebuild=args.rebuild).native_tool('run_cli')
	copy_binary(binary, args.output)
	print(f'Built {args.output} (dynamic MIR source runner)')


if __name__ == '__main__':
	main()
