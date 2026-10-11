#!/usr/bin/env python
"""Verify or regenerate the Sirius compiler using only the committed C bootstrap."""
import argparse
from itertools import zip_longest
from pathlib import Path
import sys
import tempfile

from emitter_cases import check_cases
from test_build import BuildCache, C_FLAGS, LINK_FLAGS, execute, options, require_cc, validate_jobs
from sirius_sources import ROOT, COMPILER_SOURCES, combined_sources


def compile_c(cc: str, source: Path, output: Path, runtime: Path) -> None:
	execute([cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source),
		str(runtime), '-lm', '-o', str(output)])


def canonical_bytes(path: Path) -> bytes:
	return path.read_bytes().replace(b'\r\n', b'\n')



def check_committed(committed: Path, generated: bytes) -> None:
	checked_in = canonical_bytes(committed)
	if checked_in == generated:
		print('PASS committed bootstrap C matches')
		return
	for line, (expected, actual) in enumerate(zip_longest(
		checked_in.split(b'\n'), generated.split(b'\n')), 1):
		if expected != actual:
			raise AssertionError(
				f'Committed bootstrap C differs at line {line}. '
				'Run python tools/test_selfhost.py --update-bootstrap after checking changes.\n'
				f'committed: {str((expected or b"")[:160])}\n'
				f'generated: {str((actual or b"")[:160])}')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--update-bootstrap', action='store_true',
		help='replace bootstrap_c/compiler.c after fixed-point and executable checks')
	options(parser)
	args = parser.parse_args()
	cc = require_cc()
	cache = BuildCache(cc, rebuild=args.rebuild)
	committed = ROOT / 'bootstrap_c/compiler.c'
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		source = work / 'compiler.sirius'
		# Byte-oriented source spans must not depend on Windows checkout line endings.
		source.write_bytes(combined_sources(COMPILER_SOURCES, separator=b''))

		seed = cache.seed()
		previous_c = None
		current = seed
		fixed_c = None
		fixed_binary = None
		# The old compiler can generate different C from a newer compiler. Iterate
		# until the newly generated compiler reproduces its own source exactly.
		for generation in range(1, 9):
			generated = work / f'generation_{generation}.c'
			execute([str(current), str(source), str(generated)])
			if previous_c is not None and generated.read_bytes() == previous_c.read_bytes():
				fixed_c = generated
				fixed_binary = current
				print(f'PASS C bootstrap fixed point at generation {generation}')
				break
			binary = work / (f'generation_{generation}.exe' if sys.platform == 'win32' else f'generation_{generation}')
			compile_c(cc, generated, binary, cache.runtime())
			previous_c = generated
			current = binary
		if fixed_c is None or fixed_binary is None:
			raise AssertionError('Compiler C did not reach a fixed point in 8 generations')

		# The generated compiler has been tested, not merely text-compared.
		check_cases(cc, fixed_binary, work, jobs=validate_jobs(args.jobs), runtime=cache.runtime())
		generated_c = fixed_c.read_bytes()
		if args.update_bootstrap:
			committed.write_bytes(generated_c)
			print('Updated bootstrap_c/compiler.c')
		else:
			check_committed(committed, generated_c)


if __name__ == '__main__':
	main()
