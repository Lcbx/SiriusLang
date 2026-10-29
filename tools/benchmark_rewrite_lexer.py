#!/usr/bin/env python3
"""Compare native lexer throughput; this is not an end-to-end benchmark."""
import statistics
import subprocess
import tempfile
import time
from pathlib import Path

from build_support import BuildCache, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	old = cache.native_driver('restart-lexer-bench-reference', ROOT / 'rewrite/lexer_reference_bench.sirius')
	new = cache.native_driver('restart-lexer-bench', ROOT / 'rewrite/lexer_bench.sirius',
		extra_modules=(ROOT / 'rewrite/lexer.sirius',))
	with tempfile.TemporaryDirectory() as directory:
		fixture = Path(directory) / 'input.sirius'
		for name, data in [('compiler', (ROOT / 'src/hir_analyze.sirius').read_bytes()),
				('numeric', b'1234567890 123456789.123456789 123456789e-123\n' * 4000)]:
			fixture.write_bytes(data)
			times = [[], []]
			expected = None
			for iteration in range(6):
				for index in ((0, 1) if iteration % 2 == 0 else (1, 0)):
					start = time.perf_counter()
					result = subprocess.run([str((old, new)[index]), str(fixture)], capture_output=True, check=True)
					elapsed = time.perf_counter() - start
					if expected is None:
						expected = result.stdout
					assert result.stdout == expected
					if iteration:
						times[index].append(elapsed)
			print(f'{name}: existing {statistics.median(times[0]):.3f}s, rewrite {statistics.median(times[1]):.3f}s (median of 5)')


if __name__ == '__main__':
	main()
