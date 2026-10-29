#!/usr/bin/env python
"""Execute the same Sirius source through dynamic MIR and emitted C."""
import argparse
import shutil
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, compile_native, execute, options, require_cc
from compiler_cases import REFERENCE
from project_paths import ROOT, normalize_newlines


# Output and exit status are independently specified, not just compared.
CASES = {
	'calls': ('def add(a: i64, b: i64) -> i64\n\treturn a + b\ndef main() -> i64\n\tprint(add(5, 7))\n\treturn add(20, 22)\n', b'12\n', 42),
	'loop': ('def main() -> i64\n\tvar total = 0\n\tfor x in [3, 5, 7]\n\t\ttotal += x\n\tprint(total)\n\treturn 0\n', b'15\n', 0),
	'record_cow': ('def main() -> i64\n\tvar p = {x=10, label="old"}\n\tlet saved = p\n\tp.x = 20\n\tprint(saved.x)\n\treturn p.x\n', b'10\n', 20),
	'array_cow': ('def main() -> i64\n\tvar a = [7, 8]\n\tlet saved = a\n\ta[!0] = 30\n\tprint(saved[!0])\n\treturn a[!0]\n', b'7\n', 30),
	'dict_optional': ('def main() -> i64\n\tlet d = Dict[i64, i64]().insert(2, 9)\n\treturn (d.get(2) or 0) + (d.get(3) or 4)\n', b'', 13),
	'optional_absent': ('def main() -> i64\n\tlet a = [2]\n\treturn a[?9] or 11\n', b'', 11),
	'string': ('def main() -> i64\n\tprint("a" + "bc")\n\treturn len("hello")\n', b'abc\n', 5),
	'recursive': ('def f(n: i64) -> i64\n\tif n <= 1\n\t\treturn 1\n\treturn n * f(n - 1)\ndef main() -> i64\n\treturn f(5)\n', b'', 120),
	'arguments': ('def main() -> i64\n\tlet args = arguments()\n\tprint(args[!1])\n\treturn len(args)\n', b'provided\n', 2),
	'read_file': ('def main() -> i64\n\tlet path = arguments()[!1]\n\tlet content = read_text(path) or "missing"\n\tprint(content)\n\treturn len(content)\n', b'file content\n', 12),
	'missing_file': ('def main() -> i64\n\tlet path = arguments()[!1]\n\tlet content = read_text(path) or "missing"\n\tprint(content)\n\treturn len(content)\n', b'missing\n', 7),
	'empty_file': ('def main() -> i64\n\tlet path = arguments()[!1]\n\tlet content = read_text(path) or "missing"\n\tprint(len(content))\n\treturn len(content)\n', b'0\n', 0),
	'float_arithmetic': ('def main() -> i64\n\tlet x = 1.5\n\tlet y = 2.0\n\tprint(x + y)\n\tprint(x - y)\n\tprint(x * y)\n\tprint(x / y)\n\tprint(-x)\n\treturn 0\n', b'3.5\n-0.5\n3\n0.75\n-1.5\n', 0),
	'float_nested_cow': ('def main() -> i64\n\tvar a = [1.5, 2.25]\n\tlet saved = a\n\ta[!0] = 3.5\n\tlet r = {values = a, flag = true}\n\tprint(r.values[!0])\n\tprint(saved[!0])\n\treturn 0\n', b'3.5\n1.5\n', 0),
	'float_dict_key': ('def main() -> i64\n\tlet d = Dict[f64, i64]().insert(1.25, 9)\n\treturn (d.get(1.25) or 0) + (d.get(2.0) or 3)\n', b'', 12),
	'float_function_sqrt': ('def f(x: f64) -> f64\n\treturn sqrt(x * x)\ndef main() -> i64\n\tprint(f(-3.5))\n\tif 2.0 <= 2.0 and 2.0 == 2.0 and 1.0 < 2.0\n\t\treturn 0\n\treturn 1\n', b'3.5\n', 0),
	'float_scientific': ('def main() -> i64\n\tprint(2e2)\n\tprint(1.25e-1)\n\tprint(3.5E+1)\n\tprint(-0.0)\n\treturn 0\n', b'200\n0.125\n35\n-0\n', 0),
	'eprint': ('def main() -> i64\n\tprint("stdout")\n\teprint("stderr")\n\teprint(12)\n\teprint(true)\n\teprint(1.25)\n\treturn 0\n', b'stdout\n', 0),
	'float_precision': ('def main() -> i64\n\tprint(9.999999999999995)\n\tprint(1e-320)\n\tprint(3.141592653589793)\n\treturn 0\n', b'9.99999999999999\n9.99988867182683e-321\n3.14159265358979\n', 0),
}


FAILURE_CASES = {
	'division_by_zero': ('def main() -> i64\n\tlet zero = 0\n\treturn 4 / zero\n', 'division by zero'),
	'array_out_of_bounds': ('def main() -> i64\n\tlet a = [1]\n\treturn a[!3]\n', 'array index out of bounds'),
	'optional_unwrap': ('def main() -> i64\n\tlet a = [1]\n\treturn a[?3]!\n', 'missing optional value'),
	'nested_managed_panic': ('def inner() -> i64\n\tlet data = ["alive", "held"]\n\tlet other = data\n\tlet zero = 0\n\treturn len(other) / zero\ndef main() -> i64\n\tprint("before")\n\treturn inner()\n', 'division by zero'),
}


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--runner', type=Path, help='prebuilt sirius-run binary')
	parser.add_argument('--compiler', type=Path, help='prebuilt Sirius C emitter')
	parser.add_argument('--all-fixtures', action='store_true', help='check dynamic output against all supported compiler fixtures')
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc(), rebuild=args.rebuild)
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	compiler = args.compiler.resolve() if args.compiler else cache.seed()
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		payload = work / 'payload.txt'
		payload.write_text('file content')
		(work / 'empty.txt').write_text('')
		for name, (source, expected_stdout, expected_code) in CASES.items():
			input_path = work / f'{name}.sirius'
			input_path.write_text(source)
			c_path = work / f'{name}.c'
			execute([str(compiler), str(input_path), str(c_path)])
			binary = compile_native(cache, c_path)
			runtime_args = []
			if name == 'arguments':
				runtime_args = ['provided']
			elif name == 'read_file':
				runtime_args = [str(payload)]
			elif name == 'empty_file':
				runtime_args = [str(work / 'empty.txt')]
			elif name == 'missing_file':
				runtime_args = [str(work / 'absent.txt')]
			# The executable name is observable by arguments()[0].
			# Remap argv[0] so both backends receive the source path.
			native = subprocess.run([str(input_path), *runtime_args], executable=str(binary), capture_output=True)
			dynamic = subprocess.run([str(runner), str(input_path), *runtime_args], capture_output=True)
			expected_stderr = b'stderr\n12\ntrue\n1.25\n' if name == 'eprint' else b''
			for backend, result in (('native', native), ('dynamic', dynamic)):
				if result.returncode != expected_code or normalize_newlines(result.stdout) != expected_stdout or normalize_newlines(result.stderr) != expected_stderr:
					raise AssertionError(f'{name} {backend}: expected {expected_code}/{expected_stdout!r}; got {result!r}')
			print(f'PASS MIR run {name}')
		# Both frontends must reject ill-typed source with the same diagnostics.
		bad = work / 'invalid.sirius'
		bad.write_text('def main() -> i64\n\treturn "incorrect"\n')
		c_path = work / 'invalid.c'
		native_error = subprocess.run([str(compiler), str(bad), str(c_path)], capture_output=True)
		dynamic_error = subprocess.run([str(runner), str(bad)], capture_output=True)
		if native_error.returncode != 1 or dynamic_error.returncode != 1 or normalize_newlines(native_error.stdout) != normalize_newlines(dynamic_error.stdout):
			raise AssertionError(f'compile-error mismatch: {native_error!r} vs {dynamic_error!r}')
		print('PASS MIR run diagnostic parity')
		# Runtime failures must preserve stdout and report the same panic on stderr.
		for name, (source, message) in FAILURE_CASES.items():
			input_path = work / f'{name}.sirius'
			input_path.write_text(source)
			c_path = work / f'{name}.c'
			execute([str(compiler), str(input_path), str(c_path)])
			binary = compile_native(cache, c_path)
			native = subprocess.run([str(binary)], capture_output=True)
			dynamic = subprocess.run([str(runner), str(input_path)], capture_output=True)
			expected_stdout = b'before\n' if name == 'nested_managed_panic' else b''
			expected_stderr = ('Sirius panic: ' + message + '\n').encode()
			for backend, result in (('native', native), ('dynamic', dynamic)):
				if result.returncode != 1 or normalize_newlines(result.stdout) != expected_stdout or normalize_newlines(result.stderr) != expected_stderr:
					raise AssertionError(f'runtime error mismatch {name} {backend}: {result!r}')
			print(f'PASS MIR run runtime failure {name}')
		# The existing native float-constructor fixture is now shared too.
		float_source = ROOT / 'tests/language/fixtures/06_constructor.sirius'
		float_result = subprocess.run([str(runner), str(float_source)], capture_output=True)
		if float_result.returncode != 0 or normalize_newlines(float_result.stdout) != b'5\n' or float_result.stderr:
			raise AssertionError(f'f64 constructor parity: {float_result!r}')
		print('PASS MIR run f64 constructor')
		# The dynamic backend has explicit execution budgets, unlike native code.
		budget_program = work / 'budget.sirius'
		budget_program.write_text('def loop(n: i64) -> i64\n\tif n == 0\n\t\treturn 0\n\treturn loop(n - 1)\ndef main() -> i64\n\treturn loop(10)\n')
		limit_cases = (
			(['--max-steps', '3'], 1, b'Sirius panic: step limit exceeded\n'),
			(['--max-depth', '3'], 1, b'Sirius panic: call depth limit exceeded\n'),
			(['--max-steps', '0'], 2, b'invalid value for --max-steps\n'),
			(['--max-depth', 'abc'], 2, b'invalid value for --max-depth\n'),
			(['--max-steps', '999999999999999999999'], 2, b'invalid value for --max-steps\n'),
			(['--unknown'], 2, b'unknown option: --unknown\n'),
		)
		for flags, expected_code, expected_stderr in limit_cases:
			result = subprocess.run([str(runner), *flags, str(budget_program)], capture_output=True)
			if result.returncode != expected_code or result.stdout or normalize_newlines(result.stderr) != expected_stderr:
				raise AssertionError(f'execution limit {flags}: {result!r}')
		print('PASS MIR run execution limits')
		argument_test = subprocess.run([str(runner), '--max-steps', '100000', str(work / 'arguments.sirius'), 'provided'], capture_output=True)
		if argument_test.returncode != 2 or normalize_newlines(argument_test.stdout) != b'provided\n' or argument_test.stderr:
			raise AssertionError(f'options must not enter program argv: {argument_test!r}')
		print('PASS MIR run option argument separation')
		if args.all_fixtures:
			fixtures = [(ROOT / 'tests/language/fixtures' / (name + '.sirius'),
				ROOT / 'tests/language/expected_output' / (name + '.txt')) for name in REFERENCE]
			fixtures += [(path, ROOT / 'tests/compiler/expected_output' / (path.stem + '.txt'))
				for path in sorted((ROOT / 'tests/compiler/fixtures').glob('*.sirius'))]
			passed = 0
			for source, expected in fixtures:
				runtime_args = [str(work / 'file_io.txt')] if source.stem == '48_file_io' else []
				result = subprocess.run([str(runner), str(source), *runtime_args], capture_output=True, timeout=20)
				if result.returncode != 0 or normalize_newlines(result.stdout) != normalize_newlines(expected.read_bytes()) or result.stderr:
					raise AssertionError(f'{source.name}: {result!r} != {expected.read_bytes()!r}')
				passed += 1
			print(f'PASS MIR dynamic: {passed} shared compiler fixtures')


if __name__ == '__main__':
	main()
