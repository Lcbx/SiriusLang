#!/usr/bin/env python3
"""Audit native managed-object credits, including nested values and callee frames."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, link_flags, execute, require_cc
from project_paths import executable_path, executable_path, ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		runtime = folder / 'runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
			'-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
		for name, fixture, count in (
			('nested', 'nested_credit_driver_native.sirius', 4),
			('return', 'credit_driver_native.sirius', 8),
		):
			driver = cache.native_driver('mir-' + ('nested-credits' if name == 'nested' else 'credits'),
				ROOT / 'tests/mir' / fixture)
			sources = [folder / f'{name}{i}.c' for i in range(count)]
			execute([str(driver), *(str(source) for source in sources)])
			for i, source in enumerate(sources):
				binary = executable_path(folder / f'{name}{i}')
				execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT',
					*(() if 'clang' not in Path(cache.cc).name else ('-Wno-unused-function',)),
					'-I', str(ROOT / 'runtime'), str(source), str(runtime), *link_flags(cache.cc), '-o', str(binary)])
				result = subprocess.run([str(binary)], capture_output=True, text=True)
				expected = (2 if i < 2 else 1) if name == 'nested' else (
					23 if i >= 5 else (3 if i == 2 else (2 if i >= 3 else 19)))
				assert result.returncode == expected and 'Sirius ARC audit:' not in result.stderr, (
					name, i, result.returncode, result.stdout, result.stderr)

		# Force an actual realloc of an ARC-managed object, not just its data.
		growth = folder / 'growth.c'
		growth.write_text('''#include "sirius_runtime.h"
int main(void) {
    S_Array_i64 *array = s_array_i64_make(0, 0);
    for (int i = 0; i < 100; ++i) array = s_array_i64_append_take(array, i);
    s_array_i64_release(array);
    S_String *string = s_string_from_utf8("shared");
    S_String *other = s_string_retain(string);
    s_string_release(string);
    s_string_release(other);
    return 0;
}
''')
		growth_binary = executable_path(folder / 'growth')
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
			str(growth), str(runtime), '-o', str(growth_binary)])
		growth_result = subprocess.run([str(growth_binary)], capture_output=True, text=True)
		assert growth_result.returncode == 0 and not growth_result.stderr, growth_result

		# A process-fatal panic is not a recoverable language error. The
		# abandoned caller/callee frames are reclaimed by process exit;
		# the ARC audit must not turn this into a spurious leak report.
		fatal = folder / 'fatal.c'
		fatal.write_text('''#include "sirius_runtime.h"
void callee(void) {
    S_String *inner = s_string_from_utf8("callee-owned");
    (void)inner;
    s_panic("fatal operation");
}
int main(void) {
    S_String *outer = s_string_from_utf8("caller-owned");
    (void)outer;
    callee();
    return 0;
}
''')
		fatal_binary = executable_path(folder / 'fatal')
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
			str(fatal), str(runtime), '-o', str(fatal_binary)])
		fatal_result = subprocess.run([str(fatal_binary)], capture_output=True, text=True)
		assert fatal_result.returncode == 1 and fatal_result.stderr == 'Sirius panic: fatal operation\n', fatal_result

		for name, body, error in (
			('leak', 's_string_from_utf8("leaked");', 'live objects'),
			('mismatch', 'S_String *s = s_string_from_utf8("x"); s_arc_audit_retain(s, 3);',
				'retain credit mismatch'),
		):
			source = folder / f'{name}.c'
			source.write_text('#include "sirius_runtime.h"\nint main(void) {\n' + body + '\nreturn 0;\n}\n')
			binary = executable_path(folder / name)
			execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
				str(source), str(runtime), '-o', str(binary)])
			result = subprocess.run([str(binary)], capture_output=True, text=True)
			assert result.returncode == 86 and error in result.stderr, result
	print('PASS native ARC audit (12 MIR programs, realloc, leak and retain mismatch)')


if __name__ == '__main__':
	main()
