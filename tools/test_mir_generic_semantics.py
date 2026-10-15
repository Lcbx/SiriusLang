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

		# Invalid concrete specializations must fail before execution.
		bad = library('bad', '''extern def triple(x: bool) -> bool

def bad() -> i64
	if triple(true)
		return 1
	return 0
''', '--export', 'bad')
		result = run(runner, '--link-lib', linked, bad, provider)
		assert result.returncode == 2 and b'arithmetic requires numeric operands' in result.stderr, result
		print('PASS generic semantics: let inference, specialization, structural CoW, fallback, native parity, diagnostics')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='Use an already-built runner')
	args = parser.parse_args()
	runner = args.runner or BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	check(runner.resolve())


if __name__ == '__main__':
	main()
