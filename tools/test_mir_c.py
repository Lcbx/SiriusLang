#!/usr/bin/env python
"""Compare opt-in HIR->MIR->C emission with the legacy C emitter."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from test_build import BuildCache, assert_native, execute, normalize_newlines, options, require_cc, run_native
from sirius_sources import COMPILER_SOURCES, normalize_newlines


CASES = {
	'arithmetic': 'def sum(a: i64, b: i64) -> i64\n\treturn a + b\ndef main() -> i64\n\treturn sum(20, 1) + sum(20, 1)\n',
	'factorial': 'def factorial(n: i64) -> i64\n\tif n <= 1\n\t\treturn 1\n\treturn n * factorial(n - 1)\ndef main() -> i64\n\treturn factorial(5)\n',
	'mutual': 'def even(n: i64) -> bool\n\tif n == 0\n\t\treturn true\n\treturn odd(n - 1)\ndef odd(n: i64) -> bool\n\tif n == 0\n\t\treturn false\n\treturn even(n - 1)\ndef main() -> i64\n\tif even(6)\n\t\treturn 42\n\treturn 0\n',
	'generic': 'def discard(x) -> i64\n\tlet copy = x\n\tif x == copy\n\t\treturn 1\n\treturn 0\ndef main() -> i64\n\treturn discard(0) + discard(true)\n',
	'nested': 'def inc(n: i64) -> i64\n\treturn n + 1\ndef twice(n: i64) -> i64\n\treturn inc(inc(n))\ndef main() -> i64\n\treturn twice(40)\n',
	'loop': 'def main() -> i64\n\tvar n = 0\n\tvar total = 0\n\twhile n < 5\n\t\tif n == 3\n\t\t\ttotal = total + 10\n\t\telse\n\t\t\ttotal = total + n\n\t\tn = n + 1\n\treturn total\n',
	'short_circuit': 'def main() -> i64\n\tif false and (1 / 0 > 0)\n\t\treturn 1\n\tif true or (1 / 0 > 0)\n\t\treturn 42\n\treturn 0\n',
	'string_loop': 'def main() -> i64\n\tvar s = "a"\n\tvar i = 0\n\twhile i < 6\n\t\ts = s + "b"\n\t\ti = i + 1\n\treturn len(s)\n',
	'string_alias': 'def passthrough(x: string) -> string\n\treturn x\ndef main() -> i64\n\tvar a = "ok"\n\tlet b = passthrough(a)\n\ta = "longer"\n\treturn len(a) + len(b)\n',
	'strings': 'def decorate(s: string) -> string\n\treturn s + "!"\ndef main() -> i64\n\tvar a = "hi"\n\tlet b = decorate(a)\n\tlet c = b + "\\n"\n\treturn len(a) + len(b) + len(c)\n',
	'record_call': 'def size(r) -> i64\n\treturn len(r.name)\ndef main() -> i64\n\tlet record = {name = "hello", count = 7}\n\treturn size(record) + size(record)\n',
	'record_reassign': 'def main() -> i64\n\tvar record = {name = "a", count = 1}\n\tlet before = record\n\trecord = {name = "long", count = 2}\n\treturn len(before.name) + len(record.name)\n',
	'record_generic': 'def length(r) -> i64\n\treturn len(r.name)\ndef main() -> i64\n\tlet a = {name = "a"}\n\tlet b = {name = "two", count = 2}\n\treturn length(a) + length(b)\n',
	'records': 'def main() -> i64\n\tlet inner = {count = 7, name = "Ada"}\n\tlet outer = {inner = inner, title = "outer"}\n\tlet alias = outer\n\treturn len(alias.inner.name) + len(outer.title)\n',
	'shadow': 'def main() -> i64\n\tvar x = 7\n\tif true\n\t\tlet x = 1\n\t\tif x == 0\n\t\t\treturn 99\n\treturn x\n',
	'array_cow': 'def main() -> i64\n\tvar a = [7, 8]\n\tlet before = a\n\ta[!0] = 30\n\treturn before[!0] + a[!0]\n',
	'array_strings': 'def main() -> i64\n\tvar a = ["old", "long"]\n\tlet before = a\n\ta[!0] = "new"\n\treturn len(before[!0]) + len(a[!1]) + len(a)\n',
	'array_nested': 'def main() -> i64\n\tlet inner = [1, 2]\n\tvar outer = [inner]\n\tlet saved = outer\n\touter[!0] = [7, 8]\n\treturn saved[!0][!0] + outer[!0][!1]\n',
	'dict_len': 'def main() -> i64\n\tvar d = Dict[i64,string]()\n\td = d.insert(2, "old")\n\tlet before = d\n\td = d.insert(2, "new")\n\treturn len(before) + len(d)\n',
	'dict_chained': 'def main() -> i64\n\tlet first = Dict[string,i64]().insert("a", 3)\n\tlet second = first.insert("b", 4)\n\treturn len(first) * 10 + len(second)\n',
	'dict_array_value': 'def main() -> i64\n\tlet xs = [1, 2]\n\tlet d = Dict[i64,Array[i64]]().insert(7, xs)\n\treturn len(d)\n',
	'array_record': 'def main() -> i64\n\tlet item = {name = "X", count = 4}\n\tvar array = [item]\n\tlet old = array\n\tarray[!0] = {name = "long", count = 5}\n\treturn old[!0].count + len(array[!0].name)\n',
	'optional_hit': 'def main() -> i64\n\tlet a = [4, 6]\n\treturn a[?1] or 20\n',
	'optional_miss': 'def main() -> i64\n\tlet a = [4, 6]\n\treturn a[?9] or 20\n',
	'optional_lazy': 'def main() -> i64\n\tlet a = [42]\n\tlet zero = 0\n\treturn a[?0] or (1 / zero)\n',
	'optional_unwrap': 'def main() -> i64\n\tlet a = [13]\n\treturn a[?0]!\n',
	'optional_return_hit': 'def get(index: i64) -> i64\n\tlet x = [12][?index] else return 31\n\treturn x\ndef main() -> i64\n\treturn get(0)\n',
	'optional_return_miss': 'def get(index: i64) -> i64\n\tlet x = [12][?index] else return 31\n\treturn x\ndef main() -> i64\n\treturn get(4)\n',
	'optional_dict_hit': 'def main() -> i64\n\tlet d = Dict[i64,i64]().insert(2, 7)\n\treturn d.get(2) or 15\n',
	'optional_dict_miss': 'def main() -> i64\n\tlet d = Dict[i64,i64]().insert(2, 7)\n\treturn d.get(3) or 15\n',
	'optional_string_hit': 'def main() -> i64\n\tlet a = ["hello"]\n\tlet x = a[?0] or "missing"\n\treturn len(x)\n',
	'optional_string_miss': 'def main() -> i64\n\tlet a = ["hello"]\n\tlet x = a[?3] or "missing"\n\treturn len(x)\n',
	'optional_nested': 'def main() -> i64\n\tlet d = Dict[i64,Array[i64]]().insert(2, [7, 8])\n\tlet x = d.get(2) or [1]\n\treturn x[!1]\n',


}


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cc = require_cc()
	cache = BuildCache(cc, rebuild=args.rebuild)
	compiler = cache.sirius('emitter-mir-optin', COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		for name, source in CASES.items():
			input_path = work / f'{name}.sirius'
			input_path.write_text(source)
			results = []
			for backend in ('legacy', 'mir'):
				c_path = work / f'{name}-{backend}.c'
				command = [str(compiler), str(input_path), str(c_path)]
				if backend == 'mir':
					command.insert(1, '--mir-c')
				execute(command)
				result = run_native(cache, c_path)
				results.append((result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)))
			if results[0] != results[1]:
				raise AssertionError(f'{name}: backend mismatch: {results}')
			print(f'PASS MIR-C {name} (return {results[0][0]})')
		# Unsupported MIR constructs must fail rather than silently use legacy C.
		input_path = work / 'unsupported.sirius'
		input_path.write_text('def main() -> i64\n\treturn str(42).byte_at(0) or 0\n')
		result = subprocess.run([str(compiler), '--mir-c', str(input_path), str(work / 'unsupported.c')], capture_output=True, text=True)
		if result.returncode == 0 or 'unsupported' not in result.stdout:
			raise AssertionError(f'MIR must reject non-scalar source explicitly: {result}')
		print('PASS MIR-C unsupported construct rejected')
		driver = cache.sirius('mir-c-direct', (
			'src/tokenizer.sirius', 'src/parser.sirius', 'src/types.sirius',
			'src/hir.sirius', 'src/emit_c.sirius', 'src/hir_specialize.sirius',
			'src/mir.sirius', 'src/mir_lower.sirius', 'src/mir_ownership.sirius', 'src/mir_guard.sirius', 'src/mir_lifetime.sirius', 'src/mir_cleanup.sirius', 'src/mir_program.sirius',
			'src/mir_emit_c.sirius', 'tests/mir/c_driver.sirius',
		))
		generated = [work / f'direct-{name}.c' for name in ('print', 'panic', 'uninitialized')]
		execute([str(driver), *(str(path) for path in generated)])
		for source, expected, error_fragment in (
			(generated[0], b'42\ntrue\n', None),
			(generated[1], b'', b'quoted "panic" and backslash \\ end'),
			(generated[2], b'', b'uninitialized register'),
		):
			result = run_native(cache, source)
			assert_native(result, stdout=expected, code=0 if error_fragment is None else None, stderr_contains=error_fragment)
		print('PASS MIR-C direct print, panic escaping, uninitialized register')


if __name__ == '__main__':
	main()
