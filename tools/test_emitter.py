"""Compile the Sirius stage-1 emitter through the Python bootstrap; execute emitted C."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
SOURCES = ('src/tokenizer.sirius', 'src/parser.sirius', 'src/emit_c.sirius', 'tests/emitter/driver.sirius')
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

}


def execute(command: list[str], *, check: bool = True) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(command, text=True, encoding='utf-8', capture_output=True)
	if check and result.returncode:
		raise RuntimeError(f'{" ".join(command)}\n{result.stdout}{result.stderr}')
	return result


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('C11 compiler required (cc or gcc)')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		combined = work / 'stage1.sirius'
		combined.write_text('\n'.join((ROOT / f).read_text(encoding='utf-8') for f in SOURCES), encoding='utf-8')
		bootstrap_c = work / 'bootstrap.c'
		execute([sys.executable, str(ROOT / 'bootstrap_compiler/sirius.py'), 'compile', str(combined), '-o', str(bootstrap_c)])
		binary = work / 'stage1'
		execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(bootstrap_c), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(binary)])

		positives = [(ROOT / 'tests/fixtures' / (name + '.sirius'), ROOT / 'tests/expected_output' / (name + '.txt')) for name in REFERENCE]
		positives += [(src, ROOT / 'tests/emitter/expected_output' / (src.stem + '.txt'))
			for src in sorted((ROOT / 'tests/emitter/fixtures').glob('*.sirius'))]
		for index, (source, expected) in enumerate(positives):
			# Normalize regular fixtures in case Git checked them out as CRLF.
			input_file = work / f'input_{index}.sirius'
			input_file.write_bytes(source.read_bytes().replace(b'\r\n', b'\n'))
			generated = work / f'generated_{index}.c'
			execute([str(binary), str(input_file), str(generated)])
			output_binary = work / f'program_{index}'
			execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'), str(generated), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(output_binary)])
			args = [str(output_binary)]
			if source.stem == '48_file_io':
				args.append(str(work / 'stage1_file_roundtrip.txt'))
			actual = execute(args)
			wanted = expected.read_text(encoding='utf-8')
			if actual.stdout != wanted:
				raise AssertionError(f'{source.name}: {actual.stdout!r} != {wanted!r}')
			print(f'PASS {source.name}')

		negatives = sorted((ROOT / 'tests/emitter/negative').glob('*.sirius'))
		if {source.stem for source in negatives} != set(ERRORS):
			raise AssertionError('negative test fixtures and expected diagnostics differ')
		for source in negatives:
			generated = work / 'should_not_exist.c'
			generated.unlink(missing_ok=True)
			result = execute([str(binary), str(source), str(generated)], check=False)
			if result.returncode != 1 or ERRORS[source.stem] not in result.stdout or generated.exists():
				raise AssertionError(f'{source.name}: unexpected result {result.returncode}, {result.stdout!r}, {result.stderr!r}')
			print(f'PASS {source.name} (rejected)')
		print(f'{len(positives)} generated C programs and {len(ERRORS)} diagnostics passed')


if __name__ == '__main__':
	main()
