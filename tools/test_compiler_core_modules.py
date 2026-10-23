#!/usr/bin/env python3
"""Exercise compiler tokenizer and canonical types via native Sirius imports."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES, ROOT, normalize_newlines
from test_tokenizer import fixture_bytes


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=30)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='reuse a previously built MIR runner')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	cases = sorted((ROOT / 'tests/tokenizer/fixtures').glob('*.sirius'))
	assert cases, 'missing tokenizer fixtures'
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		shutil.copy2(ROOT / 'src/tokenizer.sirius', root / 'Lexer.sirius')
		shutil.copy2(ROOT / 'src/types.sirius', root / 'Types.sirius')
		shutil.copy2(ROOT / 'tests/tokenizer/module_driver.sirius', root / 'Main.sirius')
		library = root / 'app.smirl'
		built = run(runner, '--build-modules', root, 'Main', library)
		assert built.returncode == 0, built.stderr
		first = library.read_bytes()
		for i, case in enumerate(cases):
			fixture = root / f'fixture_{i}.sirius'
			fixture.write_bytes(fixture_bytes(case))
			result = run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', library, fixture)
			expected = (ROOT / 'tests/tokenizer/expected_output' / f'{case.stem}.txt').read_bytes()
			assert result.returncode == 0 and normalize_newlines(result.stdout) == normalize_newlines(expected), (case.name, result)
		print(f'PASS native compiler tokenizer and types: {len(cases)} golden cases')
		built = run(runner, '--build-modules', root, 'Main', library)
		assert built.returncode == 0 and library.read_bytes() == first, 'non-deterministic linked tokenizer'
		(root / 'Private.sirius').write_text('import Lexer alias Lex\ndef main() -> i64\n\treturn Lex.scan_number("1", 0)\n', encoding='utf-8')
		bad = run(runner, '--build-modules', root, 'Private', library)
		assert bad.returncode != 0 and b'not exported' in bad.stderr, bad.stderr
		print('PASS compiler modules: deterministic linking, helper visibility')


if __name__ == '__main__':
	main()
