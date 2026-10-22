#!/usr/bin/env python
"""Cross-library imports, structural signature checks and hostile metadata."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *args], capture_output=True, timeout=30)


def emit(runner: Path, directory: Path, name: str, body: str, exports: tuple[str, ...]) -> Path:
	source = directory / f'{name}.sirius'
	artifact = directory / f'{name}.smirl'
	source.write_text(body)
	result = run(runner, '--emit-lib', str(artifact), *(value for symbol in exports for value in ('--export', symbol)), str(source))
	assert result.returncode == 0, (name, result)
	return artifact


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		consumer = emit(runner, work, 'consumer',
			'extern def length_and_print(values: Array[string]) -> i64\n'
			'extern def step(x: i64) -> i64\n'
			'def main() -> i64\n'
			'\tlet a = ["hello", "world"]\n'
			'\treturn step(length_and_print(a))\n', ('main',))
		provider = emit(runner, work, 'provider',
			'def length_and_print(values: Array[string]) -> i64\n'
			'\tprint(values[!0])\n'
			'\treturn len(values) + 38\n'
			'def step(x: i64) -> i64\n\treturn x + 2\n', ('length_and_print', 'step'))
		listing = run(runner, '--list-lib', str(consumer))
		assert listing.returncode == 0, listing
		assert listing.stdout == (b'main : ()->i64\n'
			b'import length_and_print : (Array[string])->i64\n'
			b'import step : (i64)->i64\n'), listing
		assert b'unresolved imports' in run(runner, '--load-lib', str(consumer)).stderr
		assert b'unresolved imports' in run(runner, str(work / 'consumer.sirius')).stderr
		linked = work / 'linked.smirl'
		for index, inputs in enumerate(((consumer, provider), (provider, consumer))):
			result = run(runner, '--link-lib', str(linked), *(str(path) for path in inputs))
			assert result.returncode == 0, (index, result)
			result = run(runner, '--entry', 'main', '--load-lib', str(linked))
			assert (result.returncode, result.stdout, result.stderr) == (42, b'hello\n', b''), result
		print('PASS MIR imports: annotated declarations, managed calls, relocation in either link order')
		# Independently defined record constructors must resolve by structural shape,
		# even when their parameter ordering and TypeIds differ.
		struct_consumer = emit(runner, work, 'struct_consumer',
			'rec Shape(x: i64, y: string)\n\tpub x\n\tpub y\n'
			'extern def value(p: Shape) -> i64\n'
			'def main() -> i64\n\treturn value(Shape(42, "struct"))\n', ('main',))
		struct_provider = emit(runner, work, 'struct_provider',
			'rec Shape(y: string, x: i64)\n\tpub y\n\tpub x\n'
			'def value(p: Shape) -> i64\n\tprint(p.y)\n\treturn p.x\n', ('value',))
		assert run(runner, '--link-lib', str(linked), str(struct_provider), str(struct_consumer)).returncode == 0
		result = run(runner, '--entry', 'main', '--load-lib', str(linked))
		assert (result.returncode, result.stdout, result.stderr) == (42, b'struct\n', b''), result
		print('PASS MIR imports: independently interned structural record types')


		# Full linking is currently required; unresolved libraries cannot be executed.
		result = run(runner, '--link-lib', str(linked), str(consumer))
		assert result.returncode == 2 and b'unresolved MIR import: length_and_print' in result.stderr, result
		wrong = emit(runner, work, 'wrong',
			'def length_and_print(values: Array[i64]) -> i64\n\treturn len(values)\n'
			'def step(x: i64) -> i64\n\treturn x + 2\n', ('length_and_print', 'step'))
		result = run(runner, '--link-lib', str(linked), str(consumer), str(wrong))
		assert result.returncode == 2 and b'MIR import signature mismatch: length_and_print' in result.stderr, result
		print('PASS MIR imports: missing symbol and structural type mismatch rejected')

		# Library metadata is untrusted. A forged signature or placeholder is invalid.
		data = consumer.read_bytes()
		assert b'(Array[string])->i64' in data
		corrupt = work / 'corrupt.smirl'
		corrupt.write_bytes(data.replace(b'(Array[string])->i64', b'(Array[boolxx])->i64', 1))
		result = run(runner, '--list-lib', str(corrupt))
		assert result.returncode == 2 and b'MIR imported signature mismatch' in result.stderr, result
		corrupt.write_bytes(data.replace(b'unresolved MIR import: length_and_print', b'unresolved MIR import: length_and_ABCDE', 1))
		result = run(runner, '--list-lib', str(corrupt))
		assert result.returncode == 2, result
		print('PASS MIR imports: forged signatures and invalid placeholders rejected')

		for name, source in (
			('no_return', 'extern def step(x: i64)\ndef main() -> i64\n\treturn step(1)\n'),
			('no_type', 'extern def step(x) -> i64\ndef main() -> i64\n\treturn step(1)\n'),
			('bad_call', 'extern def step(x: string) -> i64\ndef main() -> i64\n\treturn step(1)\n'),
		):
			path = work / f'{name}.sirius'
			path.write_text(source)
			result = run(runner, '--emit-lib', str(linked), '--export', 'main', str(path))
			assert result.returncode == 1, (name, result)
		print('PASS MIR imports: declaration and call type errors diagnosed')


if __name__ == '__main__':
	main()
