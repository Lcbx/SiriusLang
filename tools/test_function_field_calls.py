#!/usr/bin/env python3
"""Verify direct calls to record-held Fn values, including packaged generics."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export def invoke(callback, values) -> i64
	let holder = {nested = {run = callback}}
	return holder.nested.run(values[!0])

export def make_holder(callback)
	return {run = callback}
'''

APP = '''import Provider alias P

def twice(x: i64) -> i64
	return x * 2

def singleton(x: i64) -> Array[i64]
	return [x]

def main() -> i64
	let box = {run = twice}
	let nested = {inner = box}
	let lambda = {run = fn(x: i64) x * 2}
	var mutable = {run = twice}
	mutable.run = fn(x: i64) x * 2
	let packaged = P.make_holder(twice)
	let append_field = {append = singleton}
	let a = box.run(21)
	let b = nested.inner.run(21)
	let c = lambda.run(21)
	let d = mutable.run(21)
	let e = P.invoke(fn(x) x * 2, [21])
	let f = packaged.run(21)
	let g = append_field.append(21)
	let h = P.make_holder(twice).run(21)
	if a != 42 or b != 42 or c != 42 or d != 42 or e != 42 or f != 42 or g[!0] != 21 or h != 42
		return 1
	return 42
'''

NEGATIVES = {
	'type': ('''def twice(x: i64) -> i64
	return x * 2

def main() -> i64
	let holder = {run = twice}
	return holder.run("bad")
''', 'indirect call argument type mismatch'),
	'arity': ('''def twice(x: i64) -> i64
	return x * 2

def main() -> i64
	let holder = {run = twice}
	return holder.run(1, 2)
''', 'indirect call argument count mismatch'),
	'not_callable': ('''def main() -> i64
	let holder = {run = 9}
	return holder.run(21)
''', "record field 'run' is not callable"),
	'missing': ('''def main() -> i64
	let holder = {run = 9}
	return holder.missing(21)
''', 'unknown record field'),
}


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
	with tempfile.TemporaryDirectory(prefix='sirius-function-field-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		for name, (source, expected) in NEGATIVES.items():
			(root / f'{name}.sirius').write_text(source)
			bad = run(cache.seed(), '--module-root', root, name, root / f'{name}.c')
			assert bad.returncode != 0 and expected.encode() in bad.stderr, (name, bad)
		generated = root / 'App.c'
		check(run(cache.seed(), '--module-root', root, 'App', generated))
		check(run(compile_native(cache, generated)), 42)
		audited = root / 'audited'
		check(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', audited))
		check(run(audited), 42)
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
	print('PASS record field calls: direct, nested, mutable, imported, native/MIR, ARC, packages, invalid calls')


if __name__ == '__main__':
	main()
