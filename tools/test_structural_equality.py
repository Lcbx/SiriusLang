#!/usr/bin/env python3
"""Structural == and hash-compatible Dict key regression (native + optional MIR)."""
from pathlib import Path
import argparse
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, executable_path

GOOD = ROOT / 'tests/mir/source_structural_equality.sirius'
BAD = (
    ('array_equality', 'let left = [1]\n\tif left == [1]\n\t\treturn 1\n\treturn 0\n',
     'array == is reserved for elementwise comparison'),
    ('dict_key_array', 'let values = Dict[Array[i64],i64]().insert([1], 2)\n\treturn len(values)\n',
     'unsupported dictionary key type'),
    ('dict_key_optional_array', 'let values = Dict[Optional[Array[i64]],i64]()\n\treturn len(values)\n',
     'unsupported dictionary key type'),
)


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
    return subprocess.run([str(a) for a in args], capture_output=True, timeout=120)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='prebuilt updated modular compiler')
    parser.add_argument('--runner', type=Path, help='prebuilt updated MIR runner')
    parser.add_argument('--sanitize', action='store_true', help='enable ASan, UBSan and ARC audit (Clang)')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler or cache.native_tool('modular_cli')
    with tempfile.TemporaryDirectory(prefix='sirius-structural-') as temp:
        work = Path(temp)
        output = work / 'equality.c'
        compiled = run(compiler, GOOD, output)
        assert compiled.returncode == 0, compiled
        c_code = output.read_text()
        assert 's_mir_value_equal' in c_code and 's_mir_hash_key_' in c_code
        binary = executable_path(work / 'equality')
        flags = ('-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-DSIRIUS_ARC_AUDIT') if args.sanitize else ()
        built = run(cache.cc, *C_FLAGS, '-Wno-unused-function', *flags, '-I', ROOT / 'runtime',
                    output, ROOT / 'runtime/sirius_runtime.c', *link_flags(cache.cc), '-o', binary)
        assert built.returncode == 0, built.stderr.decode(errors='replace')
        native = run(binary)
        assert native.returncode == 0 and native.stdout == b'' and native.stderr == b'', native
        print('PASS native structural equality, hashing, NaNs and CoW/ARC')
        if args.runner:
            dynamic = run(args.runner, GOOD)
            assert dynamic.returncode == 0 and dynamic.stdout == b'' and dynamic.stderr == b'', dynamic
            print('PASS MIR interpreter structural equality parity')
        for name, body, expected in BAD:
            source = work / f'{name}.sirius'
            source.write_text('def main() -> i64\n\t' + body)
            failed = run(compiler, source, work / f'{name}.c')
            diagnostic = (failed.stdout + failed.stderr).decode(errors='replace')
            assert failed.returncode != 0 and expected in diagnostic, (name, failed)
            print(f'PASS invalid {name}')


if __name__ == '__main__':
    main()
