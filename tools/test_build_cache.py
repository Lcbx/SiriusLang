#!/usr/bin/env python
"""Check content-addressed cache reuse, invalidation, and concurrent publishing."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import os
import tempfile
from unittest.mock import patch

import build_support
from build_support import BuildCache, MIR_RUNNER_ENV, MIR_RUNNER_TOOLCHAIN_ENV, require_cc


def check_shared_runner(cache: BuildCache, root: Path) -> None:
	shared = root / 'shared-runner'
	shared.write_bytes(b'suite executable')
	env = {MIR_RUNNER_ENV: str(shared), MIR_RUNNER_TOOLCHAIN_ENV: cache.toolchain}
	entry = 'run_cli'
	with patch.dict(os.environ, env), patch.object(cache, 'seed', side_effect=AssertionError('build requested')):
		assert cache.native_tool(entry) == shared.resolve()
		shared.unlink()
		try:
			cache.native_tool(entry)
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


def main() -> None:
	with tempfile.TemporaryDirectory() as tmp:
		old_build = build_support.BUILD
		build_support.BUILD = Path(tmp)
		try:
			cache = BuildCache(require_cc())
			check_shared_runner(cache, Path(tmp))
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
