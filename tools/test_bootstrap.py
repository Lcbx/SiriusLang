#!/usr/bin/env python
"""Verify bootstrap self-hosting and run the generated compiler regression suite."""
import argparse
from pathlib import Path
import tempfile

from bootstrap_support import check_committed, generate_fixed_point
from compiler_cases import check_cases
from build_support import BuildCache, options, require_cc, validate_jobs
from source_manifest import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--fixed-point-only', action='store_true',
		help='skip executable fixtures; still verify committed C and fixed point')
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		generated, compiler = generate_fixed_point(cache, work)
		check_committed(ROOT / 'bootstrap/compiler.c', generated)
		if not args.fixed_point_only:
			check_cases(cache.cc, compiler, work, jobs=validate_jobs(args.jobs), runtime=cache.runtime())


if __name__ == '__main__':
	main()
