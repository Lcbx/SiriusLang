#!/usr/bin/env python3
"""Infer callback fields in generic record literals without caller annotations."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export def sum_box(box, values) -> i64
	var total = 0
	for value in values
		total += box.run(value)
	return total

export def sum_nested(box, values) -> i64
	let saved = box.outer.inner.run
	var total = 0
	for value in values
		total += saved(value)
	return total

export def sum_two(box, values) -> i64
	var total = 0
	for value in values
		total += box.first(value) + box.second(value)
	return total
'''

APP = '''import Provider alias P

def double(x)
	return x * 2

def local(box, values) -> i64
	var total = 0
	for value in values
		total += box.run(value)
	return total

def main() -> i64
	let a = local({run = fn(x) x * 2, label = "hello"}, [7, 14])
	let b = P.sum_box({run = fn(x) x * 2}, [7, 14])
	let c = P.sum_nested({outer = {inner = {run = fn(x) x * 2}}}, [7, 14])
	let d = P.sum_box({run = double}, [7, 14])
	let e = P.sum_two({first = fn(x) x + 1, second = fn(x) x - 1}, [7, 14])
	if a != 42 or b != 42 or c != 42 or d != 42 or e != 42
		return 1
	return 42
'''

NEGATIVE = (
	('conflict', '''def conflict(box, numbers, words) -> i64
	var total = 0
	for n in numbers
		total += box.run(n)
	for word in words
		total += box.run(word)
	return total

def main() -> i64
	return conflict({run = fn(x) x}, [1], ["bad"])
''', b'conflicting generic callback parameter types'),
	('unrelated_field', '''def call_other(box, values) -> i64
	var total = 0
	for n in values
		total += box.other(n)
	return total

def main() -> i64
	return call_other({run = fn(x) x, other = fn(n: i64) n}, [7])
''', b'lambda parameter requires annotation'),
	('underdetermined', '''def ignored(box, values) -> i64
	return len(values)

def main() -> i64
	return ignored({run = fn(x) x}, [7])
''', b'lambda parameter requires annotation'),
	('return_type', '''def sum_box(box, values) -> i64
	var total = 0
	for n in values
		total += box.run(n)
	return total

def main() -> i64
	return sum_box({run = fn(x) str(x)}, [7])
''', b'expected i64, found string'),
)


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=120)


def check(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--runner', type=Path)
	parser.add_argument('--compiler', type=Path, help='existing native compiler (for bootstrap development)')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.seed()
	with tempfile.TemporaryDirectory(prefix='sirius-record-callbacks-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		for name, source, expected in NEGATIVE:
			(root / 'Negative.sirius').write_text(source)
			bad = run(compiler, '--module-root', root, 'Negative', root / f'{name}.c')
			assert bad.returncode != 0 and expected in bad.stderr, (name, bad)

		generated = root / 'App.c'
		check(run(compiler, '--module-root', root, 'App', generated))
		check(run(compile_native(cache, generated)), 42)
		audited = root / 'audited'
		check(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', audited))
		check(run(audited), 42)
		if args.native_only:
			print('PASS record callback literals: native, ARC and negative cases')
			return

		runner = args.runner.resolve() if args.runner else cache.mir_runner()
		library = root / 'source.smirl'
		check(run(runner, '--build-modules', root, 'App', library))
		check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
		check(run(runner, '--emit-generic', 'Provider', root / 'Provider.smirg', root / 'Provider.sirius', root))
		(root / 'Provider.sirius').unlink()
		packaged = root / 'packaged.smirl'
		check(run(runner, '--build-modules', root, 'App', packaged))
		check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', packaged), 42)
		linked = root / 'packaged.c'
		check(run(runner, '--emit-linked-c', packaged, 'M_3_App_4_main', linked))
		check(run(compile_native(cache, linked)), 42)
		check(run(compiler, '--module-root', root, 'App', root / 'native-packaged.c'))
		check(run(compile_native(cache, root / 'native-packaged.c')), 42)

	print('PASS record callback literals: nested fields, imports, packages, native/MIR and ARC')


if __name__ == '__main__':
	main()
