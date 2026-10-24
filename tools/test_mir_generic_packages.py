#!/usr/bin/env python3
"""Source-free generic specialization, deterministic artifacts and import linking."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *args], capture_output=True, timeout=30)


def checked(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	result = run(runner, *args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='existing sirius-run executable')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).native_tool('run_cli')
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		source = work / 'Ops.sirius'
		source.write_text(
			'export def twice(x) -> i64\n\treturn x + x\n'
			'export def size(xs) -> i64\n\treturn len(xs)\n'
			'export def field(value) -> i64\n\treturn value.x\n', encoding='utf-8')
		package = work / 'Ops.smirg'
		checked(runner, '--emit-generic', 'Ops', str(package), str(source))
		first = package.read_bytes()
		checked(runner, '--emit-generic', 'Ops', str(package), str(source))
		assert first == package.read_bytes()
		source.unlink()  # Verify subsequent specialization needs no provider source.

		specialized = work / 'twice.smirl'
		checked(runner, '--specialize-generic', str(package), 'twice', str(specialized), 'i64')
		symbol = run(runner, '--list-lib', str(specialized)).stdout.decode().split(' : ')[0]
		caller = work / 'caller.sirius'
		caller.write_text(f'extern def {symbol}(x: i64) -> i64\n'
			f'def main() -> i64\n\treturn {symbol}(21)\n', encoding='utf-8')
		caller_lib = work / 'caller.smirl'
		linked = work / 'linked.smirl'
		checked(runner, '--emit-lib', str(caller_lib), '--export', 'main', str(caller))
		checked(runner, '--link-lib', str(linked), str(caller_lib), str(specialized))
		assert run(runner, '--entry', 'main', '--load-lib', str(linked)).returncode == 42
		print('PASS portable generic: source removed, independent MIR caller linked')

		for name, arg_type, body_arg, expected in (
			('size', 'Array[i64]', '[4, 5, 6]', 3),
			('size', 'Dict[string,i64]', 'Dict[string,i64]().insert("a", 1)', 1),
			('field', 'Record{1:x=i64;}', 'Point(7)', 7),
		):
			result = run(runner, '--specialize-generic', str(package), name, str(specialized), arg_type)
			assert result.returncode == 0, (name, arg_type, result.stderr)
			symbol = run(runner, '--list-lib', str(specialized)).stdout.decode().split(' : ')[0]
			annotation = arg_type
			prefix = ''
			if name == 'field':
				prefix = 'rec Point(x: i64)\n\tpub x\n'
				annotation = 'Point'
			caller.write_text(prefix + f'extern def {symbol}(value: {annotation}) -> i64\n'
				f'def main() -> i64\n\treturn {symbol}({body_arg})\n', encoding='utf-8')
			checked(runner, '--emit-lib', str(caller_lib), '--export', 'main', str(caller))
			checked(runner, '--link-lib', str(linked), str(caller_lib), str(specialized))
			assert run(runner, '--entry', 'main', '--load-lib', str(linked)).returncode == expected
		print('PASS portable generic: array, dict and structural record specialization')

		dependency_source = work / 'Apply.sirius'
		dependency_source.write_text('extern def helper(x: i64) -> i64\n'
			'export def apply(x) -> i64\n\treturn helper(x)\n', encoding='utf-8')
		dependency = work / 'Apply.smirg'
		checked(runner, '--emit-generic', 'Apply', str(dependency), str(dependency_source))
		dependency_source.unlink()
		checked(runner, '--specialize-generic', str(dependency), 'apply', str(specialized), 'i64')
		assert b'import helper : (i64)->i64' in run(runner, '--list-lib', str(specialized)).stdout
		provider = work / 'helper.sirius'
		provider.write_text('def helper(x: i64) -> i64\n\treturn x + 20\n', encoding='utf-8')
		provider_lib = work / 'helper.smirl'
		checked(runner, '--emit-lib', str(provider_lib), '--export', 'helper', str(provider))
		checked(runner, '--link-lib', str(linked), str(specialized), str(provider_lib))
		print('PASS portable generic: explicit named extern dependency survives packaging')

		for name, types in [('missing', ['i64']), ('twice', []), ('twice', ['wrong']),
			('twice', ['Record{1:x=i64;}']), ('size', ['Array[unknown]']),
			('size', ['Record{1:x=i64;1:x=i64;}'])]:
			assert run(runner, '--specialize-generic', str(package), name, str(specialized), *types).returncode != 0, (name, types)
		for bad_bytes in (b'', first[:-1], first + b'extra', first.replace(b'SIRIUS-MIR-GENERIC/1', b'SIRIUS-MIR-GENERIC/2', 1),
			first.replace(b'5\ntwice\n', b'5\nwrong\n', 1)):
			bad = work / 'bad.smirg'
			bad.write_bytes(bad_bytes)
			assert run(runner, '--specialize-generic', str(bad), 'twice', str(specialized), 'i64').returncode != 0
		print('PASS portable generic: invalid types, signatures and artifacts rejected')

		importing = work / 'Importing.sirius'
		importing.write_text('import Missing alias M\nexport def bad(x) -> i64\n\treturn x\n', encoding='utf-8')
		assert run(runner, '--emit-generic', 'Importing', str(dependency), str(importing)).returncode != 0
		no_export = work / 'Hidden.sirius'
		no_export.write_text('def hidden(x) -> i64\n\treturn x\n', encoding='utf-8')
		assert run(runner, '--emit-generic', 'Hidden', str(dependency), str(no_export)).returncode != 0
		print('PASS portable generic: unsupported source imports and private exports rejected')


if __name__ == '__main__':
	main()
