#!/usr/bin/env python3
"""Constructor-label selection in array/dictionary mutations and ARC paths."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc
from project_paths import ROOT


PREFIX = '''rec Meters(value: i64)
rec Feet(value: i64)
rec Other(value: i64)
let Length = Meters | Feet
'''

INVALID = (
    ('stored array value', 'let raw = Meters(1)\n\tlet values = Array[Length]().append(raw)'),
    ('stored dictionary value', 'let raw = Feet(1)\n\tlet values = Dict[i64,Length]().insert(1, raw)'),
    ('unrelated constructor', 'let values = Array[Length]().append(Other(1))'),
    ('wrong dictionary alternative', 'let values = Dict[i64,Length]().insert(1, Other(1))'),
    ('wrong dictionary key', 'let values = Dict[i64,Length]().insert("key", Meters(1))'),
)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='existing native module compiler')
    parser.add_argument('--runner', type=Path, help='existing MIR runner')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for fixture, status in (('source_union_labeled_collections', 107),
                                ('source_union_labeled_collections_managed', 52)):
            source = ROOT / 'tests/mir' / f'{fixture}.sirius'
            generated = root / f'{fixture}.c'
            compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
            assert compiled.returncode == 0, (fixture, compiled)
            check_native(cache, generated, code=status)
            interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
            assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (status, b'', b''), (fixture, interpreted)
            if fixture.endswith('_managed'):
                audited = root / 'arc_audit'
                built = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                    '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
                    *link_flags(cache.cc), '-o', str(audited)], capture_output=True, text=True)
                assert built.returncode == 0, built.stderr
                observed = subprocess.run([str(audited)], capture_output=True)
                assert (observed.returncode, observed.stdout, observed.stderr) == (status, b'', b''), observed
        for name, body in INVALID:
            source = root / 'invalid.sirius'
            generated = root / 'invalid.c'
            generated.unlink(missing_ok=True)
            source.write_text(PREFIX + 'def main() -> i64\n\t' + body + '\n\treturn 0\n', encoding='utf-8')
            compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
            interpreted = subprocess.run([str(runner), str(source)], capture_output=True, text=True)
            assert compiled.returncode != 0 and interpreted.returncode != 0 and not generated.exists(), (name, compiled, interpreted)
    print('PASS labeled collections: same-shape alternatives, enums, heterogeneous cases, ARC, native/MIR, negatives')


if __name__ == '__main__':
    main()
