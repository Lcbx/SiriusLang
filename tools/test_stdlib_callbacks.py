#!/usr/bin/env python3
"""Check callback-powered Std.Functional helpers, generic predicates and packages."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROGRAM = '''import Std.Functional alias F
import Helpers alias H

# An explicit context parameter removes the need for lexical captures.
def apply_box(box, values)
	return F.map_with_context(values, box, fn(value, state) state.run(value))

def main() -> i64
	let values = F.generate(4, fn(i: i64) i + 1)
	let twice: Fn[i64,i64] = H.twice
	let doubled = F.map(values, twice)
	let selected = F.filter(doubled, fn(x: i64) x > 4)
	let total = F.fold(selected, 0, fn(a: i64, b: i64) a + b)
	# Generic functions infer callback input types from concrete collection arguments.
	let inferred = F.map(values, fn(x) x * 2)
	let direct_generic = F.map(values, H.twice)
	let inferred_filter = F.filter(inferred, fn(x) x > 4)
	let inferred_fold = F.fold(["hi", "there"], 0, fn(acc, word) acc + len(word))
	let inferred_generate = F.generate(2, fn(index) str(index))
	if inferred[!3] != 8 or direct_generic[!2] != 6 or len(inferred_filter) != 2
		return 34
	if inferred_fold != 7 or inferred_generate[!0] != "0" or inferred_generate[!1] != "1"
		return 35
	if F.count_if([0, 1, 2], fn(x) x > 0) != 2
		return 36
	if not F.any([0, 1], fn(x) x == 1) or not F.all([1, 2], fn(x) x > 0)
		return 37
	if F.find_index([0, 2], fn(x) x > 0) != 1
		return 38
	if total != 14 or len(selected) != 2
		return 1
	if len(values) != 4 or values[!0] != 1 or values[!3] != 4
		return 2
	if len(F.map(Array[i64](), twice)) != 0
		return 3
	if len(F.generate(-1, twice)) != 0
		return 4
	if F.fold(Array[i64](), 42, fn(a: i64, b: i64) a + b) != 42
		return 5
	let fraction: Fn[i64,f64] = H.fraction
	let scale: Fn[f64,f64] = H.scale
	let floating = F.generate(3, fraction)
	let scaled = F.map(floating, scale)
	let kept = F.filter(scaled, fn(x: f64) x > 2.0)
	if len(kept) != 2 or kept[!0] != 3.0 or kept[!1] != 5.0
		return 6
	if F.fold(kept, 0.0, fn(a: f64, b: f64) a + b) != 8.0
		return 7
	if len(F.map(Array[f64](), scale)) != 0 or len(F.generate(-3, fraction)) != 0
		return 8
	let repeat: Fn[string,string] = H.repeat
	let words = F.map(["a", "be", ""], repeat)
	let filtered = F.filter(words, fn(s: string) len(s) > 1)
	if len(filtered) != 2 or filtered[!0] != "aa" or filtered[!1] != "bebe"
		return 9
	let label: Fn[i64,string] = H.label
	let generated = F.generate(2, label)
	if len(generated) != 2 or generated[!0] != "a" or generated[!1] != "b"
		return 16
	let join: Fn[string,string,string] = H.concat
	if F.fold(generated, "", join) != "ab"
		return 17
	if len(F.generate(-1, label)) != 0 or F.fold(Array[string](), "seed", join) != "seed"
		return 18
	let nonempty: Fn[string,bool] = H.nonempty
	if F.count_if(words, nonempty) != 2 or not F.any(words, nonempty)
		return 10
	if F.all(filtered, nonempty) == false or F.all(Array[string](), nonempty) == false
		return 11
	if F.any(Array[string](), nonempty) or F.count_if(Array[string](), nonempty) != 0
		return 12
	if F.find_index(words, nonempty) != 0 or F.find_index(["", "hi"], nonempty) != 1
		return 13
	if F.find_index([""], nonempty) != -1 or F.find_index(Array[string](), nonempty) != -1
		return 14
	let long: Fn[Array[i64],bool] = fn(xs) len(xs) > 1
	if F.count_if([[1, 2], [3]], long) != 1
		return 15
	# Array element, accumulator and result types specialize independently.
	let sum_lengths: Fn[i64,string,i64] = fn(a, word) a + len(word)
	if F.fold(["hi", "there"], 3, sum_lengths) != 10
		return 19
	if F.fold(Array[string](), 17, sum_lengths) != 17
		return 20
	if F.fold(["a", "b"], 0.5, fn(a: f64, word: string) a + 1.0) != 2.5
		return 21
	if F.fold([1, 2], "", fn(a: string, value: i64) a + str(value)) != "12"
		return 22
	# Indexed mapping is generic in the element and result type, with an
	# inferred i64 index, and returns correctly typed empty arrays.
	let indexed_sums = F.map_indexed([10, 20, 30], fn(index, value) index + value)
	if len(indexed_sums) != 3 or indexed_sums[!0] != 10 or indexed_sums[!2] != 32
		return 57
	let indexed_lengths = F.map_indexed(["a", "xyz"], fn(index, word) {index = index, size = len(word)})
	if indexed_lengths[!1].index != 1 or indexed_lengths[!1].size != 3
		return 58
	let empty_indexed = F.map_indexed(Array[bool](), fn(index, flag) {index = index, value = flag})
	if len(empty_indexed) != 0
		return 59
	let numbered = F.enumerate([4, 8, 12])
	if len(numbered) != 3 or numbered[!0].index != 0 or numbered[!2].value != 12
		return 23
	if numbered[!2].index != 2 or numbered[!1].value != 8
		return 24
	let indexed_strings = F.enumerate(["one", "two"])
	if len(indexed_strings) != 2 or indexed_strings[!1].index != 1 or indexed_strings[!0].value != "one"
		return 25
	let indexed_floats = F.enumerate([1.5, 2.5])
	if len(indexed_floats) != 2 or indexed_floats[!1].index != 1 or indexed_floats[!1].value != 2.5
		return 26
	if len(F.enumerate(Array[i64]())) != 0 or len(F.enumerate(Array[string]())) != 0
		return 27
	# enumerate infers the structural result type for arbitrary element types.
	let booleans = F.enumerate([true, false, true])
	if len(booleans) != 3 or booleans[!1].index != 1 or booleans[!1].value
		return 39
	let nested_values = F.enumerate([[1, 2], [3]])
	if nested_values[!0].index != 0 or nested_values[!1].value[!0] != 3
		return 40
	let records_indexed = F.enumerate([{name = "Ada"}, {name = "Bob"}])
	if records_indexed[!1].index != 1 or records_indexed[!0].value.name != "Ada"
		return 41
	let empty_bool = F.enumerate(Array[bool]())
	let empty_record = F.enumerate(F.generate(0, fn(i) {name = str(i)}))
	if len(empty_bool) != 0 or len(empty_record) != 0
		return 43
	# Legacy named records remain structurally assignable.
	let indexed_legacy: Array[F.IndexedI64] = F.enumerate([8, 9])
	if indexed_legacy[!1].value != 9 or indexed_legacy[!1].index != 1
		return 44
	# Generic array results need not have the input element type.
	let lengths = F.map(["hi", "there"], fn(word: string) len(word))
	if lengths[!0] != 2 or lengths[!1] != 5
		return 28
	let flags = F.generate(3, fn(i: i64) i == 1)
	if len(flags) != 3 or flags[!0] or not flags[!1]
		return 29
	let chosen = F.filter(flags, fn(flag: bool) flag)
	if len(chosen) != 1 or not chosen[!0]
		return 30
	let records = F.map([1, 2], fn(i: i64) {index = i, value = str(i)})
	if records[!0].index != 1 or records[!1].value != "2"
		return 31
	if len(F.generate(0, fn(i: i64) {index = i, value = str(i)})) != 0
		return 32
	let nested = F.map([[1, 2], [3]], fn(xs: Array[i64]) len(xs))
	if nested[!0] != 2 or nested[!1] != 1
		return 33
	# scan returns the seed plus every successive accumulator value.
	let prefixes = F.scan([1, 2, 3], 0, fn(acc, x) acc + x)
	if len(prefixes) != 4 or prefixes[!0] != 0 or prefixes[!1] != 1 or prefixes[!2] != 3 or prefixes[!3] != 6
		return 52
	let words_so_far = F.scan([1, 2, 3], "", fn(acc, x) acc + str(x))
	if words_so_far[!0] != "" or words_so_far[!2] != "12" or words_so_far[!3] != "123"
		return 53
	let record_states = F.scan(["a", "bb"], {count = 0}, fn(acc, word) {count = acc.count + len(word)})
	if len(record_states) != 3 or record_states[!0].count != 0 or record_states[!1].count != 1 or record_states[!2].count != 3
		return 54
	let empty_scan = F.scan(Array[string](), {count = 7}, fn(acc, s) {count = acc.count + len(s)})
	if len(empty_scan) != 1 or empty_scan[!0].count != 7
		return 55
	# Snapshot semantics: changing an accumulated state cannot mutate prior states.
	let arrays = F.scan([1, 2], [0], fn(acc, x) acc.append(x))
	if len(arrays) != 3 or len(arrays[!0]) != 1 or len(arrays[!1]) != 2 or arrays[!2][!2] != 2
		return 56
	# zip and zip_with are generic in both inputs and their result.
	let pairs = F.zip([1, 2, 3], ["one", "two"])
	if len(pairs) != 2 or pairs[!0].left != 1 or pairs[!1].right != "two"
		return 45
	let sums = F.zip_with([1, 2, 3], [10, 20], fn(a, b) a + b)
	if len(sums) != 2 or sums[!0] != 11 or sums[!1] != 22
		return 46
	let zipped_records = F.zip_with(["one", "two"], [1, 2], fn(s, n) {name = s, count = n})
	if zipped_records[!0].name != "one" or zipped_records[!1].count != 2
		return 47
	let nested_pairs = F.zip([[1, 2]], [true, false])
	if len(nested_pairs) != 1 or nested_pairs[!0].left[!1] != 2 or not nested_pairs[!0].right
		return 48
	let composed = F.map(F.zip([1, 2], [10, 20]), fn(pair) pair.left + pair.right)
	if composed[!1] != 22
		return 49
	if len(F.zip(Array[i64](), ["unused"])) != 0 or len(F.zip([1], Array[string]())) != 0
		return 50
	let empty_pairs = F.zip(Array[i64](), Array[string]())
	let empty_mapped = F.zip_with(Array[i64](), Array[string](), fn(a, b) str(a) + b)
	if len(empty_pairs) != 0 or len(empty_mapped) != 0
		return 51
	# One-pass partition preserves element type, order and CoW snapshots.
	let source = [1, 2, 3, 4, 5]
	let grouped = F.partition(source, fn(value) value % 2 == 0)
	if len(grouped.matched) != 2 or grouped.matched[!0] != 2 or grouped.matched[!1] != 4
		return 60
	if len(grouped.unmatched) != 3 or grouped.unmatched[!0] != 1 or grouped.unmatched[!2] != 5
		return 61
	if len(source) != 5 or source[!1] != 2
		return 62
	let word_groups = F.partition(["a", "bb", "ccc"], fn(word) len(word) > 1)
	if len(word_groups.matched) != 2 or word_groups.matched[!1] != "ccc" or word_groups.unmatched[!0] != "a"
		return 63
	let record_groups = F.partition([{x = 1}, {x = 3}, {x = 2}], fn(pair) pair.x > 1)
	if record_groups.matched[!0].x != 3 or record_groups.unmatched[!0].x != 1
		return 64
	let nested_groups = F.partition([[1, 2], [3], [4, 5]], fn(xs) len(xs) == 2)
	if len(nested_groups.matched) != 2 or nested_groups.matched[!1][!1] != 5 or nested_groups.unmatched[!0][!0] != 3
		return 65
	let none = F.partition(Array[string](), fn(word) len(word) > 0)
	if len(none.matched) != 0 or len(none.unmatched) != 0
		return 66
	let all_match = F.partition([true, true], fn(x) x)
	if len(all_match.matched) != 2 or len(all_match.unmatched) != 0
		return 67
	# Explicit state lets a noncapturing callback use a runtime value.
	let offset = 7
	let shifted = F.map_with_context([1, 2, 3], offset, fn(x, base) base + x)
	if shifted[!0] != 8 or shifted[!2] != 10
		return 68
	let box = {run = twice, name = "managed"}
	let applied = apply_box(box, [2, 3])
	if applied[!0] != 4 or applied[!1] != 6 or box.name != "managed"
		return 69
	let empty = F.map_with_context(Array[i64](), box, fn(x, state) state.name + str(x))
	if len(empty) != 0
		return 70
	let context = {offset = 3, payload = [10, 20]}
	let snapshot = context
	let with_managed = F.map_with_context([1, 2], context, fn(x, env) x + env.offset + env.payload[!0])
	if with_managed[!0] != 14 or with_managed[!1] != 15
		return 71
	if snapshot.payload[!1] != 20 or context.payload[!0] != 10
		return 72
	return 42
'''


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument("--runner", type=Path, help="existing compatible MIR runner executable")
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-stdlib-callbacks-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Functional.sirius', root / 'Std/Functional.sirius')
		(root / 'Helpers.sirius').write_text(
			'export def twice(x) -> i64\n\treturn 2 * x\n'
			'export def fraction(index) -> f64\n'
			'\tif index == 0\n\t\treturn 0.5\n'
			'\tif index == 1\n\t\treturn 1.5\n'
			'\treturn 2.5\n'
			'export def scale(x) -> f64\n\treturn 2.0 * x\n'
			'export def repeat(text) -> string\n\treturn text + text\n'
			'export def nonempty(text) -> bool\n\treturn len(text) > 0\n'
			'export def label(index) -> string\n'
			'\tif index == 0\n\t\treturn "a"\n'
			'\treturn "b"\n'
			'export def concat(a, b) -> string\n\treturn a + b\n')
		(root / 'App.sirius').write_text(PROGRAM)
		# Generic generation supports boolean results, but rejects non-function callbacks.
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let items = F.generate(2, 2)
	return 0
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"empty_array_like expects an array or a function returning a value" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let items = F.map([1, 2], fn(s: string) len(s))
	return len(items)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def unused(callback)
	return 0

def main() -> i64
	return unused(fn(x) x)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"underdetermined callback parameters" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''def conflict(callback)
	let left = callback(1)
	let right = callback("x")
	return 0

def main() -> i64
	return conflict(fn(x) x)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"conflicting generic callback parameter types" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.zip_with([1], ["a"], fn(a: string, b: string) a + b)
	return len(values)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.map_indexed([1, 2], fn(index: string, value: i64) len(index) + value)
	return len(values)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.partition([1, 2], fn(value: string) len(value) > 0)
	return len(values.matched)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.map_with_context([1, 2], 4, fn(x: i64, state: string) state)
	return len(values)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.partition([1, 2], fn(value: i64) value + 1)
	return len(values.matched)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"branch condition must be bool" in invalid.stderr, invalid
		(root / 'Invalid.sirius').write_text('''import Std.Functional alias F

def main() -> i64
	let values = F.scan([1, 2], 0, fn(acc: string, x: i64) acc)
	return len(values)
''')
		invalid = run(cache.seed(), '--module-root', root, 'Invalid', root / 'Invalid.c')
		assert invalid.returncode != 0 and b"lambda parameter type does not match inferred callback type" in invalid.stderr, invalid
		generated = root / 'App.c'
		compiled = run(cache.seed(), '--module-root', root, 'App', generated)
		assert compiled.returncode == 0, compiled
		binary = compile_native(cache, generated)
		native = run(binary)
		assert (native.returncode, native.stdout, native.stderr) == (42, b'', b''), native
		audited = root / 'audited'
		audit_build = run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', audited)
		assert audit_build.returncode == 0, audit_build
		audit_run = run(audited)
		assert (audit_run.returncode, audit_run.stdout, audit_run.stderr) == (42, b'', b''), audit_run
		runner = args.runner.resolve() if args.runner else cache.mir_runner()
		library = root / 'App.smirl'
		built = run(runner, '--build-modules', root, 'App', library)
		assert built.returncode == 0, built
		interpreted = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
		assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (42, b'', b''), interpreted
		# Package generic callback providers without retaining their source.
		helper = root / 'Helpers.sirius'
		packaged = run(runner, '--emit-generic', 'Helpers', helper.with_suffix('.smirg'), helper, root)
		assert packaged.returncode == 0, packaged
		helper.unlink()
		linked = root / 'packaged-provider.smirl'
		built = run(runner, '--build-modules', root, 'App', linked)
		assert built.returncode == 0, built
		interpreted = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', linked)
		assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (42, b'', b''), interpreted
		generated = root / 'packaged-provider.c'
		emitted = run(runner, '--emit-linked-c', linked, 'M_3_App_4_main', generated)
		assert emitted.returncode == 0, emitted
		native = run(compile_native(cache, generated))
		assert (native.returncode, native.stdout, native.stderr) == (42, b'', b''), native

		# .smirg stores generic definitions, not compiled concrete functions.
		# Inferred-result generic folds and predicates work source-free.
		stdlib = root / 'Std/Functional.sirius'
		packaged = run(runner, '--emit-generic', 'Std.Functional', stdlib.with_suffix('.smirg'), stdlib, root)
		assert packaged.returncode == 0, packaged
		stdlib.unlink()
		(root / 'Predicates.sirius').write_text('''import Std.Functional alias F
import Helpers alias H

def main() -> i64
	let nonempty: Fn[string,bool] = H.nonempty
	if F.count_if(["", "x"], nonempty) != 1
		return 1
	if not F.any(["", "x"], nonempty) or not F.all(["x"], nonempty)
		return 2
	if F.find_index(["", "x"], nonempty) != 1
		return 3
	let sum_lengths: Fn[i64,string,i64] = fn(a, word) a + len(word)
	if F.fold(["a", "bb"], 0, sum_lengths) != 3
		return 4
	let lengths = F.map(["a", "bb"], fn(s) len(s))
	if len(lengths) != 2 or lengths[!0] != 1 or lengths[!1] != 2
		return 5
	if len(F.map(Array[string](), fn(s) len(s))) != 0
		return 6
	if len(F.generate(0, fn(i) i == 0)) != 0
		return 7
	if F.count_if([1, 2], fn(x) x > 1) != 1
		return 8
	if not F.any([0, 2], fn(x) x == 2) or not F.all([1, 2], fn(x) x > 0)
		return 9
	let indexed_pairs = F.map_indexed(["hi", "world"], fn(i, word) {index = i, size = len(word)})
	if indexed_pairs[!0].index != 0 or indexed_pairs[!1].size != 5
		return 16
	if len(F.map_indexed(Array[bool](), fn(i, flag) {index = i, value = flag})) != 0
		return 17
	let indexed = F.enumerate([true, false])
	if indexed[!1].index != 1 or indexed[!1].value
		return 10
	let nested = F.enumerate([[1, 2]])
	if nested[!0].value[!1] != 2 or len(F.enumerate(Array[bool]())) != 0
		return 11
	let zipped = F.zip(["left", "right"], [1, 2, 3])
	let sums = F.zip_with([1, 2], [10, 20, 30], fn(a, b) a + b)
	if len(zipped) != 2 or zipped[!1].left != "right" or zipped[!0].right != 1
		return 12
	if len(sums) != 2 or sums[!1] != 22 or len(F.zip(Array[i64](), Array[string]())) != 0
		return 13
	let prefixes = F.scan([1, 3, 2], 0, fn(acc, n) acc + n)
	if len(prefixes) != 4 or prefixes[!0] != 0 or prefixes[!3] != 6
		return 14
	let empty_scan = F.scan(Array[string](), "seed", fn(acc, word) acc + word)
	if len(empty_scan) != 1 or empty_scan[!0] != "seed"
		return 15
	let split = F.partition(["one", "", "two"], fn(word) len(word) > 0)
	if len(split.matched) != 2 or split.matched[!1] != "two" or split.unmatched[!0] != ""
		return 18
	let empty_split = F.partition(Array[i64](), fn(value) value > 0)
	if len(empty_split.matched) != 0 or len(empty_split.unmatched) != 0
		return 19
	let ctx = {step = 3, names = ["a", "b"]}
	let contextual = F.map_with_context([1, 2], ctx, fn(x, env) x + env.step + len(env.names))
	if contextual[!0] != 6 or contextual[!1] != 7
		return 20
	if len(F.map_with_context(Array[i64](), ctx, fn(x, env) env.names[!0] + str(x))) != 0
		return 21
	return 42
''')
		linked = root / 'packaged-stdlib.smirl'
		built = run(runner, '--build-modules', root, 'Predicates', linked)
		assert built.returncode == 0, built
		interpreted = run(runner, '--entry', 'M_10_Predicates_4_main', '--load-lib', linked)
		assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (42, b'', b''), interpreted
		generated = root / 'packaged-stdlib.c'
		emitted = run(runner, '--emit-linked-c', linked, 'M_10_Predicates_4_main', generated)
		assert emitted.returncode == 0, emitted
		native = run(compile_native(cache, generated))
		assert (native.returncode, native.stdout, native.stderr) == (42, b'', b''), native

	print('PASS Std.Functional: generic array callbacks, zip/zip_with, scan, partition, folds, enumerations, predicates, native/MIR, source-free packages and empty arrays')


if __name__ == '__main__':
	main()
