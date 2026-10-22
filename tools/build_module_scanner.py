#!/usr/bin/env python3
"""Build the Sirius-native source-module discovery and interface scanner."""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, require_cc
from source_manifest import MODULE_SCANNER_SOURCES


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius-module-scan.exe' if sys.platform == 'win32' else 'sirius-module-scan'))
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	args = parser.parse_args()
	binary = BuildCache(shutil.which(args.cc) or args.cc).sirius('module-scanner', MODULE_SCANNER_SOURCES)
	args.output.parent.mkdir(parents=True, exist_ok=True)
	if binary.resolve() != args.output.resolve():
		shutil.copy2(binary, args.output)
	print(f'Built {args.output} (Sirius module scanner)')


if __name__ == '__main__':
	main()
