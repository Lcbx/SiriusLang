#!/usr/bin/env python
"""Source-level external declarations, cross-library calls and type checking."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from project_paths import executable_path, normalize_newlines, normalize_newlines
from build_support import BuildCache, require_cc, link_flags


def run(binary: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	result = subprocess.run([str(binary), *args], capture_output=True, timeout=30)
	return subprocess.CompletedProcess(result.args, result.returncode,
		normalize_newlines(result.stdout), normalize_newlines(result.stderr))


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	parser.add_argument('--compiler', type=Path)
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc())
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def source(name: str, text: str) -> Path:
			path = work / f'{name}.sirius'
			path.write_text(text)
			return path
		provider = source('provider', 'def process(xs: Array[i64]) -> i64\n\treturn xs[!0] + len(xs)\n'
			'def provider() -> i64\n\treturn process([3, 4])\n')
		consumer = source('consumer', 'extern def process(xs: Array[i64]) -> i64\n'
			'def consumer() -> i64\n\treturn process([7, 8]) + 1\n')
		p, c, linked = (work / f'{name}.smirl' for name in ('p', 'c', 'linked'))
		assert run(runner, '--emit-lib', str(p), '--export', 'provider', '--export', 'process', str(provider)).returncode == 0
		assert run(runner, '--emit-lib', str(c), '--export', 'consumer', str(consumer)).returncode == 0
		assert c.read_bytes().startswith(b'SIRIUS-MIR-LIB/2\n')
		assert run(runner, '--list-lib', str(c)).stdout == b'consumer : ()->i64\nimport process : (Array[i64])->i64\n'
		assert run(runner, '--entry', 'consumer', '--load-lib', str(c)).returncode == 2
		assert run(runner, '--emit-linked-c', str(c), 'consumer', str(work / 'unused.c')).returncode == 2
		assert run(runner, '--emit-linked-c', str(p), 'unknown', str(work / 'unused.c')).returncode == 2
		assert run(runner, '--link-lib', str(linked), str(c), str(p)).returncode == 0
		assert run(runner, '--entry', 'consumer', '--load-lib', str(linked)).returncode == 10
		compiled_c = work / 'compiled.c'
		result = run(runner, '--emit-linked-c', str(linked), 'consumer', str(compiled_c))
		assert result.returncode == 0, result
		native = executable_path(work / 'native')
		root = Path(__file__).resolve().parent.parent
		cc = shutil.which('clang') or require_cc()
		build = subprocess.run([cc, '-std=c11', '-O0', '-I', str(root / 'runtime'), str(compiled_c),
			str(root / 'runtime/sirius_runtime.c'), *link_flags(cc), '-o', str(native)], capture_output=True, timeout=60)
		assert build.returncode == 0, build.stderr
		assert run(native).returncode == 10
		assert run(runner, '--link-lib', str(linked), str(p), str(c)).returncode == 0
		assert run(runner, '--entry', 'consumer', '--load-lib', str(linked)).returncode == 10
		print('PASS extern def: source declaration, automatic import metadata, structural link, both link orders')

		missing = run(runner, '--link-lib', str(linked), str(c), str(work / 'missing.smirl'))
		assert missing.returncode == 2
		incompatible = source('incompatible', 'def process(xs: Array[bool]) -> i64\n\treturn len(xs)\n'
			'def different() -> i64\n\treturn process([true, false])\n')
		assert run(runner, '--emit-lib', str(p), '--export', 'different', '--export', 'process', str(incompatible)).returncode == 0
		mismatch = run(runner, '--link-lib', str(linked), str(c), str(p))
		assert mismatch.returncode == 2 and b'signature mismatch' in mismatch.stderr, mismatch
		print('PASS extern def: incompatible provider rejected')

		for name, text, error in (
			('missing_param', 'extern def f(x) -> i64\ndef main() -> i64\n\treturn 0\n', b'extern parameter requires a type'),
			('missing_result', 'extern def f(x: i64)\ndef main() -> i64\n\treturn 0\n', b'extern function requires a return type'),
			('has_body', 'extern def f() -> i64\n\treturn 1\ndef main() -> i64\n\treturn 0\n', b'unexpected expression'),
			('bad_call', 'extern def f(x: bool) -> i64\ndef main() -> i64\n\treturn f(1)\n', b'expected bool'),
		):
			src = source(name, text)
			result = run(runner, '--emit-lib', str(linked), '--export', 'main', str(src))
			assert result.returncode != 0 and error in result.stdout + result.stderr, (name, result)
		print('PASS extern def: declaration syntax and call types checked')

		unresolved = source('unresolved', 'extern def f(x: i64) -> i64\ndef main() -> i64\n\treturn f(3)\n')
		result = run(runner, str(unresolved))
		assert result.returncode == 2 and b'--emit-lib and linking' in result.stderr, result
		out = work / 'native.c'
		result = run(compiler, str(unresolved), str(out))
		assert result.returncode != 0 and b'library linking' in result.stdout, result
		print('PASS extern def: unresolved source execution and native compilation rejected')


if __name__ == '__main__':
	main()
