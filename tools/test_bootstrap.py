#!/usr/bin/env python
"""Verify bootstrap self-hosting and run the generated compiler regression suite."""
import argparse
from pathlib import Path
import tempfile

from bootstrap_support import check_committed, generate_fixed_point
from compiler_cases import MIR_NEGATIVE_EXCLUDE, check_cases
from build_support import BuildCache, options, require_cc, validate_jobs
from project_paths import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--fixed-point-only', action='store_true',
		help='skip executable fixtures; still verify committed C and fixed point')
	parser.add_argument('--shared-fixtures-marker', type=Path, help=argparse.SUPPRESS)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		generated, compiler = generate_fixed_point(cache, work, reuse_seed_compiler=True)
		check_committed(ROOT / 'bootstrap/compiler.c', generated)
		if not args.fixed_point_only:
			check_cases(cache.cc, compiler, work, jobs=validate_jobs(args.jobs),
				runtime=cache.runtime(), negative_exclude=MIR_NEGATIVE_EXCLUDE)
			if args.shared_fixtures_marker and compiler.resolve() == cache.native_tool('modular_cli').resolve():
				args.shared_fixtures_marker.write_text('passed', encoding='utf-8')


if __name__ == '__main__':
	main()
