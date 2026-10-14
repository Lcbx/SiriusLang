#!/usr/bin/env python
"""Run the shared compiler fixtures through the default MIR backend."""
import argparse
from pathlib import Path
import tempfile

from build_support import BuildCache, assert_native, execute, options, require_cc, run_native, validate_jobs
from compiler_cases import check_cases
from source_manifest import MIR_ONLY_COMPILER_SOURCES


# Two legacy negative fixtures require only backend limitations, not
# language errors: MIR supports f64 arrays and Optional fallbacks in elif.
NEGATIVE_EXCLUDE = ('04_unsupported_array', '27_optional_elif')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--compiler', type=Path, help='Use a prebuilt MIR compiler (skip rebuilding the compiler driver)')
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = args.compiler.resolve() if args.compiler else cache.sirius('mir-only-regression', MIR_ONLY_COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		check_cases(cache.cc, compiler, work, jobs=validate_jobs(args.jobs),
			runtime=cache.runtime(), negative_exclude=NEGATIVE_EXCLUDE)
		elif_case = work / 'optional_elif.sirius'
		elif_case.write_text('def main() -> i64\n\tlet table = Dict[i64, i64]()\n\tif false\n\t\tprint(1)\n\telif (table.get(1) or 0) == 0\n\t\tprint(2)\n\treturn 0\n')
		output = work / 'optional_elif.c'
		execute([str(compiler), str(elif_case), str(output)])
		assert_native(run_native(cache, output), stdout=b'2\n')
		print('PASS MIR-only Optional fallback in elif')
		f64_case = work / 'f64_array.sirius'
		f64_case.write_text('def main() -> i64\n\tlet xs = [1.0, 2.0]\n\tif xs[!0] == 1.0 and xs[!1] == 2.0\n\t\treturn 0\n\treturn 1\n')
		output = work / 'f64_array.c'
		execute([str(compiler), str(f64_case), str(output)])
		generated = output.read_text(encoding='utf-8')
		if 'bool init' in generated or 'uninitialized register' in generated:
			raise AssertionError('Verified MIR C must not emit redundant runtime register checks')
		assert_native(run_native(cache, output))
		print('PASS MIR-only f64 arrays and comparisons, no runtime register checks')
		f64_dict = work / 'f64_dict.sirius'
		f64_dict.write_text(
			'def main() -> i64\n'
			'\tvar d = Dict[i64, f64]()\n'
			'\td = d.insert(1, 2.5)\n'
			'\tlet snapshot = d\n'
			'\td = d.insert(1, 3.75)\n'
			'\tif (snapshot.get(1) or 0.0) == 2.5 and (d.get(1) or 0.0) == 3.75\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		output = work / 'f64_dict.c'
		execute([str(compiler), str(f64_dict), str(output)])
		assert_native(run_native(cache, output))
		print('PASS MIR-only f64 dictionary values and CoW')
		mixed_fields = work / 'mixed_fields.sirius'
		mixed_fields.write_text(
			'def main() -> i64\n'
			'\tvar item = {count=7, rate=2.5, word="ok", nums=[1, 2]}\n'
			'\tlet snapshot = item\n'
			'\titem = {item.*, count=8, rate=4.5}\n'
			'\tif snapshot.count == 7 and item.count == 8 and snapshot.rate == 2.5 and item.rate == 4.5 and item.word == "ok" and snapshot.nums[!1] == 2\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		output = work / 'mixed_fields.c'
		execute([str(compiler), str(mixed_fields), str(output)])
		if 'typedef union { int64_t bits; double real; S_String *text; S_MirRecord *record; } S_MirField;' not in output.read_text(encoding='utf-8'):
			raise AssertionError('MIR C field storage must use a compact tagged-by-type union')
		assert_native(run_native(cache, output))
		print('PASS mixed numeric/string/array record fields and snapshot CoW')
		record_update = work / 'record_update.sirius'
		record_update.write_text(
			'def main() -> i64\n'
			'\tvar unique = {count=0, name="start"}\n'
			'\twhile unique.count < 1000\n'
			'\t\tunique.count += 1\n'
			'\tlet before = unique\n'
			'\tunique.count += 1\n'
			'\tunique.name = "after"\n'
			'\tunique.name = unique.name\n'
			'\tvar outer = {inner=unique, items=[1, 2]}\n'
			'\tlet saved = outer\n'
			'\touter.inner = before\n'
			'\touter.items = outer.items.append(3)\n'
			'\tif before.count == 1000 and before.name == "start" and unique.count == 1001 and unique.name == "after" and outer.inner.count == 1000 and saved.inner.count == 1001 and len(saved.items) == 2 and len(outer.items) == 3\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		output = work / 'record_update.c'
		execute([str(compiler), str(record_update), str(output)])
		if output.read_text(encoding='utf-8').count('s_mir_record_unique_owned(') < 2:
			raise AssertionError('Record assignments must lower to CoW-aware MIR updates')
		assert_native(run_native(cache, output))
		print('PASS unique/shared record field updates, managed aliases, and nested CoW')
		collection_snapshots = work / 'collection_snapshots.sirius'
		collection_snapshots.write_text(
			'rec Item(name: string, tags: Array[string])\n'
			'def main() -> i64\n'
			'\tvar item = Item("first", ["a"])\n'
			'\tlet original_item = item\n'
			'\titem.name = "second"\n'
			'\tvar items = [item]\n'
			'\tlet original_items = items\n'
			'\titems[!0] = Item("third", ["b"])\n'
			'\titems = items.append(Item("fourth", ["c"]))\n'
			'\tvar entries = Dict[string, Array[string]]()\n'
			'\tentries = entries.insert("key", ["before"])\n'
			'\tlet original_entries = entries\n'
			'\tentries = entries.insert("key", ["after"])\n'
			'\tentries = entries.insert("other", ["extra"])\n'
			'\tif original_item.name == "first" and item.name == "second" and original_items[!0].name == "second" and items[!0].name == "third" and len(items) == 2 and len(original_items) == 1 and (original_entries.get("key") or ["missing"])[!0] == "before" and (entries.get("key") or ["missing"])[!0] == "after"\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		collection_c = work / 'collection_snapshots.c'
		execute([str(compiler), str(collection_snapshots), str(collection_c)])
		generated = collection_c.read_text(encoding='utf-8')
		if generated.count('static inline S_MirRecord *s_mir_record_unique_owned(') != 1 or generated.count('dict = s_mir_record_unique_owned(dict);') != 1:
			raise AssertionError('Managed collections must share the type-directed CoW clone helper')
		assert_native(run_native(cache, collection_c))
		print('PASS shared CoW clone: managed records, arrays, dictionaries, and nested snapshots')
		field_append = work / 'field_append.c'
		execute([str(compiler), str(Path(__file__).resolve().parent.parent / 'tests/compiler/fixtures/85_field_append_cow.sirius'), str(field_append)])
		if '].record = s_mir_array_append_owned(updated->fields[' not in field_append.read_text(encoding='utf-8'):
			raise AssertionError('Direct self-field appends must use fused CoW mutation')
		print('PASS fused record-array append is selected during MIR lowering')


if __name__ == '__main__':
	main()
