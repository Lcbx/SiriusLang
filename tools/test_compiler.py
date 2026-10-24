#!/usr/bin/env python
"""Run the shared fixture suite against the default MIR compiler."""
import argparse
import shutil
from pathlib import Path
import tempfile

from compiler_cases import check_cases
from build_support import BuildCache, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc(), rebuild=args.rebuild)
	compiler = cache.native_tool('modular_cli')
	with tempfile.TemporaryDirectory() as temporary:
		check_cases(cache.cc, compiler, Path(temporary), jobs=validate_jobs(args.jobs),
			runtime=cache.runtime(), negative_exclude=('04_unsupported_array', '27_optional_elif'))


if __name__ == '__main__':
	main()
