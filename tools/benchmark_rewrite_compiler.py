#!/usr/bin/env python3
"""Compare compilation of a program in the rewrite's currently supported subset."""
import statistics
import argparse
import subprocess
import tempfile
import time
from pathlib import Path

from build_support import BuildCache, require_cc
from project_paths import ROOT


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--bindings', type=int, default=0, help='benchmark this many distinct bindings instead of assignments')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	modules = tuple(ROOT / 'rewrite' / name for name in ('lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius', 'lower.sirius', 'interpreter.sirius', 'emit_c.sirius'))
	rewrite = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
	reference = cache.native_tool('modular_cli')
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		source = root / 'input.sirius'
		body = '\tvar x = 1\n' + '\tx = x * 3 + 1\n' * 200 + '\tprint(x)\n'
		label = '200 assignments'
		if args.bindings > 0:
			body = '\tlet x0 = 1\n' + ''.join(f'\tlet x{i} = x{i - 1} + 1\n' for i in range(1, args.bindings)) + f'\tprint(x{args.bindings - 1})\n'
			label = f'{args.bindings} bindings'
		source.write_text('def main() -> i64\n' + body + '\treturn 0\n', encoding='utf-8')
		times = [[], []]
		for iteration in range(6):
			for index in ((0, 1) if iteration % 2 == 0 else (1, 0)):
				start = time.perf_counter()
				result = subprocess.run([str((reference, rewrite)[index]), str(source), str(root / 'out.c')], capture_output=True)
				elapsed = time.perf_counter() - start
				assert result.returncode == 0, (result.stdout, result.stderr)
				if iteration:
					times[index].append(elapsed)
		print(f'{label}: existing {statistics.median(times[0]):.3f}s, rewrite {statistics.median(times[1]):.3f}s (median of 5)')


if __name__ == '__main__':
	main()
