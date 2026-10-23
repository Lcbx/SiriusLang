#!/usr/bin/env python3
"""Run parser fixtures through separate tokenizer and parser Sirius modules."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES, ROOT, normalize_newlines
from build_modules import symbol


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=30)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='reuse a previously built MIR runner')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	cases = sorted((ROOT / 'tests/parser/fixtures').glob('*.sirius'))
	assert cases, 'missing parser fixtures'
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		# Build the actual source files independently through the native import graph.
		shutil.copy2(ROOT / 'src/tokenizer.sirius', root / 'tokenizer.sirius')
		shutil.copy2(ROOT / 'src/parser.sirius', root / 'parser.sirius')
		shutil.copy2(ROOT / 'tests/parser/module_driver.sirius', root / 'Main.sirius')
		library = root / 'app.smirl'
		built = run(runner, '--build-modules', root, 'Main', library)
		assert built.returncode == 0, built.stderr
		first = library.read_bytes()
		listed = run(runner, '--list-lib', library)
		assert listed.returncode == 0, listed.stderr
		for name in (symbol('tokenizer', 'tokenize'), symbol('parser', 'parse_module')):
			assert name.encode() in listed.stdout, (name, listed.stdout)
		assert b'M_8_Frontend_' not in listed.stdout, listed.stdout
		for i, case in enumerate(cases):
			fixture = root / f'fixture_{i}.sirius'
			fixture.write_bytes(normalize_newlines(case.read_bytes()))
			result = run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', library, fixture)
			expected = (ROOT / 'tests/parser/expected_output' / f'{case.stem}.txt').read_bytes()
			assert result.returncode == 0 and normalize_newlines(result.stdout) == normalize_newlines(expected), (case.name, result)
		print(f'PASS separate tokenizer -> parser native imports: {len(cases)} golden cases')
		built = run(runner, '--build-modules', root, 'Main', library)
		assert built.returncode == 0 and library.read_bytes() == first, 'non-deterministic frontend linkage'
		(root / 'Private.sirius').write_text('import tokenizer alias T\ndef main() -> i64\n\treturn T.scan_number("1", 0)\n', encoding='utf-8')
		bad = run(runner, '--build-modules', root, 'Private', library)
		assert bad.returncode != 0 and b'not exported' in bad.stderr, bad.stderr
		print('PASS native frontend: deterministic linking, private helper rejected')


if __name__ == '__main__':
	main()
