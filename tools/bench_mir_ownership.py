#!/usr/bin/env python3
"""Compare ownership solver implementations using identical compiler and C flags.

Pass --baseline-source with a previous mir_ownership.sirius to compare against
current sources. Build times and execution times are reported separately.
"""
import argparse
from pathlib import Path
import statistics
import shutil
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, compile_c, require_cc
from project_paths import ROOT


def timed_run(command: list[str], timeout: int) -> float:
	start = perf_counter()
	subprocess.run(command, check=True, capture_output=True, timeout=timeout)
	return perf_counter() - start


def benchmark(label: str, ownership: bytes, work: Path, seed: Path, cache: BuildCache,
		repeats: int, timeout: int) -> tuple[float, ...]:
	root = work / f'{label}-modules'
	root.mkdir()
	for path in (ROOT / 'src').glob('*.sirius'):
		shutil.copy2(path, root / path.name)
	(root / 'mir_ownership.sirius').write_bytes(ownership)
	generated = work / f'{label}.c'
	binary = work / f'{label}.exe'
	generation = timed_run([str(seed), '--module-root', str(root), 'modular_cli', str(generated)], timeout)
	start = perf_counter()
	compile_c(cache.cc, generated, binary, cache.runtime())
	native = perf_counter() - start
	times = []
	for repeat in range(repeats):
		output = work / f'{label}-{repeat}.c'
		times.append(timed_run([str(binary), '--module-root', str(root), 'modular_cli', str(output)], timeout))
		if output.read_bytes() != generated.read_bytes():
			raise RuntimeError(f'{label}: self-compilation output differs from the compiled source')
	print(f'{label}: generate={generation:.3f}s native={native:.3f}s '
		f'self_median={statistics.median(times):.3f}s C_bytes={generated.stat().st_size:,}', flush=True)
	return tuple(times)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--baseline-source', type=Path, help='Previous mir_ownership.sirius')
	parser.add_argument('--repeats', type=int, default=3)
	parser.add_argument('--timeout', type=int, default=180)
	args = parser.parse_args()
	if args.repeats < 1:
		parser.error('--repeats must be positive')
	cache = BuildCache(require_cc())
	seed = cache.seed()
	current = (ROOT / 'src/mir_ownership.sirius').read_bytes()
	with tempfile.TemporaryDirectory(prefix='sirius-ownership-bench-') as tmp:
		work = Path(tmp)
		if args.baseline_source:
			before = benchmark('baseline', args.baseline_source.read_bytes(), work, seed, cache,
				args.repeats, args.timeout)
			after = benchmark('current', current, work, seed, cache, args.repeats, args.timeout)
			print(f'current / baseline execution ratio: '
				f'{statistics.median(after) / statistics.median(before):.3f}', flush=True)
		else:
			benchmark('current', current, work, seed, cache, args.repeats, args.timeout)


if __name__ == '__main__':
	main()
