#!/usr/bin/env python3
"""Computed constructor publications, conditional overrides and union patterns."""
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, check_native, require_cc


PROGRAM = '''rec Success(value: i64)
	pub extra = value + 2
	pub doubled = extra * 2
	if value > 10
		pub doubled = doubled + 1
	elif value > 0
		pub doubled = doubled + 2
rec Failure(code: i64)
let Outcome = Success | Failure

def main() -> i64
	let high: Outcome = Success(20)
	let low = Success(1)
	let zero = Success(0)
	match high
		Success(value, doubled, extra)
			return value + doubled + extra + low.doubled + zero.doubled
		Failure(code)
			return code
'''

MANAGED = '''rec Success(value: string)
	pub display = value + "!"
	if value == "a"
		pub display = display + "!"
rec Failure(code: i64)
let Outcome = Success | Failure

def main() -> i64
	let old = Success("a")
	let answer: Outcome = old
	match answer
		Success(value, display)
			return len(value) * 10 + len(display) + len(old.display)
		Failure(code)
			return code
'''

INVALID = (
	('branch new field', '''rec Bad(x: i64)
	if x > 0
		pub new = x

def main() -> i64
	return Bad(1).x
''', 'constructor branch must override an existing field'),
	('branch changes type', '''rec Bad(x: i64)
	if x > 0
		pub x = "wrong"

def main() -> i64
	return Bad(1).x
''', 'constructor branch must override an existing field'),
	('non-publication branch', '''rec Bad(x: i64)
	if x > 0
		let y = x

def main() -> i64
	return Bad(1).x
''', 'constructor branches may only override published fields'),
	('publication after conditional', '''rec Bad(x: i64)
	if x > 0
		pub x = x + 1
	pub y = x + 2

def main() -> i64
	return Bad(1).y
''', 'constructor requires publications followed by an optional conditional override'),
	('missing positional parameters', '''rec Success(value: i64)
	pub more = value + 1
rec Failure(code: i64)
let Outcome = Success | Failure

def main() -> i64
	let result: Outcome = Success(1)
	match result
		Success()
			return 1
		Failure(code)
			return 2
''', 'match pattern requires constructor parameters'),
	('too many pattern bindings', '''rec Success(value: i64)
	pub more = value + 1
rec Failure(code: i64)
let Outcome = Success | Failure

def main() -> i64
	let result: Outcome = Success(1)
	match result
		Success(value, more, surplus)
			return value
		Failure(code)
			return code
''', 'match pattern requires constructor parameters'),
)


def execute(*args: Path | str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def main() -> None:
	cache = BuildCache(require_cc())
	compiler = cache.native_tool('modular_cli')
	runner = cache.mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for name, source, code in (('numeric', PROGRAM, 99), ('managed', MANAGED, 16)):
			path = root / f'{name}.sirius'
			generated = root / f'{name}.c'
			path.write_text(source, encoding='utf-8')
			result = execute(compiler, path, generated)
			assert result.returncode == 0, (name, result.stdout, result.stderr)
			check_native(cache, generated, code=code)
			module = root / 'App.sirius'
			module.write_text(source, encoding='utf-8')
			library = root / 'App.smirl'
			result = execute(runner, '--build-modules', root, 'App', library)
			assert result.returncode == 0, (name, result.stdout, result.stderr)
			result = execute(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
			assert result.returncode == code, (name, result.returncode, result.stdout, result.stderr)
		for name, source, fragment in INVALID:
			path = root / 'invalid.sirius'
			output = root / 'invalid.c'
			output.unlink(missing_ok=True)
			path.write_text(source, encoding='utf-8')
			result = execute(compiler, path, output)
			assert result.returncode != 0 and fragment.encode() in result.stdout + result.stderr and not output.exists(), (name, result.stdout, result.stderr)
	print('PASS constructor publications: computed, conditional, managed, union match, native/MIR, negatives')


if __name__ == '__main__':
	main()
