#!/usr/bin/env python3
"""Build the Sirius-native source-module discovery and interface scanner."""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, copy_binary, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius-module-scan.exe' if sys.platform == 'win32' else 'sirius-module-scan'))
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	parser.add_argument('--rebuild', action='store_true')
	args = parser.parse_args()
	binary = BuildCache(shutil.which(args.cc) or args.cc, rebuild=args.rebuild).native_tool('module_scan_cli')
	copy_binary(binary, args.output)
	print(f'Built {args.output} (Sirius module scanner)')


if __name__ == '__main__':
	main()
