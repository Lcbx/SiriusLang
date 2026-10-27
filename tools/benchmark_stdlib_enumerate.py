#!/usr/bin/env python3
"""Compare one-pass Std.Functional.enumerate with the old two-pass algorithm.

Both variants use the same compiler, array runtime and stdlib build.  The
reference is kept here so the benchmark does not depend on an old Git checkout.
"""
import argparse
from pathlib import Path
import shutil
import statistics
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, executable_path


REFERENCE = '''def two_pass(values)
	var result = F.map(values, fn(value) {index = 0, value = value})
	for i in 0..len(result)
		result[!i].index = i
	return result
'''


def program(implementation: str, size: int, iterations: int) -> str:
	return f'''import Std.Functional alias F

{REFERENCE}
def main() -> i64
	let values = F.generate({size}, fn(i) i)
	var checksum = 0
	for rep in 0..{iterations}
		let indexed = {implementation}(values)
		checksum += indexed[!{size - 1}].index + indexed[!{size - 1}].value
	if checksum != {2 * (size - 1) * iterations}
		return 1
	return 42
'''


def build(cache: BuildCache, root: Path, name: str, size: int, iterations: int,
	optimization: int) -> Path:
	(root / 'Main.sirius').write_text(program(name, size, iterations))
	generated = root / f'{name}.c'
	subprocess.run([str(cache.seed()), '--module-root', str(root), 'Main', str(generated)],
		check=True, capture_output=True, timeout=120)
	binary = executable_path(root / name)
	flags = tuple(f'-O{optimization}' if flag == '-O0' else flag for flag in C_FLAGS)
	subprocess.run([cache.cc, *flags, '-Wno-unused-function', '-I', str(ROOT / 'runtime'),
		str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc),
		'-o', str(binary)], check=True, capture_output=True, timeout=120)
	return binary


def sample(binary: Path, runs: int) -> float:
	times = []
	for _ in range(runs + 1):
		start = perf_counter()
		process = subprocess.run([str(binary)], capture_output=True, timeout=120)
		elapsed = perf_counter() - start
		if process.returncode != 42 or process.stderr or process.stdout:
			raise RuntimeError(f'{binary.name}: unexpected result {process!r}')
		times.append(elapsed)
	return statistics.median(times[1:])


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runs', type=int, default=5)
	parser.add_argument('--size', type=int, default=2048)
	parser.add_argument('--iterations', type=int, default=3000)
	parser.add_argument('--opt', type=int, choices=(0, 1, 2, 3), default=2)
	args = parser.parse_args()
	if args.runs < 1 or args.size < 1 or args.iterations < 1:
		parser.error('runs, size and iterations must be positive')
	if 2 * (args.size - 1) * args.iterations > 2**63 - 1:
		parser.error('checksum would overflow i64')
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-enumerate-benchmark-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Functional.sirius', root / 'Std/Functional.sirius')
		before = build(cache, root, 'two_pass', args.size, args.iterations, args.opt)
		after = build(cache, root, 'F.enumerate', args.size, args.iterations, args.opt)
		old_time = sample(before, args.runs)
		new_time = sample(after, args.runs)
		print(f'{cache.cc} -O{args.opt}, {args.size} elements x {args.iterations} iterations; '
			f'median of {args.runs} runs, one warm-up:')
		print(f'  two-pass: {old_time * 1000:.1f} ms')
		print(f'  one-pass: {new_time * 1000:.1f} ms ({old_time / new_time:.2f}x speedup)')


if __name__ == '__main__':
	main()
