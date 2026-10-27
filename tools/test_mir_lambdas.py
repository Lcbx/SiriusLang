#!/usr/bin/env python3
"""Check noncapturing source lambdas, native/MIR parity and module packaging."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT, normalize_newlines

CASES = {
	'source_lambda_basic': (42, b''),
	'source_lambda_direct': (42, b''),
	'source_lambda_return': (42, b''),
	'source_lambda_zero': (42, b''),
	'source_lambda_named': (42, b''),
	'source_lambda_managed': (6, b'hello!\n'),
	'source_lambda_nested': (42, b''),
	'source_lambda_constant': (42, b''),
	'source_lambda_infer_binding': (42, b''),
	'source_lambda_infer_argument': (42, b''),
	'source_lambda_infer_return': (42, b''),
	'source_lambda_infer_nested': (42, b''),
	'source_lambda_infer_array': (42, b''),
	'source_lambda_infer_assignment': (42, b''),
	'source_lambda_infer_zero': (42, b''),

}

INVALID = (
	('capture', 'def main() -> i64\n\tlet local = 7\n\tlet f = fn(x: i64) x + local\n\treturn f(1)\n', "unknown name 'local'"),
	('nested capture', 'def main() -> i64\n\tlet f = fn(x: i64) fn(y: i64) x + y\n\treturn 0\n', "unknown name 'x'"),
	('missing parameter type', 'def main() -> i64\n\tlet f = fn(x) x + 1\n\treturn f(1)\n', 'lambda parameter requires annotation or expected Fn type'),
	('duplicate parameter', 'def main() -> i64\n\tlet f = fn(x: i64, x: i64) x\n\treturn f(1, 2)\n', 'duplicate lambda parameter'),
	('default parameter', 'def main() -> i64\n\tlet f = fn(x: i64 = 2) x\n\treturn f(1)\n', 'lambda parameter defaults'),
	('contextual arity', 'def main() -> i64\n\tlet f: Fn[i64,i64] = fn(x, y) x + y\n\treturn 0\n', 'lambda parameter count does not match'),
	('contextual parameter mismatch', 'def main() -> i64\n\tlet f: Fn[i64,i64] = fn(x: string) 1\n\treturn 0\n', 'lambda parameter type does not match'),
	('contextual return mismatch', 'def main() -> i64\n\tlet f: Fn[i64,bool] = fn(x) x + 1\n\treturn 0\n', 'lambda return type does not match'),
	('contextual capture', 'def main() -> i64\n\tlet offset = 4\n\tlet f: Fn[i64,i64] = fn(x) x + offset\n\treturn 0\n', "unknown name 'offset'"),
	('wrong signature', 'def main() -> i64\n\tlet f: Fn[string,i64] = fn(x: i64) x\n\treturn 1\n', 'expected'),
)


def run(binary: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *(str(x) for x in args)], capture_output=True, timeout=45)


def check(result: subprocess.CompletedProcess[bytes], name: str, status: int, stdout: bytes = b'') -> None:
	assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (status, stdout, b''), (name, result)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		for name, (status, stdout) in CASES.items():
			source = ROOT / 'tests' / 'mir' / (name + '.sirius')
			generated = work / (name + '.c')
			check(run(compiler, source, generated), name + '-compile', 0)
			native = compile_native(cache, generated)
			check(run(native), name + '-native', status, stdout)
			check(run(runner, source), name + '-interpreter', status, stdout)
			artifact = work / (name + '.smir')
			check(run(runner, '--emit-mir', artifact, source), name + '-serialize', 0)
			check(run(runner, '--load-mir', artifact), name + '-reload', status, stdout)
			if name == 'source_lambda_managed':
				audit = work / 'audit'
				result = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(audit)], capture_output=True)
				assert result.returncode == 0, result
				check(run(audit), name + '-arc', status, stdout)
		for name, source, diagnostic in INVALID:
			input_file = work / (name.replace(' ', '-') + '.sirius')
			input_file.write_text(source)
			result = run(compiler, input_file, work / 'invalid.c')
			assert result.returncode != 0 and diagnostic.encode() in (result.stdout + result.stderr), (name, result)
		module = work / 'modules'
		module.mkdir()
		(module / 'Helper.sirius').write_text('export def twice(x) -> i64\n\tlet f: Fn[i64,i64] = fn(v) v * 2\n\treturn f(x)\n')
		(module / 'App.sirius').write_text('import Helper alias H\ndef main() -> i64\n\treturn H.twice(21)\n')
		artifact = module / 'linked.smirl'
		result = run(runner, '--build-modules', module, 'App', artifact)
		assert result.returncode == 0, result
		check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', artifact), 'source-linked', 42)
		check(run(runner, '--emit-generic', 'Helper', module / 'Helper.smirg', module / 'Helper.sirius'), 'package', 0)
		(module / 'Helper.sirius').unlink()
		artifact = module / 'linked-sourcefree.smirl'
		result = run(runner, '--build-modules', module, 'App', artifact)
		assert result.returncode == 0, result
		check(run(runner, '--entry', 'M_3_App_4_main', '--load-lib', artifact), 'source-free', 42)
	print('PASS noncapturing lambdas: native/interpreter, MIR wire, ARC, rejects captures, generic packages')


if __name__ == '__main__':
	main()
