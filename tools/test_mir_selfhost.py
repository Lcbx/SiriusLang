#!/usr/bin/env python
"""Verify the standalone MIR compiler reaches a native C self-hosting fixed point."""
import argparse
from contextlib import nullcontext
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
from time import perf_counter

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT


SMOKE = {
	'augmented_local': (
		'def main() -> i64\n\tvar i = 0\n\tvar total = 0\n\twhile i < 7\n\t\ti += 1\n\t\ttotal += i\n\treturn total\n', 28),
	'augmented_record': (
		'rec Counter(value: i64)\n\tpub value\ndef main() -> i64\n\tvar c = Counter(1)\n\tc.value += 2\n\tc.value *= 3\n\treturn c.value\n', 9),
	'augmented_string': (
		'def main() -> i64\n\tvar text = "a"\n\ttext += "bc"\n\treturn len(text)\n', 3),
	'array_cow': (
		'def main() -> i64\n\tvar a = [1, 2]\n\tlet before = a\n\ta = a.append(3)\n\treturn len(before) * 10 + len(a)\n', 23),
	'record_array_append': (
		'rec Store(values: Array[i64])\n\tpub values\ndef main() -> i64\n\tvar s = Store([1])\n\tlet old = s\n\ts.values = s.values.append(2)\n\treturn len(old.values) * 10 + len(s.values)\n', 12),
	'join': (
		'def main() -> i64\n\treturn len("/".join(["a", "bc", "d"]))\n', 6),
}


def run(command: list[str], timeout: float) -> subprocess.CompletedProcess[bytes]:
	start = perf_counter()
	result = subprocess.run(command, capture_output=True, timeout=timeout)
	if result.returncode:
		raise AssertionError(f'Exit {result.returncode}: {command}\n{result.stdout[-3000:]!r}\n{result.stderr[-3000:]!r}')
	print(f'PASS {Path(command[0]).name}: {perf_counter() - start:.2f}s', flush=True)
	return result


def native_build(cc: str, runtime: Path, source: Path, target: Path, timeout: float) -> None:
	# The generated runtime has some specialized helpers that a given program
	# does not call; Clang diagnoses unused static inline helpers under -Werror.
	warnings = ['-Wno-unused-function'] if 'clang' in Path(cc).name.lower() else []
	run([cc, *C_FLAGS, *warnings, '-I', str(ROOT / 'runtime'),
		str(source), str(runtime), *link_flags(cc), '-o', str(target)], timeout)


def verify(work: Path, native_cc: str, timeout: float, rebuild: bool) -> None:
	cache = BuildCache(require_cc(), rebuild=rebuild)
	stage1 = cache.native_tool('modular_cli')
	runtime = cache.runtime()
	previous = None
	for generation in (2, 3, 4):
		c_source = work / f'generation_{generation}.c'
		run([str(stage1), '--module-root', str(ROOT / 'src'), 'modular_cli', str(c_source)], timeout)
		print(f'Generation {generation}: {c_source.stat().st_size:,} C bytes', flush=True)
		if previous is not None and c_source.read_bytes() != previous:
			raise AssertionError(f'MIR C differs between generations {generation - 1} and {generation}')
		previous = c_source.read_bytes()
		if generation == 4:
			break
		stage1 = work / (f'generation_{generation}.exe' if sys.platform == 'win32' else f'generation_{generation}')
		native_build(native_cc, runtime, c_source, stage1, timeout)
		if generation == 2:
			for name, (program, code) in SMOKE.items():
				fixture = work / f'{name}.sirius'
				generated = work / f'{name}.c'
				binary = work / (f'{name}.exe' if sys.platform == 'win32' else name)
				fixture.write_text(program, encoding='utf-8')
				run([str(stage1), str(fixture), str(generated)], timeout)
				native_build(native_cc, runtime, generated, binary, timeout)
				result = subprocess.run([str(binary)], capture_output=True, timeout=timeout)
				if result.returncode != code or result.stdout != b'':
					raise AssertionError(f'{name}: expected exit {code}, got {result.returncode} with {result.stdout!r} {result.stderr!r}')
				print(f'PASS MIR native fixture {name}', flush=True)
	print('PASS MIR-only native compiler reaches identical C at stages 2, 3 and 4')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-cc', default=shutil.which('clang') or require_cc(),
		help='C compiler for MIR-generated stages (defaults to clang when available)')
	parser.add_argument('--timeout', type=float, default=180,
		help='timeout in seconds for each native compilation and generation')
	parser.add_argument('--workdir', type=Path, help='keep intermediates in this directory')
	parser.add_argument('--rebuild', action='store_true', help='rebuild cached stage-1 compiler')
	args = parser.parse_args()
	if args.timeout <= 0:
		parser.error('--timeout must be positive')
	if args.workdir:
		args.workdir.mkdir(parents=True, exist_ok=True)
	with nullcontext(args.workdir) if args.workdir else tempfile.TemporaryDirectory() as directory:
		verify(Path(directory), args.native_cc, args.timeout, args.rebuild)


if __name__ == '__main__':
	main()
