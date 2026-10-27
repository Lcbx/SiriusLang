#!/usr/bin/env python3
"""Check fixed-width arithmetic, division traps, MIR parity and C signed-UB sanitization."""
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


def run(binary: Path, *args: Path | str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(binary), *(str(arg) for arg in args)], capture_output=True, timeout=45)


def main() -> None:
	cache = BuildCache(require_cc())
	compiler = cache.native_tool('modular_cli')
	runner = cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as tmp:
		work = Path(tmp)
		for name, status, output, error in (
			('source_integer_edges', 0, b'PASS integer arithmetic edges\n', b''),
			('source_integer_division_overflow', 1, b'', b'integer overflow'),
			('source_integer_remainder_overflow', 1, b'', b'integer overflow'),
		):
			source = ROOT / 'tests/mir' / (name + '.sirius')
			generated = work / (name + '.c')
			assert run(compiler, source, generated).returncode == 0
			native = compile_native(cache, generated)
			for outcome in (run(native), run(runner, source)):
				assert (outcome.returncode, normalize_newlines(outcome.stdout)) == (status, output), (name, outcome)
				assert error in outcome.stderr, (name, outcome)
			wire = work / (name + '.smir')
			assert run(runner, '--emit-mir', wire, source).returncode == 0
			loaded = run(runner, '--load-mir', wire)
			assert (loaded.returncode, normalize_newlines(loaded.stdout)) == (status, output) and error in loaded.stderr, (name, loaded)
			if status == 0:
				# Some Windows toolchains have no sanitizer runtime; probe it first.
				flags = ['-fsanitize=signed-integer-overflow', '-fno-sanitize-recover=all']
				probe = work / 'sanitizer_probe.c'
				probe.write_text('int main(void) { return 0; }\n')
				probe_binary = work / 'sanitizer_probe'
				supported = subprocess.run([cache.cc, *flags, str(probe), '-o', str(probe_binary)], capture_output=True)
				if supported.returncode == 0 and run(probe_binary).returncode == 0:
					audited = work / 'sanitized'
					cmd = [cache.cc, *C_FLAGS, '-O2', '-Wno-unused-function', *flags,
						'-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated),
						str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(audited)]
					build = subprocess.run(cmd, capture_output=True)
					assert build.returncode == 0, build.stderr
					result = run(audited)
					assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (0, output, b''), result
				else:
					print('SKIP signed-overflow sanitizer (unsupported by C toolchain)')
		driver = cache.native_driver('integer-edges', ROOT / 'tests/mir/integer_edges_driver_native.sirius')
		result = run(driver)
		assert (result.returncode, normalize_newlines(result.stdout)) == (0, b'PASS raw MIR integer division checks\n'), result
	print('PASS i64 wrapping, truncating division, traps, MIR/native/wire and UBSan')


if __name__ == '__main__':
	main()
