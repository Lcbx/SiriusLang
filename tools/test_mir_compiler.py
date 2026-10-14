#!/usr/bin/env python
"""Run the shared compiler fixtures through the default MIR backend."""
import argparse
from pathlib import Path
import tempfile
import subprocess

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
		# Module constants are typed; boolean names, negation, and shadowing
		# must survive HIR analysis, MIR lowering, and native C emission.
		bool_source = work / 'module_bool_constants.sirius'
		bool_source.write_text(
			'let ON: bool = true\n'
			'let OFF = not ON\n'
			'let ALWAYS: bool = not OFF\n'
			'def enabled() -> bool\n'
			'\treturn ALWAYS and not OFF\n'
			'def main() -> i64\n'
			'\tif not enabled() or OFF\n'
			'\t\treturn 1\n'
			'\tlet ON = false\n'
			'\tif ON\n'
			'\t\treturn 2\n'
			'\treturn 0\n')
		bool_c = work / 'module_bool_constants.c'
		execute([str(compiler), str(bool_source), str(bool_c)])
		assert_native(run_native(cache, bool_c))
		for index, initializer in enumerate(('let B: i64 = true', 'let B: bool = 1',
			'let B = true + 1', 'let B = not 3', 'let B = 1 * true')):
			negative = work / f'module_bool_invalid_{index}.sirius'
			negative.write_text(initializer + '\ndef main() -> i64\n\treturn 0\n')
			generated = work / f'module_bool_invalid_{index}.c'
			result = subprocess.run([str(compiler), str(negative), str(generated)], capture_output=True, text=True)
			if result.returncode != 1 or not result.stdout or generated.exists():
				raise AssertionError(f'Expected invalid constant rejection: {initializer}: {result.stdout!r}')
		print('PASS typed boolean module constants, compile-time not, and invalid constant rejection')
		# Structural module constants use the same canonical layout and CoW rules
		# as runtime records, including aliases and local shadowing.
		record_source = Path(__file__).resolve().parent.parent / 'tests/mir/module_record_constants.sirius'
		record_expected = Path(__file__).resolve().parent.parent / 'tests/mir/module_record_constants.txt'
		record_c = work / 'module_record_constants.c'
		execute([str(compiler), str(record_source), str(record_c)])
		assert_native(run_native(cache, record_c), stdout=record_expected.read_bytes())
		for index, initializer in enumerate((
			'let BAD = {}',
			'let BAD = {x = missing}',
			'let BAD = {x = 1}.missing',
			'let BAD: bool = {x = true}',
			'let BAD = {x = arguments()}',
			'let BAD = {x = [1, true]}',
			'let BAD = {x = 1, y = x}',
		)):
			negative = work / f'module_record_invalid_{index}.sirius'
			negative.write_text(initializer + '\ndef main() -> i64\n\treturn 0\n')
			generated = work / f'module_record_invalid_{index}.c'
			result = subprocess.run([str(compiler), str(negative), str(generated)], capture_output=True, text=True)
			if (result.returncode != 1 or generated.exists() or
				not any(error in result.stdout for error in (
					'unsupported module constant initializer', 'module constant type mismatch'))):
				raise AssertionError(f'Expected record constant rejection: {initializer}: {result.stdout!r}')
		print('PASS nested module record constants, field selections, CoW, and invalid initializers')
		# Composite constants are materialized with the usual MIR ownership rules.
		composite_source = Path(__file__).resolve().parent.parent / 'tests/mir/module_composite_constants.sirius'
		composite_expected = Path(__file__).resolve().parent.parent / 'tests/mir/module_composite_constants.txt'
		composite_c = work / 'module_composite_constants.c'
		execute([str(compiler), str(composite_source), str(composite_c)])
		assert_native(run_native(cache, composite_c), stdout=composite_expected.read_bytes())
		for index, initializer in enumerate((
			'let BAD = [1, true]',
			'let BAD = [1, 2][2]',
			'let BAD = [1, 2][-1]',
			'let BAD = len(true)',
			'let BAD = "a" + 1',
			'let BAD: i64 = "text"',
			'let BAD = {X.*}',
			'let X = [1]\nlet BAD = {X.*}',
			'let BAD = {x = 1, del y}',
			'let BAD = "\\q"',
			'let BAD = [arguments()]',
		)):
			negative = work / f'module_composite_invalid_{index}.sirius'
			negative.write_text(initializer + '\ndef main() -> i64\n\treturn 0\n')
			generated = work / f'module_composite_invalid_{index}.c'
			result = subprocess.run([str(compiler), str(negative), str(generated)], capture_output=True, text=True)
			if (result.returncode != 1 or generated.exists() or
				not any(error in result.stdout for error in (
					'unsupported module constant initializer', 'module constant type mismatch'))):
				raise AssertionError(f'Expected composite constant rejection: {initializer}: {result.stdout!r}')
		print('PASS compile-time strings/arrays/spreads, len, CoW, and invalid constant rejection')
		# Plain indexing is permitted only with an established static bound.
		static_case = work / 'static_literal_index.sirius'
		static_case.write_text(
			'def main() -> i64\n'
			'\tlet a = [10, 20, 30][1]\n'
			'\tlet b = [[1, 2], [3, 4]][0][1]\n'
			'\tprint(a + b)\n'
			'\treturn 0\n')
		static_c = work / 'static_literal_index.c'
		execute([str(compiler), str(static_case), str(static_c)])
		assert_native(run_native(cache, static_c), stdout=b'22\n')
		for suffix, expression in (('out_of_range', '[1, 2][2]'),
			('unproven', 'items[0]'), ('negative', '[1, 2][-1]')):
			negative = work / (suffix + '.sirius')
			negative.write_text('def main() -> i64\n\tlet items = [1, 2]\n\tprint(' + expression + ')\n\treturn 0\n')
			failed = work / (suffix + '.c')
			result = subprocess.run([str(compiler), str(negative), str(failed)], capture_output=True, text=True)
			if result.returncode != 1 or 'not statically proven in bounds' not in result.stdout or failed.exists():
				raise AssertionError(f'Expected static bounds rejection for {expression}: {result.stdout!r}')
		print('PASS static literal bounds proof and rejection of unproven plain indices')
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
		record_key = work / 'record_key.sirius'
		record_key.write_text(
			'rec Leaf(n: i64, label: string)\n'
			'rec Pair(left: Leaf, right: Leaf)\n'
			'def main() -> i64\n'
			'\tvar key = Pair(Leaf(2, "hi"), Leaf(3, "there"))\n'
			'\tvar table = Dict[Pair, string]()\n'
			'\ttable = table.insert(key, "old")\n'
			'\tlet snapshot = table\n'
			'\tvar numbers = Dict[Pair, i64]()\n'
			'\tnumbers = numbers.insert(Pair(Leaf(2, \"hi\"), Leaf(3, \"there\")), 6)\n'
			'\ttable = table.insert(Pair(Leaf(2, "hi"), Leaf(3, "there")), "new")\n'
			'\tkey = Pair(Leaf(9, "other"), Leaf(3, "there"))\n'
			'\tif (numbers.get(Pair(Leaf(2, \"hi\"), Leaf(3, \"there\"))) or 0) != 6\n'
			'\t\treturn 4\n'
			'\tif len(table) != 1 or (snapshot.get(Pair(Leaf(2, "hi"), Leaf(3, "there"))) or "") != "old"\n'
			'\t\treturn 1\n'
			'\tif (table.get(Pair(Leaf(2, "hi"), Leaf(3, "there"))) or "") != "new"\n'
			'\t\treturn 2\n'
			'\tif (table.get(key) or "") != ""\n'
			'\t\treturn 3\n'
			'\treturn 0\n')
		output = work / 'record_key.c'
		execute([str(compiler), str(record_key), str(output)])
		assert_native(run_native(cache, output))
		print('PASS nested structural record keys, equal independently constructed keys, and CoW')
		f64_keys = work / 'f64_keys.sirius'
		f64_keys.write_text(
			'def main() -> i64\n'
			'\tvar table = Dict[f64, i64]()\n'
			'\ttable = table.insert(0.0, 1)\n'
			'\ttable = table.insert(-0.0, 2)\n'
			'\tif len(table) == 1 and (table.get(0.0) or 0) == 2\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		output = work / 'f64_keys.c'
		execute([str(compiler), str(f64_keys), str(output)])
		assert_native(run_native(cache, output))
		print('PASS f64 key hashing normalizes signed zero')
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
		if 'typedef union { int64_t bits; double real; S_String *text; S_MirRecord *record; S_Dict *dict; } S_MirField;' not in output.read_text(encoding='utf-8'):
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
		if generated.count('static inline S_MirRecord *s_mir_record_unique_owned(') != 1 or generated.count('r = s_mir_record_unique_owned(r);') != 1 or 's_dict_insert(' not in generated:
			raise AssertionError('Managed collections must share the type-directed CoW clone helper')
		assert_native(run_native(cache, collection_c))
		print('PASS shared CoW clone: managed records, arrays, dictionaries, and nested snapshots')
		field_append = work / 'field_append.c'
		execute([str(compiler), str(Path(__file__).resolve().parent.parent / 'tests/compiler/fixtures/85_field_append_cow.sirius'), str(field_append)])
		if '].record = s_mir_array_append_owned(updated->fields[' not in field_append.read_text(encoding='utf-8'):
			raise AssertionError('Direct self-field appends must use fused CoW mutation')
		print('PASS fused record-array append is selected during MIR lowering')
		optional_fields = work / 'optional_fields.sirius'
		optional_fields.write_text(
			'rec Item(name: string, count: i64)\n'
			'def main() -> i64\n'
			'\tlet items = [Item("first", 1), Item("second", 2)]\n'
			'\tlet found = items[?1] or Item("none", 0)\n'
			'\tlet absent = items[?99] or Item("none", 0)\n'
			'\tvar table = Dict[i64,Item]().insert(5, Item("old", 5))\n'
			'\tlet snapshot = table\n'
			'\ttable = table.insert(5, Item("new", 6))\n'
			'\tlet old = snapshot.get(5) or Item("missing", 0)\n'
			'\tlet updated = table.get(5) or Item("missing", 0)\n'
			'\tlet missing = table.get(20) or Item("missing", 0)\n'
			'\tif found.count == 2 and absent.count == 0 and old.name == "old" and updated.name == "new" and missing.count == 0\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		optional_c = work / 'optional_fields.c'
		execute([str(compiler), str(optional_fields), str(optional_c)])
		optional_source = optional_c.read_text(encoding='utf-8')
		if optional_source.count('static inline S_MirRecord *s_mir_optional_from_field(') != 1 or 's_mir_optional_from_array(' in optional_source or 's_mir_optional_from_dict(' in optional_source:
			raise AssertionError('MIR arrays and dictionaries must share optional field construction')
		assert_native(run_native(cache, optional_c))
		print('PASS shared Optional field helper: arrays, dictionaries, managed records, and CoW')
		for stem, stdout, error in (
			('02_nested_checked_assignment', b'outer\ninner\n', b'array index out of bounds'),
			('03_nested_augmented_division', b'index\nrhs\n', b'division by zero'),
		):
			source = Path(__file__).resolve().parent.parent / 'tests/compiler/runtime_errors' / (stem + '.sirius')
			generated = work / (stem + '.c')
			execute([str(compiler), str(source), str(generated)])
			assert_native(run_native(cache, generated), stdout=stdout, code=1, stderr_contains=error)
			print(f'PASS nested assignment runtime failure: {stem}')


if __name__ == '__main__':
	main()
