#!/usr/bin/env python3
"""Native Sirius module builds that consume verified .smirg packages without source."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *args], capture_output=True, timeout=30)


def checked(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def source(root: Path, name: str, content: str) -> Path:
	path = root.joinpath(*name.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(content, encoding='utf-8')
	return path


def package(runner: Path, root: Path, name: str, content: str) -> Path:
	path = source(root, name, content)
	artifact = path.with_suffix('.smirg')
	checked(runner, '--emit-generic', name, str(artifact), str(path))
	path.unlink()
	return artifact


def build(runner: Path, root: Path, entry: str) -> Path:
	artifact = root / 'application.smirl'
	checked(runner, '--build-modules', str(root), entry, str(artifact))
	return artifact


def check_main(runner: Path, library: Path, entry: str, expected: int) -> None:
	result = run(runner, '--entry', f'M_{len(entry)}_{entry}_4_main', '--load-lib', str(library))
	assert result.returncode == expected, (result.returncode, result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).native_tool('run_cli')
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		package(runner, root, 'Library.Ops',
			'export def twice(x) -> i64\n\treturn x + x\n'
			'export def size(xs) -> i64\n\treturn len(xs)\n'
			'export def field(value) -> i64\n\treturn value.x\n'
			'def hidden(x)\n\treturn x\n')
		source(root, 'Adapter',
			'import Library.Ops alias Ops\n'
			'export def compute() -> i64\n\treturn Ops.twice(20) + Ops.twice(1)\n')
		source(root, 'App',
			'import Adapter alias A\nimport Library.Ops alias Ops\n'
			'rec Point(x: i64)\n\tpub x\n'
			'def main() -> i64\n'
			'\treturn A.compute() + Ops.size([1, 2]) + Ops.field(Point(5))\n')
		library = build(runner, root, 'App')
		check_main(runner, library, 'App', 49)
		first = library.read_bytes()
		build(runner, root, 'App')
		assert first == library.read_bytes(), 'package builds must be deterministic'
		listing = checked(runner, '--list-lib', str(library)).stdout
		assert listing.count(b'M_7_Library_3_Ops_5_twice_G') == 1, listing  # repeated calls share one specialization
		assert b'import ' not in listing
		print('PASS module packages: source-free, dotted paths, transitive imports, record and array arguments, deterministic output')

		source(root, 'Wrong', 'import Library.Ops alias Ops\ndef main() -> i64\n\treturn Ops.size(4)\n')
		bad = run(runner, '--build-modules', str(root), 'Wrong', str(library))
		assert bad.returncode == 2 and b'len expects' in bad.stderr, bad
		source(root, 'Private', 'import Library.Ops alias Ops\ndef main() -> i64\n\treturn Ops.hidden(1)\n')
		bad = run(runner, '--build-modules', str(root), 'Private', str(library))
		assert bad.returncode == 2 and b'not exported' in bad.stderr, bad
		print('PASS module packages: type errors and private generic functions rejected')

		# A source module wins over a same-named package; malformed source is not
		# silently bypassed by choosing the generic artifact instead.
		opath = source(root, 'Library.Ops', 'this is not valid Sirius\n')
		bad = run(runner, '--build-modules', str(root), 'App', str(library))
		assert bad.returncode == 2 and b'module error' in bad.stderr, bad
		opath.unlink()

		original = root / 'Library/Ops.smirg'
		good = original.read_bytes()
		for invalid in (good[:-10], good.replace(b'SIRIUS-MIR-GENERIC/3', b'SIRIUS-MIR-GENERIC/9', 1)):
			original.write_bytes(invalid)
			bad = run(runner, '--build-modules', str(root), 'App', str(library))
			assert bad.returncode == 2 and b'invalid generic package' in bad.stderr, bad
		original.write_bytes(good)
		other = root / 'Library/Mismatched.smirg'
		other.write_bytes(good)
		source(root, 'Mismatch', 'import Library.Mismatched alias M\ndef main() -> i64\n\treturn M.twice(21)\n')
		bad = run(runner, '--build-modules', str(root), 'Mismatch', str(library))
		assert bad.returncode == 2 and b'generic package module mismatch' in bad.stderr, bad
		print('PASS module packages: source precedence, corrupted artifacts, mismatched package identities rejected')


if __name__ == '__main__':
	main()
