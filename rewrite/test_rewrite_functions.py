#!/usr/bin/env python3
"""Exercise typed calls, recursion, lazy evaluation and string ownership."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))

from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, executable_path, normalize_newlines


def run(command):
	return subprocess.run(command, capture_output=True, timeout=60)


def main() -> None:
	cache = BuildCache(require_cc())
	modules = tuple(ROOT / 'rewrite' / name for name in
		('lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius', 'lower.sirius', 'interpreter.sirius', 'emit_c.sirius'))
	compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
	runner = cache.mir_runner()
	reference = cache.native_tool('modular_cli')
	verifier_source = ROOT / 'rewrite/function_ir_test_driver.sirius'
	verifier = cache.native_driver('rewrite-function-ir', verifier_source, extra_modules=modules)
	result = run([str(verifier)])
	assert result.returncode == 0, result
	cases = [
		('def main() -> i64\n\tprint(twice(21))\n\treturn 0\ndef twice(x: i64) -> i64\n\treturn x * 2\n', b'42\n'),
		('def factorial(n: i64) -> i64\n\tif n < 2\n\t\treturn 1\n\treturn n * factorial(n - 1)\ndef main() -> i64\n\tprint(factorial(10))\n\treturn 0\n', b'3628800\n'),
		('def even(n: i64) -> bool\n\tif n == 0\n\t\treturn true\n\treturn odd(n - 1)\ndef odd(n: i64) -> bool\n\tif n == 0\n\t\treturn false\n\treturn even(n - 1)\ndef main() -> i64\n\tprint(even(21))\n\tprint(odd(21))\n\treturn 0\n', b'false\ntrue\n'),
		('def fib(n: i64) -> i64\n\tif n < 2\n\t\treturn n\n\treturn fib(n - 1) + fib(n - 2)\ndef main() -> i64\n\tprint(fib(9))\n\treturn 0\n', b'34\n'),
		('def same(x: string) -> string\n\treturn x\ndef main() -> i64\n\tvar x = "saved"\n\tlet saved = same(x)\n\tx = same(x + "!")\n\tprint(saved)\n\tprint(x)\n\treturn 0\n', b'saved\nsaved!\n'),
		('def repeat(x: string, n: i64) -> string\n\tif n == 0\n\t\treturn ""\n\treturn x + repeat(x, n - 1)\ndef main() -> i64\n\tprint(repeat("a\\0b", 3))\n\treturn 0\n', b'a\0ba\0ba\0b\n'),
		('def first(x: string, y: string) -> string\n\treturn x\ndef main() -> i64\n\tvar x = "a"\n\tfor i in 0..20\n\t\tx = first(x + "b", x)\n\tprint(x)\n\treturn 0\n', b'a' + b'b' * 20 + b'\n'),
		('def mark(n: i64) -> i64\n\tprint(n)\n\treturn n\ndef sum(a: i64, b: i64, c: i64, d: i64, e: i64) -> i64\n\treturn a + b + c + d + e\ndef main() -> i64\n\tprint(sum(mark(1), mark(2), mark(3), mark(4), mark(5)))\n\treturn 0\n', b'1\n2\n3\n4\n5\n15\n'),
		('def lookup(x: string, i: i64) -> Optional[i64]\n\treturn x.byte_at(i)\ndef fallback(x: Optional[i64], value: i64) -> i64\n\treturn x or value\ndef main() -> i64\n\tprint(fallback(lookup("\\0", 0), 9))\n\tprint(fallback(lookup("x", 3), 9))\n\treturn 0\n', b'0\n9\n'),
		('def boom() -> i64\n\treturn 1 / 0\ndef main() -> i64\n\tprint(true or boom() == 0)\n\tprint(false and boom() == 0)\n\tprint("x".find("x") or boom())\n\treturn 0\n', b'true\nfalse\n0\n'),
		('def build(x: string, n: i64) -> string\n\tvar result = x\n\tfor i in 0..n\n\t\tif i == 1\n\t\t\tcontinue\n\t\tresult += str(i)\n\t\tif i == 3\n\t\t\tbreak\n\treturn result\ndef main() -> i64\n\tprint(build("start", 8))\n\treturn 0\n', b'start023\n'),
		('def choose(x: string, yes: bool) -> string\n\tif yes\n\t\treturn x\n\telse\n\t\treturn x + "!"\ndef main() -> i64\n\tprint(choose("ok", true))\n\tprint(choose("ok", false))\n\treturn 0\n', b'ok\nok!\n'),
		('def early(x: string) -> string\n\tlet index = x.find("x") else return x + " absent"\n\treturn x.slice(index, len(x))\ndef main() -> i64\n\tprint(early("abc"))\n\tprint(early("axb"))\n\treturn 0\n', b'abc absent\nxb\n'),
		('def id(x: i64) -> i64\n\treturn x\ndef main() -> i64\n\tprint(id(-9223372036854775807 - 1))\n\tprint(id(9223372036854775807))\n\treturn 0\n', b'-9223372036854775808\n9223372036854775807\n'),
		('def unused(x: i64, b: bool, text: string, value: Optional[i64]) -> string\n\treturn "ok"\ndef main() -> i64\n\tprint(unused(1, true, "borrowed", "".byte_at(0)))\n\treturn 0\n', b'ok\n'),
		('def unused() -> string\n\treturn "ok"\ndef main() -> i64\n\treturn 0\n', b''),
		('def a(text: string, n: i64) -> string\n\tif n == 0\n\t\treturn text\n\treturn b(text + "a", n - 1)\ndef b(text: string, n: i64) -> string\n\tif n == 0\n\t\treturn text\n\treturn a(text + "b", n - 1)\ndef main() -> i64\n\tprint(a("", 5))\n\treturn 0\n', b'ababa\n'),
		('def guard(text: string, ready: bool) -> string\n\tready else return text + " no"\n\treturn text + " yes"\ndef main() -> i64\n\tprint(guard("ok", false))\n\tprint(guard("ok", true))\n\treturn 0\n', b'ok no\nok yes\n'),
		('def first() -> string\n\tprint(1)\n\treturn "a"\ndef second() -> string\n\tprint(2)\n\treturn "b"\ndef main() -> i64\n\tprint(f"${first()}${second()}")\n\treturn 0\n', b'1\n2\nab\n'),
	]
	invalid = [
		'def main() -> i64\n\treturn wrong(true)\ndef wrong(x: i64) -> i64\n\treturn x\n',
		'def main() -> i64\n\treturn wrong(1)\ndef wrong() -> i64\n\treturn 0\n',
		'def main() -> i64\n\treturn 0\ndef wrong(x: bool) -> i64\n\tif x\n\t\treturn 1\n',
		'def main() -> i64\n\treturn 0\ndef wrong() -> string\n\tprint("missing")\n',
		'def main() -> i64\n\treturn 0\ndef wrong() -> string\n\treturn 3\n',
		'def main() -> i64\n\treturn 0\ndef wrong(x: i64) -> i64\n\tx += 1\n\treturn x\n',
	]
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for module in modules:
			(root / module.name).write_bytes(module.read_bytes())
		(root / 'Main.sirius').write_bytes((ROOT / 'rewrite/main.sirius').read_bytes())
		library = root / 'compiler.smirl'
		execute([str(runner), '--build-modules', str(root), 'Main', str(library)])
		(root / 'Check.sirius').write_bytes(verifier_source.read_bytes())
		check_library = root / 'check.smirl'
		execute([str(runner), '--build-modules', str(root), 'Check', str(check_library)])
		result = run([str(runner), '--entry', 'M_5_Check_4_main', '--load-lib', str(check_library)])
		assert result.returncode == 0, result
		print('PASS typed function IR verifier, native and MIR', flush=True)
		commands = [[str(compiler)], [str(runner), '--entry', 'M_4_Main_4_main', '--load-lib', str(library)]]
		audit_runtime = root / 'audit-runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit_runtime)])
		audit_verifier = executable_path(root / 'audit-verifier')
		compile_c(cache.cc, verifier.parent / 'generated.c', audit_verifier, audit_runtime)
		result = run([str(audit_verifier)])
		assert result.returncode == 0 and not result.stdout and not result.stderr, result
		for index, (text, expected) in enumerate(cases):
			source = root / 'input.sirius'
			source.write_text(text, encoding='utf-8')
			outputs = []
			for mode, command in enumerate(commands):
				result = run(command + ['--run', str(source)])
				assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (index, mode, result)
				generated = root / f'output{mode}.c'
				result = run(command + [str(source), str(generated)])
				assert result.returncode == 0, (index, mode, result)
				outputs.append(generated.read_bytes())
				binary = executable_path(root / f'program{mode}')
				compile_c(cache.cc, generated, binary, audit_runtime)
				result = run([str(binary)])
				assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (index, mode, result)
			assert all(output == outputs[0] for output in outputs), index
			# Interpolation is a rewrite-only feature with independent expectations.
			if 'f"' in text:
				continue
			result = run([str(runner), str(source)])
			assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (index, result)
			generated = root / 'reference.c'
			execute([str(reference), str(source), str(generated)])
			binary = executable_path(root / 'reference')
			compile_c(cache.cc, generated, binary, cache.runtime())
			result = run([str(binary)])
			assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (index, result)
		for index, text in enumerate(invalid):
			source = root / 'invalid.sirius'
			source.write_text(text, encoding='utf-8')
			for command in commands:
				generated = root / f'invalid{index}.c'
				for arguments in [['--run', str(source)], [str(source), str(generated)]]:
					result = run(command + arguments)
					assert result.returncode == 1 and b'ERROR ' in result.stderr and not generated.exists(), (text, result)
		# Traps in callees propagate to the entry, rather than becoming results.
		for expression, reason in [('1 / 0', b'division by zero'), ('"".byte_at(0)!', b'missing optional value')]:
			source = root / 'trap.sirius'
			source.write_text('def fail(x: string) -> i64\n\treturn ' + expression + '\ndef main() -> i64\n\tprint(fail("owned"))\n\treturn 0\n', encoding='utf-8')
			for command in commands:
				result = run(command + ['--run', str(source)])
				assert result.returncode != 0 and reason in result.stderr and not result.stdout, result
				generated = root / 'trap.c'
				execute(command + [str(source), str(generated)])
				binary = executable_path(root / 'trap')
				compile_c(cache.cc, generated, binary, cache.runtime())
				result = run([str(binary)])
				assert result.returncode != 0 and reason in result.stderr and not result.stdout, result
	print(f'PASS rewrite calls: {len(cases)} valid (ARC audited), {len(invalid)} invalid, 2 callee traps; native and MIR', flush=True)


if __name__ == '__main__':
	main()
