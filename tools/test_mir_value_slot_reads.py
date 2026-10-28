#!/usr/bin/env python3
"""Check record-valued array copy/CoW ownership in native and MIR execution."""
import argparse
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory

from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='prebuilt Sirius compiler')
    parser.add_argument('--runner', type=Path, help='prebuilt MIR interpreter CLI')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler or cache.seed()
    runner = args.runner or cache.native_tool('run_cli')
    with TemporaryDirectory(prefix='sirius-inline-pod-') as directory:
        folder = Path(directory)
        for stem in ('40_record_value_slots', '41_inline_scalar_record'):
            source = ROOT / 'tests/language/fixtures' / f'{stem}.sirius'
            expected = normalize_newlines((ROOT / 'tests/language/expected_output' / f'{stem}.txt').read_bytes())
            generated = folder / f'{stem}.c'
            execute([str(compiler), str(source), str(generated)])
            # The new fixture must actually use inline native registers, not
            # merely pass under the previous boxed representation.
            if stem == '41_inline_scalar_record':
                assert 'typedef struct {' in generated.read_text() and 'S_MirInline' in generated.read_text()
            binary = folder / stem
            extra = ('-Wno-unused-function',) if 'clang' in Path(cache.cc).name else ()
            execute([cache.cc, *C_FLAGS, *extra, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)])
            for backend, command in (('native', [str(binary)]), ('MIR', [str(runner), str(source)])):
                result = subprocess.run(command, capture_output=True)
                assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (0, expected, b''), (
                    stem, backend, result)
        invalid = folder / 'invalid.sirius'
        invalid.write_text('rec Item(x: i64)\ndef main() -> i64\n\tlet a = [Item(1)]\n\treturn a[!2].x\n')
        bad_c = folder / 'invalid.c'
        execute([str(compiler), str(invalid), str(bad_c)])
        bad_binary = folder / 'invalid'
        execute([cache.cc, *C_FLAGS, *extra, '-I', str(ROOT / 'runtime'),
            str(bad_c), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(bad_binary)])
        for backend, command in (('native', [str(bad_binary)]), ('MIR', [str(runner), str(invalid)])):
            result = subprocess.run(command, capture_output=True)
            assert (result.returncode, normalize_newlines(result.stdout), normalize_newlines(result.stderr)) == (
                1, b'', b'Sirius panic: array index out of bounds\n'), (backend, result)
    print('PASS record value slots: native/MIR, CoW, reads, append, ARC')


if __name__ == '__main__':
    main()
