#!/usr/bin/env python
"""Compile the Sirius tokenizer using the C bootstrap; check golden tokens."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import subprocess
import tempfile

from sirius_sources import ROOT, TOKENIZER_SOURCES, normalize_newlines
from test_build import BuildCache, options, require_cc, validate_jobs


def fixture_bytes(case: Path) -> bytes:
	# Byte offsets refer to canonical fixture bytes, even on Windows checkouts.
	data = normalize_newlines(case.read_bytes())
	if case.name == '08_crlf.sirius':
		return data.replace(b'\n', b'\r\n')
	if case.name == '18_cr.sirius':
		return data.replace(b'\r', b'\n').replace(b'\n', b'\r')
	return data


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	binary = cache.sirius('tokenizer', TOKENIZER_SOURCES + ('tests/tokenizer/driver.sirius',))
	cases = sorted((ROOT / 'tests/tokenizer/fixtures').glob('*.sirius'))
	if not cases:
		raise SystemExit('No tokenizer test cases found')
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def check(item: tuple[int, Path]) -> str:
			index, case = item
			fixture = work / f'fixture_{index}.sirius'
			fixture.write_bytes(fixture_bytes(case))
			result = subprocess.run([str(binary), str(fixture)], capture_output=True,
				text=True, encoding='utf-8')
			expected = (ROOT / 'tests/tokenizer/expected_output' / (case.stem + '.txt')).read_text(encoding='utf-8')
			if result.returncode or result.stdout != expected:
				raise AssertionError(f'FAIL {case.name}: actual={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
			return f'PASS {case.name}'
		with ThreadPoolExecutor(max_workers=validate_jobs(args.jobs)) as pool:
			for message in pool.map(check, enumerate(cases)):
				print(message)
	print(f'{len(cases)} tokenizer cases passed')


if __name__ == '__main__':
	main()
