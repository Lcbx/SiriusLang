#!/usr/bin/env python3
"""Profile S_Dict operations while the MIR bootstrap compiles real Sirius sources.

Unlike the C microbenchmarks, this measures dictionary activity in the
self-hosted compiler's own HIR/MIR pipeline. It does not change the committed
bootstrap or normal runtime. The counters are enabled only for this binary.
"""
import argparse
import hashlib
from pathlib import Path
import re
import shutil
import statistics
import sys
import subprocess
import tempfile
from time import perf_counter

from build_support import link_flags
from project_paths import ROOT

COUNTERS = ('new', 'empty_release', 'insert', 'overwrite', 'get', 'miss', 'clone',
	'cloned_entries', 'grow', 'slots', 'probes', 'equal', 'size_le5', 'size_le11', 'size_gt11')


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(command, capture_output=True, text=True)
	if result.returncode:
		raise RuntimeError(f'{command[0]} failed ({result.returncode}): {result.stderr[-3000:]}')
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=shutil.which('clang') or shutil.which('cc') or 'cc')
	parser.add_argument('--runs', type=int, default=3)
	parser.add_argument('--opt', choices=('0', '1', '2'), default='0', help='Native compiler optimization')
	parser.add_argument('--source', type=Path, help='Alternate Sirius input (default: native modular compiler root)')
	parser.add_argument('--generated-c', type=Path, help='Use a newer generated compiler C (default: committed bootstrap)')
	args = parser.parse_args()
	if args.runs < 1:
		parser.error('--runs must be positive')
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		source = args.source
		compiler_c = args.generated_c or ROOT / 'bootstrap/compiler.c'
		inputs = (compiler_c, ROOT / 'runtime/sirius_runtime.c',
			ROOT / 'runtime/sirius_runtime.h')
		key = hashlib.sha256()
		key.update((str(Path(args.cc).resolve()) + args.opt + run([args.cc, '--version']).stdout).encode())
		for file in inputs:
			key.update(file.read_bytes())
		cache_dir = ROOT / '.build' / 'profile_dict' / key.hexdigest()
		binary = cache_dir / ('compiler.exe' if sys.platform == 'win32' else 'compiler')
		if not binary.exists():
			cache_dir.mkdir(parents=True, exist_ok=True)
			run([args.cc, '-std=c11', f'-O{args.opt}', '-DS_DICT_PROFILE',
				'-I', str(ROOT / 'runtime'), str(compiler_c),
				str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(args.cc), '-o', str(binary)])
		timings = []
		counts = []
		for i in range(args.runs):
			output = work / f'output_{i}.c'
			start = perf_counter()
			command = ([str(binary), str(source), str(output)] if source else
				[str(binary), '--module-root', str(ROOT / 'src'), 'modular_cli', str(output)])
			result = run(command)
			timings.append(perf_counter() - start)
			match = re.search(r'^S_DICT_PROFILE (.+)$', result.stderr, re.M)
			if not match:
				raise RuntimeError('Runtime did not report dictionary counters')
			counts.append(dict((key, int(value)) for key, value in
				(token.split('=', 1) for token in match.group(1).split())))
			if set(counts[-1]) != set(COUNTERS):
				raise RuntimeError(f'Unexpected counters: {counts[-1]}')
			if source is None and output.read_bytes() != compiler_c.read_bytes():
				raise RuntimeError('Profiled compiler does not reproduce the committed C bootstrap')
		if any(value != counts[0] for value in counts[1:]):
			raise RuntimeError('Non-deterministic dictionary workload counters')
		print(f'Compiled: {source if args.source else "self-hosted compiler"}')
		print(f'Native -O{args.opt}: median {statistics.median(timings):.3f}s ({args.runs} runs)')
		for key in COUNTERS:
			print(f'{key:>14}: {counts[0][key]}')


if __name__ == '__main__':
	main()
