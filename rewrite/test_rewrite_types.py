#!/usr/bin/env python3
"""Check canonical optional descriptors, table validation and opaque type IDs."""
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
		('lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius', 'lower.sirius', 'emit_c.sirius'))
	driver = ROOT / 'rewrite/type_test_driver.sirius'
	native = cache.native_driver('rewrite-types', driver, extra_modules=modules)
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		audit_runtime = root / 'audit-runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit_runtime)])
		audited = executable_path(root / 'audited-driver')
		compile_c(cache.cc, native.parent / 'generated.c', audited, audit_runtime)
		# Audit canonical-type verification before checking generated programs.
		result = run([str(audited)])
		assert result.returncode == 0 and not result.stdout and not result.stderr, result
		outputs = []
		commands = [[str(native)]]
		for index, command in enumerate(commands):
			generated = root / f'output{index}.c'
			result = run(command + [str(generated)])
			assert result.returncode == 0 and not result.stdout and not result.stderr, (index, result)
			outputs.append(generated.read_bytes())
			binary = executable_path(root / f'program{index}')
			compile_c(cache.cc, generated, binary, audit_runtime)
			result = run([str(binary)])
			assert result.returncode == 0 and normalize_newlines(result.stdout) == b'97\n' and not result.stderr, (index, result)
	print('PASS canonical optional types: interning, nesting, CoW snapshots, malformed tables, reordered IDs; native and ARC', flush=True)


if __name__ == '__main__':
	main()
