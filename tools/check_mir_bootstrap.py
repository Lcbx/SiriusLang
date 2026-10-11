#!/usr/bin/env python
"""Probe whether MIR can compile the compiler itself without legacy fallback."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from sirius_sources import COMPILER_SOURCES, combined_sources
from test_build import BuildCache, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--require-ready', action='store_true',
		help='return a failure status if the MIR self-hosting path is not ready')
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = cache.sirius('mir-bootstrap-coverage', COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		source = work / 'compiler.sirius'
		source.write_bytes(combined_sources(COMPILER_SOURCES, separator=b''))
		result = subprocess.run([str(compiler), '--mir-c', str(source), str(work / 'compiler.c')],
			capture_output=True, text=True, encoding='utf-8', errors='replace')
		if result.returncode:
			print('BLOCKED MIR self-hosting:')
			print((result.stdout + result.stderr).strip())
			if args.require_ready:
				raise SystemExit(1)
			return
		print('PASS compiler source emitted through MIR; full MIR bootstrap fixed point is not yet verified')


if __name__ == '__main__':
	main()
