#!/usr/bin/env python3
"""Exercise the rewrite compiler natively and through the MIR interpreter."""
import subprocess
import re
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, executable_path, normalize_newlines


def run(args: list[str]) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run(args, capture_output=True, timeout=60)


def main() -> None:
	cache = BuildCache(require_cc())
	modules = tuple(ROOT / 'rewrite' / name for name in ('lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius', 'lower.sirius', 'interpreter.sirius', 'emit_c.sirius'))
	compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
	reference = cache.native_tool('modular_cli')
	runner = cache.mir_runner()
	verifier = cache.native_driver('restart-ir-verifier', ROOT / 'rewrite/ir_test_driver.sirius', extra_modules=modules)
	verified = run([str(verifier)])
	assert verified.returncode == 0, verified
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for module in modules:
			(root / module.name).write_bytes(module.read_bytes())
		(root / 'Main.sirius').write_bytes((ROOT / 'rewrite/main.sirius').read_bytes())
		library = root / 'compiler.smirl'
		execute([str(runner), '--build-modules', str(root), 'Main', str(library)])
		(root / 'Check.sirius').write_bytes((ROOT / 'rewrite/ir_test_driver.sirius').read_bytes())
		check_library = root / 'check.smirl'
		execute([str(runner), '--build-modules', str(root), 'Check', str(check_library)])
		verified = run([str(runner), '--entry', 'M_5_Check_4_main', '--load-lib', str(check_library)])
		assert verified.returncode == 0, verified
		print('PASS IR verifier: malformed slots, initialization, return placement, in-place updates; native and MIR')
		commands = [[str(compiler)], [str(runner), '--entry', 'M_4_Main_4_main', '--load-lib', str(library)]]
		valid = [
			('return 0', b''),
			('print(1 + 2 * 3)\n\tprint((1 + 2) * 3)\n\treturn 0', b'7\n9\n'),
			('print(9 - 3 - 2)\n\tprint(-2 * 4)\n\treturn 0', b'4\n-8\n'),
			('print(9223372036854775807 + 1)\n\tprint(-9223372036854775807 - 2)\n\treturn 0', b'-9223372036854775808\n9223372036854775807\n'),
			('print(00009)\n\treturn 0', b'9\n'),
			('let x: i64 = 3\n\tvar y = x + 1\n\ty = y * 2\n\tprint(y)\n\treturn 0', b'8\n'),
			('let unused = 1\n\tvar changed = 2\n\tchanged = 3\n\treturn 0', b''),
			('print(1 | 2 & 4)\n\tprint(7 & 1 + 2)\n\tprint(-1 & 7)\n\treturn 0', b'1\n3\n7\n'),
			('var bits = 15\n\tbits = bits & 12\n\tprint(bits | 1)\n\treturn 0', b'13\n'),
			('var i = 0\n\tvar total = 0\n\twhile i < 5\n\t\ttotal = total + i\n\t\ti = i + 1\n\tprint(total)\n\treturn 0', b'10\n'),
			('let take: bool = false\n\tvar x = 1\n\tif take\n\t\tx = 2\n\telse\n\t\tx = 3\n\tprint(x)\n\treturn 0', b'3\n'),
			('let x = 4\n\tif true\n\t\tlet x = 7\n\t\tprint(x)\n\telse\n\t\tlet x = 8\n\t\tprint(x)\n\tprint(x)\n\treturn 0', b'7\n4\n'),
			('var i = 0\n\twhile i < 3\n\t\tif i == 1\n\t\t\tprint(9)\n\t\telse\n\t\t\tprint(i)\n\t\ti = i + 1\n\twhile false\n\t\tprint(99)\n\treturn 0', b'0\n9\n2\n'),
			('var i = 0\n\tvar total = 0\n\twhile i < 2\n\t\tvar j = 0\n\t\twhile j < 3\n\t\t\ttotal = total + 1\n\t\t\tj = j + 1\n\t\ti = i + 1\n\tprint(total)\n\treturn 0', b'6\n'),
			('var active: bool = true\n\twhile active\n\t\tprint(7)\n\t\tactive = false\n\tif active == false\n\t\tprint(8)\n\tif -1 >= 0\n\t\tprint(99)\n\treturn 0', b'7\n8\n'),
		]
		invalid = ['return true', 'return 9223372036854775808', 'print(1)',
			'return 0\n\tprint(1)', 'return (1 + )', 'return @',
			'return unknown', 'let x = 1\n\tx = 2\n\treturn 0', 'var x: bool = 1\n\treturn 0',
			'let x = x\n\treturn 0', 'let x = 4\n\tlet x = x + 2\n\treturn 0', 'return ~1']
		bindings = 'var x0 = 1\n' + ''.join(f'\tlet x{i} = x{i - 1} + 1\n' for i in range(1, 40))
		valid += [
			('print(false and false)\n\tprint(false and true)\n\tprint(true and false)\n\tprint(true and true)\n\treturn 0', b'false\nfalse\nfalse\ntrue\n'),
			('print(false or false)\n\tprint(false or true)\n\tprint(true or false)\n\tprint(true or true)\n\treturn 0', b'false\ntrue\ntrue\ntrue\n'),
			('print(true or false and false)\n\tprint((true or false) and false)\n\tprint(false or 2 < 3 and not false)\n\tprint(false or false or true)\n\tprint(true and true and false)\n\treturn 0', b'true\nfalse\ntrue\ntrue\nfalse\n'),
			('var i = 0\n\twhile i < 3 and not (i == 2 or i == 4)\n\t\ti = i + 1\n\tif false or i == 2\n\t\tprint(i)\n\telse\n\t\treturn 1\n\treturn 0', b'2\n'),
			('var flag = true\n\tflag = flag and (false or true)\n\tlet copy: bool = false or flag\n\tprint(copy)\n\treturn 0', b'true\n'),
			('print(true)\n\tprint(false)\n\tprint(not false)\n\tprint(not not true)\n\tprint(2 < 3)\n\treturn 0', b'true\nfalse\ntrue\ntrue\ntrue\n'),
			('var flag = false\n\twhile not flag\n\t\tflag = not flag\n\tprint(flag)\n\tprint(not (3 >= 2))\n\treturn 0', b'true\nfalse\n'),
		]
		invalid += ['return not 1', 'let x: i64 = not false\n\treturn 0', 'print(-true)\n\treturn 0']
		invalid += ['print(1 and true)\n\treturn 0', 'print(false or 1)\n\treturn 0',
			'print(false and unknown)\n\treturn 0', 'print(true or 1)\n\treturn 0',
			'print(true and)\n\treturn 0', 'return true and false']
		invalid += ['if 1\n\t\tprint(1)\n\treturn 0', 'if true\n\t\tlet local = 1\n\treturn local',
			'var flag = true\n\tflag = 1\n\treturn 0', 'return true + 1', 'if 1 < true\n\t\tprint(0)\n\treturn 0',
			'while false\n\t\tlet local = 1\n\treturn local']
		valid += [
			('if true\n\t\tprint(7)\n\t\treturn 0\n\tprint(99)\n\treturn 0', b'7\n'),
			('if false\n\t\treturn 0\n\telse\n\t\tprint(8)\n\t\treturn 0', b'8\n'),
			('var i = 0\n\twhile i < 5\n\t\ti = i + 1\n\t\tif i == 2\n\t\t\tcontinue\n\t\tif i == 4\n\t\t\tbreak\n\t\tprint(i)\n\tprint(i)\n\treturn 0', b'1\n3\n4\n'),
			('var i = 0\n\twhile i < 2\n\t\twhile true\n\t\t\tbreak\n\t\ti = i + 1\n\tprint(i)\n\treturn 0', b'2\n'),
			('while true\n\t\tprint(9)\n\t\treturn 0\n\treturn 0', b'9\n'),
		]
		invalid += ['break\n\treturn 0', 'continue\n\treturn 0',
			'if true\n\t\treturn 0', 'while false\n\t\treturn 0',
			'while true\n\t\tbreak\n\t\tprint(1)\n\treturn 0', 'if false\n\t\treturn true\n\treturn 0']
		valid.append((bindings + '\tx0 = x39\n\tprint(x0)\n\tprint(x20)\n\treturn 0', b'40\n21\n'))
		valid += [
			('var i = 0\n\twhile i < 5\n\t\tif i == 0\n\t\t\tprint(10)\n\t\telif i == 1\n\t\t\tprint(11)\n\t\telif i == 2 or i == 3\n\t\t\tprint(12)\n\t\telse\n\t\t\tprint(14)\n\t\ti = i + 1\n\treturn 0', b'10\n11\n12\n12\n14\n'),
			('var x = 4\n\tif false\n\t\tx = 1\n\telif false\n\t\tx = 2\n\tprint(x)\n\treturn 0', b'4\n'),
			('let x = 9\n\tif false\n\t\tlet x = 1\n\telif x == 9\n\t\tlet x = 2\n\t\tprint(x)\n\telif true\n\t\tlet x = 3\n\telse\n\t\tlet x = 4\n\tprint(x)\n\treturn 0', b'2\n9\n'),
			('if false\n\t\treturn 0\n\telif true\n\t\tprint(7)\n\t\treturn 0\n\telse\n\t\treturn 0', b'7\n'),
			('if true\n\t\tif false\n\t\t\tprint(1)\n\t\telif true\n\t\t\tprint(2)\n\telif true\n\t\tprint(3)\n\treturn 0', b'2\n'),
			('var i = 0\n\twhile i < 5\n\t\ti = i + 1\n\t\tif i == 1\n\t\t\tcontinue\n\t\telif i == 3\n\t\t\tbreak\n\t\telse\n\t\t\tprint(i)\n\tprint(i)\n\treturn 0', b'2\n3\n'),
		]
		# A long chain should use an arena and a list of pending exits, not
		# parser recursion proportional to the number of elif clauses.
		chain = 'if false\n\t\tprint(99)\n' + ''.join(
			f'\telif {i} == 79\n\t\tprint({i})\n' for i in range(80))
		valid.append((chain + '\telse\n\t\tprint(100)\n\treturn 0', b'79\n'))
		invalid += [
			'elif true\n\t\treturn 0\n\treturn 0',
			'if false\n\t\treturn 0\n\telif 1\n\t\treturn 0\n\treturn 0',
			'if false\n\t\tlet local = 1\n\telif local == 1\n\t\treturn 0\n\treturn 0',
			'if false\n\t\treturn 0\n\telif true\n\t\tlet local = 1\n\treturn local',
			'if false\n\t\treturn 0\n\telif true\n\t\treturn 0',
			'if true\n\t\treturn 0\n\telse\n\t\treturn 0\n\telif true\n\t\treturn 0',
			'if false\n\t\treturn 0\n\telif\n\t\treturn 0\n\treturn 0',
		]
		valid += [
			('var total = 0\n\tfor i in -2..3\n\t\ttotal = total + i\n\tprint(total)\n\treturn 0', b'0\n'),
			('for i in 3..3\n\t\tprint(99)\n\tfor i in 4..2\n\t\tprint(99)\n\treturn 0', b''),
			('for i in 0..5\n\t\tif i == 1\n\t\t\tcontinue\n\t\telif i == 3\n\t\t\tbreak\n\t\tprint(i)\n\treturn 0', b'0\n2\n'),
			('var end = 3\n\tvar start = 0\n\tfor i in start..end\n\t\tend = 0\n\t\tstart = 9\n\t\tprint(i)\n\tprint(end)\n\treturn 0', b'0\n0\n'),
			('var end = 3\n\tfor i in 0..(end + 0)\n\t\tend = 0\n\t\tprint(i)\n\treturn 0', b'0\n1\n2\n'),
			('let i = 8\n\tfor i in i..(i + 2)\n\t\tprint(i)\n\tprint(i)\n\treturn 0', b'8\n9\n8\n'),
			('for i in 0..2\n\t\tlet i = i + 10\n\t\tprint(i)\n\treturn 0', b'10\n11\n'),
			('var total = 0\n\tfor i in 0..3\n\t\tfor j in 0..3\n\t\t\tif j == 1\n\t\t\t\tcontinue\n\t\t\tif i == 2\n\t\t\t\tbreak\n\t\t\ttotal = total + 1\n\tprint(total)\n\treturn 0', b'4\n'),
			('for i in 0..2\n\t\tvar j = 0\n\t\twhile j < 2\n\t\t\tj = j + 1\n\t\t\tcontinue\n\t\tprint(i)\n\tvar n = 0\n\twhile n < 2\n\t\tfor i in 0..2\n\t\t\tbreak\n\t\tn = n + 1\n\tprint(n)\n\treturn 0', b'0\n1\n2\n'),
			('for i in 9223372036854775806..9223372036854775807\n\t\tprint(i)\n\tfor i in (-9223372036854775807 - 1)..-9223372036854775807\n\t\tprint(i)\n\treturn 0', b'9223372036854775806\n-9223372036854775808\n'),
			('for i in 0..2\n\t\tprint(i)\n\t\treturn 0\n\treturn 0', b'0\n'),
			('for i in 0..2\n\t\tlet local = i + 4\n\t\tprint(local)\n\tlet local = 9\n\tprint(local)\n\treturn 0', b'4\n5\n9\n'),
		]
		invalid += [
			'for i in true..3\n\t\tprint(i)\n\treturn 0',
			'for i in 0..false\n\t\tprint(i)\n\treturn 0',
			'for i in 0..3\n\t\ti = 2\n\treturn 0',
			'for i in 0..3\n\t\tprint(i)\n\treturn i',
			'for i in i..3\n\t\tprint(i)\n\treturn 0',
			'for i in 0..3\n\t\tlet local = i\n\treturn local',
			'for i in 0..3\n\t\treturn 0',
			'for i in 3\n\t\tprint(i)\n\treturn 0',
			'for i in 0..\n\t\tprint(i)\n\treturn 0',
			'for in 0..3\n\t\treturn 0\n\treturn 0',
		]
		valid += [
			('print(7 / 3)\n\tprint(-7 / 3)\n\tprint(7 / -3)\n\tprint(-7 / -3)\n\treturn 0', b'2\n-2\n-2\n2\n'),
			('print(7 % 3)\n\tprint(-7 % 3)\n\tprint(7 % -3)\n\tprint(-7 % -3)\n\treturn 0', b'1\n-1\n1\n-1\n'),
			('print(24 / 3 * 2)\n\tprint(20 % 6 * 3 + 1)\n\tprint(20 / 2 / 2)\n\tprint(20 % 6 % 3)\n\treturn 0', b'16\n7\n5\n2\n'),
			('let minimum = -9223372036854775807 - 1\n\tprint(minimum / 1)\n\tprint(minimum % 1)\n\tprint(9223372036854775807 / -1)\n\treturn 0', b'-9223372036854775808\n0\n-9223372036854775807\n'),
			('print(false and (1 / 0 == 0))\n\tprint(true or (1 % 0 == 0))\n\tprint(true or ((-9223372036854775807 - 1) / -1 == 0))\n\treturn 0', b'false\ntrue\ntrue\n'),
			('var total = 0\n\tfor i in 0..6\n\t\tif i % 2 == 0\n\t\t\ttotal = total + i / 2\n\tprint(total)\n\treturn 0', b'3\n'),
		]
		invalid += ['print(true / 2)\n\treturn 0', 'print(2 % false)\n\treturn 0',
			'print(1 /)\n\treturn 0', 'let x: bool = 1 % 2\n\treturn 0']
		valid += [
			('print(1 << 2 + 1)\n\tprint(32 >> 1 + 2)\n\tprint(16 >> 1 >> 1)\n\tprint(1 << 2 == 4)\n\tprint(8 >> 1 & 3)\n\treturn 0', b'8\n4\n4\ntrue\n0\n'),
			('print(-3 >> 1)\n\tprint(-1 >> 0)\n\tprint(1 << 63)\n\tprint((-9223372036854775807 - 1) >> 63)\n\tprint((-9223372036854775807 - 1) << 1)\n\tprint(9223372036854775807 << 1)\n\treturn 0', b'9223372036854775806\n-1\n-9223372036854775808\n1\n0\n-2\n'),
			('var bits = 1\n\tfor i in 0..4\n\t\tbits = bits << 1\n\tprint(bits >> 2)\n\treturn 0', b'4\n'),
			('print(false and (1 << -1 == 0))\n\tprint(true or (1 >> 64 == 0))\n\treturn 0', b'false\ntrue\n'),
			('for count in 0..64\n\t\tprint(-1 >> count)\n\t\tprint((1 << count) >> count)\n\treturn 0', ''.join(f'{-1 if count == 0 else ((1 << 64) - 1) >> count}\n1\n' for count in range(64)).encode()),
		]
		invalid += ['print(true << 1)\n\treturn 0', 'print(1 >> false)\n\treturn 0',
			'print(1 <<)\n\treturn 0', 'let x: bool = 1 >> 2\n\treturn 0']
		valid += [
			('var x = 10\n\tx += 2 * 3\n\tprint(x)\n\tx -= 1 + 2\n\tprint(x)\n\tx *= 2 + 1\n\tprint(x)\n\tx /= 2 + 1\n\tprint(x)\n\treturn 0', b'16\n13\n39\n13\n'),
			('var x = 3\n\tx += x * 2\n\tprint(x)\n\tx -= x / 3\n\tprint(x)\n\tx *= x - 4\n\tprint(x)\n\tx /= x / 3\n\tprint(x)\n\treturn 0', b'9\n6\n12\n3\n'),
			('var x = 9223372036854775807\n\tx += 1\n\tprint(x)\n\tx -= 1\n\tprint(x)\n\tx *= 2\n\tprint(x)\n\tx /= -2\n\tprint(x)\n\treturn 0', b'-9223372036854775808\n9223372036854775807\n-2\n1\n'),
			('var total = 0\n\tfor i in 0..6\n\t\tif i % 2 == 0\n\t\t\tcontinue\n\t\ttotal += i\n\twhile total > 5\n\t\ttotal -= 2\n\tprint(total)\n\treturn 0', b'5\n'),
			('var x = 2\n\tif true\n\t\tvar x = 8\n\t\tx /= -3\n\t\tprint(x)\n\tx *= 3\n\tprint(x)\n\treturn 0', b'-2\n6\n'),
		]
		invalid += ['let x = 1\n\tx += 2\n\treturn 0', 'x += 1\n\treturn 0',
			'var x = true\n\tx += false\n\treturn 0', 'var x = 1\n\tx *= true\n\treturn 0',
			'var x = 1\n\tx +=\n\treturn 0', 'var x = 1\n\tx %= 2\n\treturn 0',
			'var x = 1\n\tx &= 2\n\treturn 0', 'for i in 0..2\n\t\ti += 1\n\treturn 0']
		valid += [
			('true else return 1 / 0\n\tprint(7)\n\treturn 0', b'7\n'),
			('false else return 0\n\tprint(99)\n\treturn 0', b''),
			('let ready = true\n\tready and (2 < 3) else return 1\n\t(3 > 4) or ready else return 2\n\tprint(8)\n\treturn 0', b'8\n'),
			('var total = 0\n\tfor i in 0..6\n\t\ti != 1 else continue\n\t\ti < 4 else break\n\t\ttotal += i\n\tprint(total)\n\treturn 0', b'5\n'),
			('var i = 0\n\tvar total = 0\n\twhile i < 6\n\t\ti += 1\n\t\ti != 2 else continue\n\t\ti < 5 else break\n\t\ttotal += i\n\tprint(total)\n\treturn 0', b'8\n'),
			('var total = 0\n\tfor i in 0..3\n\t\tfor j in 0..4\n\t\t\tj != 1 else continue\n\t\t\tj < 3 else break\n\t\t\ttotal += 1\n\tprint(total)\n\treturn 0', b'6\n'),
			('if true\n\t\tlet ready = false\n\t\tready else return 0\n\telse\n\t\treturn 0\n\treturn 0', b''),
			('false or true else return 1\n\tlet ready = true\n\tready else return 2\n\tvar x = 0\n\tx += 3\n\tx == 3 else return 3\n\tprint(x)\n\treturn 0', b'3\n'),
		]
		invalid += ['1 else return 0\n\treturn 0', 'unknown else return 0\n\treturn 0',
			'true else break\n\treturn 0', 'true else continue\n\treturn 0',
			'false else print(1)\n\treturn 0', 'true else return false\n\treturn 0',
			'true else return 0', 'true else\n\treturn 0',
			'false else return\n\treturn 0', 'for i in 0..2\n\t\ttrue else break 1\n\treturn 0']
		string_cases = [
			('let text: string = "hello"\n\tprint(text + " world")\n\tprint(len(text))\n\tprint(\'single quotes\')\n\treturn 0', b'hello world\n5\nsingle quotes\n'),
			('print("A\\0Z")\n\tprint(len("A\\0Z"))\n\tprint(len("é"))\n\tprint("\\n\\t\\r\\\\\\\"\\\'")\n\treturn 0', b'A\0Z\n3\n2\n\n\t\r\\"\'\n'),
			('print("a" < "b")\n\tprint("a" <= "a")\n\tprint("b" > "a")\n\tprint("b" >= "b")\n\tprint("a" == "a")\n\tprint("a" != "b")\n\tprint("a\\0b" != "a\\0c")\n\treturn 0', b'true\n' * 7),
			('var text = "a"\n\tlet saved = text\n\ttext += text\n\ttext = text\n\tprint(saved)\n\tprint(text)\n\ttext = "new"\n\tprint(saved)\n\tprint(text)\n\treturn 0', b'a\naa\na\nnew\n'),
			('var text = ""\n\tfor i in 0..20\n\t\ttext += "x"\n\tprint(len(text))\n\tif text == ""\n\t\ttext = "wrong"\n\telif len(text) == 20\n\t\ttext = "ok"\n\tprint(text)\n\treturn 0', b'20\nok\n'),
			('var text = "outer"\n\tif true\n\t\tlet text = "inner"\n\t\tprint(text)\n\tprint(text)\n\ttext != "outer" else return 0\n\treturn 0', b'inner\nouter\n'),
			('print("")\n\tprint(len(("ab" + "cd") + "é"))\n\tprint(false and ("a" == "b"))\n\treturn 0', b'\n6\nfalse\n'),
		]
		string_cases += [
			('let text = "hello world"\n\tprint(text.slice(0, 5))\n\tprint(text.slice(6, len(text)))\n\tprint(len(text.slice(0, 0)))\n\tprint(len(text.slice(len(text), len(text))))\n\treturn 0', b'hello\nworld\n0\n0\n'),
			('print("AéZ".slice(1, 3))\n\tprint(len("AéZ".slice(1, 2)))\n\tprint("a\\0bc".slice(1, 3))\n\tprint("a\\0bc".starts_with("a\\0"))\n\tprint("a\\0bc".ends_with("\\0bc"))\n\treturn 0', 'é\n1\n\0b\ntrue\ntrue\n'.encode()),
			('print("".starts_with(""))\n\tprint("".ends_with(""))\n\tprint("a".starts_with("ab"))\n\tprint("a".ends_with("ab"))\n\tprint("abc".starts_with("b"))\n\tprint("abc".ends_with("b"))\n\treturn 0', b'true\ntrue\nfalse\nfalse\nfalse\nfalse\n'),
			('let text = "abcd"\n\tprint((text + "!").slice(1, 4).ends_with("d"))\n\tprint(text.slice(1, 3).slice(0, 1) + "x")\n\tprint(not text.starts_with("z"))\n\treturn 0', b'true\nbx\ntrue\n'),
			('var text = "abcd"\n\tlet saved = text\n\tfor i in 0..4\n\t\ttext = text.slice(1, len(text))\n\tprint(len(text))\n\tprint(saved)\n\treturn 0', b'0\nabcd\n'),
			('print(true or "abc".slice(-1, 2).starts_with("a"))\n\tprint(false and len("abc".slice(0, 4)) == 0)\n\treturn 0', b'true\nfalse\n'),
		]
		string_cases += [
			('print(str(0))\n\tprint(str(-3))\n\tprint(str(9223372036854775807))\n\tprint(str(-9223372036854775807 - 1))\n\treturn 0', b'0\n-3\n9223372036854775807\n-9223372036854775808\n'),
			('print(str(1234).slice(1, 3))\n\tprint(str(-42).starts_with("-"))\n\tprint("value=" + str(6 * 7))\n\treturn 0', b'23\ntrue\nvalue=42\n'),
			('var text = str(0)\n\tlet saved = text\n\tfor i in 0..4\n\t\ttext = str(i)\n\tprint(text)\n\tprint(saved)\n\treturn 0', b'3\n0\n'),
		]
		string_cases += [
			('print("abc".byte_at(0) or -1)\n\tprint("abc".byte_at(-1) or -1)\n\tprint("abc".byte_at(3) or -1)\n\tprint("".byte_at(0) or -1)\n\treturn 0', b'97\n-1\n-1\n-1\n'),
			('print("Aé\\0".byte_at(1) or -1)\n\tprint("Aé\\0".byte_at(2) or -1)\n\tprint("Aé\\0".byte_at(3) or 99)\n\treturn 0', b'195\n169\n0\n'),
			('print("abcabc".find("bc") or -1)\n\tprint("abc".find("a") or 99)\n\tprint("abc".find("z") or -1)\n\tprint("".find("") or 99)\n\tprint("abc".find("") or 99)\n\treturn 0', b'1\n0\n-1\n0\n0\n'),
			('print("a\\0b".find("\\0b") or -1)\n\tprint("éx".find("x") or -1)\n\tprint(("ab" + "cd").slice(1, 4).find("c") or -1)\n\treturn 0', b'1\n2\n1\n'),
			('var found: Optional[i64] = "abc".find("a")\n\tlet saved = found\n\tfound = "abc".find("z")\n\tprint(saved or 99)\n\tprint(found or -1)\n\tfound = saved\n\tprint(found or 99)\n\treturn 0', b'0\n-1\n0\n'),
			('print("x".find("x") or (1 / 0))\n\tprint("\\0".byte_at(0) or len("x".slice(-1, 2)))\n\tprint(false or ("x".find("x") or -1) == 0)\n\treturn 0', b'0\n0\ntrue\n'),
			('var found = "".byte_at(0)\n\tfor i in 0..3\n\t\tfound = "abc".byte_at(i)\n\t\tprint(found or -1)\n\tprint("".find("x") or ("abc".find("b") or -1))\n\treturn 0', b'97\n98\n99\n1\n'),
		]
		invalid += [
			'print("x".byte_at(true) or 0)\n\treturn 0', 'print(1.find("x") or 0)\n\treturn 0',
			'print("x".find(1) or 0)\n\treturn 0', 'print("x".byte_at())\n\treturn 0',
			'print("x".find("x", "y"))\n\treturn 0', 'print("x".find("x"))\n\treturn 0',
			'eprint("x".byte_at(0))\n\treturn 0', 'return "x".find("x")',
			'print("x".find("x") or false)\n\treturn 0', 'print("x".find("x") and true)\n\treturn 0',
			'print("x".find("x") == "x".find("x"))\n\treturn 0', 'print("x".find("x") or unknown)\n\treturn 0',
			'var found = "x".find("x")\n\tfound = 1\n\treturn 0', 'let x: Optional[string] = "x".find("x")\n\treturn 0',
			'let x: i64 = "x".find("x")\n\treturn 0', 'let x: Optional[i64] = 1\n\treturn 0',
		]
		string_cases += [
			('print("abc".find("b")!)\n\tprint("\\0".byte_at(0)! + 1)\n\tprint(str("abc".byte_at(2)!).slice(0, 2))\n\treturn 0', b'1\n1\n99\n'),
			('let byte: i64 = "abc".byte_at(0) else return 1\n\tvar position = "abc".find("b") else return 1\n\tposition += 1\n\tprint(byte)\n\tprint(position)\n\treturn 0', b'97\n2\n'),
			('let byte = "\\0".byte_at(0) else return 1 / 0\n\tprint(byte)\n\treturn 0', b'0\n'),
			('let kept = "alive"\n\tprint(kept)\n\tlet byte = "".byte_at(0) else return len(kept) - 5\n\tprint(byte)\n\treturn 1', b'alive\n'),
			('let position = 8\n\tif true\n\t\tlet position = "".find("x") else return position - 8\n\t\tprint(position)\n\treturn 1', b''),
			('var total = 0\n\tfor i in 0..3\n\t\tlet byte = "abc".byte_at(i) else return 1\n\t\ttotal += byte\n\tprint(total)\n\treturn 0', b'294\n'),
			('var i = 0\n\twhile i < 3\n\t\tlet byte = "a".byte_at(i) else return 0\n\t\tprint(byte)\n\t\ti += 1\n\treturn 1', b'97\n'),
			('print(true or "".byte_at(0)! == 0)\n\tprint(false and "".find("x")! == 0)\n\treturn 0', b'true\nfalse\n'),
		]
		invalid += [
			'print(1!)\n\treturn 0', 'print("x"!)\n\treturn 0', 'print("x".find("x")!!)\n\treturn 0',
			'let x = 1 else return 0\n\treturn 0', 'var x = true else return 0\n\treturn 0',
			'let x: bool = "x".find("x") else return 0\n\treturn 0',
			'let x = "x".find("x") else return true\n\treturn 0',
			'let x = "x".find("x") else return unknown\n\treturn 0',
			'let x = "x".find("x") else return x\n\treturn 0',
			'let x = "x".find("x") else break\n\treturn 0',
			'let x = "x".find("x") else return\n\treturn 0',
			'let x = "x".find("x") else return 0',
			'let x = "x".find("x") else return 0\n\tx = 2\n\treturn 0',
		]
		valid += string_cases
		valid.append(('let len = 3\n\tlet str = 4\n\tprint(len)\n\tprint(str)\n\treturn 0', b'3\n4\n'))
		valid.append(('var eprint = 5\n\teprint += 1\n\tprint(eprint)\n\treturn 0', b'6\n'))
		invalid += ['return "x"', 'let text: string = 1\n\treturn 0',
			'var text = "x"\n\ttext = 1\n\treturn 0', 'print("a" + 1)\n\treturn 0',
			'print("a" - "b")\n\treturn 0', 'print("a" == 1)\n\treturn 0',
			'if "x"\n\t\treturn 0\n\treturn 0', 'print(not "x")\n\treturn 0',
			'print(len(1))\n\treturn 0', 'print("\\q")\n\treturn 0',
			'let text = "x"\n\ttext += "y"\n\treturn 0',
			'for i in "a".."z"\n\t\tprint(i)\n\treturn 0']
		invalid += ['print(1.slice(0, 1))\n\treturn 0', 'print("x".slice(true, 1))\n\treturn 0',
			'print("x".slice(0, "1"))\n\treturn 0', 'print("x".starts_with(1))\n\treturn 0',
			'print(false.ends_with("x"))\n\treturn 0', 'print("x".slice(0))\n\treturn 0',
			'print("x".slice(0, 1, 2))\n\treturn 0', 'print("x".ends_with())\n\treturn 0',
			'print("x".starts_with("x", "y"))\n\treturn 0', 'print("x".unknown("x"))\n\treturn 0']
		invalid += ['print(str())\n\treturn 0', 'print(str(1, 2))\n\treturn 0',
			'eprint()\n\treturn 0', 'eprint(1, 2)\n\treturn 0']
		io_cases = [
			('eprint(42)\n\teprint(-9223372036854775807 - 1)\n\teprint(true)\n\teprint(false)\n\teprint("é\\0x")\n\tprint("ok")\n\treturn 0', b'ok\n', '42\n-9223372036854775808\ntrue\nfalse\né\0x\n'.encode()),
			('for i in 0..3\n\t\teprint("item " + str(i))\n\teprint("")\n\treturn 0', b'', b'item 0\nitem 1\nitem 2\n\n'),
			('var text = "saved"\n\teprint(text)\n\ttext = str(7)\n\ttext == "7" else return 1\n\teprint(text)\n\treturn 0', b'', b'saved\n7\n'),
		]
		# Design features absent from the old compiler use independent expectations.
		interpolation_cases = [
			('print(f"")\n\tprint(f"{braces} $name $$")\n\tprint("${unknown}")\n\treturn 0', b'\n{braces} $name $$\n${unknown}\n', b''),
			('let name = "Alice"\n\tprint(f"Hello ${name}; ${6 * 7}; ${true}; ${false}")\n\tprint(f\'single ${-2}\')\n\treturn 0', b'Hello Alice; 42; true; false\nsingle -2\n', b''),
			('print(f"${"}" + "{"}:${len("ab")}:${"abc".find("b") or -1}")\n\treturn 0', b'}{:2:1\n', b''),
			('print(f"outer ${f"inner ${1 + 2}"} end")\n\tprint(f\'${f"${true}"}\')\n\treturn 0', b'outer inner 3 end\ntrue\n', b''),
			('print(f"literal \\${unknown}; ${2}; \\\\${3}")\n\tprint(f"\\n\\t${"é\\0"}Z")\n\treturn 0', 'literal ${unknown}; 2; \\3\n\n\té\0Z\n'.encode(), b''),
			('var text = "old"\n\tlet saved = text\n\tfor i in 0..4\n\t\ttext = f"${text}${i}"\n\tprint(saved)\n\tprint(text)\n\treturn 0', b'old\nold0123\n', b''),
			('print(f"${-9223372036854775807 - 1}:${9223372036854775807}")\n\tprint(f"${"\\0".byte_at(0)}:${"".byte_at(0)}")\n\treturn 0', b'-9223372036854775808:9223372036854775807\nSome(0):None\n', b''),
			('print(str(true))\n\tprint(str(false))\n\tprint(str("é\\0"))\n\tprint(str("abc".find("b")))\n\tprint(str("abc".find("z")))\n\treturn 0', 'true\nfalse\né\0\nSome(1)\nNone\n'.encode(), b''),
			('print(f"${false and 1 / 0 == 0}:${true or 1 / 0 == 0}")\n\tprint(true or len(f"${1 / 0}") == 0)\n\treturn 0', b'false:true\ntrue\n', b''),
			('print(f"${"abc".find("a") or 1 / 0}".starts_with("0"))\n\tprint(len(f"a${2}b"))\n\treturn 0', b'true\n3\n', b''),
			('let text = f"${true}"\n\teprint(f"value=${text}")\n\ttext == "true" else return 1\n\treturn 0', b'', b'value=true\n'),
			('let found = "x".find("x") else return 1\n\tprint(f"${found}")\n\treturn 0', b'0\n', b''),
		]
		invalid += ['print(f"${}")\n\treturn 0', 'print(f"${unknown}")\n\treturn 0',
			'print(f"${1 + }")\n\treturn 0', 'print(f"${1:02}")\n\treturn 0',
			'print(f"${1 2}")\n\treturn 0', 'print(f"${1")\n\treturn 0',
			'print(f"${(1]}")\n\treturn 0', 'print(f"bad\\q")\n\treturn 0',
			'print(f"${f"${unknown}"}")\n\treturn 0', 'print(f"${true + 1}")\n\treturn 0',
			'print(f"${1; 2}")\n\treturn 0', 'print(f"${1 # hidden}")\n\treturn 0',
			'print(f"${"unclosed}")\n\treturn 0', 'print("\\${name}")\n\treturn 0']
		cases = interpolation_cases + [(body, expected, b'') for body, expected in valid] + io_cases
		# Audit the generated ownership operations as well as the runtime itself.
		audit_runtime = root / 'audit-runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
			'-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit_runtime)])
		for index, (body, expected, expected_error) in enumerate(cases):
			source = root / 'input.sirius'
			source.write_text('def main() -> i64\n\t' + body + '\n', encoding='utf-8')
			outputs = []
			for mode, command in enumerate(commands):
				interpreted = run(command + ['--run', str(source)])
				assert interpreted.returncode == 0 and normalize_newlines(interpreted.stdout) == expected and normalize_newlines(interpreted.stderr) == expected_error, (index, mode, interpreted)
				generated = root / f'out{mode}.c'
				result = run(command + [str(source), str(generated)])
				assert result.returncode == 0, (index, mode, result.stderr)
				outputs.append(generated.read_bytes())
				binary = executable_path(root / f'program{mode}')
				compile_c(cache.cc, generated, binary, cache.runtime())
				executed = run([str(binary)])
				assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected and normalize_newlines(executed.stderr) == expected_error, (index, mode, executed)
				if (body, expected) in string_cases or (body, expected, expected_error) in io_cases + interpolation_cases:
					compile_c(cache.cc, generated, binary, audit_runtime)
					audited = run([str(binary)])
					assert audited.returncode == 0 and normalize_newlines(audited.stdout) == expected and normalize_newlines(audited.stderr) == expected_error, (index, mode, audited)
			assert outputs[0] == outputs[1], index
			if (body, expected, expected_error) in interpolation_cases:
				continue
			interpreted = run([str(runner), str(source)])
			assert interpreted.returncode == 0 and normalize_newlines(interpreted.stdout) == expected and normalize_newlines(interpreted.stderr) == expected_error, (index, interpreted)
			reference_c = root / 'reference.c'
			result = run([str(reference), str(source), str(reference_c)])
			assert result.returncode == 0, (index, result.stdout, result.stderr)
			reference_binary = executable_path(root / 'reference')
			compile_c(cache.cc, reference_c, reference_binary, cache.runtime())
			executed = run([str(reference_binary)])
			assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected and normalize_newlines(executed.stderr) == expected_error, (index, executed)
		for body in invalid:
			source = root / 'input.sirius'
			source.write_text('def main() -> i64\n\t' + body + '\n', encoding='utf-8')
			for mode, command in enumerate(commands):
				interpreted = run(command + ['--run', str(source)])
				assert interpreted.returncode == 1 and b'ERROR ' in interpreted.stderr, (body, mode, interpreted)
				generated = root / f'invalid{mode}.c'
				result = run(command + [str(source), str(generated)])
				assert result.returncode == 1 and b'ERROR ' in result.stderr and not generated.exists(), (body, mode, result.stderr)
		print(f'PASS rewrite compiler: {len(cases)} valid and {len(invalid)} invalid programs, native and MIR')
		print(f'PASS output ownership: {len(string_cases) + len(io_cases) + len(interpolation_cases)} cases, ARC audited')
		traps = [('1 / 0', b'division by zero'), ('1 % 0', b'division by zero'),
			('(-9223372036854775807 - 1) / -1', b'integer overflow'),
			('(-9223372036854775807 - 1) % -1', b'integer overflow'),
			('true and (1 / 0 == 0)', b'division by zero'),
			('false or (1 % 0 == 0)', b'division by zero'),
			('1 << -1', b'shift count out of range'),
			('1 >> -1', b'shift count out of range'),
			('1 << 64', b'shift count out of range'),
			('1 >> 64', b'shift count out of range'),
			('false or (1 << 64 == 0)', b'shift count out of range')]
		trap_programs = [(f'print({expression})', reason) for expression, reason in traps]
		trap_programs += [('var x = 3\n\tx /= 0', b'division by zero'),
			('var x = -9223372036854775807 - 1\n\tx /= -1', b'integer overflow'),
			('false else return 1 / 0', b'division by zero'),
			('(1 / 0 == 0) else return 0', b'division by zero'),
			('print("abc".slice(-1, 2))', b'string slice out of bounds'),
			('print("abc".slice(2, 1))', b'string slice out of bounds'),
			('print("abc".slice(0, 4))', b'string slice out of bounds'),
			('print("abc".slice(4, 4))', b'string slice out of bounds'),
			('print("".byte_at(0) or (1 / 0))', b'division by zero'),
			('print("x".find("z") or len("x".slice(-1, 2)))', b'string slice out of bounds'),
			('print("".byte_at(0)!)', b'missing optional value'),
			('print("abc".find("z")!)', b'missing optional value'),
			('let x = "".byte_at(0) else return 1 / 0\n\tprint(x)', b'division by zero')]
		interpolation_traps = [
			('print(f"prefix ${1 / 0}")', b'division by zero'),
			('print(f"${"".byte_at(0)!}")', b'missing optional value'),
			('print(f"${1 / 0}${"".byte_at(0)!}")', b'division by zero'),
			('print(f"${"".byte_at(0)!}${1 / 0}")', b'missing optional value'),
		]
		for body, reason in trap_programs + interpolation_traps:
			source = root / 'trap.sirius'
			source.write_text(f'def main() -> i64\n\t{body}\n\treturn 0\n', encoding='utf-8')
			shared = (body, reason) not in interpolation_traps
			for command in commands + ([[str(runner)]] if shared else []):
				arguments = ['--run'] if command in commands else []
				outcome = run(command + arguments + [str(source)])
				assert outcome.returncode != 0 and reason in outcome.stderr and not outcome.stdout, (body, outcome)
			for command in commands + ([[str(reference)]] if shared else []):
				generated = root / 'trap.c'
				outcome = run(command + [str(source), str(generated)])
				assert outcome.returncode == 0, outcome
				binary = executable_path(root / 'trap')
				compile_c(cache.cc, generated, binary, cache.runtime())
				outcome = run([str(binary)])
				assert outcome.returncode != 0 and reason in outcome.stderr and not outcome.stdout, (body, outcome)
		print(f'PASS arithmetic traps: {len(trap_programs) + len(interpolation_traps)} cases, native and MIR')
		examples = re.findall(r'```sirius\n(.*?)```', (ROOT / 'rewrite/FEATURES.md').read_text(encoding='utf-8'), re.S)
		for index, example in enumerate(examples):
			source = root / 'feature.sirius'
			source.write_text(example, encoding='utf-8')
			expected = ''.join(line.split('#', 1)[1].strip() + '\n' for line in example.splitlines() if 'print(' in line and '#' in line).encode()
			interpreted = run([str(runner), str(source)])
			assert interpreted.returncode == 0 and normalize_newlines(interpreted.stdout) == expected, (index, interpreted)
			generated = root / 'feature.c'
			result = run([str(reference), str(source), str(generated)])
			assert result.returncode == 0, (index, result)
			binary = executable_path(root / 'feature')
			compile_c(cache.cc, generated, binary, cache.runtime())
			executed = run([str(binary)])
			assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected, (index, executed)
		print(f'PASS feature inventory: {len(examples)} examples, native and MIR')


if __name__ == '__main__':
	main()
