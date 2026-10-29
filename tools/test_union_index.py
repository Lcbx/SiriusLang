#!/usr/bin/env python3
"""Verify UnionType.index(value) declaration-order tags and static argument validation."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc
from project_paths import ROOT


PREFIX = '''enum Color
	Red
	Green
	Blue
rec Meters(value: i64)
rec Feet(value: i64)
let Length = Meters | Feet
'''
INVALID = (
    ('stored plain record is not tagged', 'let data = Meters(2)\n\treturn Length.index(data)', 'requires a value of the specified union type'),
    ('wrong union', 'let c: Color = Red()\n\treturn Length.index(c)', 'requires a value of the specified union type'),
    ('no arguments', 'return Color.index()', 'requires exactly one argument'),
    ('multiple arguments', 'let c: Color = Red()\n\treturn Color.index(c, c)', 'requires exactly one argument'),
    ('nonunion owner', 'return Meters.index(Meters(1))', None),
    ('unknown label', 'return Color.index(Meters)', 'requires a value of the specified union type'),
    ('shadowed constructor name', 'let Red = 7\n\treturn Color.index(Red)', 'requires a value of the specified union type'),
)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.mir_runner()
    source = ROOT / 'tests/mir/source_union_index.sirius'
    with tempfile.TemporaryDirectory(prefix='sirius-union-index-') as tmp:
        folder = Path(tmp)
        generated = folder / 'index.c'
        compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True, text=True)
        assert compiled.returncode == 0, compiled.stdout + compiled.stderr
        check_native(cache, generated, code=0)
        interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
        assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (0, b'', b''), interpreted
        audited = folder / 'index_audit'
        built = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
            '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
            *link_flags(cache.cc), '-o', str(audited)], capture_output=True, text=True)
        assert built.returncode == 0, built.stderr
        observed = subprocess.run([str(audited)], capture_output=True)
        assert (observed.returncode, observed.stdout, observed.stderr) == (0, b'', b''), observed
        # Qualified exported labels must select the same tag as their values.
        modules = folder / 'modules'
        modules.mkdir()
        (modules / 'Units.sirius').write_text(
            'export enum Color\n\tRed\n\tGreen\n\tBlue\n'
            'export rec Meters(value: i64)\nexport rec Feet(value: i64)\n'
            'export let Length = Meters | Feet\n', encoding='utf-8')
        (modules / 'Main.sirius').write_text(
            'import Units alias U\n'
            'def main() -> i64\n'
            '\tlet red: U.Color = U.Red()\n'
            '\tlet feet: U.Length = U.Feet(5)\n'
            '\tif U.Color.index(U.Red) != U.Color.index(red) or U.Color.index(U.Green) != U.Color.index(U.Green())\n'
            '\t\treturn 1\n'
            '\tif U.Length.index(U.Meters) != U.Length.index(U.Meters(5)) or U.Length.index(U.Feet) != U.Length.index(feet)\n'
            '\t\treturn 2\n'
            '\treturn 0\n', encoding='utf-8')
        qualified = folder / 'qualified.c'
        linked = subprocess.run([str(compiler), '--module-root', str(modules), 'Main', str(qualified)],
            capture_output=True, text=True)
        assert linked.returncode == 0, linked.stdout + linked.stderr
        check_native(cache, qualified, code=0)
        for label, body, diagnostic in INVALID:
            invalid = folder / 'invalid.sirius'
            output = folder / 'invalid.c'
            output.unlink(missing_ok=True)
            invalid.write_text(PREFIX + 'def main() -> i64\n\t' + body + '\n', encoding='utf-8')
            native = subprocess.run([str(compiler), str(invalid), str(output)], capture_output=True, text=True)
            mirror = subprocess.run([str(runner), str(invalid)], capture_output=True, text=True)
            assert native.returncode != 0 and mirror.returncode != 0 and not output.exists(), (label, native, mirror)
            if diagnostic:
                assert diagnostic in native.stdout + native.stderr, (label, native)
    print('PASS union index: declaration-order tags, same-shaped variants, native/MIR parity, ARC, invalid arguments')


if __name__ == '__main__':
    main()
