#!/usr/bin/env python
"""Check content-addressed cache reuse, invalidation, and concurrent publishing."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import os
import tempfile
from unittest.mock import patch

import build_support
from build_support import BuildCache, MIR_RUNNER_ENV, MIR_RUNNER_TOOLCHAIN_ENV, require_cc

SUITE_NEEDS_MIR_RUNNER = False  # Runner builds are mocked in this test.


def check_shared_runner(cache: BuildCache, root: Path) -> None:
	shared = root / 'shared-runner'
	shared.write_bytes(b'suite executable')
	env = {MIR_RUNNER_ENV: str(shared), MIR_RUNNER_TOOLCHAIN_ENV: cache.toolchain}
	entry = 'run_cli'
	with patch.dict(os.environ, env), patch.object(cache, 'seed', side_effect=AssertionError('build requested')):
		assert cache.mir_runner() == shared.resolve()
		shared.unlink()
		try:
			cache.mir_runner()
		except RuntimeError as error:
			assert 'Shared MIR runner is missing' in str(error)
		else:
			raise AssertionError('Missing suite runner was silently rebuilt')
	shared.write_bytes(b'suite executable')
	# Different C compilers, other tools and explicit rebuilds use normal builds.
	class NormalBuild(Exception):
		pass
	for tool, toolchain, rebuild in (
		('modular_cli', cache.toolchain, False),
		(entry, 'another toolchain', False),
		(entry, cache.toolchain, True),
	):
		with patch.dict(os.environ, {**env, MIR_RUNNER_TOOLCHAIN_ENV: toolchain}), \
			patch.object(cache, 'rebuild', rebuild), patch.object(cache, 'seed', side_effect=NormalBuild):
			try:
				cache.native_tool(tool)
			except NormalBuild:
				pass
			else:
				raise AssertionError('A fresh/different tool build used the suite runner')


def check_runner_cache(cache: BuildCache, root: Path) -> None:
	project = root / 'project'
	(project / 'src').mkdir(parents=True)
	source = project / 'src' / 'run_cli.sirius'
	source.write_text('first', encoding='utf-8')
	seed = root / 'seed-key' / 'seed'
	runtime = root / 'runtime-key' / 'runtime.o'
	builds = []
	def generate(command):
		builds.append('generate')
		Path(command[-1]).write_bytes(b'C source')
	def compile_driver(cc, c_source, output, runtime_object):
		builds.append('compile')
		output.write_bytes(b'runner')
	with patch.object(build_support, 'ROOT', project), \
		patch.object(cache, 'seed', return_value=seed), \
		patch.object(cache, 'runtime', return_value=runtime), \
		patch.object(build_support, 'link_flags', return_value=()), \
		patch.object(build_support, 'execute', side_effect=generate), \
		patch.object(build_support, 'compile_c', side_effect=compile_driver), \
		patch.dict(os.environ, {MIR_RUNNER_ENV: '', MIR_RUNNER_TOOLCHAIN_ENV: ''}):
		first = cache.mir_runner()
		assert cache.mir_runner() == first and builds == ['generate', 'compile']
		source.write_text('changed', encoding='utf-8')
		assert cache.mir_runner() != first and builds == ['generate', 'compile'] * 2


def main() -> None:
	with tempfile.TemporaryDirectory() as tmp:
		old_build = build_support.BUILD
		build_support.BUILD = Path(tmp)
		try:
			cache = BuildCache(require_cc())
			check_shared_runner(cache, Path(tmp))
			check_runner_cache(cache, Path(tmp))
			built = []
			def write(output: Path, _: Path) -> None:
				built.append(output)
				output.write_bytes(b'cached')
			first = cache._artifact('test', (b'source1',), 'value', write)
			second = cache._artifact('test', (b'source1',), 'value', write)
			changed = cache._artifact('test', (b'source2',), 'value', write)
			if first != second or first == changed or len(built) != 2:
				raise AssertionError('Cache did not reuse or invalidate by content')
			with ThreadPoolExecutor(max_workers=4) as pool:
				paths = list(pool.map(lambda _: cache._artifact('race', (b'identical',), 'value', write), range(4)))
			if len(set(paths)) != 1 or not paths[0].is_file():
				raise AssertionError('Concurrent cache publishing failed')
			print('PASS cached artifacts, content invalidation, concurrent publishing')
		finally:
			build_support.BUILD = old_build


if __name__ == '__main__':
	main()
