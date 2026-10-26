#!/usr/bin/env python3
"""Count MIR-generated record/array/optional/dictionary allocations during self-hosting.

Instrumentation is injected into a temporary copy of generated C. Neither the
Sirius emitter, its committed bootstrap, nor the runtime ABI is changed.
Counts are workload-dependent; C allocation totals include neither strings nor
allocations internal to S_Dict or the Sirius compiler itself.
"""
import argparse
import hashlib
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import sys
import tempfile
from time import perf_counter

from build_support import BuildCache, link_flags
from project_paths import ROOT

COUNTERS = ('new', 'empty', 'one', 'many', 'clones', 'unique', 'growth',
	'dict_new', 'dict_insert', 'array_append', 'append_clones', 'array_set', 'optional')

REPORTER = r'''
static size_t s_mir_profile_new, s_mir_profile_empty, s_mir_profile_one;
static size_t s_mir_profile_clones, s_mir_profile_unique, s_mir_profile_growth;
static size_t s_mir_profile_dict_new, s_mir_profile_dict_insert;
static size_t s_mir_profile_array_append, s_mir_profile_append_clones, s_mir_profile_array_set;
static size_t s_mir_profile_optional;
static size_t s_mir_profile_type_counts[4096], s_mir_profile_type_empty[4096];
static size_t s_mir_profile_type_bytes[4096], s_mir_profile_type_clones[4096];
static void s_mir_profile_report(void) {
	fprintf(stderr, "S_MIR_ALLOC_PROFILE new=%zu empty=%zu one=%zu many=%zu clones=%zu unique=%zu growth=%zu dict_new=%zu dict_insert=%zu array_append=%zu append_clones=%zu array_set=%zu optional=%zu\n",
		s_mir_profile_new, s_mir_profile_empty, s_mir_profile_one,
		s_mir_profile_new - s_mir_profile_empty - s_mir_profile_one,
		s_mir_profile_clones, s_mir_profile_unique, s_mir_profile_growth,
		s_mir_profile_dict_new, s_mir_profile_dict_insert,
		s_mir_profile_array_append, s_mir_profile_append_clones, s_mir_profile_array_set, s_mir_profile_optional);
	for (size_t i = 0; i < 4096; ++i)
		if (s_mir_profile_type_counts[i])
			fprintf(stderr, "S_MIR_ALLOC_TYPE id=%zu new=%zu empty=%zu bytes=%zu clones=%zu\n",
				i, s_mir_profile_type_counts[i], s_mir_profile_type_empty[i],
				s_mir_profile_type_bytes[i], s_mir_profile_type_clones[i]);
}
static void s_mir_profile_allocation(int64_t id, size_t fields) {
	if (!s_mir_profile_new) atexit(s_mir_profile_report);
	++s_mir_profile_new;
	if (fields == 0) ++s_mir_profile_empty;
	else if (fields == 1) ++s_mir_profile_one;
	if (id >= 0 && id < 4096) {
		++s_mir_profile_type_counts[id];
		if (fields == 0) ++s_mir_profile_type_empty[id];
		s_mir_profile_type_bytes[id] += sizeof(S_MirRecord) + fields * sizeof(S_MirField);
	}
}
'''


def instrument(source: str) -> str:
	"""Patch only the MIR runtime helpers, asserting expected source shapes."""
	start = 'static inline S_MirRecord *s_mir_record_new(int64_t id, size_t fields) {\n'
	if source.count(start) != 1:
		raise ValueError('Missing MIR record allocator in generated C')
	source = source.replace(start, REPORTER + '\n' + start +
		'\ts_mir_profile_allocation(id, fields);\n', 1)
	# Insert counters immediately after the initial conditions, preserving behavior.
	replacements = (
		('if (need <= r->capacity) return r;', 'if (need <= r->capacity) return r;\n\t++s_mir_profile_growth;'),
		('if (record->refs == 1) return record;',
		 'if (record->refs == 1) { ++s_mir_profile_unique; return record; }\n\t++s_mir_profile_clones;'),
		('S_MirRecord *r = s_mir_record_new(id, 1); r->fields[0].dict = s_dict_new(',
		 '++s_mir_profile_dict_new;\n\tS_MirRecord *r = s_mir_record_new(id, 1); r->fields[0].dict = s_dict_new('),
		('r = s_mir_record_unique_owned(r);\n\tr->fields[0].dict = s_dict_insert(',
		 '++s_mir_profile_dict_insert;\n\tr = s_mir_record_unique_owned(r);\n\tr->fields[0].dict = s_dict_insert('),
		('if (index < 0 || (uint64_t)index >= array->length) s_panic("array index out of bounds");\n\tarray = s_mir_record_unique_owned(array);',
		 'if (index < 0 || (uint64_t)index >= array->length) s_panic("array index out of bounds");\n\t++s_mir_profile_array_set;\n\tarray = s_mir_record_unique_owned(array);'),
		('S_MirRecord *optional = s_mir_record_new(optional_type, value ? 1 : 0);',
		 '++s_mir_profile_optional;\n\tS_MirRecord *optional = s_mir_record_new(optional_type, value ? 1 : 0);'),
	)
	append_line = ('\tarray = s_mir_record_unique_owned_reserved(array, 1);\n'
		if 's_mir_record_unique_owned_reserved(array, 1)' in source else
		'\tarray = s_mir_record_unique_owned(array);\n')
	marker = append_line + '\tarray = s_mir_record_reserve_owned('
	if source.count(marker) != 1:
		raise ValueError('Expected one generated array append helper')
	source = source.replace(marker,
		'\t++s_mir_profile_array_append;\n'
		'\tif (array->refs > 1) ++s_mir_profile_append_clones;\n' + marker, 1)
	clone_marker = '	S_MirRecord *copy = s_mir_record_new(record->type_id, record->length + extra);'
	if source.count(clone_marker) != 1:
		raise ValueError('Expected one CoW clone allocator')
	source = source.replace(clone_marker,
		'\tif (record->type_id >= 0 && record->type_id < 4096) '
		'++s_mir_profile_type_clones[record->type_id];\n' + clone_marker, 1)
	for needle, replacement in replacements:
		if source.count(needle) != 1:
			raise ValueError(f'Expected exactly one generated MIR helper marker: {needle[:65]}')
		source = source.replace(needle, replacement, 1)
	return source


def parse_report(stderr: str) -> tuple[dict[str, int], dict[int, dict[str, int]]]:
	stderr = stderr.replace('\r\n', '\n')
	match = re.search(r'^S_MIR_ALLOC_PROFILE (.+)$', stderr, re.M)
	if not match:
		raise ValueError('MIR allocation profile was not reported')
	counts = {key: int(value) for key, value in
		(token.split('=', 1) for token in match.group(1).split())}
	if tuple(counts) != COUNTERS:
		raise ValueError(f'Unexpected MIR allocation counters: {counts}')
	by_type = {int(type_id): dict(new=int(new), empty=int(empty), bytes=int(size), clones=int(clones))
		for type_id, new, empty, size, clones in re.findall(
			r'^S_MIR_ALLOC_TYPE id=(\d+) new=(\d+) empty=(\d+) bytes=(\d+) clones=(\d+)$', stderr, re.M)}
	if sum(entry['new'] for entry in by_type.values()) > counts['new']:
		raise ValueError('Per-type counts exceed allocation count')
	return counts, by_type


def same_generated_c(generated: Path, seed: Path) -> bool:
	"""Compare C text, ignoring platform-specific LF/CRLF differences."""
	return generated.read_text(encoding='utf-8') == seed.read_text(encoding='utf-8')


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(command, capture_output=True, text=True, errors='replace')
	if result.returncode:
		raise RuntimeError(f'{command[0]} failed ({result.returncode}): {result.stdout[-1000:]} {result.stderr[-2000:]}')
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=shutil.which('clang') or shutil.which('cc') or 'cc')
	parser.add_argument('--runs', type=int, default=3)
	parser.add_argument('--opt', choices=('0', '1', '2'), default='0')
	parser.add_argument('--generated-c', type=Path, help='Compiler C to profile (default: committed bootstrap)')
	parser.add_argument('--source', type=Path, help='Single Sirius source instead of modular self-compilation')
	parser.add_argument('--type-names', action='store_true',
		help='resolve type IDs from the native modular compiler (self-hosting workload only)')
	args = parser.parse_args()
	if args.runs < 1:
		parser.error('--runs must be positive')
	if args.type_names and (args.source is not None or args.generated_c is not None):
		parser.error('--type-names requires the current self-hosting bootstrap and source tree')
	seed = args.generated_c or ROOT / 'bootstrap/compiler.c'
	injected = instrument(seed.read_text(encoding='utf-8'))
	key = hashlib.sha256()
	key.update((args.cc + args.opt + run([args.cc, '--version']).stdout).encode())
	key.update(injected.encode())
	key.update(repr(link_flags(args.cc)).encode())
	for file in (ROOT / 'runtime/sirius_runtime.c', ROOT / 'runtime/sirius_runtime.h'):
		key.update(file.read_bytes())
	cache = ROOT / '.build' / 'profile_mir_allocations' / key.hexdigest()
	binary = cache / ('compiler.exe' if sys.platform == 'win32' else 'compiler')
	if not binary.is_file():
		cache.mkdir(parents=True, exist_ok=True)
		instrumented = cache / 'profiled_compiler.c'
		instrumented.write_text(injected, encoding='utf-8')
		run([args.cc, '-std=c11', f'-O{args.opt}', '-I', str(ROOT / 'runtime'),
			str(instrumented), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(args.cc),
			'-o', str(binary)])
	stats: list[tuple[dict[str, int], dict[int, dict[str, int]]]] = []
	timings = []
	with tempfile.TemporaryDirectory() as temp:
		for i in range(args.runs):
			output = Path(temp) / f'output_{i}.c'
			command = ([str(binary), str(args.source), str(output)] if args.source else
				[str(binary), '--module-root', str(ROOT / 'src'), 'modular_cli', str(output)])
			start = perf_counter()
			result = run(command)
			timings.append(perf_counter() - start)
			stats.append(parse_report(result.stderr))
			# Generated C may use LF even when the committed bootstrap has CRLF.
			if args.source is None and not same_generated_c(output, seed):
				raise RuntimeError('Instrumented self-hosting output differs from the committed C bootstrap')
	if any(stat != stats[0] for stat in stats[1:]):
		raise RuntimeError('Allocation counts are not deterministic')
	print('Workload:', args.source if args.source else 'modular compiler self-hosting')
	print(f'Native -O{args.opt} instrumented median: {statistics.median(timings):.3f}s ({args.runs} runs; not a speed benchmark)')
	for name in COUNTERS:
		print(f'{name:>16}: {stats[0][0][name]}')
	type_names: dict[int, str] = {}
	if args.type_names:
		cache = BuildCache(args.cc)
		driver = cache.native_driver('mir-allocation-type-map', ROOT / 'tests/mir/allocation_type_map_driver_native.sirius')
		result = run([str(driver), str(ROOT / 'src'), 'modular_cli'])
		for line in result.stdout.splitlines():
			key, signature = line.split('\t', 1)
			type_names[int(key)] = signature
	print('Top generated MIR types (initial bytes include CoW clone allocations):')
	print('  type       allocations       empty      clones    allocated bytes  signature')
	for type_id, entry in sorted(stats[0][1].items(), key=lambda item: (-item[1]['new'], item[0]))[:15]:
		signature = type_names.get(type_id, '')
		if len(signature) > 100:
			signature = signature[:97] + '...'
		print(f"  {type_id:>4} {entry['new']:>16} {entry['empty']:>11} {entry['clones']:>11} "
			f"{entry['bytes']:>18}  {signature}")


if __name__ == '__main__':
	main()
