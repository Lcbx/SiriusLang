#!/usr/bin/env python3
"""Check declaration collection and typed calls independently of lowering."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))

from build_support import BuildCache, execute, require_cc
from project_paths import ROOT, normalize_newlines
from rewrite_paths import ENTRY, MODULES as REWRITE_MODULES, DRIVERS


def main() -> None:
	cache = BuildCache(require_cc())
	modules = REWRITE_MODULES[:4]
	driver = DRIVERS / 'frontend_test_driver.sirius'
	native = cache.native_driver('rewrite-frontend', driver, extra_modules=modules)
	valid = [
		('rec A(z: i64, a: string)\nrec B(a: string, z: i64)\ndef make() -> A\n\treturn B("ok", 7)\ndef main() -> i64\n\tprint(make().a)\n\treturn 0\n', 'make() -> 6\nmain() -> 0\nconstruct -> 6\ncall make -> 6\n'),
		('def main() -> i64\n\treturn 0\n', 'main() -> 0\n'),
		('def main() -> i64\n\treturn twice(21)\n'
		 'def twice(value: i64) -> i64\n\treturn value * 2\n',
		 'main() -> 0\ntwice( 0) -> 0\ncall twice -> 0\n'),
		('def even(n: i64) -> bool\n\tif n == 0\n\t\treturn true\n\treturn odd(n - 1)\n'
		 'def odd(n: i64) -> bool\n\tif n == 0\n\t\treturn false\n\treturn even(n - 1)\n'
		 'def main() -> i64\n\tprint(even(10))\n\treturn 0\n',
		 'even( 0) -> 1\nodd( 0) -> 1\nmain() -> 0\ncall odd -> 1\ncall even -> 1\ncall even -> 1\n'),
		('def repeat(text: string, n: i64) -> string\n\tif n == 0\n\t\treturn ""\n'
		 '\treturn text + repeat(text, n - 1)\n'
		 'def main() -> i64\n\tprint(repeat("x", 3))\n\treturn 0\n',
		 'repeat( 2 0) -> 2\nmain() -> 0\ncall repeat -> 2\ncall repeat -> 2\n'),
		('def lookup(text: string, index: i64) -> Optional[i64]\n\treturn text.byte_at(index)\n'
		 'def default(value: Optional[i64], fallback: i64) -> i64\n\treturn value or fallback\n'
		 'def main() -> i64\n\treturn default(lookup("x", 0), 0)\n',
		 'lookup( 2 0) -> 3\ndefault( 3 0) -> 0\nmain() -> 0\ncall lookup -> 3\ncall default -> 0\n'),
		('def first() -> string\n\treturn "a"\n'
		 'def second() -> bool\n\treturn true\n'
		 'def select(a: string, b: bool) -> i64\n\tif b\n\t\treturn len(a)\n\treturn 0\n'
		 'def main() -> i64\n\treturn select(first(), second())\n',
		 'first() -> 2\nsecond() -> 1\nselect( 2 1) -> 0\nmain() -> 0\ncall first -> 2\ncall second -> 1\ncall select -> 0\n'),
		('def identity(x: string) -> string\n\tif true\n\t\tlet x = "shadow"\n\t\tprint(x)\n\treturn x\n'
		 'def main() -> i64\n\tlet x = 3\n\tprint(identity("ok"))\n\treturn x\n',
		 'identity( 2) -> 2\nmain() -> 0\ncall identity -> 2\n'),
		('def main() -> i64\n\tprint(f"${later()}")\n\treturn 0\n'
		 'def later() -> string\n\treturn "ok"\n',
		 'main() -> 0\nlater() -> 2\ncall later -> 2\n'),
		('def main() -> i64\n\treturn 0\n'
		 'def strings() -> Array[string]\n\treturn ["hello"]\n',
		 'main() -> 0\nstrings() -> 5\n'),
	]
	entry = 'def main() -> i64\n\treturn 0\n'
	invalid = [
		('', 'missing main function', 1),
		('def other() -> i64\n\treturn 0\n', 'missing main function', 1),
		(entry + entry, 'duplicate function: main', 3),
		(entry + 'def f() -> i64\n\treturn 0\ndef f(x: i64) -> i64\n\treturn x\n', 'duplicate function: f', 5),
		('def main(x: i64) -> i64\n\treturn x\n', 'main must have signature', 1),
		('def main() -> string\n\treturn ""\n', 'main must have signature', 1),
		(entry + 'def len(x: string) -> i64\n\treturn 0\n', 'reserved builtin function', 3),
		(entry + 'def f(x: i64, x: string) -> i64\n\treturn 0\n', 'duplicate parameter', 3),
		(entry + 'def f(x: i64) -> i64\n\tx = 2\n\treturn x\n', 'immutable binding', 4),
		(entry + 'def f(x: i64) -> i64\n\tlet x = 2\n\treturn x\n', 'duplicate binding', 4),
		(entry + 'def f(x: string) -> i64\n\treturn x\n', 'return type mismatch', 4),
		(entry + 'def f() -> string\n\ttrue else return 1\n\treturn ""\n', 'return type mismatch', 4),
		(entry + 'def f() -> string\n\tlet x = "".byte_at(0) else return 1\n\treturn ""\n', 'return type mismatch', 4),
		(entry + 'def f() -> bool\n\treturn 0\n', 'return type mismatch', 4),
		(entry + 'def f() -> Optional[i64]\n\treturn 0\n', 'return type mismatch', 4),
		('def main() -> i64\n\treturn unknown()\n', 'unknown function', 2),
		('def main() -> i64\n\treturn f(1)\ndef f() -> i64\n\treturn 0\n', 'argument count mismatch', 2),
		('def main() -> i64\n\treturn f()\ndef f(x: i64) -> i64\n\treturn x\n', 'argument count mismatch', 2),
		('def main() -> i64\n\treturn f(true, 1)\ndef f(x: i64, y: bool) -> i64\n\treturn x\n', 'argument type mismatch', 2),
		('def main() -> i64\n\tlet f = 3\n\treturn f()\ndef f() -> i64\n\treturn 0\n', 'local value is not callable', 3),
		(entry + 'def f() -> i64\n\treturn x\n', 'unknown name: x', 4),
		('def main() -> i64\n\tlet x = 2\n\treturn 0\ndef f() -> i64\n\treturn x\n', 'unknown name: x', 5),
		(entry + 'def f() -> i64\n\treturn g(false)\ndef g(x: i64) -> i64\n\treturn f()\n', 'argument type mismatch', 4),
		('def main() -> i64\n\treturn unknown()\ndef f() -> i64\n\treturn 0\ndef f() -> i64\n\treturn 1\n', 'duplicate function: f', 5),
	]
	for declaration in ['def f(x) -> i64', 'def f(x: string,) -> i64',
		'def f(x: Optional[]) -> i64', 'def f()']:
		invalid.append((entry + declaration + '\n\treturn 0\n', 'expected', 3))
	invalid.append((entry + 'def strings() -> Array[string]\n\treturn 0\n', 'return type mismatch', 4))
	for expression in ['f(,)', 'f(1,)', 'f(1 2)']:
		invalid.append(('def main() -> i64\n\treturn ' + expression + '\n', 'expected', 2))
	# Each incomplete header must diagnose EOF, without indexing past tokens.
	header = 'def helper(x: Optional[i64], y: string) -> string'
	for end in range(1, len(header) + 1):
		invalid.append((entry + header[:end], '', 3))
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		commands = [[str(native)]]
		source = root / 'input.sirius'
		for text, expected in valid:
			source.write_text(text, encoding='utf-8')
			for command in commands:
				result = subprocess.run(command + [str(source)], capture_output=True, timeout=60)
				assert result.returncode == 0 and normalize_newlines(result.stdout) == expected.encode() and not result.stderr, (text, result)
		for text, message, line in invalid:
			source.write_text(text, encoding='utf-8')
			for command in commands:
				result = subprocess.run(command + [str(source)], capture_output=True, timeout=60)
				assert result.returncode == 1 and not result.stdout and result.stderr.startswith(f'ERROR {line}: '.encode()) and message.encode() in result.stderr, (text, result)
	print(f'PASS rewrite frontend: {len(valid)} valid, {len(invalid)} invalid; native')


if __name__ == '__main__':
	main()
