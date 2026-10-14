#!/usr/bin/env python
"""Run the retained legacy C emitter against its regression suite."""
import argparse
from pathlib import Path
import tempfile

from build_support import BuildCache, options, require_cc, validate_jobs
from compiler_cases import check_cases
from source_manifest import LEGACY_COMPILER_SOURCES


# Record destructuring is currently implemented only by the MIR backend.
MIR_ONLY_POSITIVE = ('86_record_destructure',)
MIR_ONLY_NEGATIVE = ('40_destructure_unknown_field', '41_destructure_duplicate_name',
	'42_destructure_nonrecord', '43_destructure_scope', '44_destructure_empty',
	'45_var_destructure')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = cache.sirius('legacy-compiler', LEGACY_COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as temporary:
		check_cases(cache.cc, compiler, Path(temporary), jobs=validate_jobs(args.jobs),
			runtime=cache.runtime(), positive_exclude=MIR_ONLY_POSITIVE,
			negative_exclude=MIR_ONLY_NEGATIVE)


if __name__ == '__main__':
	main()
