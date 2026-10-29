#!/usr/bin/env python3
"""Optional transitive .smirg bundles with offline loading and safe overrides."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from project_paths import executable_path
from build_support import BuildCache, compile_c, require_cc


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(value) for value in args)], capture_output=True, timeout=40)


def checked(runner: Path, *args: object) -> None:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		build_root = work / 'build'
		build_root.mkdir()
		for module, source in (
			('Leaf', 'export def increment(x) -> i64\n\treturn x + 1\n'),
			('Middle', 'import Leaf alias L\nexport def increment(x) -> i64\n\treturn L.increment(x) + 1\n'),
			('Top', 'import Middle alias M\nexport def increment(x) -> i64\n\treturn M.increment(x) + 1\n'),
		):
			file = build_root / f'{module}.sirius'
			file.write_text(source, encoding='utf-8')
			cmd = ['--emit-generic', module, build_root / f'{module}.smirg', file]
			if module != 'Leaf':
				cmd.append(build_root)
			checked(runner, *cmd)
			file.unlink()
		bare = (build_root / 'Top.smirg').read_bytes()
		assert bare.startswith(b'SIRIUS-MIR-GENERIC/3\n')
		bundle = work / 'bundle.smirg'
		checked(runner, '--bundle-generic', build_root / 'Top.smirg', bundle, build_root)
		first = bundle.read_bytes()
		assert first.startswith(b'SIRIUS-MIR-BUNDLE/1\n')
		checked(runner, '--bundle-generic', build_root / 'Top.smirg', bundle, build_root)
		assert first == bundle.read_bytes(), 'bundle must be reproducible'
		assert run(runner, '--bundle-generic', bundle, work / 'invalid.smirg', build_root).returncode != 0
		print('PASS bundle: deterministic transitive dependency collection')

		dist = work / 'dist'
		dist.mkdir()
		(dist / 'Top.smirg').write_bytes(first)
		app = dist / 'App.sirius'
		app.write_text('import Top alias T\ndef main() -> i64\n\treturn T.increment(39)\n', encoding='utf-8')
		output = dist / 'app.smirl'
		checked(runner, '--build-modules', dist, 'App', output)
		baseline = output.read_bytes()
		checked(runner, '--build-modules', dist, 'App', output)
		assert output.read_bytes() == baseline
		result = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', output)
		assert result.returncode == 42, result
		generated = dist / 'app.c'
		checked(runner, '--emit-linked-c', output, 'M_3_App_4_main', generated)
		native = executable_path(dist / 'app')
		compile_c(cache.cc, generated, native, cache.runtime())
		assert run(native).returncode == 42
		print('PASS bundle: self-contained transitive generic specialization and interpreter/native execution')

		# Separate package and source both take precedence over embedded copies.
		(dist / 'Leaf.smirg').write_bytes((build_root / 'Leaf.smirg').read_bytes())
		checked(runner, '--build-modules', dist, 'App', output)
		assert baseline == output.read_bytes()
		(dist / 'Leaf.smirg').write_bytes(b'invalid package')
		bad = run(runner, '--build-modules', dist, 'App', output)
		assert bad.returncode != 0 and b'invalid generic package' in bad.stderr, bad
		(dist / 'Leaf.smirg').unlink()
		(dist / 'Leaf.sirius').write_text('export def increment(x) -> bool\n\treturn true\n', encoding='utf-8')
		bad = run(runner, '--build-modules', dist, 'App', output)
		assert bad.returncode != 0 and b'dependency interface mismatch' in bad.stderr, bad
		(dist / 'Leaf.sirius').unlink()
		(dist / 'Leaf.sirius').write_text('broken source\n', encoding='utf-8')
		bad = run(runner, '--build-modules', dist, 'App', output)
		assert bad.returncode != 0, bad
		(dist / 'Leaf.sirius').unlink()
		print('PASS bundle: external package and source precedence; incompatible and invalid overrides rejected')

		# Reject tampering with the archive index and package payloads.
		for invalid in (first[:-1], first + b'garbage', first.replace(b'4\nLeaf\n', b'4\nFake\n', 1)):
			(dist / 'Top.smirg').write_bytes(invalid)
			bad = run(runner, '--build-modules', dist, 'App', output)
			assert bad.returncode != 0 and b'invalid generic bundle' in bad.stderr, bad
		(dist / 'Top.smirg').write_bytes(bare)
		missing = run(runner, '--build-modules', dist, 'App', output)
		assert missing.returncode != 0 and b'not found' in missing.stderr, missing
		print('PASS bundle: corruption and missing standalone dependencies rejected')
		(build_root / 'Leaf.smirg').unlink()
		missing = run(runner, '--bundle-generic', build_root / 'Top.smirg', bundle, build_root)
		assert missing.returncode != 0 and b'missing generic package dependency' in missing.stderr, missing
		print('PASS bundle: missing package detected at packaging time')


if __name__ == '__main__':
	main()
