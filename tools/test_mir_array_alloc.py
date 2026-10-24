#!/usr/bin/env python3
"""Check native MIR shared-array append CoW and avoidable capacity growth."""
from pathlib import Path
import tempfile

from build_support import BuildCache, assert_native, execute, require_cc, run_native
from profile_mir_allocations import instrument, parse_report
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	compiler = cache.native_tool('modular_cli')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		source = ROOT / 'tests/mir/array_append_reserved.sirius'
		generated = work / 'array_append_reserved.c'
		execute([str(compiler), str(source), str(generated)])
		code = generated.read_text(encoding='utf-8')
		assert 's_mir_record_unique_owned_reserved(array, 1)' in code
		assert_native(run_native(cache, generated))
		profiled = work / 'profiled.c'
		profiled.write_text(instrument(code), encoding='utf-8')
		result = run_native(cache, profiled)
		assert_native(result)
		counts, by_type = parse_report(result.stderr.decode('utf-8', errors='replace'))
		if counts['append_clones'] != 2 or counts['growth'] != 0:
			raise AssertionError(f'Expected two cloned appends without realloc: {counts}')
		if sum(row['clones'] for row in by_type.values()) != counts['clones']:
			raise AssertionError('Per-type CoW clone counts do not match totals')
		print('PASS shared scalar/nested array append CoW and no redundant realloc')


if __name__ == '__main__':
	main()
