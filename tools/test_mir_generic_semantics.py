#!/usr/bin/env python3
"""Exercise public Sirius semantics across independently compiled generic libraries.

The temporary extern def bridge is used until path-based source imports exist.
Test assertions intentionally concern values, CoW and type errors, not HIR encoding.
"""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES, ROOT


def run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *(str(arg) for arg in args)], capture_output=True, timeout=60)


def must_run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(binary, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def check(runner: Path) -> None:
	with tempfile.TemporaryDirectory(prefix='sirius-generic-semantics-') as directory:
		work = Path(directory)
		def source(name: str, contents: str) -> Path:
			path = work / f'{name}.sirius'
			path.write_text(contents, encoding='utf-8')
			return path
		def library(name: str, contents: str, *flags: str) -> Path:
			path = work / f'{name}.smirl'
			must_run(runner, '--emit-lib', path, *flags, source(name, contents))
			return path

		provider = library('provider', '''def triple(x)
	let twice = x + x
	let result = twice + x
	return result

def same(x)
	let equal = x == x
	return equal

def choice(x)
	let condition = x or true
	return condition

def clone(x)
	let copy = x
	return copy

def fallback(x)
	let item = x
	if item > 4
		return item
	return x
''', '--template', 'triple', '--template', 'same', '--template', 'choice',
			'--template', 'clone', '--template', 'fallback')
		consumer = library('consumer', '''rec Point(x: i64, y: i64)
extern def triple(x: i64) -> i64
extern def same(x: string) -> bool
extern def choice(x: bool) -> bool
extern def clone(x: Point) -> Point
extern def fallback(x: i64) -> i64

def main() -> i64
	var original = Point(2, 3)
	let saved = clone(original)
	original.x = 90
	if saved.x != 2 or original.x != 90 or not same("a") or not choice(false)
		return 1
	if triple(4) != 12 or fallback(8) != 8
		return 2
	return 0
''', '--export', 'main')
		string = library('string', '''extern def triple(x: string) -> string

def string_main() -> i64
	if triple("ha") == "hahaha"
		return 0
	return 1
''', '--export', 'string_main')
		linked = work / 'linked.smirl'
		for order in ((provider, consumer, string), (string, consumer, provider)):
			must_run(runner, '--link-lib', linked, *order)
			for entry in ('main', 'string_main'):
				result = run(runner, '--entry', entry, '--load-lib', linked)
				assert (result.returncode, result.stdout, result.stderr) == (0, b'', b''), result

		# Confirm that a linked generic specialization is also executable as C.
		c_file = work / 'linked.c'
		must_run(runner, '--emit-c-lib', c_file, '--entry', 'main', linked)
		cc = shutil.which('clang') or require_cc()
		native = work / 'native'
		result = subprocess.run([cc, '-std=c11', '-O0', '-I', str(ROOT / 'runtime'),
			str(c_file), str(ROOT / 'runtime/sirius_runtime.c'), '-lm', '-o', str(native)],
			capture_output=True, timeout=90)
		assert result.returncode == 0, result.stderr
		assert run(native).returncode == 0

		# End-user syntax/semantics: generic structural records, field access and
		# homogeneous arrays, with type identities fixed only at specialization.
		structures = library('structures', '''def select(x)
	let record = {value = x, enabled = true}
	return record.value

def wrap(x)
	return {value = x, enabled = true}

def read(x)
	return x.value

def pair(x)
	let elements = [x, x]
	return elements

def mismatch(x)
	return [x, 42]

def size(x)
	let values = [x, x]
	return len(values)

def as_text(x)
	return str(x)

def root(x)
	return sqrt(x)

def missing(x)
	return x.absent
''', '--template', 'select', '--template', 'wrap',
			'--template', 'read', '--template', 'pair', '--template', 'size',
			'--template', 'as_text', '--template', 'root', '--template', 'mismatch', '--template', 'missing')
		structures_consumer = library('structures_consumer', '''rec Wrapped(value: i64, enabled: bool)
extern def select(x: i64) -> i64
extern def wrap(x: i64) -> Wrapped
extern def read(x: Wrapped) -> i64
extern def pair(x: string) -> Array[string]
extern def size(x: bool) -> i64
extern def as_text(x: i64) -> string
extern def root(x: f64) -> f64

def records_main() -> i64
	var record = wrap(7)
	let saved = record
	record.value = 8
	var values = pair("a")
	let previous = values
	values[!0] = "b"
	if saved.value != 7 or record.value != 8 or previous[!0] != "a" or values[!0] != "b"
		return 1
	if select(4) != 4 or read(saved) != 7 or len(values) != 2 or size(true) != 2 or as_text(19) != "19" or root(9.0) != 3.0
		return 2
	return 0
''', '--export', 'records_main')
		for order in ((structures, structures_consumer), (structures_consumer, structures)):
			must_run(runner, '--link-lib', linked, *order)
			result = run(runner, '--entry', 'records_main', '--load-lib', linked)
			assert (result.returncode, result.stdout, result.stderr) == (0, b'', b''), result
		must_run(runner, '--emit-c-lib', c_file, '--entry', 'records_main', linked)
		result = subprocess.run([cc, '-std=c11', '-O0', '-I', str(ROOT / 'runtime'),
			str(c_file), str(ROOT / 'runtime/sirius_runtime.c'), '-lm', '-o', str(native)],
			capture_output=True, timeout=90)
		assert result.returncode == 0, result.stderr
		assert run(native).returncode == 0

		for name, definition, expression, fragment in (
			('mismatch', 'extern def mismatch(x: bool) -> Array[bool]', 'mismatch(true)', b'expected bool, found i64'),
			('missing', 'extern def missing(x: Wrapped) -> i64', 'missing(Wrapped(1, true))', b"unknown record field 'absent'"),
			('as_text', 'extern def as_text(x: bool) -> string', 'as_text(true)', b'str expects one i64 argument'),
		):
			contents = ('rec Wrapped(value: i64, enabled: bool)\n' + definition +
				'\ndef bad() -> i64\n\tlet unused = ' + expression + '\n\treturn 0\n')
			broken = library('broken_' + name, contents, '--export', 'bad')
			result = run(runner, '--link-lib', linked, broken, structures)
			assert result.returncode == 2 and fragment in result.stderr, result

		# Invalid concrete specializations must fail before execution.
		bad = library('bad', '''extern def triple(x: bool) -> bool

def bad() -> i64
	if triple(true)
		return 1
	return 0
''', '--export', 'bad')
		result = run(runner, '--link-lib', linked, bad, provider)
		assert result.returncode == 2 and b'arithmetic requires numeric operands' in result.stderr, result
		print('PASS generic semantics: let inference, records/arrays, builtin calls, structural CoW, fallback, native parity, diagnostics')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='Use an already-built runner')
	args = parser.parse_args()
	runner = args.runner or BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	check(runner.resolve())


if __name__ == '__main__':
	main()
