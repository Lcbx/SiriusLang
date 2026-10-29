#!/usr/bin/env python3
"""Check qualified exported function values across module linking and re-exports."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(value) for value in command], capture_output=True, timeout=90)


def check(result: subprocess.CompletedProcess[bytes], status: int, stdout: bytes = b'') -> None:
	assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (status, stdout, b''), result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		(work / 'Helpers.sirius').write_text(
			'export def double(x: i64) -> i64\n\treturn x * 2\n'
			'export def increment(x: i64) -> i64\n\treturn x + 1\n'
			'export def shout(x: string) -> string\n\treturn x + "!"\n'
			'def private(x: i64) -> i64\n\treturn x + 99\n', encoding='utf-8')
		(work / 'Reexport.sirius').write_text(
			'import Helpers alias H\nexport H.double alias twice\n', encoding='utf-8')
		(work / 'Main.sirius').write_text(
			'import Helpers alias H\nimport Reexport alias R\n'
			'def apply(f: Fn[i64,i64], x: i64) -> i64\n\treturn f(x)\n'
			'def choose() -> Fn[i64,i64]\n\treturn R.twice\n'
			'def main() -> i64\n'
			'\tlet f = H.double\n'
			'\tlet g: Fn[i64,i64] = R.twice\n'
			'\tif apply(f, 20) != 40\n\t\treturn 1\n'
			'\tif apply(g, 21) != 42\n\t\treturn 2\n'
			'\tif apply(H.double, 21) != 42\n\t\treturn 5\n'
			'\tlet chosen = choose()\n\tif chosen(20) != 40\n\t\treturn 3\n'
			'\tlet inc = H.increment\n\tif inc(41) != 42\n\t\treturn 4\n'
			'\tlet shout: Fn[string,string] = H.shout\n'
			'\tprint(shout("hello"))\n\treturn 42\n', encoding='utf-8')
		library = work / 'linked.smirl'
		result = run(runner, '--build-modules', work, 'Main', library)
		assert result.returncode == 0, result
		check(run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', library), 42, b'hello!\n')
		generated = work / 'linked.c'
		check(run(runner, '--emit-linked-c', library, 'M_4_Main_4_main', generated), 0)
		binary = work / 'linked'
		result = run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c', *link_flags(cache.cc), '-o', binary)
		assert result.returncode == 0, result
		check(run(binary), 42, b'hello!\n')
		for name, text, expected in (
			('private', 'import Helpers alias H\ndef main() -> i64\n\tlet f = H.private\n\treturn f(21)\n', b'not exported'),
			('unknown', 'import Helpers alias H\ndef main() -> i64\n\tlet f = H.unknown\n\treturn f(21)\n', b'not exported'),
			('signature', 'import Helpers alias H\ndef main() -> i64\n\tlet f: Fn[string,i64] = H.double\n\treturn 0\n', b'expected'),
			('unqualified', 'import Helpers alias H\ndef main() -> i64\n\tlet f = double\n\treturn f(21)\n', b'unknown name'),
		):
			(work / 'Invalid.sirius').write_text(text, encoding='utf-8')
			failure = run(runner, '--build-modules', work, 'Invalid', work / 'bad.smirl')
			assert failure.returncode != 0 and expected in failure.stdout + failure.stderr, (name, failure)
	print('PASS qualified imported function values: source/re-export, native/MIR, ARC, visibility and typing')


if __name__ == '__main__':
	main()
