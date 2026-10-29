#!/usr/bin/env python3
"""Exercise deep source/package graphs without repeated root discovery."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=60)


def checked(runner: Path, *args: object) -> None:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).mir_runner()
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		for index in range(12):
			name = f'P{index:02d}'
			previous = f'P{index - 1:02d}'
			body = ('export def value(x) -> i64\n\treturn x\n' if index == 0 else
				f'import {previous} alias Prev\nexport def value(x) -> i64\n\treturn Prev.value(x) + 1\n')
			source = root / f'{name}.sirius'
			source.write_text(body, encoding='utf-8')
			if index == 0:
				checked(runner, '--emit-generic', name, root / f'{name}.smirg', source)
			else:
				checked(runner, '--emit-generic', name, root / f'{name}.smirg', source, root)
			source.unlink()
		for index in range(30):
			name = f'N{index:02d}'
			if index == 0:
				content = 'import P11 alias Top\nexport def value() -> i64\n\treturn Top.value(10)\n'
			else:
				content = f'import N{index - 1:02d} alias Prev\nimport P11 alias Top\nexport def value() -> i64\n\treturn Prev.value() + 1\n'
			(root / f'{name}.sirius').write_text(content, encoding='utf-8')
		(root / 'App.sirius').write_text('import N29 alias N\nimport P11 alias P\ndef main() -> i64\n\treturn N.value() + P.value(0)\n', encoding='utf-8')
		output = root / 'result.smirl'
		checked(runner, '--build-modules', root, 'App', output)
		first = output.read_bytes()
		checked(runner, '--build-modules', root, 'App', output)
		assert output.read_bytes() == first, 'linked MIR must be deterministic'
		result = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', output)
		assert result.returncode == 61, (result.stdout, result.stderr)
		print('PASS incremental discovery: deep source/package graph, shared dependencies, deterministic result')
		(root / 'CycleA.sirius').write_text('import CycleB alias B\nexport def value() -> i64\n\treturn B.value()\n', encoding='utf-8')
		(root / 'CycleB.sirius').write_text('import CycleA alias A\nexport def value() -> i64\n\treturn A.value()\n', encoding='utf-8')
		cycle = run(runner, '--build-modules', root, 'CycleA', output)
		assert cycle.returncode != 0 and b'cyclic' in cycle.stderr, cycle.stderr
		print('PASS incremental discovery: source cycles rejected')
		(root / 'CycleSource.sirius').write_text('export def value() -> i64\n\treturn 1\n', encoding='utf-8')
		provider = root / 'CyclePackage.sirius'
		provider.write_text('import CycleSource alias S\nexport def apply(x) -> i64\n\treturn S.value() + x\n', encoding='utf-8')
		checked(runner, '--emit-generic', 'CyclePackage', root / 'CyclePackage.smirg', provider, root)
		provider.unlink()
		(root / 'CycleSource.sirius').write_text('import CyclePackage alias P\nexport def value() -> i64\n\treturn P.apply(0)\n', encoding='utf-8')
		cycle = run(runner, '--build-modules', root, 'CyclePackage', output)
		assert cycle.returncode != 0 and b'cyclic' in cycle.stderr, cycle.stderr
		print('PASS incremental discovery: mixed package/source cycles rejected')


if __name__ == '__main__':
	main()
