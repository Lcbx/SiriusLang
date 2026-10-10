#!/usr/bin/env python
"""Compile/run independent C references; not Sirius compiler tests."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
from pathlib import Path
import subprocess
import sys
import tempfile

from sirius_sources import ROOT
from test_build import BuildCache, C_FLAGS, execute, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	runtime = cache.runtime()
	cases = sorted((ROOT / 'tests/expected_c').glob('*.c'))
	if not cases:
		raise AssertionError('No expected C references found')

	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def check(item: tuple[int, Path]) -> str:
			index, source = item
			stem = source.stem
			fixture = ROOT / 'tests/fixtures' / f'{stem}.sirius'
			stdout = ROOT / 'tests/expected_output' / f'{stem}.txt'
			if not fixture.is_file() or not stdout.is_file():
				raise AssertionError(f'Missing expected C/output for {stem}')
			folder = work / f'case_{index}'
			folder.mkdir()
			binary = folder / ('program.exe' if sys.platform == 'win32' else 'program')
			execute([cache.cc, *C_FLAGS, '-I', str(ROOT / 'include'), str(source), str(runtime),
				'-lm', '-o', str(binary)])
			result = subprocess.run([str(binary)], text=True, encoding='utf-8', capture_output=True, cwd=folder)
			expected = stdout.read_text(encoding='utf-8')
			if result.returncode or result.stdout != expected:
				raise AssertionError(f'FAILED {stem}: exit={result.returncode} stdout={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
			return f'PASS {stem}'

		def panic(item: tuple[int, tuple[str, dict]]) -> str:
			index, (stem, expected) = item
			folder = work / f'panic_{index}'
			folder.mkdir()
			binary = folder / ('program.exe' if sys.platform == 'win32' else 'program')
			execute([cache.cc, *C_FLAGS, '-I', str(ROOT / 'include'),
				str(ROOT / 'tests/runtime_errors' / f'{stem}.c'), str(runtime), '-lm', '-o', str(binary)])
			result = subprocess.run([str(binary)], capture_output=True, text=True, encoding='utf-8', cwd=folder)
			if result.returncode != expected['exit'] or expected['stderr_contains'] not in result.stderr:
				raise AssertionError(f'Runtime failure mismatch {stem}: {result}')
			return f'PASS {stem} (expected runtime panic)'

		errors = json.loads((ROOT / 'tests/runtime_errors/expectations.json').read_text(encoding='utf-8'))
		with ThreadPoolExecutor(max_workers=validate_jobs(args.jobs)) as pool:
			for message in pool.map(check, enumerate(cases)):
				print(message)
			for message in pool.map(panic, enumerate(errors.items())):
				print(message)
	print(f'Verified {len(cases)} C reference examples')


if __name__ == '__main__':
	main()
