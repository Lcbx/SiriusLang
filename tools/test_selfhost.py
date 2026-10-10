#!/usr/bin/env python
"""Verify or regenerate the Sirius compiler using only the committed C bootstrap."""
import argparse
from itertools import zip_longest
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from emitter_cases import check_cases
from sirius_sources import ROOT, COMPILER_SOURCES, combined_sources


def execute(args: list[str]) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(args, capture_output=True, text=True, encoding='utf-8')
	if result.returncode:
		raise RuntimeError(f'{" ".join(args)}\n{result.stdout}{result.stderr}')
	return result


def compile_c(cc: str, source: Path, output: Path) -> None:
	execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic',
		'-I', str(ROOT / 'include'), str(source),
		str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(output)])


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
	args = parser.parse_args()
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('A C11 compiler (cc or gcc) is required')
	committed = ROOT / 'bootstrap_c/compiler.c'
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		source = work / 'compiler.sirius'
		# Byte-oriented source spans must not depend on Windows checkout line endings.
		source.write_bytes(combined_sources(COMPILER_SOURCES, separator=b''))

		seed = work / ('seed.exe' if sys.platform == 'win32' else 'seed')
		compile_c(cc, committed, seed)
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
			compile_c(cc, generated, binary)
			previous_c = generated
			current = binary
		if fixed_c is None or fixed_binary is None:
			raise AssertionError('Compiler C did not reach a fixed point in 8 generations')

		# The generated compiler has been tested, not merely text-compared.
		check_cases(cc, fixed_binary, work)
		generated_c = fixed_c.read_bytes()
		if args.update_bootstrap:
			committed.write_bytes(generated_c)
			print('Updated bootstrap_c/compiler.c')
		else:
			check_committed(committed, generated_c)


if __name__ == '__main__':
	main()
