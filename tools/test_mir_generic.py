#!/usr/bin/env python3
"""Portable generic packages, concrete linking, C emission, and corruption checks."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from project_paths import executable_path
from build_support import BuildCache, compile_c, require_cc


def run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *map(str, args)], capture_output=True, timeout=45)


def checked(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(binary, *args)
	assert result.returncode == 0, (args, result)
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		provider = work / 'Ops.sirius'
		provider.write_text('export def twice(x) -> i64\n\treturn x + x\n'
			'export def size(xs) -> i64\n\treturn len(xs)\n', encoding='utf-8')
		package = work / 'Ops.smirg'
		checked(runner, '--emit-generic', 'Ops', package, provider)
		first = package.read_bytes()
		assert first.startswith(b'SIRIUS-MIR-GENERIC/3\n')
		checked(runner, '--emit-generic', 'Ops', package, provider)
		assert package.read_bytes() == first
		provider.unlink()

		concrete = work / 'concrete.smirl'
		checked(runner, '--specialize-generic', package, 'twice', concrete, 'i64')
		name = checked(runner, '--list-lib', concrete).stdout.decode().split(' : ', 1)[0]
		assert name.isidentifier(), name
		caller = work / 'caller.sirius'
		caller.write_text(f'extern def {name}(x: i64) -> i64\n'
			f'def main() -> i64\n\treturn {name}(21)\n', encoding='utf-8')
		consumer = work / 'consumer.smirl'
		checked(runner, '--emit-lib', consumer, '--export', 'main', caller)
		linked = work / 'linked.smirl'
		for order in ((consumer, concrete), (concrete, consumer)):
			checked(runner, '--link-lib', linked, *order)
			assert run(runner, '--entry', 'main', '--load-lib', linked).returncode == 42
		assert linked.read_bytes().startswith(b'SIRIUS-MIR-LIB/2\n')
		generated = work / 'linked.c'
		checked(runner, '--emit-linked-c', linked, 'main', generated)
		binary = executable_path(work / 'linked')
		compile_c(cache.cc, generated, binary, cache.runtime())
		assert run(binary).returncode == 42
		print('PASS generic: deterministic source-free package, both link orders, native/interpreted parity')

		for kind, value, expected in (('string', '"hello"', 5), ('Array[i64]', '[1, 2, 3]', 3)):
			checked(runner, '--specialize-generic', package, 'size', concrete, kind)
			name = checked(runner, '--list-lib', concrete).stdout.decode().split(' : ', 1)[0]
			caller.write_text(f'extern def {name}(x: {kind}) -> i64\n'
				f'def main() -> i64\n\treturn {name}({value})\n', encoding='utf-8')
			checked(runner, '--emit-lib', consumer, '--export', 'main', caller)
			checked(runner, '--link-lib', linked, consumer, concrete)
			assert run(runner, '--entry', 'main', '--load-lib', linked).returncode == expected
		print('PASS generic: concrete string and array specialization')

		for symbol, types in (('absent', ('i64',)), ('twice', ()), ('twice', ('bool',)),
			('size', ('Unknown',))):
			result = run(runner, '--specialize-generic', package, symbol, concrete, *types)
			assert result.returncode != 0, (symbol, types, result)
		for bad_data in (b'', first[:-1], first + b'garbage',
			first.replace(b'SIRIUS-MIR-GENERIC/3', b'SIRIUS-MIR-GENERIC/2', 1),
			first.replace(b'SIRIUS-MIR-GENERIC/3', b'SIRIUS-MIR-GENERIC/1', 1)):
			bad = work / 'bad.smirg'
			bad.write_bytes(bad_data)
			assert run(runner, '--specialize-generic', bad, 'twice', concrete, 'i64').returncode != 0
		print('PASS generic: invalid signatures and malformed packages rejected')


if __name__ == '__main__':
	main()
