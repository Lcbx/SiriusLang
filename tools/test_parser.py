"""Compile the Sirius parser using the committed C bootstrap; check AST cases."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def run(command: list[str]) -> None:
	result = subprocess.run(command, capture_output=True, text=True, encoding='utf-8')
	if result.returncode:
		raise SystemExit(f'FAILED: {" ".join(command)}\n{result.stdout}{result.stderr}')


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if not cc:
		raise SystemExit('A C11 compiler (cc or gcc) is required')

	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		combined = work / 'parser_test.sirius'
		combined.write_text('\n'.join((ROOT / name).read_text(encoding='utf-8') for name in (
			'src/tokenizer.sirius', 'src/parser.sirius', 'tests/parser/driver.sirius')),
			encoding='utf-8')
		c_source = work / 'parser.c'
		binary = work / ('parser.exe' if sys.platform == 'win32' else 'parser')
		seed = work / ('seed.exe' if sys.platform == 'win32' else 'seed')
		run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(ROOT / 'bootstrap_c/compiler.c'), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(seed)])
		run([str(seed), str(combined), str(c_source)])
		run([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I',
			str(ROOT / 'include'), str(c_source), str(ROOT / 'include/sirius_runtime.c'),
			'-lm', '-o', str(binary)])

		cases = sorted((ROOT / 'tests/parser/fixtures').glob('*.sirius'))
		if not cases:
			raise SystemExit('No parser test fixtures found')
		for case in cases:
			fixture = work / case.name
			fixture.write_bytes(case.read_bytes().replace(b'\r\n', b'\n'))
			result = subprocess.run([str(binary), str(fixture)], capture_output=True,
				text=True, encoding='utf-8')
			expected = (ROOT / 'tests/parser/expected_output' / (case.stem + '.txt')).read_text(encoding='utf-8')
			if result.returncode or result.stdout != expected:
				raise SystemExit(f'FAIL {case.name}: actual={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}')
			print(f'PASS {case.name}')

		# Syntax smoke tests on the stage-0 compiler's complete positive corpus.
		programs = sorted((ROOT / 'tests/fixtures').glob('*.sirius'))
		programs += [ROOT / 'src/tokenizer.sirius', ROOT / 'src/parser.sirius', ROOT / 'src/emit_c.sirius']
		for case in programs:
			result = subprocess.run([str(binary), str(case)], capture_output=True,
				text=True, encoding='utf-8')
			if result.returncode or not result.stdout.strip().splitlines()[-1].startswith('ROOT '):
				raise SystemExit(f'FAIL parser smoke {case.name}: {result.stdout!r}\n{result.stderr}')
		print(f'{len(cases)} parser golden cases and {len(programs)} syntax smoke tests passed')


if __name__ == '__main__':
	main()
