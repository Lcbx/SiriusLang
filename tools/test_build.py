#!/usr/bin/env python
"""Shared, content-addressed C build cache for Sirius test runners.

The compiler seed, runtime object and compiled Sirius test drivers are stored in
.build/cache. Independent test fixtures still use isolated temporary directories.
"""
import argparse
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
from typing import Callable

from sirius_sources import ROOT, combined_sources

BUILD = ROOT / '.build' / 'cache'
C_FLAGS = ('-std=c11', '-O0', '-Wall', '-Wextra', '-Werror', '-pedantic')
# Windows PE executables default to a small stack; self-hosting needs more.
LINK_FLAGS = ('-Wl,--stack,16777216',) if sys.platform == 'win32' else ()


def options(parser: argparse.ArgumentParser) -> None:
	parser.add_argument('--jobs', type=int, default=min(4, os.cpu_count() or 1),
		help='parallel test cases (default: min(4, CPU count); 1 for debugging)')
	parser.add_argument('--rebuild', action='store_true',
		help='clear cached binaries/objects before building')


def validate_jobs(jobs: int) -> int:
	if jobs < 1:
		raise SystemExit('--jobs must be >= 1')
	return jobs


def require_cc() -> str:
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('A C11 compiler (cc or gcc) is required')
	return cc


def execute(command: list[str]) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(command, capture_output=True, text=True, encoding='utf-8', errors='replace')
	if result.returncode:
		status = f'{result.returncode} (0x{result.returncode & 0xffffffff:08X})'
		raise RuntimeError(f'Exit code {status}: {" ".join(command)}\n{result.stdout}{result.stderr}')
	return result


class BuildCache:
	def __init__(self, cc: str, rebuild: bool = False):
		self.cc = cc
		if rebuild:
			shutil.rmtree(BUILD, ignore_errors=True)
		identity = execute([cc, '--version']).stdout.splitlines()[0]
		stat = Path(cc).stat()
		self.toolchain = f'{Path(cc).resolve()}|{stat.st_size}|{stat.st_mtime_ns}|{identity}|{sys.platform}'

	def _artifact(self, category: str, inputs: tuple[bytes, ...], filename: str,
		build: Callable[[Path, Path], None]) -> Path:
		hash_value = hashlib.sha256()
		for value in (self.toolchain.encode(), category.encode(), repr((C_FLAGS, LINK_FLAGS)).encode(), *inputs):
			hash_value.update(len(value).to_bytes(8, 'big'))
			hash_value.update(value)
		folder = BUILD / category / hash_value.hexdigest()
		artifact = folder / filename
		if artifact.is_file():
			return artifact
		folder.parent.mkdir(parents=True, exist_ok=True)
		with tempfile.TemporaryDirectory(prefix='building-', dir=folder.parent) as tmp:
			staging = Path(tmp) / 'result'
			staging.mkdir()
			build(staging / filename, staging)
			if not (staging / filename).is_file():
				raise RuntimeError(f'Build produced no artifact: {filename}')
			try:
				staging.rename(folder)
			except OSError:
				# Another test runner may have published the same artifact first.
				if not artifact.is_file():
					raise
		return artifact

	def _headers(self) -> tuple[bytes, ...]:
		return tuple(path.name.encode() + b'\0' + path.read_bytes()
			for path in sorted((ROOT / 'include').glob('*.h')))

	def runtime(self) -> Path:
		source = ROOT / 'include/sirius_runtime.c'
		inputs = (source.read_bytes(), *self._headers())
		def build(output: Path, _: Path) -> None:
			execute([self.cc, *C_FLAGS, '-I', str(ROOT / 'include'), '-c', str(source), '-o', str(output)])
		return self._artifact('runtime', inputs, 'sirius_runtime.o', build)

	def _binary(self, category: str, source: Path, inputs: tuple[bytes, ...]) -> Path:
		runtime = self.runtime()
		filename = 'program.exe' if sys.platform == 'win32' else 'program'
		def build(output: Path, _: Path) -> None:
			execute([self.cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(source), str(runtime),
				'-lm', '-o', str(output)])
		return self._artifact(category, (source.read_bytes(), runtime.parent.name.encode(), *inputs), filename, build)

	def seed(self) -> Path:
		return self._binary('seed', ROOT / 'bootstrap_c/compiler.c', ())

	def sirius(self, category: str, sources: tuple[str, ...], separator: bytes = b'\n') -> Path:
		data = combined_sources(sources, separator=separator)
		seed = self.seed()
		runtime = self.runtime()
		filename = 'program.exe' if sys.platform == 'win32' else 'program'
		def build(output: Path, folder: Path) -> None:
			combined = folder / 'source.sirius'
			combined.write_bytes(data)
			generated = folder / 'generated.c'
			execute([str(seed), str(combined), str(generated)])
			execute([self.cc, *C_FLAGS, *LINK_FLAGS, '-I', str(ROOT / 'include'), str(generated), str(runtime),
				'-lm', '-o', str(output)])
		return self._artifact(category, (data, seed.parent.name.encode(), runtime.parent.name.encode()), filename, build)
