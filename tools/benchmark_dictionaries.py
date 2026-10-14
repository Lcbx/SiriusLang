#!/usr/bin/env python
"""Benchmark generic S_Dict against linear lookup and managed updates (not a compiler benchmark)."""
import argparse
import csv
from io import StringIO
from pathlib import Path
import shutil
import statistics
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def run_benchmark(cc: str, name: str, runs: int, directory: Path) -> None:
	binary = directory / name
	subprocess.run([cc, '-O2', '-std=c11', '-I', str(ROOT / 'runtime'),
		str(ROOT / 'benchmarks' / f'{name}.c'), str(ROOT / 'runtime/sirius_runtime.c'),
		'-o', str(binary)], check=True)
	measurements: dict[tuple[str, str], dict[str, list[float]]] = {}
	for _ in range(runs):
		result = subprocess.run([str(binary)], text=True, capture_output=True, check=True)
		for record in csv.DictReader(StringIO(result.stdout)):
			key = (record['size'], record.get('operation', record.get('scenario', '')))
			entry = measurements.setdefault(key, {})
			for field, value in record.items():
				if field in ('size', 'operation', 'scenario', 'operations'):
					continue
				entry.setdefault(field, []).append(float(value))
	print(f'\n{name} (median of {runs} runs; ns/operation)')
	for key, values in measurements.items():
		print(f'{key[0]:>5} {key[1]:<14} ' + ' '.join(f'{field}={statistics.median(times):.1f}' for field, times in values.items()))


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=shutil.which('cc') or 'cc')
	parser.add_argument('--runs', type=int, default=3)
	args = parser.parse_args()
	if args.runs < 1:
		parser.error('--runs must be positive')
	with tempfile.TemporaryDirectory() as path:
		for name in ('dictionary', 'dictionary_managed'):
			run_benchmark(args.cc, name, args.runs, Path(path))


if __name__ == '__main__':
	main()
