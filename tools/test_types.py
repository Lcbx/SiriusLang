#!/usr/bin/env python
"""Test Sirius type interning and typed HIR using the committed C bootstrap."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import re
import tempfile

from source_manifest import ROOT
from build_support import BuildCache, execute, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	cases = (
		('canonical', ('src/types.sirius', 'tests/types/driver.sirius'), 'PASS canonical type descriptors\n'),
		('emitter', ('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/compile_common.sirius', 'src/hir.sirius', 'src/emit_c.sirius', 'tests/types/emitter_driver.sirius'), 'PASS canonical emitter types\n'),
		('hir', ('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/hir.sirius', 'tests/types/hir_driver.sirius'), 'PASS typed HIR resolution\n'),
		('specializations', ('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/hir.sirius', 'tests/types/specialization_driver.sirius'), 'PASS HIR specialization registry\n'),
		('worklist', ('src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius', 'src/hir.sirius', 'src/hir_specialize.sirius', 'tests/types/worklist_driver.sirius'), 'PASS HIR worklist without backend\n'),
	)
	def check(case: tuple[str, tuple[str, ...], str]) -> str:
		name, sources, expected = case
		binary = cache.sirius('type-' + name, sources)
		output = execute([str(binary)]).stdout
		if output != expected:
			raise AssertionError(f'Unexpected {name} output: {output!r}')
		if name == 'hir':
			return 'PASS typed HIR resolution'
		if name == 'specializations':
			return 'PASS HIR specialization registry'
		if name == 'worklist':
			return 'PASS HIR worklist without backend'
		return f'PASS {name} type table'

	with ThreadPoolExecutor(max_workers=validate_jobs(args.jobs)) as pool:
		for message in pool.map(check, cases):
			print(message)

	# The generated symbol set is inspected, not the textual layout of the C.
	with tempfile.TemporaryDirectory() as tmp:
		generated = Path(tmp) / 'generic_record.c'
		execute([str(cache.seed()), str(ROOT / 'tests/compiler/fixtures/11_generic_records.sirius'), str(generated)])
		symbols = set(re.findall(r'\bs_sum_xy__\d+\b', generated.read_text(encoding='utf-8')))
		if len(symbols) != 1:
			raise AssertionError(f'Expected one structural-record specialization; found {symbols}')
		print('PASS canonical generic specialization cache')


if __name__ == '__main__':
	main()
