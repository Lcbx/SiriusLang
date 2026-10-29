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

from project_paths import ROOT, executable_path, normalize_newlines

BUILD = ROOT / '.build' / 'cache'
C_FLAGS = ('-std=c11', '-O0', '-Wall', '-Wextra', '-Werror', '-pedantic')
MIR_RUNNER_ENV = 'SIRIUS_TEST_MIR_RUNNER'
MIR_RUNNER_TOOLCHAIN_ENV = 'SIRIUS_TEST_MIR_RUNNER_TOOLCHAIN'
# Windows PE executables default to a small stack; self-hosting needs more.
# MSVC link.exe and MinGW GNU ld use different flags. Include the math library
# only where it is separate from the C runtime (not with the MSVC/UCRT target).
def link_flags(cc: str) -> tuple[str, ...]:
	if sys.platform != 'win32':
		return ('-lm',)
	target = execute([cc, '-dumpmachine']).stdout.strip().lower()
	if 'msvc' in target:
		return ('-Xlinker', '/STACK:16777216')
	if 'mingw' in target or 'windows-gnu' in target:
		return ('-Wl,--stack,16777216', '-lm')
	raise RuntimeError(f'Unsupported Windows C compiler target: {target!r}')


def options(parser: argparse.ArgumentParser) -> None:
	parser.add_argument('--jobs', type=int, default=min(4, os.cpu_count() or 1),
		help='parallel test cases (default: min(4, CPU count); 1 for debugging)')
	parser.add_argument('--rebuild', action='store_true',
		help='clear cached binaries/objects before building')


def validate_jobs(jobs: int) -> int:
	if jobs < 1:
		raise SystemExit('--jobs must be >= 1')
	return jobs


def copy_binary(source: Path, output: Path) -> Path:
	"""Publish a cached executable without copying it onto itself."""
	output.parent.mkdir(parents=True, exist_ok=True)
	if source.resolve() != output.resolve():
		shutil.copy2(source, output)
	return output


def require_cc() -> str:
	cc = shutil.which('clang') or shutil.which('cc') or shutil.which('gcc')
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
		self.rebuild = rebuild
		if rebuild:
			shutil.rmtree(BUILD, ignore_errors=True)
		identity = execute([cc, '--version']).stdout.splitlines()[0]
		stat = Path(cc).stat()
		self.toolchain = f'{Path(cc).resolve()}|{stat.st_size}|{stat.st_mtime_ns}|{identity}|{sys.platform}'

	def _artifact(self, category: str, inputs: tuple[bytes, ...], filename: str,
		build: Callable[[Path, Path], None]) -> Path:
		hash_value = hashlib.sha256()
		for value in (self.toolchain.encode(), category.encode(), repr((C_FLAGS, link_flags(self.cc))).encode(), *inputs):
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
		filename = executable_path(Path('program')).name
		def build(output: Path, _: Path) -> None:
			compile_c(self.cc, source, output, runtime)
		return self._artifact(category, (source.read_bytes(), runtime.parent.name.encode(), *inputs), filename, build)

	def seed(self) -> Path:
		return self._binary('seed', ROOT / 'bootstrap/compiler.c', ())

	def mir_runner(self) -> Path:
		"""Return the cached native MIR runner, or the pinned suite copy."""
		return self.native_tool('run_cli')

	def native_tool(self, entry: str) -> Path:
		"""Reuse the suite runner, or build a CLI with the checked-in C seed."""
		if entry == 'run_cli' and not self.rebuild:
			shared = os.environ.get(MIR_RUNNER_ENV)
			if shared and os.environ.get(MIR_RUNNER_TOOLCHAIN_ENV) == self.toolchain:
				binary = Path(shared).resolve()
				if not binary.is_file():
					raise RuntimeError(f'Shared MIR runner is missing: {binary}')
				return binary
		sources = tuple(path for path in sorted((ROOT / 'src').glob('*.sirius')))
		seed = self.seed()
		runtime = self.runtime()
		filename = executable_path(Path('program')).name
		inputs = (entry.encode(), seed.parent.name.encode(), runtime.parent.name.encode(),
			*(path.name.encode() + b'\0' + path.read_bytes() for path in sources))
		def build(output: Path, folder: Path) -> None:
			generated = folder / 'generated.c'
			execute([str(seed), '--module-root', str(ROOT / 'src'), entry, str(generated)])
			compile_c(self.cc, generated, output, runtime)
		return self._artifact('native-' + entry, inputs, filename, build)

	def native_driver(self, category: str, source: Path, *, extra_modules: tuple[Path, ...] = ()) -> Path:
		"""Build a test entry as Main using actual native compiler source modules."""
		modules = sorted((ROOT / 'src').glob('*.sirius'))
		seed = self.seed()
		runtime = self.runtime()
		filename = executable_path(Path('program')).name
		inputs = (source.read_bytes(), seed.parent.name.encode(), runtime.parent.name.encode(),
			*(path.name.encode() + b'\0' + path.read_bytes() for path in extra_modules),
			*(path.name.encode() + b'\0' + path.read_bytes() for path in modules))
		def build(output: Path, folder: Path) -> None:
			root = folder / 'modules'
			root.mkdir()
			for module in modules:
				shutil.copyfile(module, root / module.name)
			for module in extra_modules:
				shutil.copyfile(module, root / module.name)
			shutil.copyfile(source, root / 'Main.sirius')
			generated = folder / 'generated.c'
			execute([str(seed), '--module-root', str(root), 'Main', str(generated)])
			compile_c(self.cc, generated, output, runtime)
		return self._artifact('native-driver-' + category, inputs, filename, build)

def compile_c(cc: str, source: Path, output: Path, runtime: Path) -> None:
	"""Link a C source file against the Sirius runtime with strict C11 flags."""
	# The MIR C emitter declares generic static-inline helpers, some unused by
	# a particular specialization. Clang warns about these under -Wall.
	extra = ('-Wno-unused-function',) if 'clang' in Path(cc).name else ()
	execute([cc, *C_FLAGS, *extra, '-I', str(ROOT / 'runtime'),
		str(source), str(runtime), *link_flags(cc), '-o', str(output)])


def compile_native(cache: BuildCache, source: Path, output: Path | None = None) -> Path:
	"""Compile generated/reference C with the shared strict flags and runtime."""
	if output is None:
		output = executable_path(source.with_suffix(''))
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
