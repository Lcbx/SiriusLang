#!/usr/bin/env python3
"""Check callback-powered Std.Functional helpers, generic predicates and packages."""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROGRAM = '''import Std.Functional alias F
import Helpers alias H

def main() -> i64
	let values = F.generate_i64(4, fn(i: i64) i + 1)
	let doubled = F.map_i64(values, H.twice)
	let selected = F.filter_i64(doubled, fn(x: i64) x > 4)
	let total = F.fold_i64(selected, 0, fn(a: i64, b: i64) a + b)
	if total != 14 or len(selected) != 2
		return 1
	if len(values) != 4 or values[!0] != 1 or values[!3] != 4
		return 2
	if len(F.map_i64(Array[i64](), H.twice)) != 0
		return 3
	if len(F.generate_i64(-1, H.twice)) != 0
		return 4
	if F.fold_i64(Array[i64](), 42, fn(a: i64, b: i64) a + b) != 42
		return 5
	let floating = F.generate_f64(3, H.fraction)
	let scaled = F.map_f64(floating, H.scale)
	let kept = F.filter_f64(scaled, fn(x) x > 2.0)
	if len(kept) != 2 or kept[!0] != 3.0 or kept[!1] != 5.0
		return 6
	if F.fold_f64(kept, 0.0, fn(a, b) a + b) != 8.0
		return 7
	if len(F.map_f64(Array[f64](), H.scale)) != 0 or len(F.generate_f64(-3, H.fraction)) != 0
		return 8
	let words = F.map_string(["a", "be", ""], H.repeat)
	let filtered = F.filter_string(words, fn(s) len(s) > 1)
	if len(filtered) != 2 or filtered[!0] != "aa" or filtered[!1] != "bebe"
		return 9
	let generated = F.generate_string(2, H.label)
	if len(generated) != 2 or generated[!0] != "a" or generated[!1] != "b"
		return 16
	if F.fold_string(generated, "", H.concat) != "ab"
		return 17
	if len(F.generate_string(-1, H.label)) != 0 or F.fold_string(Array[string](), "seed", H.concat) != "seed"
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
	return 42
'''


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def main() -> None:
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
		runner = cache.native_tool('run_cli')
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
		# Generic predicates in Std.Functional can be used source-free as well.
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

	print('PASS Std.Functional: i64/f64/string callbacks, polymorphic predicates, native/MIR, source-free packages and empty arrays')


if __name__ == '__main__':
	main()
