#!/usr/bin/env python3
"""Compile and run the standalone Sirius examples with their golden output."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, compile_c, require_cc
from project_paths import ROOT, executable_path, normalize_newlines


def main() -> None:
	examples = ROOT / 'examples'
	sources = sorted(path for path in examples.glob('*.sirius') if path.name != 'filesystem_walk.sirius')
	expected = examples / 'expected_output'
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='built sirius-run executable for MIR/native parity')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else None
	if runner and not runner.is_file():
		parser.error(f'not a file: {runner}')
	assert sources, 'No standalone examples found'
	assert {path.stem for path in sources} == {path.stem for path in expected.glob('*.txt')}, 'Example output fixtures do not match example sources'
	cache = BuildCache(shutil.which('clang') or require_cc())
	compiler = cache.seed()
	runtime = cache.runtime()
	with tempfile.TemporaryDirectory(prefix='sirius-examples-') as tmp:
		work = Path(tmp)
		for source in sources:
			generated = work / (source.stem + '.c')
			binary = executable_path(work / source.stem)
			compile_result = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, timeout=120)
			assert compile_result.returncode == 0, (source.name, compile_result.stdout, compile_result.stderr)
			compile_c(cache.cc, generated, binary, runtime)
			result = subprocess.run([str(binary)], capture_output=True, timeout=120)
			wanted = normalize_newlines((expected / (source.stem + '.txt')).read_bytes())
			assert result.returncode == 0 and normalize_newlines(result.stdout) == wanted and not result.stderr, (
				source.name, result.returncode, result.stdout, result.stderr, wanted)
			if runner:
				interpreted = subprocess.run([str(runner), str(source)], capture_output=True, timeout=120)
				assert (interpreted.returncode, normalize_newlines(interpreted.stdout),
					normalize_newlines(interpreted.stderr)) == (result.returncode, wanted, b''), (
						source.name, interpreted.returncode, interpreted.stdout, interpreted.stderr, wanted)
			print(f'PASS {source.name}' + (' (native + MIR)' if runner else ''), flush=True)
	print(f'PASS {len(sources)} standalone examples')


if __name__ == '__main__':
	main()
