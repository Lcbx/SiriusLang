#!/usr/bin/env python
"""Check content-addressed cache reuse, invalidation, and concurrent publishing."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import tempfile

import test_build
from test_build import BuildCache, require_cc


def main() -> None:
	with tempfile.TemporaryDirectory() as tmp:
		old_build = test_build.BUILD
		test_build.BUILD = Path(tmp)
		try:
			cache = BuildCache(require_cc())
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
			test_build.BUILD = old_build


if __name__ == '__main__':
	main()
