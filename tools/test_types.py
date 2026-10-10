"""Test the Sirius canonical type table using only the committed C bootstrap."""
from pathlib import Path
import re
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
		for name, sources, expected in (
			('canonical', ('src/types.sirius', 'tests/types/driver.sirius'), 'PASS canonical type descriptors\n'),
			('emitter', ('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/hir.sirius', 'src/emit_c.sirius', 'tests/types/emitter_driver.sirius'), 'PASS canonical emitter types\n'),
		):
			source = work / f'{name}.sirius'
			source.write_bytes(combined_sources(sources))
			generated = work / f'{name}.c'
			execute([str(seed), str(source), str(generated)])
			binary = work / (f'{name}.exe' if sys.platform == 'win32' else name)
			execute(base + [str(generated), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(binary)])
			output = execute([str(binary)])
			if output != expected:
				raise AssertionError(f'Unexpected {name} output: {output!r}')
			print(f'PASS {name} type table')
		# The new HIR checks are independently exercised without the C emitter.
		source = work / 'hir.sirius'
		source.write_bytes(combined_sources(('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/hir.sirius', 'tests/types/hir_driver.sirius')))
		generated = work / 'hir.c'
		execute([str(seed), str(source), str(generated)])
		binary = work / ('hir.exe' if sys.platform == 'win32' else 'hir')
		execute(base + [str(generated), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(binary)])
		if execute([str(binary)]) != 'PASS typed HIR resolution\n':
			raise AssertionError('Typed HIR resolution failed')
		print('PASS typed HIR resolution')
		# Structurally identical records must share one generic specialization.
		source = ROOT / 'tests/emitter/fixtures/11_generic_records.sirius'
		generated = work / 'generic_record.c'
		execute([str(seed), str(source), str(generated)])
		symbols = set(re.findall(r'\bs_sum_xy__\d+\b', generated.read_text(encoding='utf-8')))
		if len(symbols) != 1:
			raise AssertionError(f'Expected one structural-record specialization; found {symbols}')
		print('PASS canonical generic specialization cache')


if __name__ == '__main__':
	main()
