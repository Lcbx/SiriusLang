#!/usr/bin/env python3
"""Exercise managed Sirius values with native ASan/UBSan and ARC auditing.

Check native output against independent golden expectations; optionally run a
prebuilt MIR interpreter for parity (building it is expensive). This compiler
revision has no native borrow-elision path. Probe sanitizer availability,
rather than assuming it (notably on Windows).
"""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from project_paths import ROOT, executable_path, normalize_newlines


# Shared elements, CoW updates, managed calls, labeled union payloads and
# nested collections. Expectations are independent of either execution mode.
CASES = (
	('tests/language/fixtures/39_value_slot_copies.sirius', 0, 'tests/language/expected_output/39_value_slot_copies.txt'),
	('tests/language/fixtures/40_record_value_slots.sirius', 0, 'tests/language/expected_output/40_record_value_slots.txt'),
	('tests/language/fixtures/41_inline_scalar_record.sirius', 0, 'tests/language/expected_output/41_inline_scalar_record.txt'),
	('tests/mir/source_function_managed.sirius', 5, None),
	('tests/mir/source_union_labeled_collections_managed.sirius', 52, None),
	('tests/mir/source_union_nested_managed.sirius', 11, None),
	('tests/mir/source_union_dict_managed.sirius', 4, None),
	('tests/mir/source_union_optional_dict.sirius', 15, None),
)

SANITIZER_FLAGS = ('-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-sanitize-recover=all')


def supports_sanitizers(cc: str, work: Path) -> bool:
	source = work / 'probe.c'
	source.write_text('int main(void) { return 0; }\n', encoding='utf-8')
	binary = executable_path(work / 'probe')
	compiled = subprocess.run([cc, *C_FLAGS, *SANITIZER_FLAGS,
		str(source), '-o', str(binary)], capture_output=True)
	if compiled.returncode:
		return False
	# The compiler can accept flags without providing a working sanitizer runtime.
	return subprocess.run([str(binary)], capture_output=True).returncode == 0


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='use an already built native compiler')
	parser.add_argument('--runner', type=Path,
		help='also compare each program with a prebuilt MIR interpreter')
	parser.add_argument('--require-sanitizers', action='store_true',
		help='fail instead of skip if the selected C compiler lacks ASan/UBSan')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		if not supports_sanitizers(cache.cc, folder):
			message = f'{cache.cc} lacks a working ASan/UBSan runtime'
			if args.require_sanitizers:
				raise RuntimeError(message)
			print(f'SKIP MIR sanitizers: {message}')
			return
		compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
		interpreter = args.runner.resolve() if args.runner else None
		env = dict(os.environ)
		env['ASAN_OPTIONS'] = 'detect_leaks=1:halt_on_error=1'
		env['UBSAN_OPTIONS'] = 'halt_on_error=1'
		for number, (relative, expected_code, output_path) in enumerate(CASES):
			source = ROOT / relative
			expected_output = (ROOT / output_path).read_bytes() if output_path else (
				b'hello\n' if source.name == 'source_function_managed.sirius' else b'')
			expected_output = normalize_newlines(expected_output)
			if interpreter is not None:
				interpreted = subprocess.run([str(interpreter), str(source)], capture_output=True)
				assert (interpreted.returncode, normalize_newlines(interpreted.stdout), interpreted.stderr) == (
					expected_code, expected_output, b''), (relative, 'MIR', interpreted)
			generated = folder / f'case{number}.c'
			execute([str(compiler), str(source), str(generated)])
			binary = executable_path(folder / f'case{number}')
			execute([cache.cc, *C_FLAGS, '-Wno-unused-function', *SANITIZER_FLAGS,
				'-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
				str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
				*link_flags(cache.cc), '-o', str(binary)])
			native = subprocess.run([str(binary)], capture_output=True, env=env)
			assert (native.returncode, normalize_newlines(native.stdout), native.stderr) == (
				expected_code, expected_output, b''), (relative, 'ASan/UBSan + ARC', native)
	mode = 'MIR/native parity and' if interpreter is not None else 'native'
	print(f'PASS {mode} ASan, UBSan and ARC audit: {len(CASES)} managed-value cases')


if __name__ == '__main__':
	main()
