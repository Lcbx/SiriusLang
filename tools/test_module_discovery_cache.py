#!/usr/bin/env python3
"""Discovery retries reuse typed source interfaces without caching across builds."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def checked(runner: Path, *args: object) -> None:
	result = subprocess.run([str(runner), *(str(value) for value in args)], capture_output=True)
	assert result.returncode == 0, (result.stdout, result.stderr)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).native_tool('run_cli')
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		config = root / 'Config.sirius'
		config.write_text('export let BASE = 21\n', encoding='utf-8')
		(root / 'Helper.sirius').write_text('import Config alias C\nexport def value() -> i64\n\treturn C.BASE\n', encoding='utf-8')
		for index in range(5):
			name = f'Generic{index}'
			source = root / f'{name}.sirius'
			source.write_text('export def twice(x) -> i64\n\treturn x + x\n', encoding='utf-8')
			checked(runner, '--emit-generic', name, root / f'{name}.smirg', source)
			source.unlink()
		imports = ''.join(f'import Generic{index} alias G{index}\n' for index in range(5))
		(root / 'App.sirius').write_text('import Helper alias H\n' + imports +
			'def main() -> i64\n\treturn G0.twice(H.value())\n', encoding='utf-8')
		output = root / 'result.smirl'
		for expected in (42, 40):
			checked(runner, '--build-modules', root, 'App', output)
			result = subprocess.run([str(runner), '--entry', 'M_3_App_4_main', '--load-lib', str(output)], capture_output=True)
			assert result.returncode == expected, result.stderr
			config.write_text('export let BASE = 20\n', encoding='utf-8')
		print('PASS module cache: repeated package discovery and fresh interfaces across builds')
		config.unlink()
		result = subprocess.run([str(runner), '--build-modules', str(root), 'App', str(output)], capture_output=True)
		assert result.returncode != 0 and b'not found' in result.stderr, result.stderr
		print('PASS module cache: deleted sources are not accepted from a previous build')


if __name__ == '__main__':
	main()
