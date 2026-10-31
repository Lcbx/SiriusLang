#!/usr/bin/env python3
"""Check flat structural records, typed value copies and managed-field ownership."""
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
	driver = ROOT / 'rewrite/record_ir_test_driver.sirius'
	verifier = cache.native_driver('rewrite-record-ir', driver, extra_modules=modules)
	result = run([str(verifier)])
	assert result.returncode == 0 and not result.stderr, result
	cases = [
		('rec Token(kind: i64, start: i64, end: i64, line: i64, column: i64)\ndef main() -> i64\n\tlet t = Token(4, 10, 15, 2, 3)\n\tprint(t.kind)\n\tprint(t.start)\n\tprint(t.end)\n\tprint(t.line)\n\tprint(t.column)\n\treturn 0\n', b'4\n10\n15\n2\n3\n'),
		('rec Pair(left: string, right: string)\ndef swap(p: Pair) -> Pair\n\treturn Pair(p.right, p.left)\ndef main() -> i64\n\tlet text = "x"\n\tvar p = Pair(text, text)\n\tlet saved = p\n\tp = swap(Pair(p.left + "a", p.right + "b"))\n\tprint(saved.left)\n\tprint(saved.right)\n\tprint(p.left)\n\tprint(p.right)\n\treturn 0\n', b'x\nx\nxb\nxa\n'),
		('rec Item(z: i64, a: string, flag: bool)\ndef main() -> i64\n\tlet p = Item(-7, "a\\0b", true)\n\tprint(p.z)\n\tprint(p.a)\n\tprint(p.flag)\n\tprint(p.a.slice(0, 1).starts_with("a"))\n\treturn 0\n', b'-7\na\0b\ntrue\ntrue\n'),
		('rec A(z: i64, a: string)\nrec B(a: string, z: i64)\ndef id(p: A) -> B\n\treturn p\ndef main() -> i64\n\tlet p: A = B("ok", 8)\n\tlet q = id(p)\n\tprint(q.a)\n\tprint(q.z)\n\treturn 0\n', b'ok\n8\n'),
		('rec Wide(z: i64, a: string)\nrec Small(a: string, z: i64)\ndef small(p: Small) -> Small\n\treturn p\ndef main() -> i64\n\tlet p = Wide(3, "saved")\n\tlet q: Small = p\n\tvar r = small(p)\n\tr = Wide(4, "new")\n\tprint(q.a)\n\tprint(r.a)\n\treturn 0\n', b'saved\nnew\n'),
		('def make() -> Small\n\treturn Wide("yes", 7)\nrec Small(text: string, n: i64)\nrec Wide(text: string, n: i64)\ndef main() -> i64\n\tprint(make().text)\n\treturn 0\n', b'yes\n'),
		('rec R(text: string, n: i64)\ndef repeat(r: R) -> R\n\tif r.n == 0\n\t\treturn r\n\treturn repeat(R(r.text + "x", r.n - 1))\ndef main() -> i64\n\tlet r = R("a", 8)\n\tprint(repeat(r).text)\n\tprint(r.text)\n\treturn 0\n', b'axxxxxxxx\na\n'),
		('rec R(text: string)\ndef id(r: R) -> R\n\treturn r\ndef main() -> i64\n\tvar r = R("a")\n\tlet saved = id(r)\n\tfor i in 0..10\n\t\tr = R(r.text + "b")\n\tprint(saved.text)\n\tprint(r.text)\n\treturn 0\n', b'a\nabbbbbbbbbb\n'),
		('rec Pair(z: i64, a: i64)\ndef mark(n: i64) -> i64\n\tprint(n)\n\treturn n\ndef main() -> i64\n\tlet p = Pair(mark(1), mark(2))\n\tprint(p.z)\n\tprint(p.a)\n\treturn 0\n', b'1\n2\n1\n2\n'),
		('rec Empty()\nrec Wide(n: i64)\ndef empty(p: Empty) -> Empty\n\treturn p\ndef main() -> i64\n\tlet p: Empty = Empty()\n\tlet q = empty(p)\n\tlet r = empty(Empty())\n\treturn 0\n', b''),
		('rec Small(text: string, n: i64)\nrec Wide(n: i64, text: string)\ndef choose(n: i64) -> Small\n\tif n == 0\n\t\treturn Wide(n, "zero")\n\telif n == 1\n\t\treturn Wide(n, "one")\n\telse\n\t\treturn Wide(n, "other")\ndef main() -> i64\n\tfor i in 0..3\n\t\tprint(choose(i).text)\n\treturn 0\n', b'zero\none\nother\n'),
		('rec Small(text: string, n: i64)\nrec Wide(n: i64, text: string)\ndef absent(text: string) -> Small\n\tlet i = text.find("x") else return Wide(1, "absent")\n\treturn Wide(2, text.slice(i, len(text)))\ndef main() -> i64\n\tprint(absent("abc").text)\n\tprint(absent("axb").text)\n\treturn 0\n', b'absent\nxb\n'),
		('rec Small(text: string, n: i64)\nrec Wide(n: i64, text: string)\ndef check(r: Small) -> bool\n\treturn len(r.text) > 0\ndef boom() -> Wide\n\treturn Wide(1 / 0, "bad")\ndef main() -> i64\n\tprint(true or check(boom()))\n\tprint(false and check(boom()))\n\tprint(check(Wide(1, "ok")))\n\treturn 0\n', b'true\nfalse\ntrue\n'),
		('rec Label(text: string)\ndef pass(p: Label, yes: bool) -> Label\n\tyes else return Label("no")\n\treturn p\ndef main() -> i64\n\tprint(pass(Label("yes"), true).text)\n\tprint(pass(Label("yes"), false).text)\n\treturn 0\n', b'yes\nno\n'),
		('rec Unused(text: string)\ndef unused(p: Unused) -> Unused\n\treturn p\ndef main() -> i64\n\treturn 0\n', b''),
		('rec Limits(n: i64, flag: bool)\ndef main() -> i64\n\tprint(Limits(-9223372036854775807 - 1, false).n)\n\tprint(Limits(9223372036854775807, true).n)\n\treturn 0\n', b'-9223372036854775808\n9223372036854775807\n'),
	]
	invalid = [
		'rec R(x: string)\ndef missing() -> R\n\tprint("no return")\ndef main() -> i64\n\treturn 0\n',
		'rec Wide(x: i64, y: i64)\nrec Small(x: i64)\ndef main() -> i64\n\tlet s: Small = Wide(1, 2)\n\treturn 0\n',
		'rec Wide(x: i64, y: i64)\nrec Small(x: i64)\ndef use(s: Small) -> i64\n\treturn s.x\ndef main() -> i64\n\treturn use(Wide(1, 2))\n',
		'rec Wide(x: i64, y: i64)\nrec Small(x: i64)\ndef bad() -> Small\n\treturn Wide(1, 2)\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: i64, x: string)\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: i64)\nrec R(y: i64)\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: i64)\ndef R() -> i64\n\treturn 0\ndef main() -> i64\n\treturn 0\n',
		'rec main(x: i64)\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: Optional[i64])\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: R)\ndef main() -> i64\n\treturn 0\n',
		'def main() -> i64\n\tlet x: Missing = 1\n\treturn 0\n',
		'def missing(x: Missing) -> i64\n\treturn 0\ndef main() -> i64\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tlet r = R()\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tlet r = R(true)\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tlet R = 1\n\tlet r = R(2)\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\treturn R(1).missing\n',
		'def main() -> i64\n\treturn 1.x\n',
		'rec A(x: i64)\nrec B(y: i64)\ndef main() -> i64\n\tlet b: B = A(1)\n\treturn 0\n',
		'rec A(x: i64)\nrec B(x: string)\ndef main() -> i64\n\tlet b: B = A(1)\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tlet r = R(1)\n\tr = R(2)\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tvar r = R(1)\n\tr.x = 2\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tprint(R(1))\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tprint(str(R(1)))\n\treturn 0\n',
		'rec R(x: i64)\ndef main() -> i64\n\tprint(R(1) == R(1))\n\treturn 0\n',
	]
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for module in modules:
			(root / module.name).write_bytes(module.read_bytes())
		(root / 'Main.sirius').write_bytes((ROOT / 'rewrite/main.sirius').read_bytes())
		library = root / 'compiler.smirl'
		execute([str(runner), '--build-modules', str(root), 'Main', str(library)])
		(root / 'Check.sirius').write_bytes(driver.read_bytes())
		check_library = root / 'check.smirl'
		execute([str(runner), '--build-modules', str(root), 'Check', str(check_library)])
		result = run([str(runner), '--entry', 'M_5_Check_4_main', '--load-lib', str(check_library)])
		assert result.returncode == 0 and not result.stderr, result
		audit_runtime = root / 'audit-runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit_runtime)])
		audited = executable_path(root / 'audited-ir')
		compile_c(cache.cc, verifier.parent / 'generated.c', audited, audit_runtime)
		result = run([str(audited)])
		assert result.returncode == 0 and not result.stderr, result
		for command in [[str(verifier)], [str(runner), '--entry', 'M_5_Check_4_main', '--load-lib', str(check_library)], [str(audited)]]:
			result = run(command + ['--trap'])
			assert result.returncode == 0 and not result.stdout and normalize_newlines(result.stderr) == b'ERROR: division by zero\n', result
		print('PASS record interning, typed IR copies, initialization, and audited interpreter frames', flush=True)
		commands = [[str(compiler)], [str(runner), '--entry', 'M_4_Main_4_main', '--load-lib', str(library)]]
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
			assert outputs[0] == outputs[1], index
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
		# Fatal native traps and interpreter errors must propagate through calls
		# whose parameters own references to managed record fields.
		source = root / 'trap.sirius'
		source.write_text('rec R(text: string)\ndef fail(r: R) -> i64\n\treturn 1 / 0\ndef main() -> i64\n\tprint(fail(R("owned")))\n\treturn 0\n', encoding='utf-8')
		for command in commands:
			result = run(command + ['--run', str(source)])
			assert result.returncode != 0 and not result.stdout and b'division by zero' in result.stderr, result
			generated = root / 'trap.c'
			execute(command + [str(source), str(generated)])
			binary = executable_path(root / 'trap')
			compile_c(cache.cc, generated, binary, cache.runtime())
			result = run([str(binary)])
			assert result.returncode != 0 and not result.stdout and b'division by zero' in result.stderr, result
	print(f'PASS rewrite records: {len(cases)} valid (ARC audited), {len(invalid)} invalid; native and MIR', flush=True)


if __name__ == '__main__':
	main()
