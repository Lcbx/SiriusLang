#!/usr/bin/env python
"""Cross-library imports: named structural signatures and call relocation."""
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
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		provider = work / 'provider.sirius'
		provider.write_text('def process(xs: Array[i64]) -> i64\n\treturn xs[!0] + len(xs)\n'
			'def provider() -> i64\n\treturn process([3, 4])\n')
		consumer = work / 'consumer.sirius'
		consumer.write_text('def process(xs: Array[i64]) -> i64\n\treturn -1\n'
			'def consumer() -> i64\n\treturn process([7, 8]) + 1\n')
		p = work / 'provider.smirl'
		c = work / 'consumer.smirl'
		linked = work / 'linked.smirl'
		assert run(runner, '--emit-lib', str(p), '--export', 'provider', '--expose', 'process', str(provider)).returncode == 0
		assert run(runner, '--emit-lib', str(c), '--export', 'consumer', '--import', 'process', str(consumer)).returncode == 0
		assert p.read_bytes().startswith(b'SIRIUS-MIR-LIB/1\n')
		assert c.read_bytes().startswith(b'SIRIUS-MIR-LIB/2\n')
		assert run(runner, '--list-lib', str(c)).stdout == b'consumer : ()->i64\nimport process : (Array[i64])->i64\n'
		unlinked = run(runner, '--entry', 'consumer', '--load-lib', str(c))
		assert unlinked.returncode == 2 and b'unresolved imports' in unlinked.stderr, unlinked
		assert run(runner, '--link-lib', str(linked), str(c), str(p)).returncode == 0
		assert linked.read_bytes().startswith(b'SIRIUS-MIR-LIB/1\n')
		assert run(runner, '--entry', 'consumer', '--load-lib', str(linked)).returncode == 10
		assert run(runner, '--entry', 'provider', '--load-lib', str(linked)).returncode == 5
		assert run(runner, '--entry', 'process', '--load-lib', str(linked)).returncode == 2  # takes an argument
		print('PASS MIR imports: v2 consumer, v1 provider, managed call relocation and execution')

		# Order changes remapped local function and composite TypeIds.
		other = work / 'other.smirl'
		assert run(runner, '--link-lib', str(other), str(p), str(c)).returncode == 0
		assert run(runner, '--entry', 'consumer', '--load-lib', str(other)).returncode == 10
		assert run(runner, '--link-lib', str(other), str(c), str(p)).returncode == 0
		assert linked.read_bytes() == other.read_bytes()
		print('PASS MIR imports: both link orders and deterministic byte representation')

		unrelated_source = work / 'unrelated.sirius'
		unrelated_source.write_text('def unrelated() -> i64\n\treturn 4\n')
		unrelated = work / 'unrelated.smirl'
		assert run(runner, '--emit-lib', str(unrelated), '--export', 'unrelated', str(unrelated_source)).returncode == 0
		missing = run(runner, '--link-lib', str(other), str(c), str(unrelated))
		assert missing.returncode == 2 and b'unresolved MIR import: process' in missing.stderr, missing
		mismatch = work / 'mismatch.sirius'
		mismatch.write_text('def process(xs: Array[bool]) -> i64\n\treturn len(xs)\n'
			'def different() -> i64\n\treturn process([true, false])\n')
		bad = work / 'bad.smirl'
		assert run(runner, '--emit-lib', str(bad), '--export', 'different', '--expose', 'process', str(mismatch)).returncode == 0
		rejected = run(runner, '--link-lib', str(other), str(c), str(bad))
		assert rejected.returncode == 2 and b'MIR import signature mismatch: process' in rejected.stderr, rejected
		print('PASS MIR imports: missing and structurally incompatible provider rejected')

		# A forged import signature must not bypass type matching at link time.
		forged = work / 'forged.smirl'
		forged.write_bytes(c.read_bytes().replace(b'(Array[i64])->i64', b'(Array[f64])->i64', 1))
		result = run(runner, '--list-lib', str(forged))
		assert result.returncode == 2 and b'MIR imported signature mismatch' in result.stderr, result
		print('PASS MIR imports: forged import metadata rejected')

		assert run(runner, '--emit-lib', str(other), '--export', 'consumer', '--import', 'unknown', str(consumer)).returncode == 2
		assert run(runner, '--emit-lib', str(other), '--export', 'consumer', '--import', 'consumer', str(consumer)).returncode == 2
		assert run(runner, '--emit-lib', str(other), '--export', 'consumer', '--expose', 'consumer', str(consumer)).returncode == 2
		assert run(runner, '--emit-lib', str(other), '--export', 'consumer', '--import', 'process', '--import', 'process', str(consumer)).returncode == 2
		print('PASS MIR imports: invalid imports and conflicting source declarations rejected')


if __name__ == '__main__':
	main()
