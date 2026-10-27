#!/usr/bin/env python3
"""Verify an escaping MIR closure environment using existing owned records."""
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, execute, link_flags, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-closure-environment', ROOT / 'tests/mir/closure_environment_driver_native.sirius')
	with tempfile.TemporaryDirectory() as tmp:
		generated = Path(tmp) / 'closure.c'
		result = execute([str(driver), str(generated)])
		assert 'PASS owned closure environment' in result.stdout, result.stdout
		binary = compile_native(cache, generated)
		native = subprocess.run([str(binary)], capture_output=True, timeout=30)
		assert (native.returncode, native.stdout, native.stderr) == (42, b'', b''), native
		audit = Path(tmp) / 'closure-audit'
		execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
				str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(audit)])
		checked = subprocess.run([str(audit)], capture_output=True, timeout=30)
		assert (checked.returncode, checked.stdout, checked.stderr) == (42, b'', b''), checked
	print('PASS MIR owned closure environment: interpreter, wire, link, native, ARC')


if __name__ == '__main__':
	main()
