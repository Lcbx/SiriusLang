#!/usr/bin/env python
"""Probe native modular MIR compilation against the committed C seed."""
import argparse
from pathlib import Path
import subprocess
import tempfile
from time import perf_counter

from project_paths import ROOT
from build_support import BuildCache, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--require-ready', action='store_true',
		help='return a failure status if the MIR self-hosting path is not ready')
	parser.add_argument('--timeout', type=float, default=None,
		help='maximum seconds allowed for modular C generation')
	args = parser.parse_args()
	if args.timeout is not None and args.timeout <= 0:
		parser.error('--timeout must be positive')
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	start = perf_counter()
	compiler = cache.native_tool('modular_cli')
	print(f'Native modular compiler build: {perf_counter() - start:.2f}s', flush=True)
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		start = perf_counter()
		try:
			result = subprocess.run([str(compiler), '--module-root', str(ROOT / 'src'), 'modular_cli', str(work / 'compiler.c')],
				capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=args.timeout)
		except subprocess.TimeoutExpired:
			print(f'BLOCKED modular C generation: exceeded {args.timeout:g}s', flush=True)
			if args.require_ready:
				raise SystemExit(1)
			return
		print(f'modular C generation: {perf_counter() - start:.2f}s', flush=True)
		if result.returncode:
			print('BLOCKED modular self-hosting:')
			print((result.stdout + result.stderr).strip())
			if args.require_ready:
				raise SystemExit(1)
			return
		if (work / 'compiler.c').read_bytes() != (ROOT / 'bootstrap/compiler.c').read_bytes():
			raise AssertionError('Native compiler C differs from the committed bootstrap')
		print('PASS modular compiler reproduces the committed C bootstrap')


if __name__ == '__main__':
	main()
