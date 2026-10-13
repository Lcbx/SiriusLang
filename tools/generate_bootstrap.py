#!/usr/bin/env python
"""Regenerate committed C from Sirius sources after a self-hosting fixed point."""
import argparse
import tempfile
from pathlib import Path

from bootstrap_support import check_committed, generate_fixed_point
from build_support import BuildCache, require_cc
from source_manifest import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--check', action='store_true',
		help='verify committed C instead of overwriting it; no fixtures in either mode')
	parser.add_argument('--rebuild', action='store_true', help='discard cached native builds')
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	with tempfile.TemporaryDirectory() as tmp:
		generated, _ = generate_fixed_point(cache, Path(tmp))
		committed = ROOT / 'bootstrap/compiler.c'
		if args.check:
			check_committed(committed, generated)
		else:
			if committed.read_bytes() != generated:
				committed.write_bytes(generated)
				print('Updated bootstrap/compiler.c (fixtures not run)')
			else:
				print('bootstrap/compiler.c already current (fixtures not run)')


if __name__ == '__main__':
	main()
