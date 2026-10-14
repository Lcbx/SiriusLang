#!/usr/bin/env python
"""Build a separate executable retaining the legacy C emitter."""
import argparse
from pathlib import Path
import shutil
import sys

from build_support import BuildCache, require_cc
from source_manifest import LEGACY_COMPILER_SOURCES


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--output', type=Path,
		default=Path('sirius-legacy.exe' if sys.platform == 'win32' else 'sirius-legacy'))
	parser.add_argument('--rebuild', action='store_true', help='clear cached native builds')
	args = parser.parse_args()
	compiler = BuildCache(require_cc(), rebuild=args.rebuild).sirius(
		'legacy-compiler', LEGACY_COMPILER_SOURCES, separator=b'')
	args.output.parent.mkdir(parents=True, exist_ok=True)
	if compiler.resolve() != args.output.resolve():
		shutil.copy2(compiler, args.output)
	print(f'Built {args.output} (legacy C emitter)')


if __name__ == '__main__':
	main()
