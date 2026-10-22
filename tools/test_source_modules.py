#!/usr/bin/env python3
"""End-to-end source-module compilation using independent concrete MIR artifacts."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_modules import ModuleError, build, discover, symbol
from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def write(root: Path, module: str, source: str) -> None:
	path = root.joinpath(*module.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(source, encoding='utf-8')


def run(*args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run(args, capture_output=True, timeout=30)


def check_failure(root: Path, entry: str, reason: str) -> None:
	try:
		discover(root, entry)
	except ModuleError as exc:
		assert reason in str(exc), (reason, exc)
	else:
		raise AssertionError(f'expected {reason!r} for module {entry}')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		write(root, 'A.B', 'export def value() -> i64\n\treturn 19\n')
		write(root, 'C.B', 'export def value() -> i64\n\treturn 23\n')
		write(root, 'Utility.Combine',
			'import A.B alias First\nimport C.B alias Second\n'
			'export def total() -> i64\n'
			'\tlet label = "First.value()"  # Second.value() is not a call\n'
			'\tif label == "First.value()"\n'
			'\t\treturn First.value() + Second.value()\n'
			'\treturn 0\n')
		write(root, 'Main',
			'import Utility.Combine alias Combine\n'
			'def main() -> i64\n\treturn Combine.total()\n')
		output = root / 'linked.smirl'
		entry = build(root, 'Main', output, runner)
		assert entry == symbol('Main', 'main')
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (42, b'', b''), result
		print('PASS source modules: transitive imports, qualified functions and isolated public names')

		first = output.read_bytes()
		build(root, 'Main', output, runner)
		assert output.read_bytes() == first, 'linked libraries must be deterministic'
		listing = run(str(runner), '--list-lib', str(output))
		assert listing.returncode == 0
		for name in ('A.B', 'C.B'):
			assert symbol(name, 'value').encode() in listing.stdout
		print('PASS source modules: reproducible output, canonical mangled exports')

		# The module builder does not rewrite bodies or string/comment contents.
		original = next(m for m in discover(root, 'Main') if m.name == 'Utility.Combine')
		assert 'First.value()' in original.path.read_text()
		assert 'Second.value() is not a call' in original.path.read_text()
		assert 'total' in original.exports
		assert symbol('A.B', 'value') != symbol('A_B', 'value')
		print('PASS source modules: untouched source, collision-free encoded paths')

		write(root, 'Private', 'def secret() -> i64\n\treturn 7\nexport def safe() -> i64\n\treturn 1\n')
		write(root, 'Bad', 'import Private alias P\ndef main() -> i64\n\treturn P.secret()\n')
		try:
			build(root, 'Bad', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'not exported' in str(exc), exc
		else:
			raise AssertionError('private function incorrectly accessible')
		write(root, 'Missing', 'import Not.Present\ndef main() -> i64\n\treturn 0\n')
		check_failure(root, 'Missing', 'not found')
		write(root, 'Cycle1', 'import Cycle2\nexport def step() -> i64\n\treturn 1\n')
		write(root, 'Cycle2', 'import Cycle1\nexport def step() -> i64\n\treturn 2\n')
		check_failure(root, 'Cycle1', 'cyclic source imports')
		write(root, 'Duplicate', 'import A.B alias Same\nimport C.B alias Same\ndef main() -> i64\n\treturn 0\n')
		check_failure(root, 'Duplicate', 'duplicate import alias')
		write(root, 'Generic', 'export def hidden(x) -> i64\n\treturn 1\n')
		check_failure(root, 'Generic', 'exported function parameters require annotations')
		write(root, 'UnsupportedType', 'export def other(x: Shape) -> i64\n\treturn 1\n')
		check_failure(root, 'UnsupportedType', 'unsupported exported signature')
		print('PASS source modules: missing, cycles, private names, aliases and unsupported signatures rejected')

		write(root, 'Lists', 'export def count(xs: Array[Dict[string, i64]]) -> i64\n\treturn len(xs)\n')
		assert discover(root, 'Lists')[-1].exports['count'].parameters == 'xs: Array[Dict[string, i64]]'
		print('PASS source modules: nested generic type annotations')

		# Exercise typed HIR import signatures, not just scanner metadata.
		write(root, 'Collection.Fns',
			'export def size(values: Array[Dict[string, i64]]) -> i64\n'
			'\treturn len(values)\n')
		write(root, 'CollectionApp',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\tlet values = Dict[string, i64]().insert("answer", 42)\n'
			'\treturn F.size([values])\n')
		entry = build(root, 'CollectionApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (1, b'', b''), result
		write(root, 'WrongArgType',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\treturn F.size([4])\n')
		try:
			build(root, 'WrongArgType', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'expected' in str(exc), exc
		else:
			raise AssertionError('incompatible imported collection type accepted')
		write(root, 'WrongArgCount',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\treturn F.size()\n')
		try:
			build(root, 'WrongArgCount', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'argument count mismatch' in str(exc), exc
		else:
			raise AssertionError('incorrect imported function arity accepted')
		print('PASS source modules: typed nested collections, incompatible signatures rejected')

		# The runner exposes namespaced exports without requiring source rewriting.
		unnamed = root / 'unnamed.sirius'
		unnamed.write_text('def value() -> i64\n\treturn 9\n')
		artifact = root / 'renamed.smirl'
		assert run(str(runner), '--emit-lib', str(artifact), '--export-as', 'value', 'Renamed', str(unnamed)).returncode == 0
		assert run(str(runner), '--entry', 'Renamed', '--load-lib', str(artifact)).returncode == 9
		assert run(str(runner), '--emit-lib', str(artifact), '--export-as', 'value', 'bad.name', str(unnamed)).returncode == 2
		print('PASS source modules: explicitly aliased library exports validated')


if __name__ == '__main__':
	main()
