#!/usr/bin/env python3
"""Cross-module union aliases, qualified patterns, visibility and .smirg imports."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def write(root: Path, module: str, source: str) -> Path:
	path = root.joinpath(*module.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(source, encoding='utf-8')
	return path


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *args], capture_output=True, timeout=60)


def build(runner: Path, root: Path, module: str, expected: int = 42) -> bytes:
	artifact = root / 'linked.smirl'
	result = run(runner, '--build-modules', str(root), module, str(artifact))
	assert result.returncode == 0, (result.stdout, result.stderr)
	result = run(runner, '--entry', f'M_{len(module)}_{module}_4_main', '--load-lib', str(artifact))
	assert result.returncode == expected, (result.returncode, result.stdout, result.stderr)
	return artifact.read_bytes()


def reject(runner: Path, root: Path, module: str, expected: bytes) -> None:
	result = run(runner, '--build-modules', str(root), module, str(root / 'bad.smirl'))
	assert result.returncode == 2 and expected in result.stderr, (expected, result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).mir_runner()
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		variants = write(root, 'Core.Variants',
			'export rec Success(value: i64)\n\tpub value\n'
			'export rec Failure(code: i64)\n\tpub code\n'
			'rec Secret(flag: bool)\n\tpub flag\n'
			'export let Result = Success | Failure\n'
			'let Private = Success | Secret\n'
			'export def check(result: Result) -> i64\n'
			'\tmatch result\n'
			'\t\tSuccess(value)\n\t\t\treturn value\n'
			'\t\tFailure(code)\n\t\t\treturn -code\n'
			'export def make(unused) -> Result\n\treturn Success(42)\n')
		write(root, 'Facade',
			'import Core.Variants alias V\n'
			'export V.Success alias Good\n'
			'export V.Failure alias Bad\n'
			'export V.Result alias Outcome\n'
			'export V.make alias create\n')
		write(root, 'App',
			'import Facade alias F\n'
			'def main() -> i64\n'
			'\tlet result: F.Outcome = F.create(0)\n'
			'\tmatch result\n'
			'\t\tF.Good(n)\n\t\t\treturn n\n'
			'\t\tF.Bad(error)\n\t\t\treturn -error\n')
		write(root, 'Direct',
			'import Core.Variants alias V\n'
			'def main() -> i64\n'
			'\tlet result: V.Result = V.Success(42)\n'
			'\treturn V.check(result)\n')
		assert build(runner, root, 'Direct')
		original = build(runner, root, 'App')
		assert build(runner, root, 'App') == original, 'nondeterministic module linking'
		print('PASS modules: qualified union types, constructors, patterns, re-exports, deterministic linking')

		write(root, 'Private',
			'import Core.Variants alias V\n'
			'def main() -> i64\n\tlet value: V.Private = V.Success(42)\n\treturn value.value\n')
		reject(runner, root, 'Private', b'not exported')
		write(root, 'Hidden',
			'import Core.Variants alias V\n'
			'def main() -> i64\n'
			'\tlet result: V.Result = V.Success(42)\n'
			'\tmatch result\n'
			'\t\tV.Success(n)\n\t\t\treturn n\n'
			'\t\tV.Secret(other)\n\t\t\treturn 0\n')
		reject(runner, root, 'Hidden', b'not exported')
		write(root, 'Duplicate',
			'import Core.Variants alias V\n'
			'def main() -> i64\n'
			'\tlet result: V.Result = V.Success(42)\n'
			'\tmatch result\n'
			'\t\tV.Success(n)\n\t\t\treturn n\n'
			'\t\tV.Success(other)\n\t\t\treturn other\n')
		reject(runner, root, 'Duplicate', b'duplicate match alternative')
		write(root, 'NotExhaustive',
			'import Core.Variants alias V\n'
			'def main() -> i64\n'
			'\tlet result: V.Result = V.Success(42)\n'
			'\tmatch result\n'
			'\t\tV.Success(n)\n\t\t\treturn n\n')
		reject(runner, root, 'NotExhaustive', b'non-exhaustive match')
		print('PASS modules: private types/constructors, duplicate arms, nonexhaustive matches rejected')

		package = variants.with_suffix('.smirg')
		result = run(runner, '--emit-generic', 'Core.Variants', str(package), str(variants))
		assert result.returncode == 0, (result.stdout, result.stderr)
		variants.unlink()
		reject(runner, root, 'Direct', b'not exported')  # Concrete functions are not in .smirg.
		reject(runner, root, 'Private', b'not exported')
		reject(runner, root, 'Hidden', b'not exported')
		package_build = build(runner, root, 'App')
		assert build(runner, root, 'App') == package_build
		print('PASS modules: source-free .smirg constructors, exported unions and generic re-export')


if __name__ == '__main__':
	main()
