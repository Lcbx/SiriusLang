#!/usr/bin/env python3
"""Instrument MIR-generated C and report self-compilation phase timings.

Profiles the standalone self-hosted compiler. Phase IDs below describe the
current specialization order, verified against compile_c_mir and mir_build_program.
This is a diagnostic tool: it never modifies compiler sources or committed C.
"""
import argparse
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
from time import perf_counter

from build_support import BuildCache, C_FLAGS, require_cc
from source_manifest import MIR_ONLY_COMPILER_SOURCES, ROOT, combined_sources

# These IDs are checked against the committed bootstrap's top-level
# specialization calls. Deeper IDs are deliberately not named here: they
# change when source ordering or specialization changes, and stale mappings
# previously produced misleading timings (e.g. "HIR specialization" with 168
# calls, despite being a once-per-compilation stage).
PHASES = {
	1: 'compile_c_mir',
	2: 'parse_module',
	3: 'hir_module_declarations',
	4: 'mir_build_program',
	5: 'mir_emit_c_verified',
}

HOOKS = r'''
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#define PROFILE_MAX 256
static uint64_t p_inclusive[PROFILE_MAX], p_exclusive[PROFILE_MAX], p_calls[PROFILE_MAX];
static struct { int id; uint64_t started, child; } p_stack[128];
static int p_depth;
static uint64_t p_now(void) {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (uint64_t)t.tv_sec * 1000000000ull + t.tv_nsec;
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
	"""Insert profiling only at known specialized-function boundaries."""
	source = HOOKS + source
	for index, name in PHASES.items():
		pattern = rf'(^static [^\n]+\bs_mir_f{index}\([^\n;]*\)\n\{{)(.*?)(?=\nstatic [^\n]+\bs_mir_f\d+\([^\n;]*\)\n\{{|\nint main\()'
		found = list(re.finditer(pattern, source, re.M | re.S))
		if len(found) != 1:
			raise ValueError(f'Cannot locate the {name} specialization f{index}; profile IDs may have changed')
		match = found[0]
		body = match.group(2)
		if body.count('return returned;') != 1:
			raise ValueError(f'Unexpected exit layout for {name}')
		body = body.replace('return returned;', f'p_leave({index});\n\treturn returned;')
		source = source[:match.start()] + match.group(1) + f'\n\tp_enter({index});' + body + source[match.end():]
	source = source.replace('int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_mir_f0(); }',
		'int main(int argc, char **argv) { s_set_arguments(argc, argv); int result = (int)s_mir_f0(); p_report(); return result; }')
	return source


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
		compiler_source = work / 'mir_compiler.sirius'
		compiler_source.write_bytes(combined_sources(MIR_ONLY_COMPILER_SOURCES, separator=b''))
		generated = args.generated_c or work / 'compiler.c'
		if not args.generated_c:
			stage1 = args.stage1 or BuildCache(require_cc()).sirius('mir-only-profile-stage1', MIR_ONLY_COMPILER_SOURCES, separator=b'')
			run([str(stage1), str(compiler_source), str(generated)], args.timeout)
		print(f'Generated compiler C: {generated.stat().st_size:,} bytes', flush=True)
		modified = work / 'compiler.profile.c'
		modified.write_text(instrument(generated.read_text()))
		binary = work / 'compiler.profile'
		c_flags = [f'-O{args.opt}' if flag == '-O0' else flag for flag in C_FLAGS]
		compile_cmd = [args.cc, *c_flags, '-Wno-unused-function', '-I', str(ROOT / 'runtime'), str(modified),
			str(ROOT / 'runtime' / 'sirius_runtime.c'), '-lm', '-o', str(binary)]
		run(compile_cmd, args.timeout)
		out = work / 'next.c'
		start = perf_counter()
		result = run([str(binary), str(compiler_source), str(out)], args.timeout)
		wall = perf_counter() - start
		if out.read_bytes() != generated.read_bytes():
			raise RuntimeError('Profiled compiler changed its C output')
		print(f'Native MIR self-compile (-O{args.opt}): {wall:.3f}s; output matches')
		print('Phase,calls,inclusive_ms,exclusive_ms')
		observed = set()
		for line in result.stderr.splitlines():
			if line.startswith('PROFILE ') and not line.startswith('PROFILE id,'):
				index, calls, inclusive, exclusive = line.split(' ', 1)[1].split(',')
				if int(calls) != 1:
					raise RuntimeError(f'Stale profiler mapping for {PHASES[int(index)]}: expected one call, got {calls}')
				observed.add(int(index))
				print(f'{PHASES[int(index)]},{calls},{inclusive},{exclusive}')
		if observed != set(PHASES):
			raise RuntimeError(f'Missing profiler phases: {set(PHASES) - observed}')


if __name__ == '__main__':
	main()
