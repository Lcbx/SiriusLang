#!/usr/bin/env python
"""Unit checks for the shared fixed-point algorithm and tool command dispatch."""
from pathlib import Path
from types import SimpleNamespace
from tempfile import TemporaryDirectory
from unittest.mock import patch
import subprocess
import sys

import bootstrap_support
import generate_bootstrap
import test_bootstrap


class FakeCache:
	cc = 'cc'

	def seed(self) -> Path:
		return Path('seed')

	def runtime(self) -> Path:
		return Path('runtime.o')


def main() -> None:
	with TemporaryDirectory() as tmp:
		work = Path(tmp)
		calls = []

		def execute(command):
			target = Path(command[-1])
			calls.append(target)
			target.write_bytes(b'generated\n')
			return subprocess.CompletedProcess(command, 0, '', '')

		def compile_c(cc, source, output, runtime):
			output.write_bytes(b'executable')

		# The generated C must match across platforms; only the binary suffix varies.
		for platform, suffix in (('linux', ''), ('win32', '.exe')):
			calls.clear()
			with patch.object(bootstrap_support, 'sys', SimpleNamespace(platform=platform)), \
				 patch.object(bootstrap_support, 'execute', execute), \
				 patch.object(bootstrap_support, 'compile_c', compile_c):
				generated, binary = bootstrap_support.generate_fixed_point(FakeCache(), work)
				assert generated == b'generated\n'
				assert binary == work / f'generation_1{suffix}'
				assert len(calls) == 2
				try:
					bootstrap_support.check_committed(work / 'generation_1.c', b'other\n')
				except AssertionError as e:
					assert 'differs' in str(e)
				else:
					raise AssertionError('Stale bootstrap accepted')

		cached = work / 'cached'
		cached.mkdir()
		(cached / 'generated.c').write_bytes(b'generated\n')
		class SharedCache(FakeCache):
			def native_tool(self, entry: str) -> Path:
				assert entry == 'modular_cli'
				return cached / 'compiler'
		with patch.object(bootstrap_support, 'execute', execute), \
			 patch.object(bootstrap_support, 'compile_c', side_effect=AssertionError('unexpected rebuild')):
			generated, binary = bootstrap_support.generate_fixed_point(SharedCache(), work, reuse_seed_compiler=True)
			assert generated == b'generated\n' and binary == cached / 'compiler'
			(cached / 'generated.c').write_bytes(b'different\n')
			try:
				bootstrap_support.generate_fixed_point(SharedCache(), work, reuse_seed_compiler=True)
			except AssertionError as e:
				assert 'Cached modular compiler source differs' in str(e)
			else:
				raise AssertionError('Mismatched cached compiler accepted')

		with patch.object(bootstrap_support, 'execute', side_effect=lambda command: Path(command[-1]).write_bytes(
				f'{len(calls)}\n'.encode())), \
			 patch.object(bootstrap_support, 'compile_c', compile_c):
			# Simulate successive changing generations.
			count = [0]
			def different(command):
				count[0] += 1
				Path(command[-1]).write_bytes(str(count[0]).encode())
			with patch.object(bootstrap_support, 'execute', different):
				try:
					bootstrap_support.generate_fixed_point(FakeCache(), work, max_generations=3)
				except AssertionError as e:
					assert 'fixed point' in str(e)
				else:
					raise AssertionError('Non-convergent bootstrap accepted')

		root = work / 'project'
		(root / 'bootstrap').mkdir(parents=True)
		committed = root / 'bootstrap/compiler.c'
		committed.write_bytes(b'old\n')
		checks = []
		with patch.object(generate_bootstrap, 'ROOT', root), \
			 patch.object(generate_bootstrap, 'BuildCache', return_value=FakeCache()), \
			 patch.object(generate_bootstrap, 'require_cc', return_value='cc'), \
			 patch.object(generate_bootstrap, 'generate_fixed_point', return_value=(b'new\n', Path('compiler'))), \
			 patch.object(sys, 'argv', ['generate_bootstrap.py']):
			generate_bootstrap.main()
		assert committed.read_bytes() == b'new\n'

		with patch.object(generate_bootstrap, 'ROOT', root), \
			 patch.object(generate_bootstrap, 'BuildCache', return_value=FakeCache()), \
			 patch.object(generate_bootstrap, 'require_cc', return_value='cc'), \
			 patch.object(generate_bootstrap, 'generate_fixed_point', return_value=(b'other\n', Path('compiler'))), \
			 patch.object(sys, 'argv', ['generate_bootstrap.py', '--check']):
			try:
				generate_bootstrap.main()
			except AssertionError as e:
				assert 'differs' in str(e)
			else:
				raise AssertionError('--check accepted stale C')
		assert committed.read_bytes() == b'new\n'

		with patch.object(test_bootstrap, 'ROOT', root), \
			 patch.object(test_bootstrap, 'BuildCache', return_value=FakeCache()), \
			 patch.object(test_bootstrap, 'require_cc', return_value='cc'), \
			 patch.object(test_bootstrap, 'generate_fixed_point', return_value=(b'new\n', Path('compiler'))), \
			 patch.object(test_bootstrap, 'check_cases', side_effect=lambda *args, **kwargs: checks.append(True)), \
			 patch.object(sys, 'argv', ['test_bootstrap.py']):
			test_bootstrap.main()
		assert checks == [True]
		marker = work / 'fixtures-passed'
		for compiler, expected in ((Path('shared'), True), (Path('other'), False)):
			if marker.exists():
				marker.unlink()
			cache = FakeCache()
			cache.native_tool = lambda entry: Path('shared')
			with patch.object(test_bootstrap, 'ROOT', root), \
				patch.object(test_bootstrap, 'BuildCache', return_value=cache), \
				patch.object(test_bootstrap, 'require_cc', return_value='cc'), \
				patch.object(test_bootstrap, 'generate_fixed_point', return_value=(b'new\n', compiler)), \
				patch.object(test_bootstrap, 'check_cases'), \
				patch.object(sys, 'argv', ['test_bootstrap.py', '--shared-fixtures-marker', str(marker)]):
				test_bootstrap.main()
			assert marker.exists() == expected
		print('PASS bootstrap tools: convergence, stale C, regeneration and tests')


if __name__ == '__main__':
	main()
