#!/usr/bin/env python3
"""Check negative array indices in source, C, MIR wire, ARC and interpreter."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


def run(binary: Path, *args: Path | str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *(str(arg) for arg in args)], capture_output=True, timeout=45)


def check(result: subprocess.CompletedProcess[bytes], code: int, stdout: bytes = b'', stderr: bytes = b'') -> None:
	assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (code, stdout, stderr), result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
	runner = args.runner.resolve() if args.runner else cache.mir_runner()
	source_dir = ROOT / 'tests/mir'
	with tempfile.TemporaryDirectory() as folder:
		work = Path(folder)
		for name, code, stdout, stderr in (
			('source_negative_array_index', 0, b'PASS negative indexing\n', b''),
			('source_negative_array_index_oob', 1, b'', b'Sirius panic: array index out of bounds\n'),
			('source_negative_array_index_write_oob', 1, b'', b'Sirius panic: array index out of bounds\n'),
		):
			source = source_dir / (name + '.sirius')
			generated = work / (name + '.c')
			check(run(compiler, source, generated), 0)
			binary = compile_native(cache, generated)
			check(run(binary), code, stdout, stderr)
			# MIR interpretation must use the same signed-index semantics.
			interpreter = run(runner, source)
			assert interpreter.returncode == code and normalize_newlines(interpreter.stdout) == stdout and stderr in normalize_newlines(interpreter.stderr), (name, interpreter)
			saved = work / (name + '.smir')
			check(run(runner, '--emit-mir', saved, source), 0)
			reloaded = run(runner, '--load-mir', saved)
			assert reloaded.returncode == code and normalize_newlines(reloaded.stdout) == stdout and stderr in normalize_newlines(reloaded.stderr), (name, reloaded)
			if code == 0:
				audited = work / 'audit'
				cmd = [cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
					'-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
					*link_flags(cache.cc), '-o', str(audited)]
				built = subprocess.run(cmd, capture_output=True)
				assert built.returncode == 0, built.stderr
				check(run(audited), code, stdout, stderr)
	print('PASS negative indexing: checked/optional, CoW, OOB, MIR/native/wire/ARC')


if __name__ == '__main__':
	main()
