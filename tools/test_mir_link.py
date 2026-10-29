#!/usr/bin/env python
"""Cross-artifact concrete MIR linking: type interning, call relocation and rejection."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from project_paths import normalize_newlines


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	result = subprocess.run([str(runner), *args], capture_output=True, timeout=30)
	return subprocess.CompletedProcess(result.args, result.returncode,
		normalize_newlines(result.stdout), normalize_newlines(result.stderr))


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='prebuilt sirius-run binary')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).mir_runner()
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		first = work / 'first.sirius'
		first.write_text('def nested(n: i64) -> i64\n\tif n == 0\n\t\treturn 0\n\treturn n + nested(n - 1)\n'
			'def alpha() -> i64\n\tlet r = {z = [3, 4], a = {name = "A"}}\n\tprint(r.a.name)\n\treturn nested(r.z[!0])\n'
			'def alpha_copy() -> i64\n\treturn alpha() + 1\n')
		second = work / 'second.sirius'
		second.write_text('def value() -> i64\n\tlet r = {a = {name = "B"}, z = [5, 6]}\n\tprint(r.a.name)\n\treturn r.z[!1]\n'
			'def beta() -> i64\n\treturn value() + 2\n')
		third = work / 'third.sirius'
		third.write_text('def gamma() -> i64\n\tlet d = Dict[string, i64]().insert("x", 11)\n\treturn d.get("x") or 0\n')
		libraries = [work / f'{name}.smirl' for name in ('a', 'b', 'c')]
		for source, library, exports in ((first, libraries[0], ('alpha', 'alpha_copy')),
			(second, libraries[1], ('beta',)), (third, libraries[2], ('gamma',))):
			result = run(runner, '--emit-lib', str(library), *(arg for name in exports for arg in ('--export', name)), str(source))
			assert result.returncode == 0, result
		linked = work / 'linked.smirl'
		result = run(runner, '--link-lib', str(linked), *(str(path) for path in libraries))
		assert result.returncode == 0, result
		assert linked.read_bytes().startswith(b'SIRIUS-MIR-LIB/2\n')
		assert run(runner, '--list-lib', str(linked)).stdout == (
			b'alpha : ()->i64\nalpha_copy : ()->i64\nbeta : ()->i64\ngamma : ()->i64\n')
		for entry, output, code in (('alpha', b'A\n', 6), ('alpha_copy', b'A\n', 7),
			('beta', b'B\n', 8), ('gamma', b'', 11)):
			combined = run(runner, '--entry', entry, '--load-lib', str(linked))
			original = next(lib for lib, exported in zip(libraries, (('alpha', 'alpha_copy'), ('beta',), ('gamma',))) if entry in exported)
			unlinked = run(runner, '--entry', entry, '--load-lib', str(original))
			assert (combined.stdout, combined.stderr, combined.returncode) == (output, b'', code), (entry, combined)
			assert (combined.stdout, combined.stderr, combined.returncode) == (
				unlinked.stdout, unlinked.stderr, unlinked.returncode), entry
		print('PASS MIR linker: independent type tables, internal call relocation, nested types, dictionary values')

		# A linked artifact remains a fully valid input to the library linker.
		another = work / 'another.smirl'
		result = run(runner, '--link-lib', str(another), str(libraries[2]), str(libraries[1]), str(libraries[0]))
		assert result.returncode == 0, result
		assert run(runner, '--list-lib', str(another)).stdout == (
			b'gamma : ()->i64\nbeta : ()->i64\nalpha : ()->i64\nalpha_copy : ()->i64\n')
		for entry in ('alpha', 'alpha_copy', 'beta', 'gamma'):
			assert run(runner, '--entry', entry, '--load-lib', str(linked)).stdout == (
				run(runner, '--entry', entry, '--load-lib', str(another)).stdout)
		# Stable link output for identical ordered input artifacts.
		assert run(runner, '--link-lib', str(another), *(str(path) for path in libraries)).returncode == 0
		assert linked.read_bytes() == another.read_bytes()
		print('PASS MIR linker: link-order independence of behavior and deterministic output')

		result = run(runner, '--link-lib', str(another), str(libraries[0]), str(libraries[0]))
		assert result.returncode == 2 and b'duplicate MIR export' in result.stderr, result
		result = run(runner, '--link-lib', str(another), str(libraries[0]))
		assert result.returncode == 0, result
		assert run(runner, '--entry', 'alpha', '--load-lib', str(another)).returncode == 6
		corrupt = work / 'corrupt.smirl'
		corrupt.write_bytes(libraries[1].read_bytes()[:-5])
		result = run(runner, '--link-lib', str(another), str(libraries[0]), str(corrupt))
		assert result.returncode == 2 and b'invalid MIR library:' in result.stderr, result
		print('PASS MIR linker: duplicate exports, invalid input and missing input rejected')


if __name__ == '__main__':
	main()
