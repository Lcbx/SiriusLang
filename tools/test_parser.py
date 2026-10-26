#!/usr/bin/env python
"""Run parser golden and syntax smoke tests through native imports."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import subprocess
import tempfile

from project_paths import ROOT, normalize_newlines
from build_support import BuildCache, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	binary = cache.native_driver('parser', ROOT / 'tests/parser/module_driver.sirius')
	cases = sorted((ROOT / 'tests/parser/fixtures').glob('*.sirius'))
	if not cases:
		raise SystemExit('No parser test fixtures found')
	programs = sorted((ROOT / 'tests/language/fixtures').glob('*.sirius'))
	programs += sorted((ROOT / 'src').glob('*.sirius'))

	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def golden(item: tuple[int, Path]) -> str:
			index, case = item
			fixture = work / f'fixture_{index}.sirius'
			fixture.write_bytes(normalize_newlines(case.read_bytes()))
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
