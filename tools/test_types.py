#!/usr/bin/env python
"""Test Sirius type interning and typed HIR using the committed C bootstrap."""
import argparse
from concurrent.futures import ThreadPoolExecutor

from project_paths import ROOT
from build_support import BuildCache, execute, options, require_cc, validate_jobs


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	cases = (
		('canonical', 'native_driver.sirius', 'PASS canonical type descriptors\n'),
		('union', 'union_driver_native.sirius', 'PASS structural union types and exhaustive match checking\n'),
		('union-source', 'union_source_driver_native.sirius', 'PASS source tagged union aliases and HIR match analysis\n'),
		('hir', 'hir_driver_native.sirius', 'PASS typed HIR resolution\n'),
		('specializations', 'specialization_driver_native.sirius', 'PASS HIR specialization registry\n'),
		('worklist', 'worklist_driver_native.sirius', 'PASS HIR worklist without backend\n'),
		('semantic', 'semantic_driver_native.sirius', 'PASS canonical semantic snapshots\n'),
	)

	def check(case: tuple[str, str, str]) -> str:
		name, source, expected = case
		binary = cache.native_driver('type-' + name, ROOT / 'tests/types' / source)
		output = execute([str(binary)]).stdout
		if output != expected:
			raise AssertionError(f'Unexpected {name} output: {output!r}')
		if name == 'hir':
			return 'PASS typed HIR resolution'
		if name == 'specializations':
			return 'PASS HIR specialization registry'
		if name == 'worklist':
			return 'PASS HIR worklist without backend'
		if name == 'semantic':
			return 'PASS canonical semantic snapshots'
		return f'PASS {name} type table'

	with ThreadPoolExecutor(max_workers=validate_jobs(args.jobs)) as pool:
		for message in pool.map(check, cases):
			print(message)



if __name__ == '__main__':
	main()
