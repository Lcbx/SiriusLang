#!/usr/bin/env python
"""Compile and test the Sirius emitter using the committed C bootstrap."""
import argparse
from pathlib import Path
import tempfile

from compiler_cases import check_cases
from source_manifest import COMPILER_SOURCES
from build_support import BuildCache, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = cache.sirius('compiler', COMPILER_SOURCES)
	with tempfile.TemporaryDirectory() as temporary:
		check_cases(cache.cc, compiler, Path(temporary), jobs=validate_jobs(args.jobs), runtime=cache.runtime())


if __name__ == '__main__':
	main()
