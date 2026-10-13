#!/usr/bin/env python
"""Exercise new MIR-only lowering features against the legacy native backend."""
import argparse
from pathlib import Path
import tempfile

from source_manifest import COMPILER_SOURCES
from build_support import BuildCache, assert_native, execute, options, require_cc, run_native


CASES = {
	'named_record': ('rec Point(x: i64, y: i64)\n\tpub x\n\tpub y\ndef add(p: Point) -> i64\n\treturn p.x + p.y\ndef main() -> i64\n\treturn add(Point(20, 22))\n', 42, b''),
	'annotated_array': ('rec Point(x: i64, y: i64)\n\tpub x\n\tpub y\ndef first(items: Array[Point]) -> i64\n\treturn items[!0].x\ndef main() -> i64\n\treturn first([Point(42, 0)])\n', 42, b''),
	'published_expression': ('rec Label(x: i64)\n\tpub x\n\tpub name = "abc"\ndef main() -> i64\n\tlet label = Label(39)\n\treturn label.x + len(label.name)\n', 42, b''),
	'constants': ('let BASE = 20\nlet ANSWER = BASE * 2 + 2\ndef main() -> i64\n\treturn ANSWER\n', 42, b''),
	'str_print': ('def main() -> i64\n\tprint(str(42))\n\treturn len(str(42))\n', 2, b'42\n'),
	'argv': ('def main() -> i64\n\treturn len(arguments())\n', 2, b''),
	'write': ('def main() -> i64\n\tlet args = arguments()\n\tif write_text(args[!1], "test")\n\t\treturn 0\n\treturn 1\n', 0, b''),
	'read': ('def main() -> i64\n\tlet args = arguments()\n\tlet contents = read_text(args[!1]) or ""\n\treturn len(contents)\n', 4, b''),

	'array_append': ('def main() -> i64\n\tlet empty = Array[i64]()\n\tlet one = empty.append(21)\n\tlet two = one.append(21)\n\treturn len(empty) + two[!0] + two[!1]\n', 42, b''),
	'array_append_owned': ('def main() -> i64\n\tlet a = ["old"]\n\tlet b = a.append("new")\n\treturn len(a[!0]) + len(b[!1]) + len(b)\n', 8, b''),
	'for_range': ('def main() -> i64\n\tvar total = 0\n\tfor i in 1..6\n\t\ttotal = total + i\n\treturn total\n', 15, b''),
	'for_array': ('def main() -> i64\n\tvar total = 0\n\tfor item in [1, 2, 3]\n\t\ttotal = total + item\n\treturn total\n', 6, b''),
	'for_continue_break': ('def main() -> i64\n\tvar total = 0\n\tfor i in 0..6\n\t\tif i == 2\n\t\t\tcontinue\n\t\tif i == 5\n\t\t\tbreak\n\t\ttotal = total + i\n\treturn total\n', 8, b''),
	'string_comparisons': ('def main() -> i64\n\tif "b" > "a" and "ab" < "ac" and "abc" == "abc" and "z" != "a" and "aa" <= "ab" and "z" >= "y"\n\t\treturn 42\n\treturn 0\n', 42, b''),
	'guard_marker_literal': ('def main() -> i64\n\treturn len("__mir_guarded")\n', 13, b''),
	'guard_return': ('def main() -> i64\n\tlet found = "abc".find("z") or -1\n\tfound >= 0 else return 42\n\treturn 0\n', 42, b''),
	'annotated_constructor_constant': ('let EXTRA = 9\nrec Box(x: i64)\n\tpub x\n\tpub extra = EXTRA\ndef value(box: Box) -> i64\n\treturn box.x + box.extra\ndef main() -> i64\n\treturn value(Box(33))\n', 42, b''),
	'constructor_module_constant': ('let EXTRA = 13\nrec Box(x: i64)\n\tpub x\n\tpub extra = EXTRA\ndef main() -> i64\n\tlet box = Box(29)\n\treturn box.x + box.extra\n', 42, b''),
	'constructor_calls': ('def increment(x: i64) -> i64\n\treturn x + 1\nrec Box(x: i64)\n\tpub value = increment(x)\ndef main() -> i64\n\treturn Box(41).value\n', 42, b''),
	'record_array_update': ('rec Box(values: Array[i64])\n\tpub values\ndef main() -> i64\n\tvar box = Box([3, 4])\n\tlet saved = box\n\tbox.values[!0] = 42\n\treturn saved.values[!0] + box.values[!0]\n', 45, b''),
	'record_field_update': ('rec Box(name: string, value: i64)\n\tpub name\n\tpub value\ndef main() -> i64\n\tvar box = Box("old", 1)\n\tlet saved = box\n\tbox.value = 41\n\tbox.name = "new"\n\treturn saved.value + box.value + len(saved.name) + len(box.name)\n', 48, b''),
	'string_slice': ('def main() -> i64\n\treturn len("abcdef".slice(1, 4))\n', 3, b''),
	'string_prefix_suffix': ('def main() -> i64\n\tif "alphabet".starts_with("alpha") and "alphabet".ends_with("bet")\n\t\treturn 42\n\treturn 0\n', 42, b''),
	'string_byte_at': ('def main() -> i64\n\treturn "Z".byte_at(0) or 0\n', 90, b''),
	'string_find': ('def main() -> i64\n\treturn "abcde".find("cd") or 0\n', 2, b''),
	'string_join': ('def main() -> i64\n\treturn len("-".join(["a", "bc", "d"]))\n', 6, b''),
	'while_continue_break': ('def main() -> i64\n\tvar i = 0\n\tvar total = 0\n\twhile i < 6\n\t\ti = i + 1\n\t\tif i == 2\n\t\t\tcontinue\n\t\tif i == 5\n\t\t\tbreak\n\t\ttotal = total + i\n\treturn total\n', 8, b''),

}


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = cache.sirius('mir-migration', COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		path = work / 'data.txt'
		path.write_text('test', encoding='utf-8')
		for name, (source, exit_code, stdout) in CASES.items():
			input_path = work / f'{name}.sirius'
			input_path.write_text(source, encoding='utf-8')
			for backend in ('legacy', 'mir'):
				c_path = work / f'{name}-{backend}.c'
				command = [str(compiler), str(input_path), str(c_path)]
				if backend == 'mir':
					command.insert(1, '--mir-c')
				execute(command)
				arguments = (str(path),) if name in ('argv', 'read', 'write') else ()
				result = run_native(cache, c_path, args=arguments)
				assert_native(result, stdout=stdout, code=exit_code)
			print(f'PASS {name} (both backends)')


if __name__ == '__main__':
	main()
