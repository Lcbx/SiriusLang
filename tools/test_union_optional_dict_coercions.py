#!/usr/bin/env python3
"""Check recursive union conversion through Optional and dictionaries."""
import argparse
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc, check_native
from project_paths import ROOT


CASES = (
	('source_union_optional_dict', 15),
	('source_union_dict_conversion', 14),
	('source_union_dict_managed', 4),
	('source_union_dict_growth', 31),
)

INVALID = (
	('optional primitive', 'let result: Optional[Result] = Dict[i64,i64]().get(1)'),
	('dictionary key mismatch', 'let result: Dict[i64,Result] = Dict[string,Success]()'),
	('dictionary value mismatch', 'let result = Dict[i64,Result]().insert(1, 42)'),
	('dictionary wrong variant', 'let result = Dict[i64,Result]().insert(1, Other(true))'),
	('nested dictionary mismatch', 'let result: Dict[i64,Array[Result]] = Dict[i64,Array[i64]]()'),
)

PREFIX = '''rec Success(value: i64)
rec Failure(code: i64)
rec Other(flag: bool)
let Result = Success | Failure
def main() -> i64
'''


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='Use an existing native compiler')
	parser.add_argument('--runner', type=Path, help='Use an existing MIR runner')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		for name, code in CASES:
			source = ROOT / 'tests/mir' / f'{name}.sirius'
			generated = folder / f'{name}.c'
			execute([str(compiler), str(source), str(generated)])
			check_native(cache, generated, code=code)
			interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
			assert interpreted.returncode == code and not interpreted.stderr, (name, interpreted)
			if name == 'source_union_dict_managed':
				binary = folder / 'managed_audit'
				execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)])
				audited = subprocess.run([str(binary)], capture_output=True)
				assert audited.returncode == code and not audited.stderr, audited
		for name, line in INVALID:
			source = folder / 'invalid.sirius'
			generated = folder / 'invalid.c'
			generated.unlink(missing_ok=True)
			source.write_text(PREFIX + '\t' + line + '\n\treturn 0\n', encoding='utf-8')
			compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True)
			interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
			assert compiled.returncode != 0 and interpreted.returncode != 0 and not generated.exists(), (name, compiled, interpreted)
	print('PASS Optional/Dict union coercions: native/MIR, managed ARC, growth, function returns, negatives')


if __name__ == '__main__':
	main()
