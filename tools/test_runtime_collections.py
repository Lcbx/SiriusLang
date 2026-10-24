#!/usr/bin/env python
"""Check generic runtime array growth, element aliasing, and CoW semantics."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, compile_c, options, require_cc
from project_paths import executable_path, ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	with tempfile.TemporaryDirectory() as temporary:
		output = executable_path(Path(temporary) / 'runtime_collections')
		compile_c(cache.cc, ROOT / 'tests/runtime/collections.c', output, cache.runtime())
		result = subprocess.run([str(output)], capture_output=True, text=True, check=True)
		assert result.stdout == 'PASS runtime collection aliasing, growth, managed values and CoW\n', result
		print(result.stdout, end='')


if __name__ == '__main__':
	main()
