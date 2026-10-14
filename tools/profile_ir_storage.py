#!/usr/bin/env python3
"""Count generated MIR record/array allocations while compiling Sirius sources.

Instruments a disposable copy of the C bootstrap; no production code changes.
Counts initial allocations and copy-on-write clones by generated type ID.
Does not count realloc, retained allocations, or peak memory.
Type IDs are specific to the bootstrap.
"""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from source_manifest import COMPILER_SOURCES, ROOT, combined_sources


COUNTERS = '''
static uint64_t ir_allocs[8192], ir_bytes[8192], ir_zero[8192];
static uint64_t ir_cow[8192], ir_cow_bytes[8192];
static void ir_report(void) {
  fprintf(stderr, "IR_STORAGE id,count,bytes,empty,cow_clones,cow_bytes\\n");
  for (int i = 0; i < 8192; i++) if (ir_allocs[i])
    fprintf(stderr, "IR_STORAGE %d,%llu,%llu,%llu,%llu,%llu\\n", i,
      (unsigned long long)ir_allocs[i], (unsigned long long)ir_bytes[i],
      (unsigned long long)ir_zero[i],
      (unsigned long long)ir_cow[i], (unsigned long long)ir_cow_bytes[i]);
}
'''


def instrument(source: str) -> str:
	needle = 'typedef struct S_MirRecord S_MirRecord;'
	assert source.count(needle) == 1
	source = source.replace(needle, COUNTERS + needle, 1)
	needle = 'S_MirRecord *r = calloc(1, sizeof(S_MirRecord) + fields * sizeof(S_MirField));'
	assert source.count(needle) == 1
	source = source.replace(needle, '''if (id >= 0 && id < 8192) {
    ir_allocs[id]++;
    ir_bytes[id] += sizeof(S_MirRecord) + fields * sizeof(S_MirField);
    ir_zero[id] += fields == 0;
  }
  ''' + needle, 1)
	needle = 'S_MirRecord *copy = s_mir_record_new(record->type_id, record->length);'
	assert source.count(needle) == 1
	source = source.replace(needle, '''if (record->type_id >= 0 && record->type_id < 8192) {
    ir_cow[record->type_id]++;
    ir_cow_bytes[record->type_id] += sizeof(S_MirRecord) + record->length * sizeof(S_MirField);
  }
  ''' + needle, 1)
	needle = 'return (int)s_mir_f0(); }'
	assert source.count(needle) == 1
	source = source.replace(needle, 'int result = (int)s_mir_f0(); ir_report(); return result; }', 1)
	return source


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--cc', default=shutil.which('clang') or 'cc')
	parser.add_argument('--opt', default='0', choices=('0', '1', '2'))
	parser.add_argument('--compiler-c', type=Path, default=ROOT / 'bootstrap/compiler.c')
	parser.add_argument('--source', type=Path)
	args = parser.parse_args()
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		c_file = work / 'instrumented.c'
		c_file.write_text(instrument(args.compiler_c.read_text()))
		binary = work / 'compiler'
		subprocess.run([args.cc, '-std=c11', f'-O{args.opt}', '-I', str(ROOT / 'runtime'),
			str(c_file), str(ROOT / 'runtime/sirius_runtime.c'), '-lm', '-o', str(binary)], check=True)
		source = args.source or work / 'compiler.sirius'
		if args.source is None:
			source.write_bytes(combined_sources(COMPILER_SOURCES, separator=b''))
		output = work / 'out.c'
		result = subprocess.run([str(binary), str(source), str(output)],
			capture_output=True, text=True, check=True)
		if args.source is None and output.read_bytes() != args.compiler_c.read_bytes():
			raise RuntimeError('Instrumented compiler changed its self-hosted C output')
		rows = []
		for line in result.stderr.splitlines():
			if line.startswith('IR_STORAGE ') and 'id,count' not in line:
				idx, count, size, empty, cow, cow_size = (int(s) for s in line.split(' ', 1)[1].split(','))
				rows.append((idx, count, size, empty, cow, cow_size))
		print('type_id,count,bytes,zero_length,cow_clones,cow_bytes')
		for idx, count, size, empty, cow, cow_size in sorted(rows, key=lambda r: -r[2]):
			print(f'{idx},{count},{size},{empty},{cow},{cow_size}')
		print(f'TOTAL,{sum(r[1] for r in rows)},{sum(r[2] for r in rows)},{sum(r[3] for r in rows)},{sum(r[4] for r in rows)},{sum(r[5] for r in rows)}')


if __name__ == '__main__':
	main()
