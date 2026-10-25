#!/usr/bin/env python3
"""Check contextual generic Fn specialization through HIR, imports and packages."""
import argparse
from pathlib import Path
import subprocess
import tempfile
from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


LOCAL = '''def double(x)
	return x * 2

def add(x)
	return x + 1

def identity(x)
	return x

def apply(f: Fn[i64,i64], x: i64) -> i64
	return f(x)

def choose() -> Fn[i64,i64]
	return double

def main() -> i64
	let callback: Fn[i64,i64] = double
	if apply(callback, 21) != 42
		return 1
	if apply(double, 21) != 42
		return 2
	var current: Fn[i64,i64] = add
	current = double
	if current(21) != 42
		return 3
	let numeric: Fn[i64,i64] = identity
	let textual: Fn[string,string] = identity
	if numeric(42) != 42 or textual("hello") != "hello"
		return 6
	let callbacks: Array[Fn[i64,i64]] = [double, add]
	let selected: Fn[i64,i64] = callbacks[!0]
	if selected(21) != 42
		return 4
	let returned = choose()
	if returned(21) != 42
		return 5
	return 42
'''

INVALID = {
	'missing context': ('def double(x)\n\treturn x * 2\ndef main() -> i64\n\tlet f = double\n\treturn 0\n', 'require annotated parameters'),
	'wrong parameter': ('def double(x: string) -> string\n\treturn x + x\ndef main() -> i64\n\tlet f: Fn[i64,string] = double\n\treturn 0\n', 'parameter type does not match'),
	'wrong return': ('def double(x)\n\treturn "hello"\ndef main() -> i64\n\tlet f: Fn[i64,i64] = double\n\treturn 0\n', 'return type does not match'),
	'wrong arity': ('def double(x)\n\treturn x * 2\ndef main() -> i64\n\tlet f: Fn[i64,i64,i64] = double\n\treturn 0\n', 'parameter count does not match'),
	'wrong argument': ('def double(x: string) -> string\n\treturn x + x\ndef use(f: Fn[i64,i64]) -> i64\n\treturn f(1)\ndef main() -> i64\n\treturn use(double)\n', 'parameter type does not match'),
}


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(x) for x in args], capture_output=True, timeout=100)


def checked(result: subprocess.CompletedProcess[bytes], code: int, label: str) -> None:
	assert (result.returncode, result.stdout, result.stderr) == (code, b'', b''), (label, result)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		work = Path(directory)
		source = work / 'local.sirius'
		source.write_text(LOCAL)
		checked(run(runner, source), 42, 'local interpreted')
		library = work / 'local.smirl'
		c_file = work / 'local.c'
		checked(run(runner, '--emit-lib', library, '--export', 'main', source), 0, 'local library')
		checked(run(runner, '--emit-linked-c', library, 'main', c_file), 0, 'local C')
		checked(run(compile_native(cache, c_file)), 42, 'local native')
		audited = work / 'arc-audit'
		checked(run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', ROOT / 'runtime', c_file, ROOT / 'runtime/sirius_runtime.c', *link_flags(cache.cc), '-o', audited), 0, 'ARC build')
		checked(run(audited), 42, 'ARC audit')
		for name, (program, diagnostic) in INVALID.items():
			source.write_text(program)
			failure = run(runner, source)
			assert failure.returncode != 0 and diagnostic.encode() in failure.stdout + failure.stderr, (name, failure)

		(work / 'Ops.sirius').write_text('export def double(x) -> i64\n\treturn x * 2\nexport def add(x) -> i64\n\treturn x + 1\n')
		(work / 'Reexport.sirius').write_text('import Ops alias O\nexport O.double alias twice\nexport def relay(x) -> i64\n\treturn O.double(x)\n')
		(work / 'Main.sirius').write_text('import Ops alias O\nimport Reexport alias R\n'
			'def apply(f: Fn[i64,i64], x: i64) -> i64\n\treturn f(x)\n'
			'def choose() -> Fn[i64,i64]\n\treturn R.twice\n'
			'def main() -> i64\n'
			'\tlet f: Fn[i64,i64] = O.double\n'
			'\tif apply(f, 21) != 42\n\t\treturn 1\n'
			'\tif apply(R.twice, 21) != 42\n\t\treturn 2\n'
			'\tlet values: Array[Fn[i64,i64]] = [O.double, R.twice]\n'
			'\tlet selected: Fn[i64,i64] = values[!1]\n'
			'\tif selected(21) != 42 or apply(R.relay, 21) != 42\n\t\treturn 4\n'
			'\tlet chosen = choose()\n'
			'\tif chosen(21) != 42\n\t\treturn 3\n'
			'\treturn 42\n')
		linked = work / 'linked.smirl'
		for source_free in (False, True):
			if source_free:
				package = work / 'Ops.smirg'
				result = run(runner, '--emit-generic', 'Ops', package, work / 'Ops.sirius')
				assert result.returncode == 0, result
				result = run(runner, '--emit-generic', 'Reexport', work / 'Reexport.smirg', work / 'Reexport.sirius', work)
				assert result.returncode == 0, result
				(work / 'Ops.sirius').unlink()
				(work / 'Reexport.sirius').unlink()
			result = run(runner, '--build-modules', work, 'Main', linked)
			assert result.returncode == 0, (source_free, result)
			checked(run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', linked), 42, 'linked interpreted')
			generated = work / 'linked.c'
			checked(run(runner, '--emit-linked-c', linked, 'M_4_Main_4_main', generated), 0, 'linked C')
			checked(run(compile_native(cache, generated)), 42, 'linked native')
	print('PASS contextual generic function values: bindings, arguments, returns, arrays, assignment, modules and packages')


if __name__ == '__main__':
	main()
