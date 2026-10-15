#!/usr/bin/env python
"""Shared runtime and diagnostic cases for Sirius-written compiler generations."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import subprocess
import sys

from build_support import BuildCache, compile_c, execute, validate_jobs

from source_manifest import ROOT, normalize_newlines

REFERENCE = ('01_scalars', '02_branches', '03_loops', '04_recursion', '38_module_constants', '29_strings_and_arguments', '36_string_byte_at', '06_constructor', '07_record_spread', '10_generic', '05_records', '20_structural_layout', '17_dict')
ERRORS = {
	'01_immutable': 'cannot assign immutable',
	'02_type_mismatch': 'expected bool, found i64',
	'03_missing_name': "unknown name 'missing'",
	'04_unsupported_array': 'expected i64, found f64',
	'06_loop_control': 'loop control outside loop',
	'07_wrong_arguments': 'argument count mismatch',
	'08_constant_forward': 'unsupported module constant initializer',
	'09_bad_recur': 'recur must call the enclosing function',
	'10_duplicate_param': 'duplicate parameter',
	'11_missing_record_field': "unknown record field 'y'",
	'12_record_immutable': 'cannot assign immutable',
	'13_record_type_mismatch': 'expected i64, found bool',
	'14_deleted_missing_field': "unknown deleted field 'y'",
	'15_managed_record': 'invalid record field type',
	'16_constructor_override': 'constructor field overrides are not supported yet',
	'18_empty_record': 'empty records are not supported yet',
	'19_generic_wrong_annotation': 'expected i64, found bool',
	'20_recursive_unannotated_generic': 'recursive generic call requires an explicit return type',
	'21_generic_no_return': 'cannot infer function return type',
	'22_immutable_array_update': 'indexed update requires a mutable local array',
	'23_array_wrong_element': 'expected i64, found string',
	'24_array_element_type': 'expected string, found i64',
	'25_dict_wrong_value': 'expected string, found i64',
	'26_optional_wrong_fallback': 'expected string, found i64',
	'27_optional_elif': 'optional fallback in elif requires CFG lowering',
	'28_string_method_wrong_type': 'expected i64, found string',
	'29_write_text_wrong_type': 'expected string, found i64',
	'30_string_join_wrong_type': 'expected Array[string], found Array[i64]',
	'31_string_slice_bad_arity': 'string method argument count mismatch',
	'32_def_constructor': 'def constructors are no longer supported; use rec',
	'33_duplicate_module_name': "duplicate function 'VALUE'",
	'34_hir_scope_escape': "unknown name 'hidden'",
	'35_guard_type': 'expected bool, found i64',
	'36_guard_exit': 'guard else requires return, break, or continue',
	'37_unwrap_type': 'unboxing requires an Optional',
	'38_legacy_index': "expected ']', found '?'",
	'39_field_append_wrong_type': 'expected i64, found string',
	'40_destructure_unknown_field': "unknown record field 'nonexistent'",
	'41_destructure_duplicate_name': "duplicate binding 'x'",
	'42_destructure_nonrecord': 'record destructuring requires a record',
	'43_destructure_scope': "unknown name 'local'",
	'44_destructure_empty': 'record destructuring requires at least one field',
	'45_var_destructure': 'record destructuring requires let',
	'46_nested_immutable': "cannot assign immutable binding 'x'",
	'47_nested_missing_field': "unknown record field 'other'",
	'48_nested_wrong_value': 'expected i64, found string',
	'49_nested_bad_index': 'expected i64, found string',
	'51_nested_invalid_augmented_type': 'unsupported MIR augmented assignment',
	'52_generic_local_wrong_operator': 'arithmetic requires numeric operands',
	'53_generic_local_duplicate': "duplicate binding 'value'",

}



def check_cases(cc: str, compiler: Path, work: Path, jobs: int = 4,
	runtime: Path | None = None, positive_exclude: tuple[str, ...] = (),
	negative_exclude: tuple[str, ...] = (),
	expected_errors: dict[str, str] | None = None) -> tuple[int, int]:
	"""Test independent cases concurrently, reporting in deterministic fixture order."""
	jobs = validate_jobs(jobs)
	positives = [(ROOT / 'tests/fixtures' / (name + '.sirius'),
		ROOT / 'tests/expected_output' / (name + '.txt')) for name in REFERENCE]
	positives += [(src, ROOT / 'tests/compiler/expected_output' / (src.stem + '.txt'))
		for src in sorted((ROOT / 'tests/compiler/fixtures').glob('*.sirius'))]
	positives = [(source, expected) for source, expected in positives if source.stem not in positive_exclude]
	if not positives:
		raise AssertionError('No positive emitter cases found')
	runtime = runtime or BuildCache(cc).runtime()

	def positive(item: tuple[int, tuple[Path, Path]]) -> str:
		index, (source, expected) = item
		folder = work / f'positive_{index}'
		folder.mkdir()
		input_file = folder / 'input.sirius'
		input_file.write_bytes(normalize_newlines(source.read_bytes()))
		generated = folder / 'generated.c'
		execute([str(compiler), str(input_file), str(generated)])
		output_binary = folder / ('program.exe' if sys.platform == 'win32' else 'program')

		compile_c(cc, generated, output_binary, runtime)
		args = [str(output_binary)]
		if source.stem == '48_file_io':
			args.append(str(folder / 'stage1_file_roundtrip.txt'))
		result = subprocess.run(args, capture_output=True, text=True, encoding='utf-8')
		wanted = expected.read_text(encoding='utf-8')
		if result.returncode or result.stdout != wanted:
			raise AssertionError(f'{source.name}: exit={result.returncode}, output={result.stdout!r} != {wanted!r}; {result.stderr}')
		return f'PASS {source.name}'

	negatives = sorted((ROOT / 'tests/compiler/negative').glob('*.sirius'))
	if {source.stem for source in negatives} != set(ERRORS):
		raise AssertionError('negative test fixtures and expected diagnostics differ')
	negatives = [source for source in negatives if source.stem not in negative_exclude]
	expected_errors = expected_errors or ERRORS

	def negative(item: tuple[int, Path]) -> str:
		index, source = item
		folder = work / f'negative_{index}'
		folder.mkdir()
		generated = folder / 'should_not_exist.c'
		result = subprocess.run([str(compiler), str(source), str(generated)],
			capture_output=True, text=True, encoding='utf-8')
		if result.returncode != 1 or expected_errors[source.stem] not in result.stdout or generated.exists():
			raise AssertionError(f'{source.name}: unexpected result {result.returncode}, {result.stdout!r}, {result.stderr!r}')
		return f'PASS {source.name} (rejected)'

	with ThreadPoolExecutor(max_workers=jobs) as pool:
		for message in pool.map(positive, enumerate(positives)):
			print(message)
		for message in pool.map(negative, enumerate(negatives)):
			print(message)
	print(f'{len(positives)} generated C programs and {len(negatives)} diagnostics passed')
	return len(positives), len(negatives)
