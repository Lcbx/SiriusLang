#!/usr/bin/env python3
"""Explicit Union.Label(record) typing, mixed-size array storage, and ARC parity."""
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
    ('already tagged', 'let x: Length = Meters(1)\n\treturn consume(Length.Feet(x))', 'cannot relabel'),
    ('non record', 'return consume(Length.Meters(4))', 'requires a record payload'),
    ('wrong structure', 'return consume(Length.Meters({other = 2}))', 'record is incompatible'),
    ('unknown label', 'return consume(Length.Yards(Other(2)))', 'unknown union alternative'),
    ('no argument', 'return consume(Length.Meters())', 'requires one record argument'),
    ('too many arguments', 'return consume(Length.Meters(Other(2), Other(3)))', 'requires one record argument'),
)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    with tempfile.TemporaryDirectory() as tmp:
        folder = Path(tmp)
        source = ROOT / 'tests/mir/source_union_explicit_construction.sirius'
        generated = folder / 'explicit.c'
        compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
        assert compiled.returncode == 0, compiled.stderr
        check_native(cache, generated, code=58)
        interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
        assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (58, b'', b''), interpreted
        audited = folder / 'audit'
        built = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
            '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
            *link_flags(cache.cc), '-o', str(audited)], capture_output=True, text=True)
        assert built.returncode == 0, built.stderr
        observed = subprocess.run([str(audited)], capture_output=True)
        assert (observed.returncode, observed.stdout, observed.stderr) == (58, b'', b''), observed
        for name, body, diagnostic in INVALID:
            source = folder / 'invalid.sirius'
            generated = folder / 'invalid.c'
            generated.unlink(missing_ok=True)
            source.write_text(PREFIX + 'def consume(x: Length) -> i64\n\treturn 0\n'
                              + 'def main() -> i64\n\t' + body + '\n', encoding='utf-8')
            result = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
            mirror = subprocess.run([str(runner), str(source)], capture_output=True, text=True)
            assert result.returncode != 0 and mirror.returncode != 0 and not generated.exists(), (name, result, mirror)
            assert diagnostic in result.stdout + result.stderr, (name, result)
    print('PASS explicit union construction: mixed payload sizes, arrays, Dict, ARC, native/MIR, negatives')


if __name__ == '__main__':
    main()
