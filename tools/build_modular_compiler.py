#!/usr/bin/env python3
"""Build a compiler CLI directly from Sirius native modules.

The checked-in C bootstrap already implements --module-root. No flat source
manifest, interpreter or external MIR runner is involved in this build.
"""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, copy_binary, require_cc


def build_modular_compiler(cache: BuildCache, output: Path) -> Path:
	binary = cache.native_tool('modular_cli')
	return copy_binary(binary, output)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius-modular.exe' if sys.platform == 'win32' else 'sirius-modular'))
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	parser.add_argument('--rebuild', action='store_true')
	args = parser.parse_args()
	cache = BuildCache(shutil.which(args.cc) or args.cc, rebuild=args.rebuild)
	build_modular_compiler(cache, args.output.resolve())
	print(f'Built native modular compiler: {args.output}')


if __name__ == '__main__':
	main()
