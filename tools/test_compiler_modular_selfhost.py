#!/usr/bin/env python3
"""Verify independent modular self-hosting without flat concatenation or runner."""
import argparse
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from bootstrap_support import check_committed
from build_support import BuildCache, assert_native, compile_c, execute, require_cc, run_native
from project_paths import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	args = parser.parse_args()
	cache = BuildCache(shutil.which(args.cc) or args.cc)
	seed = cache.seed()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		generated = root / 'generation_1.c'
		execute([str(seed), '--module-root', str(ROOT / 'src'), 'modular_cli', str(generated)])
		check_committed(ROOT / 'bootstrap/compiler.c', generated.read_bytes())
		compiler = root / ('generation_1.exe' if sys.platform == 'win32' else 'generation_1')
		compile_c(cache.cc, generated, compiler, cache.runtime())
		second_c = root / 'generation_2.c'
		execute([str(compiler), '--module-root', str(ROOT / 'src'), 'modular_cli', str(second_c)])
		assert second_c.read_bytes() == generated.read_bytes(), 'native modular C fixed point differs'
		print('PASS compiler CLI native module-root self-hosting (generations 1 and 2)')

		for name in ('01_forward_calls', '25_array_records', '96_resolved_assignment_scopes'):
			fixture = ROOT / 'tests/compiler/fixtures' / f'{name}.sirius'
			generated_fixture = root / f'{name}.c'
			execute([str(compiler), str(fixture), str(generated_fixture)])
			result = run_native(cache, generated_fixture)
			expected = (ROOT / 'tests/compiler/expected_output' / f'{name}.txt').read_bytes()
			assert_native(result, stdout=expected)
		for name in ('01_immutable', '03_missing_name'):
			fixture = ROOT / 'tests/compiler/negative' / f'{name}.sirius'
			generated_fixture = root / f'{name}.c'
			result = subprocess.run([str(compiler), str(fixture), str(generated_fixture)], capture_output=True)
			assert result.returncode == 1 and b'ERROR' in result.stdout and not generated_fixture.exists(), (name, result)
		print('PASS native modular compiler smoke fixtures (3 positive, 2 negative)')

		modules = root / 'example'
		modules.mkdir()
		(modules / 'Math.sirius').write_text('export def twice(value: i64) -> i64\n\treturn value * 2\n', encoding='utf-8')
		(modules / 'Main.sirius').write_text('import Math alias M\ndef main() -> i64\n\tprint(M.twice(21))\n\treturn 0\n', encoding='utf-8')
		modular_c = root / 'modular.c'
		execute([str(compiler), '--module-root', str(modules), 'Main', str(modular_c)])
		assert_native(run_native(cache, modular_c), stdout=b'42\n')
		data = modular_c.read_bytes()
		execute([str(compiler), '--module-root', str(modules), 'Main', str(modular_c)])
		assert modular_c.read_bytes() == data, 'direct module compilation is not deterministic'
		(modules / 'Bad.sirius').write_text('import Missing alias M\ndef main() -> i64\n\treturn 0\n', encoding='utf-8')
		missing_c = root / 'missing.c'
		failed = subprocess.run([str(compiler), '--module-root', str(modules), 'Bad', str(missing_c)], capture_output=True)
		assert failed.returncode == 1 and b'module error' in failed.stderr and not missing_c.exists(), failed
		(modules / 'Bad.sirius').write_text('def main() -> string\n\treturn "wrong"\n', encoding='utf-8')
		failed = subprocess.run([str(compiler), '--module-root', str(modules), 'Bad', str(missing_c)], capture_output=True)
		assert failed.returncode == 1 and b'main() must return i64' in failed.stderr and not missing_c.exists(), failed
		print('PASS direct module-root CLI: deterministic native C and errors')


if __name__ == '__main__':
	main()
