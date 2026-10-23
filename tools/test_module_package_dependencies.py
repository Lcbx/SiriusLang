#!/usr/bin/env python3
"""Source-free chained package imports with pinned typed dependency interfaces."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=30)


def checked(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		ops = root / 'Ops.sirius'
		ops.write_text('export def twice(x) -> i64\n\treturn x + x\n', encoding='utf-8')
		checked(runner, '--emit-generic', 'Ops', root / 'Ops.smirg', ops)
		ops.unlink()
		adapter = root / 'Adapter.sirius'
		adapter.write_text('import Ops alias O\nexport def apply(x) -> i64\n\treturn O.twice(x) + 2\n', encoding='utf-8')
		assert run(runner, '--emit-generic', 'Adapter', root / 'Adapter.smirg', adapter).returncode != 0
		checked(runner, '--emit-generic', 'Adapter', root / 'Adapter.smirg', adapter, root)
		first = (root / 'Adapter.smirg').read_bytes()
		checked(runner, '--emit-generic', 'Adapter', root / 'Adapter.smirg', adapter, root)
		assert first == (root / 'Adapter.smirg').read_bytes()
		adapter.unlink()
		app = root / 'App.sirius'
		app.write_text('import Adapter alias A\nimport Ops alias O\n'
			'def main() -> i64\n\treturn A.apply(20) + O.twice(0)\n', encoding='utf-8')
		output = root / 'result.smirl'
		checked(runner, '--build-modules', root, 'App', output)
		linked = output.read_bytes()
		checked(runner, '--build-modules', root, 'App', output)
		assert linked == output.read_bytes()
		assert run(runner, '--entry', 'M_3_App_4_main', '--load-lib', output).returncode == 42
		print('PASS source-free chained generic packages and deterministic builds')
		# Package code can use imported constants and named record types.
		settings = root / 'Settings.sirius'
		settings.write_text('export let OFFSET = 2\n', encoding='utf-8')
		points = root / 'Geometry.sirius'
		points.write_text('export rec Point(x: i64)\n\tpub x\n', encoding='utf-8')
		provider = root / 'Wrapped.sirius'
		provider.write_text('import Settings alias S\nimport Geometry alias G\n'
			'export def add(value) -> i64\n\tlet p: G.Point = G.Point(value)\n\treturn p.x + S.OFFSET\n', encoding='utf-8')
		checked(runner, '--emit-generic', 'Wrapped', root / 'Wrapped.smirg', provider, root)
		provider.unlink()
		wrapped_app = root / 'WrappedApp.sirius'
		wrapped_app.write_text('import Wrapped alias W\ndef main() -> i64\n\treturn W.add(40)\n', encoding='utf-8')
		checked(runner, '--build-modules', root, 'WrappedApp', output)
		assert run(runner, '--entry', 'M_10_WrappedApp_4_main', '--load-lib', output).returncode == 42
		settings.write_text('export let OFFSET = 3\n', encoding='utf-8')
		stale = run(runner, '--build-modules', root, 'WrappedApp', output)
		assert stale.returncode != 0 and b'dependency interface mismatch' in stale.stderr, stale.stderr
		settings.write_text('export let OFFSET = 2\n', encoding='utf-8')
		print('PASS imported constructors, constant values and interface pinning')


		# The captured public interface is part of the package contract.
		ops.write_text('export def twice(x) -> bool\n\treturn true\n', encoding='utf-8')
		checked(runner, '--emit-generic', 'Ops', root / 'Ops.smirg', ops)
		ops.unlink()
		mismatch = run(runner, '--build-modules', root, 'App', output)
		assert mismatch.returncode != 0 and b'dependency interface mismatch' in mismatch.stderr, mismatch.stderr
		print('PASS incompatible dependency package replacement rejected')

		# Malformed dependency manifest must not be accepted as a source override.
		(root / 'Adapter.smirg').write_bytes(first[:-1])
		bad = run(runner, '--build-modules', root, 'App', output)
		assert bad.returncode != 0 and b'invalid generic package' in bad.stderr, bad.stderr
		(root / 'Adapter.smirg').write_bytes(first)
		print('PASS corrupted dependency package rejected')

		# Source takes precedence over a dependency package, including malformed source.
		ops.write_text('broken source\n', encoding='utf-8')
		bad = run(runner, '--build-modules', root, 'App', output)
		assert bad.returncode != 0, bad.stderr
		print('PASS dependency source precedence preserved')


if __name__ == '__main__':
	main()
