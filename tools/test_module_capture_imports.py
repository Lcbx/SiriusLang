#!/usr/bin/env python3
"""Imported generic bodies retain defining-module imports despite caller alias collisions."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_c, link_flags, require_cc
from project_paths import ROOT


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=90)


def checked(runner: Path, *args: object) -> None:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		core = root / 'Core.sirius'
		core.write_text('''export rec Box(value: i64)
	pub value
export rec Good(value: i64)
	pub value
export rec Bad(code: i64)
	pub code
export let Choice = Good | Bad
export def bump(value) -> i64
	return value + 1
''', encoding='utf-8')
		ops = root / 'Ops.sirius'
		ops.write_text('''import Core alias C
export rec Snapshot(value: i64)
	pub value
	pub updated = C.bump(value)
export def wrap(value)
	return C.Box(C.bump(value))
export def read(value)
	let item: C.Box = C.Box(value)
	return item.value
export def classify(value)
	let choice: C.Choice = C.Good(value)
	match choice
		C.Good(found)
			return found + 1
		C.Bad(missing)
			return 0
''', encoding='utf-8')
		(root / 'Decoy.sirius').write_text('''export def bump(value) -> i64
	return value + 100
''', encoding='utf-8')
		(root / 'App.sirius').write_text('''import Ops alias O
import Decoy alias C

def main() -> i64
	let result = O.wrap(40)
	let snapshot = O.Snapshot(40)
	if snapshot.updated != 41
		return 4
	if result.value != 41 or O.read(41) != 41
		return 1
	if C.bump(1) != 101
		return 2
	if O.classify(40) != 41
		return 3
	return result.value + 1
''', encoding='utf-8')

		def exercise(label: str) -> None:
			library = root / f'{label}.smirl'
			checked(runner, '--build-modules', root, 'App', library)
			result = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
			assert result.returncode == 42, (label, result)
			original = library.read_bytes()
			checked(runner, '--build-modules', root, 'App', library)
			assert library.read_bytes() == original, 'nondeterministic linking'
			generated = root / f'{label}.c'
			checked(runner, '--emit-linked-c', library, 'M_3_App_4_main', generated)
			native = root / f'{label}-native'
			compile_c(cache.cc, generated, native, cache.runtime())
			assert subprocess.run([str(native)], capture_output=True).returncode == 42
			audited = root / f'{label}-audited'
			build = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function',
				'-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated),
				str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(audited)],
				capture_output=True)
			assert build.returncode == 0, (build.stdout, build.stderr)
			assert subprocess.run([str(audited)], capture_output=True).returncode == 42

		exercise('source')
		print('PASS imported generic bodies: defining-module alias and caller collision')
		checked(runner, '--emit-generic', 'Core', root / 'Core.smirg', core)
		checked(runner, '--emit-generic', 'Ops', root / 'Ops.smirg', ops, root)
		core.unlink()
		ops.unlink()
		exercise('packages')
		print('PASS imported generic bodies: source-free packages and deterministic linking')

		# Caller imports must not rescue an unbound alias in the owner.
		(root / 'BadOps.sirius').write_text('''export def bad(value)
	return C.bump(value)
''', encoding='utf-8')
		(root / 'BadApp.sirius').write_text('''import BadOps alias O
import Core alias C

def main() -> i64
	return O.bad(40)
''', encoding='utf-8')
		result = run(runner, '--build-modules', root, 'BadApp', root / 'bad.smirl')
		assert result.returncode == 2 and b'unresolved defining-module constructor name: C' in result.stderr, result
		print('PASS unresolved owner imports rejected despite caller alias')

		(root / 'BadCtor.sirius').write_text('''import Core alias C
export rec Snapshot(value: i64)
	pub value
	pub invalid = C.missing(value)
''', encoding='utf-8')
		(root / 'CtorApp.sirius').write_text('''import BadCtor alias B

def main() -> i64
	return B.Snapshot(42).value
''', encoding='utf-8')
		result = run(runner, '--build-modules', root, 'CtorApp', root / 'bad-constructor.smirl')
		assert result.returncode == 2 and b'not exported' in result.stderr, result
		print('PASS unexported constructor dependency member rejected')


if __name__ == '__main__':
	main()
