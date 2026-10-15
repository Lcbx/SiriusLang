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

from source_manifest import ROOT, combined_sources, normalize_newlines

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
			for path in sorted((ROOT / 'runtime').glob('*.h')))

	def runtime(self) -> Path:
		source = ROOT / 'runtime/sirius_runtime.c'
		inputs = (source.read_bytes(), *self._headers())
		def build(output: Path, _: Path) -> None:
			execute([self.cc, *C_FLAGS, '-I', str(ROOT / 'runtime'), '-c', str(source), '-o', str(output)])
		return self._artifact('runtime', inputs, 'sirius_runtime.o', build)

	def _binary(self, category: str, source: Path, inputs: tuple[bytes, ...]) -> Path:
		runtime = self.runtime()
		filename = 'program.exe' if sys.platform == 'win32' else 'program'
		def build(output: Path, _: Path) -> None:
			compile_c(self.cc, source, output, runtime)
		return self._artifact(category, (source.read_bytes(), runtime.parent.name.encode(), *inputs), filename, build)

	def seed(self) -> Path:
		return self._binary('seed', ROOT / 'bootstrap/compiler.c', ())

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
			compile_c(self.cc, generated, output, runtime)
		return self._artifact(category, (data, seed.parent.name.encode(), runtime.parent.name.encode()), filename, build)


def compile_c(cc: str, source: Path, output: Path, runtime: Path) -> None:
	"""Link a C source file against the Sirius runtime with strict C11 flags."""
	# The MIR C emitter declares generic static-inline helpers, some unused by
	# a particular specialization. Clang warns about these under -Wall.
	extra = ('-Wno-unused-function',) if 'clang' in Path(cc).name else ()
	execute([cc, *C_FLAGS, *extra, *LINK_FLAGS, '-I', str(ROOT / 'runtime'),
		str(source), str(runtime), '-lm', '-o', str(output)])


def compile_native(cache: BuildCache, source: Path, output: Path | None = None) -> Path:
	"""Compile generated/reference C with the shared strict flags and runtime."""
	if output is None:
		output = source.with_suffix('.exe' if sys.platform == 'win32' else '')
	compile_c(cache.cc, source, output, cache.runtime())
	return output


def run_native(cache: BuildCache, source: Path, *, output: Path | None = None,
	args: tuple[str, ...] = (), cwd: Path | None = None) -> subprocess.CompletedProcess[bytes]:
	"""Compile and run C, preserving process exit codes and raw stderr."""
	binary = compile_native(cache, source, output)
	return subprocess.run([str(binary), *args], cwd=cwd, capture_output=True)


def assert_native(result: subprocess.CompletedProcess[bytes], *, stdout: bytes = b'',
	code: int | None = 0, stderr_contains: bytes | None = None) -> None:
	"""Check native output and status. code=None requires a nonzero exit."""
	actual = normalize_newlines(result.stdout)
	failed_status = result.returncode == 0 if code is None else result.returncode != code
	if actual != normalize_newlines(stdout) or failed_status or (
		stderr_contains is not None and stderr_contains not in normalize_newlines(result.stderr)
	):
		raise AssertionError(f'Unexpected native execution: {result!r}; expected stdout={stdout!r}, '
			f'exit={"nonzero" if code is None else code}, stderr_contains={stderr_contains!r}')


def check_native(cache: BuildCache, source: Path, *, stdout: bytes = b'',
	code: int | None = 0, stderr_contains: bytes | None = None,
	output: Path | None = None, args: tuple[str, ...] = (), cwd: Path | None = None) -> None:
	"""Compile and assert a native executable in one step."""
	assert_native(run_native(cache, source, output=output, args=args, cwd=cwd),
		stdout=stdout, code=code, stderr_contains=stderr_contains)


def check_sirius_driver(cache: BuildCache, category: str, sources: tuple[str, ...],
	expected: str) -> str:
	"""Compile a Sirius test driver, execute it, and check its text output."""
	result = execute([str(cache.sirius(category, sources))]).stdout
	if result != expected:
		raise AssertionError(f'Unexpected {category} output: {result!r}; expected {expected!r}')
	return result
