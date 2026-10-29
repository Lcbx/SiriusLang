#!/usr/bin/env python3
"""Check nested union injection through records, arrays and typed literals."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc, check_native
from project_paths import ROOT


INVALID = (
	('non-member record', '''rec Success(value: i64)
rec Failure(code: i64)
rec Other(flag: bool)
let Outcome = Success | Failure
def main() -> i64
	let values: Array[Outcome] = [Other(true)]
	return 0
'''),
	('wrong record shape', '''rec Success(value: i64)
rec Failure(code: i64)
let Outcome = Success | Failure
rec Accepted(value: Outcome)
rec Rejected(other: Success)
def use(value: Accepted) -> i64
	return 0
def main() -> i64
	return use(Rejected(Success(1)))
'''),
	('primitive value', '''rec Success(value: i64)
rec Failure(code: i64)
let Outcome = Success | Failure
def main() -> i64
	let values: Array[Outcome] = [1]
	return 0
'''),
	('mixed array without an expected type', '''rec Success(value: i64)
rec Failure(code: i64)
let Outcome = Success | Failure
def main() -> i64
	let values = [Success(1), Failure(2)]
	return 0
'''),
)


def main() -> None:
	cache = BuildCache(require_cc())
	compiler = cache.seed()
	runner = cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		for name, status in (('nested_coercions', 42), ('nested_managed', 11), ('mixed_arrays', 42), ('hir_conversion_plans', 0)):
			source = ROOT / 'tests/mir' / f'source_union_{name}.sirius'
			generated = folder / f'{name}.c'
			execute([str(compiler), str(source), str(generated)])
			check_native(cache, generated, code=status)
			interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
			assert interpreted.returncode == status and not interpreted.stderr, (name, interpreted)
			if name in ('nested_managed', 'hir_conversion_plans'):
				binary = folder / 'managed_audit'
				execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)])
				audited = subprocess.run([str(binary)], capture_output=True)
				assert audited.returncode == status and not audited.stderr, audited
		for name, source in INVALID:
			input_path = folder / 'negative.sirius'
			generated = folder / 'negative.c'
			generated.unlink(missing_ok=True)
			input_path.write_text(source, encoding='utf-8')
			compiled = subprocess.run([str(compiler), str(input_path), str(generated)], capture_output=True)
			interpreted = subprocess.run([str(runner), str(input_path)], capture_output=True)
			assert compiled.returncode != 0 and interpreted.returncode != 0 and not generated.exists(), (name, compiled, interpreted)
	print('PASS nested union coercions: array/record reconstruction, mixed literals, native/MIR, ARC, negatives')


if __name__ == '__main__':
	main()
