#!/usr/bin/env python
"""Named concrete MIR library exports, structural signatures and malformed metadata."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def run(runner: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *args], capture_output=True, timeout=30)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='prebuilt sirius-run binary')
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		source = work / 'multi.sirius'
		source.write_text('def first() -> i64\n\tprint("first")\n\treturn 1\n'
			'def second() -> i64\n\tprint("second")\n\treturn first() + 1\n'
			'def helper() -> i64\n\treturn 3\n')
		library = work / 'multi.smirl'
		assert run(runner, '--emit-lib', str(library), '--export', 'first', '--export', 'second', str(source)).returncode == 0
		data = library.read_bytes()
		assert data.startswith(b'SIRIUS-MIR-LIB/1\n')
		assert run(runner, '--list-lib', str(library)).stdout == b'first : ()->i64\nsecond : ()->i64\n'
		for name, output, code in [('first', b'first\n', 1), ('second', b'second\nfirst\n', 2)]:
			result = run(runner, '--entry', name, '--load-lib', str(library))
			assert (result.stdout, result.stderr, result.returncode) == (output, b'', code), (name, result)
		assert run(runner, '--entry', 'missing', '--load-lib', str(library)).returncode == 2
		assert run(runner, '--load-mir', str(library)).returncode == 2
		assert run(runner, '--emit-lib', str(library), '--export', 'missing', str(source)).returncode == 1
		assert run(runner, '--emit-lib', str(library), '--export', 'first', '--export', 'first', str(source)).returncode == 2
		assert run(runner, '--emit-lib', str(library), '--export', 'bad.name', str(source)).returncode == 1
		assert run(runner, '--export', 'first', str(source)).returncode == 2
		print('PASS MIR library: multiple exports, calls, selection, unknown and duplicate symbols')

		# Compare equivalent types despite different specialization/interning order.
		struct = work / 'struct.sirius'
		struct.write_text('def noise() -> bool\n\treturn true\n'
			'def shape()\n\treturn {z = ["text"], a = {x = 3, y = false}}\n')
		a = work / 'a.smirl'
		b = work / 'b.smirl'
		assert run(runner, '--emit-lib', str(a), '--export', 'noise', '--export', 'shape', str(struct)).returncode == 0
		assert run(runner, '--emit-lib', str(b), '--export', 'shape', '--export', 'noise', str(struct)).returncode == 0
		list_a = run(runner, '--list-lib', str(a)).stdout.splitlines()
		list_b = run(runner, '--list-lib', str(b)).stdout.splitlines()
		assert sorted(list_a) == sorted(list_b), (list_a, list_b)
		assert b'Record{' in list_a[1] and b'Array[string]' in list_a[1]
		print('PASS MIR library: canonical signatures across independent TypeId orders')

		# Metadata is validated against the actual deserialized MIR, not trusted.
		bad = work / 'bad.smirl'
		for malformed in (b'', data[:-1], data + b'extra', data.replace(b'SIRIUS-MIR-LIB/1', b'SIRIUS-MIR-LIB/2', 1),
			data.replace(b'()->i64', b'()->f64', 1)):
			bad.write_bytes(malformed)
			result = run(runner, '--list-lib', str(bad))
			assert result.returncode == 2 and b'invalid MIR library:' in result.stderr or (
				result.returncode == 2 and b'cannot read MIR library' in result.stderr), result
		print('PASS MIR library: incompatible, truncated and forged signatures rejected')


if __name__ == '__main__':
	main()
