#!/usr/bin/env python3
"""Parametric HIR specialization checks for current source-free generic packages."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from project_paths import executable_path
from build_support import BuildCache, compile_c, require_cc


def run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *map(str, args)], capture_output=True, timeout=45)


def checked(binary: Path, *args: object) -> bytes:
	result = run(binary, *args)
	assert result.returncode == 0, (args, result)
	return result.stdout


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		provider = work / 'Checks.sirius'
		provider.write_text('let OFFSET = 3\n'
			'export def shifted(x) -> i64\n\treturn x + OFFSET\n'
			'export def routed(x) -> i64\n\tif x < 0\n\t\treturn -x\n\treturn x + 1\n'
			'export def same(x, y) -> bool\n\treturn x == y\n', encoding='utf-8')
		package = work / 'Checks.smirg'
		checked(runner, '--emit-generic', 'Checks', package, provider)
		artifact = package.read_bytes()
		provider.unlink()
		concrete = work / 'concrete.smirl'
		consumer = work / 'consumer.smirl'
		linked = work / 'linked.smirl'
		caller = work / 'caller.sirius'
		for generic, kind, annotation, value, expected in (
			('shifted', 'i64', 'i64', '12', 15),
			('routed', 'i64', 'i64', '-6', 6),
			('same', 'i64', 'i64', '42, 42', 7),
			('same', 'string', 'string', '"text", "text"', 7),
		):
			checked(runner, '--specialize-generic', package, generic, concrete, *([kind, kind] if generic == 'same' else [kind]))
			symbol = checked(runner, '--list-lib', concrete).decode().split(' : ', 1)[0]
			return_type = 'bool' if generic == 'same' else 'i64'
			result = f'if {symbol}({value})\n\t\treturn 7\n\treturn 1' if generic == 'same' else f'return {symbol}({value})'
			caller.write_text(f'extern def {symbol}(x: {annotation}' + (f', y: {annotation}' if generic == 'same' else '') + f') -> {return_type}\n'
				f'def main() -> i64\n\t{result}\n', encoding='utf-8')
			checked(runner, '--emit-lib', consumer, '--export', 'main', caller)
			checked(runner, '--link-lib', linked, consumer, concrete)
			assert run(runner, '--entry', 'main', '--load-lib', linked).returncode == expected
			generated = work / 'output.c'
			checked(runner, '--emit-linked-c', linked, 'main', generated)
			binary = executable_path(work / 'native')
			compile_c(cache.cc, generated, binary, cache.runtime())
			assert run(binary).returncode == expected
		print('PASS generic semantics: constants, branches, equality, i64/string and native parity')

		for generic, kind in (('routed', 'bool'), ('routed', 'string'), ('shifted', 'bool')):
			result = run(runner, '--specialize-generic', package, generic, concrete, kind)
			assert result.returncode != 0, (generic, kind, result)
		# Generic package snapshots must not be silently reinterpreted after tampering.
		bad = work / 'corrupt.smirg'
		for original, replacement in ((b'SIRIUS-TEMPLATE-DECL/1', b'SIRIUS-TEMPLATE-DECL/0'),
			(b'SIRIUS-TEMPLATE-AST/1', b'SIRIUS-TEMPLATE-AST/0')):
			assert original in artifact
			bad.write_bytes(artifact.replace(original, replacement, 1))
			assert run(runner, '--specialize-generic', bad, 'shifted', concrete, 'i64').returncode != 0
		print('PASS generic semantics: invalid operator types and tampered syntax/declarations rejected')


if __name__ == '__main__':
	main()
