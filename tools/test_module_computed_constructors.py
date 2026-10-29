#!/usr/bin/env python3
"""Imported computed constructors, named re-exports, and source-free packages."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, check_native, require_cc


def run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *(str(x) for x in args)], capture_output=True, timeout=90)


def write(root: Path, module: str, code: str) -> Path:
	path = root.joinpath(*module.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(code, encoding='utf-8')
	return path


def build(runner: Path, root: Path, entry: str, expected: int, cache: BuildCache) -> None:
	library = root / 'program.smirl'
	result = run(runner, '--build-modules', root, entry, library)
	assert result.returncode == 0, (result.stdout, result.stderr)
	result = run(runner, '--entry', f'M_{len(entry)}_{entry}_4_main', '--load-lib', library)
	assert result.returncode == expected, (result.returncode, result.stdout, result.stderr)
	generated = root / 'linked.c'
	result = run(runner, '--emit-linked-c', library, f'M_{len(entry)}_{entry}_4_main', generated)
	assert result.returncode == 0, (result.stdout, result.stderr)
	check_native(cache, generated, code=expected)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		provider_text = '''export rec Success(value: i64)
	pub value
	pub doubled = value * 2
	if value > 10
		pub doubled = doubled + 1
export rec Failure(code: i64)
	pub code
export let Outcome = Success | Failure
'''
		provider = write(root, 'Provider', provider_text)
		write(root, 'Facade', 'import Provider alias P\nexport P.Success alias Good\nexport P.Failure alias Bad\nexport P.Outcome alias Result\n')
		write(root, 'App', '''import Facade alias F

def main() -> i64
	let item: F.Result = F.Good(20)
	match item
		F.Good(value, doubled)
			return value + doubled
		F.Bad(code)
			return -code
''')
		build(runner, root, 'App', 61, cache)
		print('PASS imported/re-exported computed constructor')
		package = root / 'Provider.smirg'
		result = run(runner, '--emit-generic', 'Provider', package, provider)
		assert result.returncode == 0, (result.stdout, result.stderr)
		provider.unlink()
		build(runner, root, 'App', 61, cache)
		print('PASS source-free computed constructor')
		# Package an independent client and require invalidation if only the
		# constructor's expression changes (the record layout is unchanged).
		client = write(root, 'Client', 'import Provider alias P\nexport def score(x) -> i64\n\treturn P.Success(x).doubled\n')
		package_client = root / 'Client.smirg'
		result = run(runner, '--emit-generic', 'Client', package_client, client, root)
		assert result.returncode == 0, (result.stdout, result.stderr)
		client.unlink()
		write(root, 'Provider', provider_text.replace('pub doubled = doubled + 1', 'pub doubled = doubled + 2'))
		# Check the package dependency explicitly.
		write(root, 'Consumer', 'import Client alias C\ndef main() -> i64\n\treturn C.score(20)\n')
		result = run(runner, '--build-modules', root, 'Consumer', root / 'invalid.smirl')
		assert result.returncode == 2 and b'interface mismatch' in result.stderr, (result.stdout, result.stderr)
		print('PASS constructor-body changes invalidate dependent packages')
		# Constants and helpers refer to the defining module, not caller names.
		write(root, 'Provider', '''let OFFSET = 7

def step(x: i64) -> i64
	return x + OFFSET

def double(x: i64) -> i64
	return step(x) * 2

export rec Success(value: i64)
	pub value
	pub doubled = double(value)

export rec Failure(code: i64)
	pub code
export let Outcome = Success | Failure
''')
		write(root, 'App', '''import Facade alias F
let OFFSET = 100

def step(x: i64) -> i64
	return x - 100

def double(x: i64) -> i64
	return 0

def main() -> i64
	let item: F.Result = F.Good(20)
	match item
		F.Good(value, doubled)
			return value + doubled
		F.Bad(code)
			return -code
''')
		build(runner, root, 'App', 74, cache)
		print('PASS defining-module constant and transitive helper resolution')
		package = root / 'Provider.smirg'
		result = run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius')
		assert result.returncode == 0, (result.stdout, result.stderr)
		(root / 'Provider.sirius').unlink()
		build(runner, root, 'App', 74, cache)
		print('PASS hygienic captures through source-free and re-exported constructors')
		# Changing a private constant or helper invalidates the client package.
		client = write(root, 'Client', 'import Provider alias P\nexport def score(x) -> i64\n\treturn P.Success(x).doubled\n')
		result = run(runner, '--emit-generic', 'Client', root / 'Client.smirg', client, root)
		assert result.returncode == 0, (result.stdout, result.stderr)
		client.unlink()
		write(root, 'Consumer', 'import Client alias C\ndef main() -> i64\n\treturn C.score(20)\n')
		write(root, 'Provider', '''let OFFSET = 8

def step(x: i64) -> i64
	return x + OFFSET

def double(x: i64) -> i64
	return step(x) * 2

export rec Success(value: i64)
	pub value
	pub doubled = double(value)

export rec Failure(code: i64)
	pub code
export let Outcome = Success | Failure
''')
		result = run(runner, '--build-modules', root, 'Consumer', root / 'invalid.smirl')
		assert result.returncode == 2 and b'interface mismatch' in result.stderr, (result.stdout, result.stderr)
		print('PASS private dependency changes invalidate packages')
		# Never capture a missing provider name from the importing module.
		write(root, 'Provider', 'export rec Success(value: i64)\n\tpub value\n\tpub doubled = value + UNDECLARED\n')
		result = run(runner, '--build-modules', root, 'App', root / 'invalid.smirl')
		assert result.returncode == 2, (result.stdout, result.stderr)
		print('PASS unresolved defining-module names rejected')



if __name__ == '__main__':
	main()
