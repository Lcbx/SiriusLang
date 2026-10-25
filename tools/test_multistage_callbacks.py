#!/usr/bin/env python3
"""Check callback inference when one callback result types another callback."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


APP = '''import Std.Functional alias F
import Provider alias P
import Reexport alias R

def stringify(x)
	return str(x)

def size(s)
	return len(s)

def pipeline(consumer, transform, values) -> i64
	var total = 0
	for value in values
		total += consumer(transform(value))
	return total

def triple(final, middle, first, values) -> i64
	var total = 0
	for value in values
		total += final(middle(first(value)))
	return total

def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

def relay(callback, values) -> i64
	return consume(callback, values)

def relay_twice(callback, values) -> i64
	return relay(callback, values)

def branch_relay(callback, values, enabled: bool) -> i64
	if enabled
		return relay(callback, values)
	return 0

def main() -> i64
	let forwarded = relay(fn(x) x * 2, [7, 14])
	let twice = relay_twice(fn(x) x * 2, [7, 14])
	let branched = branch_relay(fn(x) x * 2, [7, 14], true)
	let packaged = P.relay(fn(x) x * 2, [7, 14])
	let direct = pipeline(fn(s) len(s), fn(x) str(x), [7, 12, 100])
	let named = pipeline(size, stringify, [7, 12, 100])
	let chained = triple(fn(n) n * 2, fn(s) len(s), fn(x) str(x), [7, 12, 100])
	let imported = P.pipeline(fn(s) len(s), fn(x) str(x), [7, 12, 100])
	let reexported = R.relay(fn(s) len(s), fn(x) str(x), [7, 12, 100])
	let nested = pipeline(fn(s) len(s), fn(x) str(x), F.map([7, 12, 100], fn(x) x + 1))
	if forwarded != 42 or twice != 42 or branched != 42 or packaged != 42 or direct != 6 or named != 6 or chained != 12 or imported != 6 or reexported != 6 or nested != 6
		return 1
	return 42
'''

PROVIDER = '''export def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

export def relay(callback, values) -> i64
	return consume(callback, values)

export def pipeline(consumer, transform, values) -> i64
	var total = 0
	for value in values
		total += consumer(transform(value))
	return total
'''

REEXPORT = '''import Provider alias P
export P.pipeline alias relay
'''

NEGATIVE_CASES = (
	('''def pipeline(consumer, transform, values) -> i64
	var total = 0
	for value in values
		total += consumer(transform(value))
	return total

def main() -> i64
	return pipeline(fn(s) s + 1, fn(x) str(x), [7])
''', b'binary operand types differ'),
	('''def inconsistent(consumer, transform, values) -> i64
	var total = 0
	for value in values
		total += consumer(value)
		total += consumer(transform(value))
	return total

def main() -> i64
	return inconsistent(fn(x) x, fn(x) str(x), [7])
''', b'ambiguous generic callback parameter types'),
	('''def left(callback, values) -> i64
	return right(callback, values)

def right(callback, values) -> i64
	return left(callback, values)

def main() -> i64
	return left(fn(x) x * 2, [7])
''', b'lambda parameter requires annotation or expected Fn type'),
	('''def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total

def conflicting(callback, left, right) -> i64
	return consume(callback, left) + consume(callback, right)

def main() -> i64
	return conflicting(fn(x) x, [1], ["a"])
''', b'ambiguous generic callback parameter types'),
	('''def unused(first, second) -> i64
	return 0

def main() -> i64
	return unused(fn(x) x, fn(y) y)
''', b'lambda parameter requires annotation or expected Fn type'),
)


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def check_success(result: subprocess.CompletedProcess[bytes], code: int = 0) -> None:
	assert result.returncode == code and result.stderr == b'', result
	if code != 0:
		assert result.stdout == b'', result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true', help='skip slow MIR runner build and package checks')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-multistage-') as tmp:
		root = Path(tmp)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Functional.sirius', root / 'Std/Functional.sirius')
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'Reexport.sirius').write_text(REEXPORT)
		(root / 'App.sirius').write_text(APP)
		for source, diagnostic in NEGATIVE_CASES:
			(root / 'Negative.sirius').write_text(source)
			invalid = run(cache.seed(), '--module-root', root, 'Negative', root / 'Negative.c')
			assert invalid.returncode != 0 and (not diagnostic or diagnostic in invalid.stderr), invalid

		generated = root / 'App.c'
		check_success(run(cache.seed(), '--module-root', root, 'App', generated))
		check_success(run(compile_native(cache, generated)), 42)
		binary = root / 'audited'
		check_success(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', binary))
		check_success(run(binary), 42)

		if args.native_only:
			print('PASS multi-stage callbacks: native and ARC (MIR not checked)')
			return
		runner = cache.native_tool('run_cli')
		library = root / 'App.smirl'
		check_success(run(runner, '--build-modules', root, 'App', library))
		check_success(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
		# Pure re-export facades contain no generic declarations to package.
		for module, source in (('Provider', root / 'Provider.sirius'), ('Std.Functional', root / 'Std/Functional.sirius')):
			check_success(run(runner, '--emit-generic', module, source.with_suffix('.smirg'), source, root))
			source.unlink()
		library = root / 'packaged.smirl'
		check_success(run(runner, '--build-modules', root, 'App', library))
		check_success(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library), 42)
		generated = root / 'packaged.c'
		check_success(run(runner, '--emit-linked-c', library, 'M_3_App_4_main', generated))
		check_success(run(compile_native(cache, generated)), 42)

	print('PASS multi-stage callbacks: dependent results, chains, imports, packages, MIR and ARC')


if __name__ == '__main__':
	main()
