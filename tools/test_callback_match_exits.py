#!/usr/bin/env python3
"""Test callback inference at exhaustive match joins and early exits."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export rec Number(value: i64)
	pub value
export rec Word(value: string)
	pub value
export let Item = Number | Word

export def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

export def match_join(callback, values, item: Item) -> i64
	var local = callback
	match item
		Number(n)
			local = callback
		Word(s)
			local = callback
	return consume(local, values)

export def match_return(callback, values, item: Item) -> i64
	var local = callback
	match item
		Number(n)
			local = fn(x: i64) x + 1
			return 0
		Word(s)
			local = callback
	return consume(local, values)

export def if_return(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = fn(x: i64) x + 1
		return 0
	return consume(local, values)

export def guard_return(callback, values, enabled: bool) -> i64
	var local = callback
	enabled else return 0
	return consume(local, values)

export def matched_values(callback, item: Item) -> i64
	match item
		Number(n)
			return callback(n)
		Word(s)
			return 0
'''

APP = '''import Provider alias P

def main() -> i64
	let item: P.Item = P.Word("ok")
	let a = P.match_join(fn(x) x * 2, [7, 14], item)
	let b = P.match_return(fn(x) x * 2, [7, 14], item)
	let c = P.if_return(fn(x) x * 2, [7, 14], false)
	let d = P.guard_return(fn(x) x * 2, [7, 14], true)
	if a != 42 or b != 42 or c != 42 or d != 42
		return 1
	return 42
'''

NEGATIVES = {
	'match_partial': '''rec A(v: i64)
	pub v
rec B(value: string)
	pub value
let Item = A | B

def consume(callback, values) -> i64
	return callback(values[!0])

def relay(callback, values, item: Item) -> i64
	var local = callback
	match item
		A(v)
			local = fn(x: i64) x + 1
		B(v)
			local = callback
	return consume(local, values)

def main() -> i64
	let item: Item = B("a")
	return relay(fn(x) x * 2, [7, 14], item)
''',
	'match_conflict': '''rec A(v: i64)
	pub v
rec B(value: string)
	pub value
let Item = A | B

def relay(callback, item: Item) -> i64
	match item
		A(v)
			let n = callback(7)
		B(v)
			let s = callback("seven")
	return 0

def main() -> i64
	let item: Item = A(1)
	return relay(fn(x) x * 2, item)
''',
}


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=100)


def check(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--package-runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-callback-match-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		for name, source in NEGATIVES.items():
			(root / f'{name}.sirius').write_text(source)
			bad = run(cache.seed(), '--module-root', root, name, root / f'{name}.c')
			expected = b'ambiguous generic callback parameter types' if name == 'match_conflict' else b'lambda parameter requires annotation'
			assert bad.returncode != 0 and expected in bad.stderr, (name, bad)
		# Pattern bindings in a local generic function also constrain callbacks.
		local = PROVIDER + "\ndef main() -> i64\n\tlet item: Item = Number(2)\n\treturn matched_values(fn(x) x * 2, item)\n"
		(root / 'Local.sirius').write_text(local)
		local_output = root / 'Local.c'
		check(run(cache.seed(), '--module-root', root, 'Local', local_output))
		check(run(compile_native(cache, local_output)), 4)
		generated = root / 'App.c'
		check(run(cache.seed(), '--module-root', root, 'App', generated))
		check(run(compile_native(cache, generated)), 42)
		binary = root / 'audited'
		check(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', binary))
		check(run(binary), 42)
		if not args.native_only:
			runner = args.package_runner or cache.native_tool('run_cli')
			library = root / 'App.smirl'
			check(run(runner, '--build-modules', root, 'App', library))
			check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
			package = root / 'Provider.smirg'
			check(run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius', root))
			(root / 'Provider.sirius').unlink()
			generated = root / 'packaged.c'
			check(run(cache.seed(), '--module-root', root, 'App', generated))
			check(run(compile_native(cache, generated)), 42)
			library = root / 'packaged.smirl'
			check(run(runner, '--build-modules', root, 'App', library))
			check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
	print('PASS callback match: exhaustive joins, early returns, native/MIR, ARC and packages')


if __name__ == '__main__':
	main()
