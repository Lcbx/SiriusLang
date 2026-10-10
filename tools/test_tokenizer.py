#!/usr/bin/env python
"""Compile the Sirius tokenizer using the committed C bootstrap; check golden tokens."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from sirius_sources import ROOT, TOKENIZER_SOURCES, combined_sources


def run(command: list[str]) -> None:
	result = subprocess.run(command, capture_output=True, text=True)
	if result.returncode:
		raise SystemExit(f'FAILED: {" ".join(command)}\n{result.stdout}{result.stderr}')


def fixture_bytes(case: Path) -> bytes:
	# Byte offsets in golden streams refer to canonical fixture bytes. Git may
	# check out ordinary text files with CRLF on Windows.
	data = case.read_bytes().replace(b'\r\n', b'\n')
	if case.name == '08_crlf.sirius':
		return data.replace(b'\n', b'\r\n')
	if case.name == '18_cr.sirius':
		return data.replace(b'\r', b'\n').replace(b'\n', b'\r')
	return data


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if not cc:
		raise SystemExit('A C11 compiler (cc or gcc) is required')

	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		combined = work / 'tokenizer_test.sirius'
		combined.write_bytes(combined_sources(TOKENIZER_SOURCES + ('tests/tokenizer/driver.sirius',)))
		c_source = work / 'tokenizer.c'
		binary = work / ('tokenizer.exe' if sys.platform == 'win32' else 'tokenizer')
		seed = work / ('seed.exe' if sys.platform == 'win32' else 'seed')
		run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(ROOT / 'bootstrap_c/compiler.c'), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(seed)])
		run([str(seed), str(combined), str(c_source)])
		run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(c_source), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(binary)])

		cases = sorted((ROOT / 'tests/tokenizer/fixtures').glob('*.sirius'))
		if not cases:
			raise SystemExit('No tokenizer test cases found')
		for case in cases:
			fixture = work / case.name
			fixture.write_bytes(fixture_bytes(case))
			result = subprocess.run([str(binary), str(fixture)], capture_output=True,
				text=True, encoding='utf-8')
			expected = (ROOT / 'tests/tokenizer/expected_output' / (case.stem + '.txt')).read_text(encoding='utf8')
			if result.returncode or result.stdout != expected:
				raise SystemExit(f'FAIL {case.name}: actual={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
			print(f'PASS {case.name}')
		print(f'{len(cases)} tokenizer cases passed')


if __name__ == '__main__':
	main()
