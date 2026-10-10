"""Test the Sirius canonical type table using only the committed C bootstrap."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from sirius_sources import ROOT, combined_sources


def execute(args: list[str]) -> str:
	result = subprocess.run(args, capture_output=True, text=True, encoding='utf-8')
	if result.returncode:
		raise RuntimeError(f'{" ".join(args)}\n{result.stdout}{result.stderr}')
	return result.stdout


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('C11 compiler required (cc or gcc)')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		seed = work / ('seed.exe' if sys.platform == 'win32' else 'seed')
		base = [cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic',
			'-I', str(ROOT / 'include')]
		execute(base + [str(ROOT / 'bootstrap_c/compiler.c'), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(seed)])
		source = work / 'types.sirius'
		source.write_bytes(combined_sources(('src/types.sirius', 'tests/types/driver.sirius')))
		generated = work / 'types.c'
		execute([str(seed), str(source), str(generated)])
		binary = work / ('test_types.exe' if sys.platform == 'win32' else 'test_types')
		execute(base + [str(generated), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(binary)])
		output = execute([str(binary)])
		if output != 'PASS canonical type descriptors\n':
			raise AssertionError(f'Unexpected type-table output: {output!r}')
		print('PASS canonical type table')


if __name__ == '__main__':
	main()
