#!/usr/bin/env python3
"""Exercise the rewrite compiler natively and through the MIR interpreter."""
import subprocess
import re
import tempfile
from pathlib import Path

from build_support import BuildCache, compile_c, execute, require_cc
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
			'return 0\n\tprint(1)', 'return 1 / 2', 'return (1 + )', 'return @',
			'return unknown', 'let x = 1\n\tx = 2\n\treturn 0', 'var x: bool = 1\n\treturn 0',
			'let x = x\n\treturn 0', 'let x = 4\n\tlet x = x + 2\n\treturn 0', 'return ~1']
		bindings = 'var x0 = 1\n' + ''.join(f'\tlet x{i} = x{i - 1} + 1\n' for i in range(1, 40))
		valid += [
			('print(true)\n\tprint(false)\n\tprint(not false)\n\tprint(not not true)\n\tprint(2 < 3)\n\treturn 0', b'true\nfalse\ntrue\ntrue\ntrue\n'),
			('var flag = false\n\twhile not flag\n\t\tflag = not flag\n\tprint(flag)\n\tprint(not (3 >= 2))\n\treturn 0', b'true\nfalse\n'),
		]
		invalid += ['return not 1', 'let x: i64 = not false\n\treturn 0', 'print(-true)\n\treturn 0']
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
		for index, (body, expected) in enumerate(valid):
			source = root / 'input.sirius'
			source.write_text('def main() -> i64\n\t' + body + '\n', encoding='utf-8')
			outputs = []
			for mode, command in enumerate(commands):
				interpreted = run(command + ['--run', str(source)])
				assert interpreted.returncode == 0 and normalize_newlines(interpreted.stdout) == expected, (index, mode, interpreted)
				generated = root / f'out{mode}.c'
				result = run(command + [str(source), str(generated)])
				assert result.returncode == 0, (index, mode, result.stderr)
				outputs.append(generated.read_bytes())
				binary = executable_path(root / f'program{mode}')
				compile_c(cache.cc, generated, binary, cache.runtime())
				executed = run([str(binary)])
				assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected, (index, mode, executed)
			assert outputs[0] == outputs[1], index
			interpreted = run([str(runner), str(source)])
			assert interpreted.returncode == 0 and normalize_newlines(interpreted.stdout) == expected, (index, interpreted.stderr, interpreted.stdout)
			reference_c = root / 'reference.c'
			result = run([str(reference), str(source), str(reference_c)])
			assert result.returncode == 0, (index, result.stdout, result.stderr)
			reference_binary = executable_path(root / 'reference')
			compile_c(cache.cc, reference_c, reference_binary, cache.runtime())
			executed = run([str(reference_binary)])
			assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected, (index, executed)
		for body in invalid:
			source = root / 'input.sirius'
			source.write_text('def main() -> i64\n\t' + body + '\n', encoding='utf-8')
			for mode, command in enumerate(commands):
				interpreted = run(command + ['--run', str(source)])
				assert interpreted.returncode == 1 and b'ERROR ' in interpreted.stderr, (body, mode, interpreted)
				generated = root / f'invalid{mode}.c'
				result = run(command + [str(source), str(generated)])
				assert result.returncode == 1 and b'ERROR ' in result.stderr and not generated.exists(), (body, mode, result.stderr)
		print(f'PASS rewrite compiler: {len(valid)} valid and {len(invalid)} invalid programs, native and MIR')
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
