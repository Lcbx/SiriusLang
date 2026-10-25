#!/usr/bin/env python3
"""Check deterministic overload resolution, mangling, re-exports and native/MIR parity."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def write(root: Path, name: str, source: str) -> None:
	(root / f'{name}.sirius').write_text(source, encoding='utf-8')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory(prefix='sirius-overloads-') as directory:
		root = Path(directory)
		write(root, 'Ops',
			'export def twice(x: i64) -> i64\n\treturn 2 * x\n'
			'export def twice(x: f64) -> f64\n\treturn 2.0 * x\n'
			'export def twice(x: string) -> string\n\treturn x + x\n')
		write(root, 'Forward', 'import Ops alias O\nexport O.twice alias repeat\n')
		write(root, 'App',
			'import Ops alias O\nimport Forward alias F\n'
			'def twice(x: i64) -> i64\n\treturn 2 * x\n'
			'def twice(x: f64) -> f64\n\treturn 2.0 * x\n'
			'def apply(f: Fn[i64,i64], x: i64) -> i64\n\treturn f(x)\n'
			'def main() -> i64\n'
			'\tlet local: Fn[i64,i64] = twice\n'
			'\tlet imported: Fn[string,string] = O.twice\n'
			'\tlet forwarded: Fn[f64,f64] = F.repeat\n'
			'\tif twice(21) != 42 or twice(2.0) != 4.0\n\t\treturn 1\n'
			'\tif apply(local, 21) != 42 or imported("go") != "gogo"\n\t\treturn 2\n'
			'\tif O.twice(10) != 20 or F.repeat(1.5) != 3.0 or forwarded(2.0) != 4.0\n\t\treturn 3\n'
			'\treturn 42\n')
		library = root / 'linked.smirl'
		built = run(runner, '--build-modules', root, 'App', library)
		assert built.returncode == 0, built
		result = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
		assert (result.returncode, result.stdout, result.stderr) == (42, b'', b''), result
		generated = root / 'linked.c'
		built = run(runner, '--emit-linked-c', library, 'M_3_App_4_main', generated)
		assert built.returncode == 0, built
		binary = root / 'main'
		built = run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c', *link_flags(cache.cc), '-o', binary)
		assert built.returncode == 0, built
		result = run(binary)
		assert (result.returncode, result.stdout, result.stderr) == (42, b'', b''), result
		for name, definitions, call, message in (
			('duplicate', 'def f(x: i64) -> i64\n\treturn x\ndef f(y: i64) -> i64\n\treturn y\n', 'return 0', 'duplicate overload'),
			('visibility', 'export def f(x: i64) -> i64\n\treturn x\ndef f(x: string) -> i64\n\treturn 2\n', 'return 0', 'consistent export visibility'),
			('generic', 'def f(x) -> i64\n\treturn 1\ndef f(x: string) -> i64\n\treturn 2\n', 'return 0', 'annotated parameter'),
			('unmatched', 'def f(x: i64) -> i64\n\treturn x\ndef f(x: string) -> i64\n\treturn 1\n', 'return f(true)', 'no matching overload'),
			('untyped', 'def f(x: i64) -> i64\n\treturn x\ndef f(x: string) -> i64\n\treturn 1\n', 'let g = f\n\treturn 0', 'expected Fn'),
		):
			write(root, 'Bad', definitions + 'def main() -> i64\n\t' + call + '\n')
			failure = run(runner, '--build-modules', root, 'Bad', root / 'bad.smirl')
			assert failure.returncode != 0 and message.encode() in failure.stdout + failure.stderr, (name, failure)
	print('PASS first-class overloaded functions: local/imports/reexports, native/MIR, ARC, diagnostics')


if __name__ == '__main__':
	main()
