#!/usr/bin/env python3
"""Compare the rewrite lexer with existing golden and generated inputs."""
import random
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, execute, require_cc
from project_paths import ROOT, normalize_newlines
from test_tokenizer import fixture_bytes


def run(command: list[str]) -> bytes:
	result = subprocess.run(command, capture_output=True, timeout=30)
	assert result.returncode == 0, (command, result.stderr)
	return normalize_newlines(result.stdout)


def main() -> None:
	cache = BuildCache(require_cc())
	old = cache.native_driver('restart-lexer-reference', ROOT / 'tests/tokenizer/module_driver.sirius')
	driver = ROOT / 'rewrite/lexer_test_driver.sirius'
	new = cache.native_driver('restart-lexer', driver, extra_modules=(ROOT / 'rewrite/lexer.sirius',))
	runner = cache.mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		(root / 'lexer.sirius').write_bytes((ROOT / 'rewrite/lexer.sirius').read_bytes())
		(root / 'Main.sirius').write_bytes(driver.read_bytes())
		library = root / 'lexer.smirl'
		execute([str(runner), '--build-modules', str(root), 'Main', str(library)])
		cases = [(p.name, fixture_bytes(p)) for p in sorted((ROOT / 'tests/tokenizer/fixtures').glob('*.sirius'))]
		cases.append(('numbers', b'0 1 0.5 .5 1. 1..2 1e2 1E-2 1e+ 1e .1e3 0123\n'))
		sources = sorted((ROOT / 'src').glob('*.sirius'))
		source_names = {p.name for p in sources}
		cases += [(p.name, p.read_bytes()) for p in sources]
		rng = random.Random(49)
		alphabet = b'0123456789.eE+-_ abc\n\t#()[]{}\'"\\\r'
		cases += [(f'random-{i}', bytes(rng.choice(alphabet) for _ in range(200))) for i in range(100)]
		for name, data in cases:
			fixture = root / 'input.sirius'
			fixture.write_bytes(data)
			expected = run([str(old), str(fixture)])
			golden = ROOT / 'tests/tokenizer/expected_output' / (Path(name).stem + '.txt')
			if golden.is_file():
				assert expected == normalize_newlines(golden.read_bytes()), name
			assert run([str(new), str(fixture)]) == expected, name
			if name not in source_names:
				assert run([str(runner), '--entry', 'M_4_Main_4_main', '--load-lib', str(library), str(fixture)]) == expected, name
		print(f'PASS rewrite lexer: {len(cases)} native cases; fixtures and generated cases on MIR')


if __name__ == '__main__':
	main()
