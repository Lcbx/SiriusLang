#!/usr/bin/env python
"""Run verified tagged-union MIR through interpreter and native C."""
import tempfile
from pathlib import Path

from project_paths import ROOT
from build_support import BuildCache, check_native, execute, require_cc


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-union', ROOT / 'tests/mir/union_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		output = Path(directory) / 'union.c'
		bad_output = Path(directory) / 'union-mismatched.c'
		result = execute([str(driver), str(output), str(bad_output)])
		if result.stdout != 'PASS tagged union MIR runtime\n':
			raise AssertionError(result.stdout)
		check_native(cache, output, code=42, stdout=b'')
		check_native(cache, bad_output, code=None, stderr_contains=b'union variant mismatch')
		compiler = cache.seed()
		for name, status in (('success', 42), ('failure', 15), ('direct', 37), ('positional', 42), ('fallthrough', 15), ('managed', 10), ('same_shape', 31), ('empty_labels', 123)):
			sirius = ROOT / 'tests/mir' / f'source_union_{name}.sirius'
			source = Path(directory) / f'source-union-{name}.c'
			execute([str(compiler), str(sirius), str(source)])
			check_native(cache, source, code=status)
		ambiguous = ROOT / 'tests/mir/source_union_ambiguous_record.sirius'
		bad = Path(directory) / 'ambiguous.c'
		from subprocess import run
		result = run([str(compiler), str(ambiguous), str(bad)], capture_output=True, text=True)
		if result.returncode == 0 or 'expected union, found record' not in result.stderr:
			raise AssertionError(f'expected ambiguous record injection rejection: {result}')
	# Same-shaped labeled alternatives require explicit tags in both backends.
	labeled_driver = cache.native_driver('mir-labeled-union', ROOT / 'tests/mir/labeled_union_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		valid = Path(directory) / 'labeled.c'
		invalid = Path(directory) / 'labeled-invalid.c'
		result = execute([str(labeled_driver), str(valid), str(invalid)])
		if result.stdout != 'PASS labeled union MIR tags, interpreter, serialization and ownership\n':
			raise AssertionError(result.stdout)
		check_native(cache, valid, code=4)
		check_native(cache, invalid, code=None, stderr_contains=b'union variant mismatch')
	print('PASS union MIR and Sirius source: ownership, interpreter and native C')


if __name__ == '__main__':
	main()
