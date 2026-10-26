#!/usr/bin/env python3
"""Test path-sensitive callback aliases across conditionals and loops."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

export def branch_join(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = callback
	else
		local = callback
	return consume(local, values)

export def field_join(callback, values, enabled: bool) -> i64
	var holder = {run = callback}
	if enabled
		holder.run = callback
	else
		holder.run = callback
	return consume(holder.run, values)

export def elif_join(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = callback
	elif len(values) > 0
		local = callback
	else
		local = callback
	return consume(local, values)

export def optional_branch(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = callback
	return consume(local, values)

export def loop_preserves(callback, values) -> i64
	var local = callback
	for value in values
		local = callback
	return consume(local, values)
'''

APP = '''import Provider alias P

def twice(x)
	return x * 2

def main() -> i64
	let a = P.branch_join(fn(x) x * 2, [7, 14], true)
	let b = P.branch_join(twice, [7, 14], false)
	let c = P.field_join(fn(x) x * 2, [7, 14], true)
	let d = P.field_join(twice, [7, 14], false)
	let e = P.elif_join(fn(x) x * 2, [7, 14], false)
	let f = P.optional_branch(fn(x) x * 2, [7, 14], false)
	let g = P.loop_preserves(fn(x) x * 2, [7, 14])
	if a != 42 or b != 42 or c != 42 or d != 42 or e != 42 or f != 42 or g != 42
		return 1
	return 42
'''

NEGATIVES = {
	'partial': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = fn(x: i64) x + 1
	return consume(local, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14], true)
''',
	'branch_order': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values, enabled: bool) -> i64
	var local = callback
	if enabled
		local = fn(x: i64) x + 1
	else
		local = callback
	return consume(local, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14], true)
''',
	'field': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values, enabled: bool) -> i64
	var holder = {run = callback}
	if enabled
		holder.run = fn(x: i64) x + 1
	return consume(holder.run, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14], true)
''',
	'loop': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values) -> i64
	var local = callback
	for value in values
		local = fn(x: i64) x + 1
	return consume(local, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14])
''',
	'loop_introduce': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values) -> i64
	var local = fn(x: i64) x + 1
	for value in values
		local = callback
	return consume(local, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14])
''',
	'while': '''def consume(callback, values) -> i64
	var result = 0
	for value in values
		result += callback(value)
	return result

def relay(callback, values) -> i64
	var local = callback
	while false
		local = fn(x: i64) x + 1
	return consume(local, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14])
''',
	'conflict': '''def relay(callback, enabled: bool) -> i64
	if enabled
		let left = callback(7)
	else
		let right = callback("seven")
	return 0

def main() -> i64
	return relay(fn(x) x * 2, true)
''',
}


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def check(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--package-runner', type=Path, help='already-built Sirius runner for generic-package emission')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-callback-flow-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		for name, source in NEGATIVES.items():
			path = root / f'Negative_{name}.sirius'
			path.write_text(source)
			bad = run(cache.seed(), '--module-root', root, f'Negative_{name}', root / f'{name}.c')
			expected = b'conflicting generic callback parameter types' if name == 'conflict' else b'underdetermined callback parameters'
			assert bad.returncode != 0 and expected in bad.stderr, (name, bad)
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
			package = root / 'Provider.smirg'
			check(run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius', root))
			(root / 'Provider.sirius').unlink()
			generated = root / 'packaged.c'
			check(run(cache.seed(), '--module-root', root, 'App', generated))
			check(run(compile_native(cache, generated)), 42)
	print('PASS callback flow: conditional joins, loops, conflicts, ARC and packages')


if __name__ == '__main__':
	main()
