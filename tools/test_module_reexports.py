#!/usr/bin/env python3
"""Named function, constructor and constant re-exports through Sirius module interfaces."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_modules import discover
from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=30)


def checked(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def write(root: Path, name: str, source: str) -> Path:
	path = root / f'{name}.sirius'
	path.write_text(source, encoding='utf-8')
	return path


def build(runner: Path, root: Path, module: str) -> Path:
	output = root / 'result.smirl'
	checked(runner, '--build-modules', root, module, output)
	return output


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		write(root, 'Provider', 'export rec Point(x: i64, y: i64)\n\tpub x\n\tpub y\n'
			'export let SIZE = 3\n'
			'export def answer() -> i64\n\treturn 19\n'
			'export def twice(x) -> i64\n\treturn x + x\n'
			'def hidden() -> i64\n\treturn 1\n')
		write(root, 'Facade', 'import Provider alias P\n'
			'export P.answer alias result\n'
			'export P.answer\n'
			'export P.Point alias Spot\n'
			'export P.SIZE alias WIDTH\n'
			'export P.twice alias double\n')
		write(root, 'Facade2', 'import Facade alias F\n'
			'export F.result alias value\n'
			'export F.answer\n'
			'export F.Spot alias Position\n'
			'export F.WIDTH alias LENGTH\n'
			'export F.double alias times_two\n'
			'export def identity(p: F.Spot) -> F.Spot\n\treturn p\n')
		write(root, 'App', 'import Facade2 alias F\n'
			'def main() -> i64\n'
			'\tlet p: F.Position = F.Position(4, 6)\n'
			'\tlet q: F.Position = F.identity(p)\n'
			'\treturn F.value() + F.LENGTH + F.times_two(5) + q.x + q.y + F.answer() - 19\n')
		library = build(runner, root, 'App')
		scanned = {module.name: module for module in discover(root, 'App')}
		assert 'Position' in scanned['Facade2'].types
		assert 'LENGTH' in scanned['Facade2'].constants
		assert scanned['Facade2'].exports['times_two'].parameters == 'x'
		assert scanned['Facade2'].exports['value'].returns == 'i64'
		first = library.read_bytes()
		assert run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library).returncode == 42
		build(runner, root, 'App')
		assert first == library.read_bytes()
		print('PASS transitive named re-exports: functions, constants, constructors and aliases')

		write(root, 'Private', 'import Provider alias P\nexport P.hidden alias leaked\n')
		write(root, 'PrivateApp', 'import Private alias P\ndef main() -> i64\n\treturn P.leaked()\n')
		bad = run(runner, '--build-modules', root, 'PrivateApp', library)
		assert bad.returncode != 0 and b'not exported' in bad.stderr, bad.stderr
		write(root, 'Collision', 'import Provider alias P\n'
			'export def same() -> i64\n\treturn 1\nexport P.answer alias same\n')
		write(root, 'CollisionApp', 'import Collision alias C\ndef main() -> i64\n\treturn C.same()\n')
		bad = run(runner, '--build-modules', root, 'CollisionApp', library)
		assert bad.returncode != 0 and b'duplicate exported name' in bad.stderr, bad.stderr
		write(root, 'Unknown', 'export Missing.answer alias result\n')
		write(root, 'UnknownApp', 'import Unknown alias U\ndef main() -> i64\n\treturn 0\n')
		bad = run(runner, '--build-modules', root, 'UnknownApp', library)
		assert bad.returncode != 0 and b'unknown re-export module alias' in bad.stderr, bad.stderr
		write(root, 'Repeated', 'import Provider alias P\nexport P.answer\nexport P.answer\n')
		write(root, 'RepeatedApp', 'import Repeated alias R\ndef main() -> i64\n\treturn R.answer()\n')
		bad = run(runner, '--build-modules', root, 'RepeatedApp', library)
		assert bad.returncode != 0 and b'duplicate exported name' in bad.stderr, bad.stderr
		print('PASS re-export visibility, name collisions and missing aliases')

		# A facade that also exports a generic body can be distributed as .smirg.
		write(root, 'Pack', 'import Provider alias P\n'
			'export P.answer alias result\n'
			'export P.Point alias Spot\n'
			'export P.SIZE alias WIDTH\n'
			'export P.twice alias double\n'
			'export def extra(x) -> i64\n\treturn P.twice(x)\n')
		checked(runner, '--emit-generic', 'Pack', root / 'Pack.smirg', root / 'Pack.sirius', root)
		(root / 'Pack.sirius').unlink()
		write(root, 'PackageApp', 'import Pack alias P\ndef main() -> i64\n'
			'\tlet p: P.Spot = P.Spot(4, 6)\n'
			'\treturn P.result() + P.WIDTH + P.double(5) + p.x + p.y\n')
		library = build(runner, root, 'PackageApp')
		assert run(runner, '--entry', 'M_10_PackageApp_4_main', '--load-lib', library).returncode == 42
		print('PASS source-free generic package with named re-export interface')


if __name__ == '__main__':
	main()
