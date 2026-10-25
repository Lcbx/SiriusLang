#!/usr/bin/env python3
"""Test generic callback constraints through record fields and local aliases."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

export def relay_record(callback, values) -> i64
	let holder = {run = callback}
	let copied = holder
	return consume(copied.run, values)

export def relay_mutable(callback, values) -> i64
	var local = callback
	local = callback
	return consume(local, values)

export def relay_field_mutable(callback, values) -> i64
	var holder = {run = callback}
	holder.run = callback
	return consume(holder.run, values)

export def relay_lengths(callback, values) -> i64
	let holder = {run = callback}
	let local = holder.run
	var total = 0
	for value in values
		total += len(local(value))
	return total
'''

APP = '''import Provider alias P

def double(x)
	return x * 2

def stringify(x)
	return str(x)

def call_via_local(callback, values) -> i64
	let holder = {run = callback}
	let local = holder.run
	var total = 0
	for value in values
		total += local(value)
	return total

def shadowed(callback, values) -> i64
	let holder = {run = callback}
	if false
		let holder = {run = fn(s: string) len(s)}
	return P.consume(holder.run, values)

def main() -> i64
	let a = P.relay_record(fn(x) x * 2, [7, 14])
	let b = P.relay_mutable(double, [7, 14])
	let c = P.relay_field_mutable(fn(x) x * 2, [7, 14])
	let d = call_via_local(fn(x) x * 2, [7, 14])
	let e = shadowed(fn(x) x * 2, [7, 14])
	let f = P.relay_lengths(stringify, [7, 14])
	if a != 42 or b != 42 or c != 42 or d != 42 or e != 42 or f != 3
		return 1
	return 42
'''

NEGATIVE = '''import Provider alias P

def replaced(callback, values) -> i64
	var holder = callback
	holder = fn(s: string) len(s)
	return P.consume(holder, values)

def main() -> i64
	return replaced(fn(x) x * 2, [7, 14])
'''


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def check(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true', help='skip source-free package checks')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-callback-aliases-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		(root / 'Negative.sirius').write_text(NEGATIVE)
		bad = run(cache.seed(), '--module-root', root, 'Negative', root / 'Negative.c')
		assert bad.returncode != 0 and b'lambda parameter requires annotation' in bad.stderr, bad
		generated = root / 'App.c'
		check(run(cache.seed(), '--module-root', root, 'App', generated))
		check(run(compile_native(cache, generated)), 42)
		binary = root / 'audited'
		check(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', binary))
		check(run(binary), 42)
		if not args.native_only:
			runner = cache.native_tool('run_cli')
			package = root / 'Provider.smirg'
			check(run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius', root))
			(root / 'Provider.sirius').unlink()
			generated = root / 'packaged.c'
			check(run(cache.seed(), '--module-root', root, 'App', generated))
			check(run(compile_native(cache, generated)), 42)
	print('PASS callback aliases: records, mutable bindings, imports, negative cases, ARC and packages')


if __name__ == '__main__':
	main()
