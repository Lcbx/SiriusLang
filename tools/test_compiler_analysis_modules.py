#!/usr/bin/env python3
"""Execute HIR analysis and MIR compiler passes through native module imports."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc
from project_paths import ROOT, normalize_newlines


MODULES = (
	'tokenizer', 'parser', 'types', 'compile_common', 'hir', 'hir_resolve', 'hir_constants',
	'hir_declarations', 'hir_analyze', 'hir_specialize', 'hir_parametric', 'mir', 'mir_cfg',
	'mir_verify', 'mir_common', 'module', 'mir_lower', 'mir_ownership', 'mir_guard',
	'mir_lifetime', 'mir_cleanup', 'mir_opt', 'mir_program', 'mir_emit_c',
)


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=120)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).mir_runner()
	with tempfile.TemporaryDirectory() as directory:
		root = Path(directory)
		for name in MODULES:
			shutil.copy2(ROOT / f'src/{name}.sirius', root / f'{name}.sirius')
		for kind, fixture, expected in (
			('HIR', 'hir/analysis_module_driver.sirius', b'HIR analysis modules OK\n'),
			('MIR', 'mir/core_module_driver.sirius', b'MIR core modules OK\n'),
			('MIR compiler', 'mir/passes_module_driver.sirius', b'MIR compiler modules OK\n'),
		):
			shutil.copy2(ROOT / 'tests' / fixture, root / 'Main.sirius')
			library = root / 'core.smirl'
			built = run(runner, '--build-modules', root, 'Main', library)
			assert built.returncode == 0, (kind, built.stderr)
			data = library.read_bytes()
			executed = run(runner, '--entry', 'M_4_Main_4_main', '--load-lib', library)
			assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected, (kind, executed)
			rebuilt = run(runner, '--build-modules', root, 'Main', library)
			assert rebuilt.returncode == 0 and library.read_bytes() == data, (kind, rebuilt.stderr)
			print(f'PASS {kind} native imported modules, execution and deterministic linking')


if __name__ == '__main__':
	main()
