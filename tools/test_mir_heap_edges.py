#!/usr/bin/env python3
"""Check independently scanned native heap edges against ARC credits."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, link_flags, execute, require_cc
from project_paths import executable_path, ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.native_driver('mir-nested-credits', ROOT / 'tests/mir/nested_credit_driver_native.sirius')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		runtime = folder / 'runtime.o'
		execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
			'-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
		generated = [folder / f'original{i}.c' for i in range(4)]
		execute([str(driver), *(str(source) for source in generated)])
		for i in (0, 1, 2, 3):
			code = generated[i].read_text()
			old_main = 'int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_mir_f2(); }'
			assert old_main in code
			faults = [('valid', '', '')]
			if i < 2:
				faults.extend([
					('missing_child_credit', 's_mir_record_release(root->fields[0].record);',
						'heap child credits missing'),
					('dangling_child', 'root->fields[0].record = (S_MirRecord *)(uintptr_t)0x1234;',
						'untracked heap child edge'),
				])
			else:
				faults.extend([
					('extra_root_credit', 's_mir_record_retain(root);',
						'heap root/child credit mismatch'),
					('missing_dict_credit', 's_mir_record_release(root->fields[1].record->fields[0].record);',
						'heap child credits missing'),
					('missing_dict_key_credit',
						's_string_release(root->fields[1].record->fields[0].record->fields[0].record->fields[0].text);',
						'heap child credits missing'),
					('dangling_dict_child', 's_dict_release(root->fields[0].record->fields[0].dict);',
						'untracked heap child edge'),
				])
			for name, injected, error in faults:
				main_source = '''int main(void) {
	S_MirRecord *root = s_mir_f1();
	const void *roots[] = {root};
	%s
	s_arc_audit_check_roots(roots, 1);
	s_mir_record_release(root);
	return 0;
}
''' % injected
				source = folder / f'{i}_{name}.c'
				source.write_text(code.replace(old_main, main_source))
				binary = executable_path(folder / f'{i}_{name}')
				execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT',
					*(() if 'clang' not in Path(cache.cc).name else ('-Wno-unused-function',)),
					'-I', str(ROOT / 'runtime'), str(source), str(runtime), *link_flags(cache.cc), '-o', str(binary)])
				result = subprocess.run([str(binary)], capture_output=True, text=True)
				if error:
					assert result.returncode == 86 and error in result.stderr, (i, name, result)
				else:
					assert result.returncode == 0 and not result.stderr, (i, name, result)
	print('PASS native heap edge audit (4 rooted graphs, 12 injected faults)')


if __name__ == '__main__':
	main()
