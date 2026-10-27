#!/usr/bin/env python3
"""Validate named function values, MIR relocation, and native/interpreter parity."""
from pathlib import Path
import subprocess
import tempfile
from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc, run_native, assert_native, compile_native
from project_paths import ROOT, normalize_newlines


SOURCES = {
	'source_function_values': (42, b''),
	'source_function_return': (42, b''),
	'source_function_managed': (5, b'hello\n'),
}

INVALID = (
	'def f(value: i64) -> i64\n\treturn value\ndef main() -> i64\n\tlet g: Fn[string,i64] = f\n\treturn 0\n',
	'def f(value: i64) -> i64\n\treturn value\ndef main() -> i64\n\tlet g = f\n\treturn g()\n',
	'def f(value) -> i64\n\treturn value\ndef main() -> i64\n\tlet g = f\n\treturn 0\n',
)


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-function-value', ROOT / 'tests/mir/function_value_driver_native.sirius')
	compiler = cache.native_tool('modular_cli')
	runner = cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		source = work / 'indirect.c'
		result = execute([str(driver), str(source)])
		assert 'PASS MIR function references' in result.stdout, result.stdout
		assert_native(run_native(cache, source), code=42, stdout=b'')
		for name, (code, stdout) in SOURCES.items():
			input_file = ROOT / 'tests/mir' / (name + '.sirius')
			output_file = work / (name + '.c')
			execute([str(compiler), str(input_file), str(output_file)])
			binary = compile_native(cache, output_file)
			native = subprocess.run([str(binary)], capture_output=True)
			dynamic = subprocess.run([str(runner), str(input_file)], capture_output=True)
			for backend, actual in (('native', native), ('interpreter', dynamic)):
				assert (actual.returncode, normalize_newlines(actual.stdout), normalize_newlines(actual.stderr)) == (code, stdout, b''), (name, backend, actual)
			if name == 'source_function_managed':
				audit = work / 'audit'
				execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(output_file), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(audit)])
				checked = subprocess.run([str(audit)], capture_output=True)
				assert (checked.returncode, normalize_newlines(checked.stdout), normalize_newlines(checked.stderr)) == (code, stdout, b''), checked
		for i, text in enumerate(INVALID):
			bad = work / f'invalid-{i}.sirius'
			bad.write_text(text)
			result = subprocess.run([str(compiler), str(bad), str(work / 'bad.c')], capture_output=True)
			assert result.returncode != 0, (i, result)
	print('PASS function values: MIR, native/interpreted source, ARC audit, invalid signatures')


if __name__ == '__main__':
	main()
