#!/usr/bin/env python3
"""Compare ownership solver implementations using identical compiler and C flags.

Pass --baseline-source with a previous mir_ownership.sirius to compare against
current sources. Build times and execution times are reported separately.
"""
import argparse
from pathlib import Path
import statistics
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, compile_c, require_cc
from source_manifest import MIR_ONLY_COMPILER_SOURCES, ROOT, normalize_newlines


def timed_run(command: list[str], timeout: int) -> float:
	start = perf_counter()
	subprocess.run(command, check=True, capture_output=True, timeout=timeout)
	return perf_counter() - start


def build_source(ownership: bytes) -> bytes:
	parts = []
	for name in MIR_ONLY_COMPILER_SOURCES:
		if name == 'src/mir_ownership.sirius':
			parts.append(normalize_newlines(ownership))
		else:
			parts.append(normalize_newlines((ROOT / name).read_bytes()))
	return b''.join(parts)


def benchmark(label: str, ownership: bytes, work: Path, seed: Path, cache: BuildCache,
		repeats: int, timeout: int) -> tuple[float, ...]:
	source = work / f'{label}.sirius'
	generated = work / f'{label}.c'
	binary = work / f'{label}.exe'
	source.write_bytes(build_source(ownership))
	generation = timed_run([str(seed), str(source), str(generated)], timeout)
	start = perf_counter()
	compile_c(cache.cc, generated, binary, cache.runtime())
	native = perf_counter() - start
	times = []
	for repeat in range(repeats):
		output = work / f'{label}-{repeat}.c'
		times.append(timed_run([str(binary), str(source), str(output)], timeout))
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
