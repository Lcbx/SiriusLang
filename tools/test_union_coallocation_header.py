#!/usr/bin/env python3
"""Verify the experimental fused union shares the ordinary record header.

Use a real emitted C runtime for a union-constructing Sirius program. A small
C probe checks the ABI layout and the independent lifetime of a retained
payload after its coallocated wrapper is released.
"""
import argparse
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

from benchmark_union_compiler_coallocation import compile_native
from build_support import BuildCache
from project_paths import executable_path


SOURCE = '''rec Small(value: i64, a: i64, b: i64, c: i64)
rec Large(value: i64, tag: string)
let Choice = Small | Large

def main() -> i64
\tlet x: Choice = Small(4, 0, 0, 0)
\tmatch x
\t\tSmall(value, a, b, c)
\t\t\treturn value
\t\tLarge(value, tag)
\t\t\treturn 0
'''


PROBE = r'''
/* Compare against an independent declaration of the *ordinary* header. */
typedef struct { int64_t type_id; size_t refs, length, capacity; } ExpectedHeader;
_Static_assert(sizeof(S_MirRecord) == sizeof(ExpectedHeader),
               "experimental coallocation must not enlarge every record");

int main(int argc, char **argv) {
#ifdef SIRIUS_EXPERIMENTAL_UNION_COALLOC
    S_MirRecord *payload = s_mir_record_new_fused(RECORD_ID, 4, UNION_ID);
    payload->fields[0].bits = 17;
    S_MirRecord *wrapper = s_mir_fused_wrapper(payload);
    wrapper->fields[0].bits = 0;
    if (!s_mir_is_fused(payload) || !s_mir_is_fused(wrapper)) return 10;
    S_MirRecord *escaped = s_mir_record_retain(payload);
    s_mir_record_release(wrapper);
    if (escaped->fields[0].bits != 17 || escaped->refs != 1) return 11;
    s_mir_record_release(escaped);
#endif
    return sirius_original_main(argc, argv) - 4;
}
'''


def check(cc: str, compiler: Path, sanitize: bool) -> None:
    with tempfile.TemporaryDirectory(prefix='sirius-coalloc-header-') as tmp:
        folder = Path(tmp)
        src = folder / 'program.sirius'
        c = folder / 'program.c'
        src.write_text(SOURCE, encoding='utf-8')
        subprocess.run([str(compiler), str(src), str(c)], check=True)
        content = c.read_text(encoding='utf-8')
        ids = re.search(r'record = s_mir_record_new_fused\((\d+), 4, (\d+)\)', content)
        if not ids:
            raise AssertionError('MIR did not select the fresh-record fusion path')
        old_main = 'int main(int argc, char **argv)'
        if content.count(old_main) != 1:
            raise AssertionError('cannot locate generated entry point')
        content = content.replace(old_main, 'int sirius_original_main(int argc, char **argv)')
        content += PROBE.replace('RECORD_ID', ids.group(1)).replace('UNION_ID', ids.group(2))
        c.write_text(content, encoding='utf-8')
        for fused in (False, True):
            exe = executable_path(folder / ('fused' if fused else 'baseline'))
            compile_native(cc, c, exe, fused=fused, audit=True, sanitize=sanitize)
            outcome = subprocess.run([str(exe)], capture_output=True, text=True)
            if outcome.returncode or outcome.stdout or outcome.stderr:
                raise AssertionError((fused, outcome.returncode, outcome.stdout, outcome.stderr))
    print(f'PASS {cc}: ordinary header size, escaped fused record, native ARC'
          + (' and sanitizers' if sanitize else ''))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='clang')
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    cc = shutil.which(args.cc)
    if not cc:
        parser.error(f'C compiler unavailable: {args.cc}')
    check(cc, BuildCache(cc).seed(), args.sanitize)


if __name__ == '__main__':
    main()
