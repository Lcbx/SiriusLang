#!/usr/bin/env python3
"""Profile generated MIR functions while the modular compiler self-compiles.

IDs are generated and unstable; do not interpret them as named compiler phases.
The instrumented C is temporary; compiler sources and committed C are unchanged.
"""
import argparse
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, C_FLAGS, require_cc, link_flags
from project_paths import ROOT

# Generated MIR function IDs are unstable across module-root builds.
# Collect IDs dynamically instead of guessing a mapping to compiler phases.
HOOKS = r'''
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif
#define PROFILE_MAX 4096
static uint64_t p_inclusive[PROFILE_MAX], p_exclusive[PROFILE_MAX], p_calls[PROFILE_MAX];
static struct { int id; uint64_t started, child; } p_stack[128];
static int p_depth;
static uint64_t p_now(void) {
	#ifdef _WIN32
	static LARGE_INTEGER frequency;
	LARGE_INTEGER counter;
	if (!frequency.QuadPart) QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&counter);
	return (uint64_t)((double)counter.QuadPart * 1000000000.0 / (double)frequency.QuadPart);
	#else
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (uint64_t)t.tv_sec * 1000000000ull + t.tv_nsec;
	#endif
}
static void p_enter(int id) {
	if (p_depth >= 128) { fprintf(stderr, "profile stack exceeded\\n"); return; }
	p_stack[p_depth].id = id;
	p_stack[p_depth].started = p_now();
	p_stack[p_depth].child = 0;
	p_depth++;
}
static void p_leave(int id) {
	uint64_t ended = p_now();
	if (!p_depth || p_stack[p_depth-1].id != id) { fprintf(stderr, "profile stack mismatch\\n"); return; }
	p_depth--;
	uint64_t duration = ended - p_stack[p_depth].started;
	p_inclusive[id] += duration;
	p_exclusive[id] += duration - p_stack[p_depth].child;
	p_calls[id]++;
	if (p_depth) p_stack[p_depth-1].child += duration;
}
static void p_report(void) {
	fprintf(stderr, "PROFILE id,calls,inclusive_ms,exclusive_ms\\n");
	for (int i = 0; i < PROFILE_MAX; i++)
		if (p_calls[i]) fprintf(stderr, "PROFILE %d,%llu,%.3f,%.3f\\n", i,
			(unsigned long long)p_calls[i], p_inclusive[i]/1e6, p_exclusive[i]/1e6);
}
'''.replace('\\\\n', '\\n')


def instrument(source: str) -> str:
	"""Instrument generated functions by their actual IDs; never assume stable IDs."""
	pattern = r'^static [^\n]+\bs_mir_f(\d+)\([^\n;]*\)\n\{'
	matches = list(re.finditer(pattern, source, re.M))
	if not matches:
		raise ValueError('No generated MIR functions found')
	if max(int(m.group(1)) for m in matches) >= 4096:
		raise ValueError('Too many generated MIR functions for profiler')
	for match in reversed(matches):
		index = int(match.group(1))
		start = match.end()
		ending = source.find('\n}', start)
		if ending < 0:
			raise ValueError(f'Unclosed generated function {index}')
		body = source[start:ending]
		if body.count('return returned;') != 1:
			continue
		body = body.replace('return returned;', f'p_leave({index});\n\treturn returned;')
		source = source[:start] + f'\n\tp_enter({index});' + body + source[ending:]
	main = re.search(r'(int main\(int argc, char \*\*argv\) \{ s_set_arguments\(argc, argv\); (?:\(void\)&s_mir_f\d+; )*)return \(int\)(s_mir_f\d+\(\)); \}', source)
	if main is None:
		raise ValueError('Unknown generated MIR main() layout')
	source = source[:main.start()] + (
		main.group(1) + 'int result = (int)' + main.group(2) + '; p_report(); return result; }') + source[main.end():]
	return HOOKS + source


def run(args: list[str], timeout: int) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(args, capture_output=True, text=True, timeout=timeout)
	if result.returncode:
		raise RuntimeError(f'{args[0]} returned {result.returncode}: {result.stderr[-3000:]}')
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--generated-c', type=Path, help='Reuse a generated MIR compiler C file')
	parser.add_argument('--stage1', type=Path, help='Use an existing compiler to generate C, bypassing cache build')
	parser.add_argument('--keep-dir', type=Path, help='Store generated C and profiling binary here')
	parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
	parser.add_argument('--timeout', type=int, default=240)
	parser.add_argument('--opt', choices=('0', '1', '2'), default='0', help='Native C optimization level')
	args = parser.parse_args()
	if args.keep_dir:
		args.keep_dir.mkdir(parents=True, exist_ok=True)
	with tempfile.TemporaryDirectory() as tmp:
		work = args.keep_dir or Path(tmp)
		generated = args.generated_c or work / 'compiler.c'
		if not args.generated_c:
			stage1 = args.stage1 or BuildCache(require_cc()).seed()
			run([str(stage1), '--module-root', str(ROOT / 'src'), 'modular_cli', str(generated)], args.timeout)
		print(f'Generated compiler C: {generated.stat().st_size:,} bytes', flush=True)
		modified = work / 'compiler.profile.c'
		modified.write_text(instrument(generated.read_text()))
		binary = work / 'compiler.profile'
		c_flags = [f'-O{args.opt}' if flag == '-O0' else flag for flag in C_FLAGS]
		compile_cmd = [args.cc, *c_flags, '-Wno-unused-function', '-I', str(ROOT / 'runtime'), str(modified),
			str(ROOT / 'runtime' / 'sirius_runtime.c'), *link_flags(args.cc), '-o', str(binary)]
		run(compile_cmd, args.timeout)
		out = work / 'next.c'
		start = perf_counter()
		result = run([str(binary), '--module-root', str(ROOT / 'src'), 'modular_cli', str(out)], args.timeout)
		wall = perf_counter() - start
		if out.read_bytes() != generated.read_bytes():
			raise RuntimeError('Profiled compiler changed its C output')
		print(f'Native MIR self-compile (-O{args.opt}): {wall:.3f}s; output matches')
		print('function_id,calls,inclusive_ms,exclusive_ms')
		rows = []
		for line in result.stderr.splitlines():
			if line.startswith('PROFILE ') and not line.startswith('PROFILE id,'):
				index, calls, inclusive, exclusive = line.split(' ', 1)[1].split(',')
				rows.append((int(index), int(calls), float(inclusive), float(exclusive)))
		if not rows:
			raise RuntimeError('No generated MIR function samples; profiler layout changed')
		for index, calls, inclusive, exclusive in sorted(rows, key=lambda item: -item[3])[:30]:
			print(f's_mir_f{index},{calls},{inclusive:.3f},{exclusive:.3f}')
		print('Function IDs are generated and unstable; no semantic phase names are inferred.')


if __name__ == '__main__':
	main()
