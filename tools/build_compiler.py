#!/usr/bin/env python
"""Build an executable Sirius compiler from committed bootstrap/compiler.c."""
import argparse
from pathlib import Path
import sys

from build_support import BuildCache, copy_binary, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path, default=Path('sirius.exe' if sys.platform == 'win32' else 'sirius'),
		help='output executable path (default: ./sirius or ./sirius.exe)')
	parser.add_argument('--rebuild', action='store_true', help='clear cached native builds')
	args = parser.parse_args()
	compiler = BuildCache(require_cc(), rebuild=args.rebuild).seed()
	copy_binary(compiler, args.output)
	print(f'Built {args.output} from committed bootstrap/compiler.c')


if __name__ == '__main__':
	main()
