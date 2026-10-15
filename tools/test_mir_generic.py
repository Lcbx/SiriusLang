#!/usr/bin/env python3
"""Link-time MIR generic specialization from independently saved provider libraries."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def run(binary: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *map(str, args)], capture_output=True, timeout=40)


def must_run(binary: Path, *args: str) -> subprocess.CompletedProcess[bytes]:
	result = run(binary, *args)
	assert result.returncode == 0, (args, result)
	return result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	root = Path(__file__).resolve().parent.parent
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		def source(name: str, text: str) -> Path:
			file = work / f'{name}.sirius'
			file.write_text(text)
			return file

		provider = source('provider', 'def identity(x)\n\treturn x\n'
			'def doubled(x)\n\treturn identity(x) + identity(x)\n')
		lib = work / 'provider.smirl'
		must_run(runner, '--emit-lib', lib, '--template', 'identity', '--template', 'doubled', provider)
		assert lib.read_bytes().startswith(b'SIRIUS-MIR-LIB/3\n')
		assert run(runner, '--list-lib', lib).stdout == b'template identity\ntemplate doubled\n'
		provider.unlink()  # The saved library is independent of its source file.
		assert run(runner, '--load-lib', lib).returncode == 2

		consumers = (
			('int', 'extern def identity(x: i64) -> i64\ndef test_int() -> i64\n\treturn identity(42)\n', 42),
			('str', 'extern def identity(x: string) -> string\ndef test_str() -> i64\n\treturn len(identity("sirius"))\n', 6),
			('arr', 'extern def identity(xs: Array[i64]) -> Array[i64]\ndef test_arr() -> i64\n\treturn identity([3, 4])[!1]\n', 4),
			('record', 'rec Point(x: i64, y: i64)\n\tpub x\n\tpub y\n'
				'extern def identity(point: Point) -> Point\ndef test_record() -> i64\n'
				'\tlet p = Point(8, 12)\n\treturn identity(p).y\n', 12),
			('nested', 'extern def doubled(x: string) -> string\ndef test_nested() -> i64\n'
				'\treturn len(doubled("hi"))\n', 4),
			('same', 'extern def identity(x: i64) -> i64\ndef test_same() -> i64\n'
				'\treturn identity(15)\n', 15),
		)
		imports = []
		for name, text, _ in consumers:
			file = source(name, text)
			output = work / f'{name}.smirl'
			must_run(runner, '--emit-lib', output, '--export', f'test_{name}', file)
			imports.append(output)
		combined = work / 'linked.smirl'
		must_run(runner, '--link-lib', combined, *imports, lib)
		assert run(runner, '--list-lib', combined).stdout == b''.join(
			f'test_{name} : ()->i64\n'.encode() for name, _, _ in consumers)
		for name, _, expected in consumers:
			result = run(runner, '--entry', f'test_{name}', '--load-lib', combined)
			assert result.returncode == expected, (name, result)
		must_run(runner, '--link-lib', combined, lib, *imports)
		for name, _, expected in consumers:
			assert run(runner, '--entry', f'test_{name}', '--load-lib', combined).returncode == expected
		print('PASS generic: template-only v3, i64/string/array/record, nested functions, reuse, link orders')

		# The resulting concrete library uses the unchanged native/interpreted MIR
		# representation; both backends must execute the same specialization.
		c_file = work / 'native.c'
		must_run(runner, '--emit-c-lib', c_file, '--entry', 'test_record', combined)
		cc = shutil.which('clang') or require_cc()
		native = work / 'native'
		result = subprocess.run([cc, '-std=c11', '-O0', '-I', str(root / 'runtime'),
			str(c_file), str(root / 'runtime/sirius_runtime.c'), '-lm', '-o', str(native)],
			capture_output=True, timeout=90)
		assert result.returncode == 0, result.stderr
		assert run(native).returncode == 12
		print('PASS generic: native and interpreted linked program parity')

		# A concrete export remains a higher-priority match than any template.
		exact = source('exact', 'def identity(x: i64) -> i64\n\treturn x + 100\n'
			'def entry() -> i64\n\treturn identity(2)\n')
		exact_lib = work / 'exact.smirl'
		must_run(runner, '--emit-lib', exact_lib, '--export', 'entry', '--expose', 'identity', exact)
		must_run(runner, '--link-lib', combined, imports[0], exact_lib, lib)
		assert run(runner, '--entry', 'test_int', '--load-lib', combined).returncode == 142
		print('PASS generic: concrete export preferred to template')

		wrong = source('wrong', 'extern def identity(x: i64) -> bool\n'
			'def wrong() -> i64\n\tif identity(2)\n\t\treturn 1\n\treturn 0\n')
		wrong_lib = work / 'wrong.smirl'
		must_run(runner, '--emit-lib', wrong_lib, '--export', 'wrong', wrong)
		result = run(runner, '--link-lib', combined, wrong_lib, lib)
		assert result.returncode == 2 and b'signature mismatch' in result.stderr, result
		print('PASS generic: result type mismatch rejected')

		unrelated = source('unrelated', 'def other(x)\n\treturn x\n')
		other_lib = work / 'other.smirl'
		must_run(runner, '--emit-lib', other_lib, '--template', 'other', unrelated)
		result = run(runner, '--link-lib', combined, imports[0], other_lib)
		assert result.returncode == 2 and b'unresolved MIR import' in result.stderr, result
		result = run(runner, '--link-lib', combined, imports[0], lib, lib)
		assert result.returncode == 2 and b'ambiguous MIR generic template' in result.stderr, result
		print('PASS generic: missing and ambiguous template rejected')

		dependent = source('dependent', 'extern def hidden(x: i64) -> i64\n'
			'def identity(x)\n\treturn hidden(x)\n')
		dependent_lib = work / 'dependent.smirl'
		must_run(runner, '--emit-lib', dependent_lib, '--template', 'identity', dependent)
		# A specialized provider may import a concrete function from another library.
		bonus = source('bonus', 'def hidden(x: i64) -> i64\n\treturn x + 9\n'
			'def bonus_entry() -> i64\n\treturn hidden(0)\n')
		bonus_lib = work / 'bonus.smirl'
		must_run(runner, '--emit-lib', bonus_lib, '--export', 'bonus_entry', '--expose', 'hidden', bonus)
		must_run(runner, '--link-lib', combined, imports[0], dependent_lib, bonus_lib)
		assert run(runner, '--entry', 'test_int', '--load-lib', combined).returncode == 51
		must_run(runner, '--link-lib', combined, bonus_lib, dependent_lib, imports[0])
		assert run(runner, '--entry', 'test_int', '--load-lib', combined).returncode == 51
		print('PASS generic: a specialization imports a concrete function in either link order')

		# A newly generated specialization can request another generic template,
		# which may in turn have an unresolved external dependency.
		outer = source('outer', 'extern def middle(x: i64) -> i64\n'
			'def outer(x)\n\treturn middle(x) + 1\n')
		middle = source('middle', 'extern def last(x: i64) -> i64\n'
			'def middle(x)\n\treturn last(x) + 1\n')
		last = source('last', 'def last(x: i64) -> i64\n\treturn x * 2\n'
			'def last_entry() -> i64\n\treturn last(0)\n')
		outer_lib = work / 'outer.smirl'
		middle_lib = work / 'middle.smirl'
		last_lib = work / 'last.smirl'
		must_run(runner, '--emit-lib', outer_lib, '--template', 'outer', outer)
		must_run(runner, '--emit-lib', middle_lib, '--template', 'middle', middle)
		must_run(runner, '--emit-lib', last_lib, '--export', 'last_entry', '--expose', 'last', last)
		chain_consumer = source('chain', 'extern def outer(x: i64) -> i64\n'
			'def chain() -> i64\n\treturn outer(5)\n')
		chain_lib = work / 'chain.smirl'
		must_run(runner, '--emit-lib', chain_lib, '--export', 'chain', chain_consumer)
		must_run(runner, '--link-lib', combined, chain_lib, outer_lib, middle_lib, last_lib)
		assert run(runner, '--entry', 'chain', '--load-lib', combined).returncode == 12
		must_run(runner, '--link-lib', combined, last_lib, middle_lib, outer_lib, chain_lib)
		assert run(runner, '--entry', 'chain', '--load-lib', combined).returncode == 12
		assert run(runner, '--list-lib', combined).stdout == b'last_entry : ()->i64\nlast : (i64)->i64\nchain : ()->i64\n'
		print('PASS generic: transitive template dependencies, private exports, link ordering')

		# Missing, incompatible, and ambiguous providers must fail at link time.
		missing = run(runner, '--link-lib', combined, chain_lib, outer_lib, middle_lib)
		assert missing.returncode == 2 and b'unresolved MIR import: last' in missing.stderr, missing
		wrong_provider = source('wrong_provider', 'def last(x: bool) -> i64\n\treturn 1\n'
			'def wrong_entry() -> i64\n\treturn last(true)\n')
		wrong_lib = work / 'wrong_provider.smirl'
		must_run(runner, '--emit-lib', wrong_lib, '--export', 'wrong_entry', '--expose', 'last', wrong_provider)
		wrong_result = run(runner, '--link-lib', combined, chain_lib, outer_lib, middle_lib, wrong_lib)
		assert wrong_result.returncode == 2 and b'MIR import signature mismatch: last' in wrong_result.stderr, wrong_result
		print('PASS generic: missing and mismatched transitive dependencies rejected')


		# Independent structural record definitions must agree across a generic
		# template, a concrete external provider, and its consumer.
		point = 'rec Point(x: i64, y: i64)\n\tpub x\n\tpub y\n'
		record_wrapper = source('record_wrapper', point + 'extern def score(p: Point) -> i64\n'
			'def score_plus_one(p)\n\treturn score(p) + 1\n')
		record_provider = source('record_provider', point + 'def score(p: Point) -> i64\n'
			'\treturn p.x + p.y\n'
			'def score_entry() -> i64\n\treturn score(Point(0, 0))\n')
		record_consumer = source('record_consumer', point +
			'extern def score_plus_one(p: Point) -> i64\n'
			'def point_entry() -> i64\n\treturn score_plus_one(Point(3, 4))\n')
		record_wrapper_lib = work / 'record_wrapper.smirl'
		record_provider_lib = work / 'record_provider.smirl'
		record_consumer_lib = work / 'record_consumer.smirl'
		must_run(runner, '--emit-lib', record_wrapper_lib, '--template', 'score_plus_one', record_wrapper)
		must_run(runner, '--emit-lib', record_provider_lib, '--export', 'score_entry', '--expose', 'score', record_provider)
		must_run(runner, '--emit-lib', record_consumer_lib, '--export', 'point_entry', record_consumer)
		must_run(runner, '--link-lib', combined, record_consumer_lib, record_wrapper_lib, record_provider_lib)
		assert run(runner, '--entry', 'point_entry', '--load-lib', combined).returncode == 8
		must_run(runner, '--emit-c-lib', c_file, '--entry', 'point_entry', combined)
		result = subprocess.run([cc, '-std=c11', '-O0', '-I', str(root / 'runtime'),
			str(c_file), str(root / 'runtime/sirius_runtime.c'), '-lm', '-o', str(native)],
			capture_output=True, timeout=90)
		assert result.returncode == 0, result.stderr
		assert run(native).returncode == 8
		print('PASS generic: structural records and interpreted/native parity across dependencies')

		provider = source('provider', 'def identity(x)\n\treturn x\n'
			'def doubled(x)\n\treturn identity(x) + identity(x)\n')
		for flags, expected in ((('--template', 'nonexistent'), b'unknown generic template'),
			(('--template', 'identity', '--template', 'identity'), b'duplicate MIR generic template')):
			result = run(runner, '--emit-lib', work / 'invalid.smirl', *flags, provider)
			assert result.returncode != 0 and expected in result.stdout + result.stderr, result
		truncated = work / 'truncated.smirl'
		truncated.write_bytes(lib.read_bytes()[:-40])
		result = run(runner, '--list-lib', truncated)
		assert result.returncode != 0 and b'invalid MIR library' in result.stderr, result
		print('PASS generic: declaration errors and corrupted artifact rejection')


if __name__ == '__main__':
	main()
