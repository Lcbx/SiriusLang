#!/usr/bin/env python3
"""Test generic callback hints from later arguments and nested generic results."""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


APP = '''import Std.Functional alias F
import Provider alias P

def twice(x)
	return x * 2

def sum_with(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

def main() -> i64
	let basic = sum_with(fn(x) x * 2, [1, 2, 3])
	let named = sum_with(twice, [1, 2, 3])
	let nested = sum_with(fn(x) x * 2, F.map([1, 2, 3], fn(y) y + 1))
	let imported = P.sum_with(fn(x) x + 1, [1, 2, 3])
	if basic != 12 or named != 12 or nested != 18 or imported != 9
		return 1
	return 42
'''

PROVIDER = '''export def sum_with(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total
'''

NEGATIVE_CASES = (
	('''def conflict(callback, numbers, words) -> i64
	var total = 0
	for value in numbers
		total += callback(value)
	for word in words
		total += callback(word)
	return total

def main() -> i64
	return conflict(fn(x) x, [1], ["bad"])
''', b'ambiguous generic callback parameter types'),
	('''def ignored(callback, values) -> i64
	return len(values)

def main() -> i64
	return ignored(fn(x) x, [1])
''', b'lambda parameter requires annotation or expected Fn type'),
)


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def check_success(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result
	if code != 0:
		assert result.stdout == b'', result


def main() -> None:
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-callback-order-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Functional.sirius', root / 'Std/Functional.sirius')
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'App.sirius').write_text(APP)
		(root / 'Negative.sirius').write_text('')
		for source, diagnostic in NEGATIVE_CASES:
			(root / 'Negative.sirius').write_text(source)
			invalid = run(cache.seed(), '--module-root', root, 'Negative', root / 'Negative.c')
			assert invalid.returncode != 0 and diagnostic in invalid.stderr, invalid

		generated = root / 'App.c'
		check_success(run(cache.seed(), '--module-root', root, 'App', generated))
		check_success(run(compile_native(cache, generated)), 42)
		binary = root / 'audited'
		check_success(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', binary))
		check_success(run(binary), 42)

		runner = cache.native_tool('run_cli')
		library = root / 'App.smirl'
		check_success(run(runner, '--build-modules', root, 'App', library))
		check_success(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
		# Both the generic provider and its nested generic dependency are source-free.
		for module, source in (('Provider', root / 'Provider.sirius'), ('Std.Functional', root / 'Std/Functional.sirius')):
			check_success(run(runner, '--emit-generic', module, source.with_suffix('.smirg'), source, root))
			source.unlink()
		library = root / 'packaged.smirl'
		check_success(run(runner, '--build-modules', root, 'App', library))
		check_success(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
		generated = root / 'packaged.c'
		check_success(run(runner, '--emit-linked-c', library, 'M_3_App_4_main', generated))
		check_success(run(compile_native(cache, generated)), 42)

	print('PASS generic callbacks: later argument types, nested results, imports, packages, native/MIR and ARC')


if __name__ == '__main__':
	main()
