#!/usr/bin/env python
"""Shared self-hosted C generation and byte-for-byte bootstrap verification."""
from itertools import zip_longest
from pathlib import Path
import sys

from build_support import BuildCache, compile_c, execute
from project_paths import ROOT, normalize_newlines


def generate_fixed_point(cache: BuildCache, work: Path, *, max_generations: int = 8) -> tuple[bytes, Path]:
	"""Return self-reproducing C and its compiler binary, valid while work exists."""
	current = cache.seed()
	previous = None
	for generation in range(1, max_generations + 1):
		generated = work / f'generation_{generation}.c'
		execute([str(current), '--module-root', str(ROOT / 'src'), 'modular_cli', str(generated)])
		result = generated.read_bytes()
		if previous == result:
			print(f'PASS C bootstrap fixed point at generation {generation}')
			return result, current
		binary = work / (f'generation_{generation}.exe' if sys.platform == 'win32' else f'generation_{generation}')
		compile_c(cache.cc, generated, binary, cache.runtime())
		current = binary
		previous = result
	raise AssertionError(f'Compiler C did not reach a fixed point in {max_generations} generations')


def check_committed(committed: Path, generated: bytes) -> None:
	checked_in = normalize_newlines(committed.read_bytes())
	if checked_in == generated:
		print('PASS committed bootstrap C matches')
		return
	for line, (expected, actual) in enumerate(zip_longest(
		checked_in.split(b'\n'), generated.split(b'\n')), 1):
		if expected != actual:
			raise AssertionError(
				f'Committed bootstrap C differs at line {line}. '
				'Run python tools/generate_bootstrap.py after checking changes.\n'
				f'committed: {str((expected or b"")[:160])}\n'
				f'generated: {str((actual or b"")[:160])}')
