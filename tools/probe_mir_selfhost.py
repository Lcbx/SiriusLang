#!/usr/bin/env python
"""Probe whether MIR can compile the compiler itself without legacy fallback."""
import argparse
from pathlib import Path
import subprocess
import tempfile
from time import perf_counter

from source_manifest import MIR_ONLY_COMPILER_SOURCES, combined_sources
from build_support import BuildCache, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--require-ready', action='store_true',
		help='return a failure status if the MIR self-hosting path is not ready')
	parser.add_argument('--timeout', type=float, default=None,
		help='maximum seconds allowed for MIR-only C generation')
	args = parser.parse_args()
	if args.timeout is not None and args.timeout <= 0:
		parser.error('--timeout must be positive')
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	start = perf_counter()
	compiler = cache.sirius('mir-only-probe', MIR_ONLY_COMPILER_SOURCES, separator=b'')
	print(f'MIR-only compiler build: {perf_counter() - start:.2f}s', flush=True)
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		source = work / 'compiler.sirius'
		source.write_bytes(combined_sources(MIR_ONLY_COMPILER_SOURCES, separator=b''))
		start = perf_counter()
		try:
			result = subprocess.run([str(compiler), str(source), str(work / 'compiler.c')],
				capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=args.timeout)
		except subprocess.TimeoutExpired:
			print(f'BLOCKED MIR-only C generation: exceeded {args.timeout:g}s', flush=True)
			if args.require_ready:
				raise SystemExit(1)
			return
		print(f'MIR-only C generation: {perf_counter() - start:.2f}s', flush=True)
		if result.returncode:
			print('BLOCKED MIR-only self-hosting:')
			print((result.stdout + result.stderr).strip())
			if args.require_ready:
				raise SystemExit(1)
			return
		print('PASS MIR-only compiler source emitted through MIR; full bootstrap fixed point is not yet verified')


if __name__ == '__main__':
	main()
