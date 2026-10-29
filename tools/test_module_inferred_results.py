#!/usr/bin/env python3
"""Check specialization-dependent exported results through imports and packages."""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def checked(*args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(*args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def main() -> None:
	cache = BuildCache(require_cc())
	runner = cache.mir_runner()
	with tempfile.TemporaryDirectory(prefix='sirius-inferred-result-') as tmp:
		root = Path(tmp)
		provider = root / 'Ops.sirius'
		provider.write_text('''def add_two(value: i64) -> i64
	return value + 2

export def identity(value)
	return value

export def wrap(value)
	return {item = value}

export def wrap_many(value)
	return [wrap(value), wrap(value)]

export def fold(values, initial, callback)
	var total = initial
	for value in values
		total = callback(total, value)
	return total

export def private_helper(value)
	return add_two(value)
''', encoding='utf-8')
		(root / 'Facade.sirius').write_text('''import Ops alias O
export O.identity alias copied
export O.fold alias reduce
export O.private_helper alias helper
''', encoding='utf-8')
		(root / 'App.sirius').write_text('''import Ops alias O
import Facade alias F

def add_two(value: i64) -> i64
	return 100

def lengths(current: i64, word: string) -> i64
	return current + len(word)

def main() -> i64
	let initial = F.copied([20, 22])
	let score: Fn[i64,string,i64] = lengths
	let total = F.reduce(["hi", "there"], 3, score)
	let records = O.wrap_many(21)
	let text = O.wrap("hello")
	if total != 10 or text.item != "hello" or len(records) != 2
		return 1
	if records[!0].item != 21 or F.helper(40) != 42
		return 2
	return initial[!0] + initial[!1]
''', encoding='utf-8')
		def exercise(label: str) -> None:
			generated = root / f'{label}.c'
			checked(cache.seed(), '--module-root', root, 'App', generated)
			assert run(compile_native(cache, generated)).returncode == 42
			audited = root / f'{label}-audited'
			checked(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
				'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
				*link_flags(cache.cc), '-o', audited)
			assert run(audited).returncode == 42
			library = root / f'{label}.smirl'
			checked(runner, '--build-modules', root, 'App', library)
			assert run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library).returncode == 42

		exercise('source')
		packaged = root / 'Ops.smirg'
		checked(runner, '--emit-generic', 'Ops', packaged, provider, root)
		provider.unlink()
		exercise('packaged')

		(root / 'Invalid.sirius').write_text('''export def inconsistent(value)
	if value > 0
		return [value]
	return ["wrong"]
''', encoding='utf-8')
		(root / 'BadApp.sirius').write_text('''import Invalid alias I

def main() -> i64
	let value = I.inconsistent(1)
	return len(value)
''', encoding='utf-8')
		failure = run(cache.seed(), '--module-root', root, 'BadApp', root / 'bad.c')
		assert failure.returncode != 0 and b'inconsistent return types' in failure.stderr, failure

		(root / 'Invalid.sirius').write_text('''export def recurse(value)
	return recurse(value)
''', encoding='utf-8')
		(root / 'BadApp.sirius').write_text('''import Invalid alias I

def main() -> i64
	return I.recurse(1)
''', encoding='utf-8')
		failure = run(cache.seed(), '--module-root', root, 'BadApp', root / 'bad.c')
		assert failure.returncode != 0, failure

	print('PASS exported inferred results: arrays, records, heterogeneous folds, imports, re-exports, .smirg, ARC and diagnostics')


if __name__ == '__main__':
	main()
