#!/usr/bin/env python
"""Compile the Sirius parser using the C bootstrap; check AST cases."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import subprocess
import tempfile

from sirius_sources import ROOT, PARSER_SOURCES
from test_build import BuildCache, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	binary = cache.sirius('parser', PARSER_SOURCES + ('tests/parser/driver.sirius',))
	cases = sorted((ROOT / 'tests/parser/fixtures').glob('*.sirius'))
	if not cases:
		raise SystemExit('No parser test fixtures found')
	programs = sorted((ROOT / 'tests/fixtures').glob('*.sirius'))
	programs += [ROOT / 'src/tokenizer.sirius', ROOT / 'src/parser.sirius', ROOT / 'src/emit_c.sirius']

	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def golden(item: tuple[int, Path]) -> str:
			index, case = item
			fixture = work / f'fixture_{index}.sirius'
			fixture.write_bytes(case.read_bytes().replace(b'\r\n', b'\n'))
			result = subprocess.run([str(binary), str(fixture)], capture_output=True,
				text=True, encoding='utf-8')
			expected = (ROOT / 'tests/parser/expected_output' / (case.stem + '.txt')).read_text(encoding='utf-8')
			if result.returncode or result.stdout != expected:
				raise AssertionError(f'FAIL {case.name}: actual={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
			return f'PASS {case.name}'

		def smoke(case: Path) -> None:
			result = subprocess.run([str(binary), str(case)], capture_output=True,
				text=True, encoding='utf-8')
			if result.returncode or not result.stdout.strip().splitlines()[-1].startswith('ROOT '):
				raise AssertionError(f'FAIL parser smoke {case.name}: {result.stdout!r}\n{result.stderr}')

		with ThreadPoolExecutor(max_workers=validate_jobs(args.jobs)) as pool:
			for message in pool.map(golden, enumerate(cases)):
				print(message)
			list(pool.map(smoke, programs))
	print(f'{len(cases)} parser golden cases and {len(programs)} syntax smoke tests passed')


if __name__ == '__main__':
	main()
