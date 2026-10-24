#!/usr/bin/env python3
"""Check automatic native ARC roots across active MIR calls and globals."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, execute, require_cc, link_flags
from project_paths import executable_path, ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-inband-ownership', ROOT / 'tests/mir/inband_ownership_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		generated = [folder / f'case{i}.c' for i in range(4)]
		execute([str(driver), *(str(source) for source in generated)])
		for i, source in enumerate(generated):
			code = source.read_text()
			assert 's_arc_audit_frame_enter(&s_arc_frame' in code
			assert 's_arc_audit_check_frames()' in code
			faults = [('valid', code, '')]
			if i == 0:
				# Registering the wrong slot hides one owner and duplicates another.
				old = '{&r0,&r1,&r2,&r3,&r4,}'
				assert old in code
				faults.append(('missing_slot', code.replace(old, '{&r0,&r1,&r2,&r3,&r3,}'),
					'heap root/child credit mismatch'))
				old = 's_mir_record_store(&r4, s_mir_f0(s_mir_record_retain(r3)));'
				assert old in code
				faults.append(('surplus_credit', code.replace(old, old + '\n\ts_mir_record_retain(r4);'),
					'heap root/child credit mismatch'))
			if i == 2:
				# Leaving the caller's moved root active while the callee owns it
				# must fail *inside* the callee, before returning to the caller.
				old = 's_call_arg0 = r3;\n\t\tr3 = NULL;'
				assert old in code
				faults.append(('duplicate_transfer', code.replace(old,
					's_call_arg0 = r3;\n\t\t/* injected missing transfer clear */'),
					'heap root/child credit mismatch'))
			for name, text, error in faults:
				case = folder / f'{i}_{name}.c'
				case.write_text(text)
				binary = executable_path(folder / f'{i}_{name}')
				execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
					'-I', str(ROOT / 'runtime'), str(case), str(ROOT / 'runtime/sirius_runtime.c'),
					*link_flags(cache.cc), '-o', str(binary)])
				# Include command-line arguments: these live in a native global array.
				result = subprocess.run([str(binary), 'one', 'two'], capture_output=True, text=True)
				if error:
					assert result.returncode == 86 and error in result.stderr, (i, name, result)
				else:
					assert result.returncode == (2 if i % 2 == 0 else 0) and not result.stderr, (i, name, result)
	print('PASS automatic frame roots (4 return paths, nested calls, native globals, 3 injected faults)')


if __name__ == '__main__':
	main()
