#!/usr/bin/env python
"""Compile and test the Sirius emitter using the committed C bootstrap."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from emitter_cases import check_cases
from sirius_sources import ROOT, COMPILER_SOURCES, combined_sources


def execute(command: list[str]) -> None:
	result = subprocess.run(command, text=True, encoding='utf-8', capture_output=True)
	if result.returncode:
		raise RuntimeError(f'{" ".join(command)}\n{result.stdout}{result.stderr}')


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('C11 compiler required (cc or gcc)')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		combined = work / 'stage1.sirius'
		combined.write_bytes(combined_sources(COMPILER_SOURCES))
		generated = work / 'bootstrap.c'
		seed = work / ('seed.exe' if sys.platform == 'win32' else 'seed')
		execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(ROOT / 'bootstrap_c/compiler.c'), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(seed)])
		execute([str(seed), str(combined), str(generated)])
		compiler = work / ('stage1.exe' if sys.platform == 'win32' else 'stage1')
		execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
			str(generated), str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(compiler)])
		check_cases(cc, compiler, work)


if __name__ == '__main__':
	main()
