#!/usr/bin/env python3
"""Test higher-order callback inference through nested structural record paths."""
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

export def nested_literal(callback, values) -> i64
	let box = {outer = {inner = {run = callback}}}
	let saved = box.outer.inner.run
	return consume(saved, values)

export def nested_copy(callback, values) -> i64
	let box = {inner = {run = callback}}
	let copy = {container = box}
	let moved = copy.container.inner
	return consume(moved.run, values)

export def nested_mutable(callback, values) -> i64
	var box = {inner = {run = callback}, other = {run = callback}}
	box.inner.run = callback
	let next = box.inner
	return consume(next.run, values)

export def subtree_replace(callback, values) -> i64
	var box = {inner = {run = callback}, other = {run = callback}}
	box.inner = {run = callback}
	return consume(box.inner.run, values)

export def subtree_copy(callback, values) -> i64
	var box = {inner = {run = callback}, other = {run = callback}}
	box.inner = box.other
	return consume(box.inner.run, values)

export def nested_branch(callback, values, enabled: bool) -> i64
	var box = {outer = {inner = {run = callback}}}
	if enabled
		box.outer.inner = {run = callback}
	else
		box.outer.inner.run = callback
	let saved = box.outer.inner.run
	return consume(saved, values)
'''

APP = '''import Provider alias P

def double(x)
	return x * 2

def main() -> i64
	let a = P.nested_literal(fn(x) x * 2, [7, 14])
	let b = P.nested_copy(double, [7, 14])
	let c = P.nested_mutable(fn(x) x * 2, [7, 14])
	let d = P.subtree_replace(double, [7, 14])
	let e = P.subtree_copy(double, [7, 14])
	let f = P.nested_branch(fn(x) x * 2, [7, 14], true)
	let g = P.nested_branch(double, [7, 14], false)
	if a != 42 or b != 42 or c != 42 or d != 42 or e != 42 or f != 42 or g != 42
		return 1
	return 42
'''

NEGATIVE = '''def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

def relay(callback, values) -> i64
	var box = {inner = {run = callback}}
	box.inner = {run = fn(s: string) len(s)}
	return consume(box.inner.run, values)

def main() -> i64
	return relay(fn(x) x * 2, [7, 14])
'''


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=120)


def check(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--package-runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-nested-callbacks-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		(root / 'Negative.sirius').write_text(NEGATIVE)
		bad = run(cache.seed(), '--module-root', root, 'Negative', root / 'negative.c')
		assert bad.returncode != 0, bad
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
	print('PASS nested callback aliases: record literals, copies, assignments, ARC, MIR and packages')


if __name__ == '__main__':
	main()
