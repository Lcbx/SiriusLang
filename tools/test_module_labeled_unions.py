#!/usr/bin/env python3
"""Qualified labeled constructors, same-shape unions and contextual arrays."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, require_cc, link_flags


def write(root: Path, name: str, text: str) -> None:
    (root / f'{name}.sirius').write_text(text, encoding='utf-8')


def compile_module(compiler: Path, root: Path, name: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run([str(compiler), '--module-root', str(root), name,
        str(root / f'{name}.c')], capture_output=True, text=True, timeout=120)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path,
        help='existing compiler binary (otherwise build from current Sirius sources)')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        write(root, 'Units',
            'export rec Meters(value: i64)\n\tpub value\n'
            'export rec Feet(value: i64)\n\tpub value\n'
            'export let Length = Meters | Feet\n'
            'export enum Color\n\tRed\n\tGreen\n')
        write(root, 'Main',
            'import Units alias U\n'
            'def distance(x: U.Length) -> i64\n'
            '\tmatch x\n\t\tU.Meters(value)\n\t\t\treturn value\n'
            '\t\tU.Feet(value)\n\t\t\treturn value + 10\n'
            'def color(x: U.Color) -> i64\n'
            '\tmatch x\n\t\tU.Red()\n\t\t\treturn 30\n'
            '\t\tU.Green()\n\t\t\treturn 40\n'
            'def construct() -> U.Length\n\treturn U.Feet(4)\n'
            'def main() -> i64\n'
            '\tlet a: U.Length = U.Meters(2)\n'
            '\tlet b: U.Length = U.Feet(3)\n'
            '\tlet c: U.Color = U.Red()\n'
            '\treturn distance(a) + distance(b) + distance(construct()) + color(c)\n')
        compiled = compile_module(compiler, root, 'Main')
        assert compiled.returncode == 0, compiled.stderr
        check_native(cache, root / 'Main.c', code=59)

        write(root, 'ArrayMain',
            'import Units alias U\n'
            'def main() -> i64\n'
            '\tlet values: Array[U.Length] = [U.Meters(2), U.Feet(3)]\n'
            '\tlet colors: Array[U.Color] = [U.Red(), U.Green()]\n'
            '\tvar total = 0\n'
            '\tfor value in values\n\t\tmatch value\n'
            '\t\t\tU.Meters(n)\n\t\t\t\ttotal += n\n'
            '\t\t\tU.Feet(n)\n\t\t\t\ttotal += n * 10\n'
            '\tfor value in colors\n\t\tmatch value\n'
            '\t\t\tU.Red()\n\t\t\t\ttotal += 5\n'
            '\t\t\tU.Green()\n\t\t\t\ttotal += 7\n'
            '\treturn total\n')
        compiled = compile_module(compiler, root, 'ArrayMain')
        assert compiled.returncode == 0, compiled.stderr
        check_native(cache, root / 'ArrayMain.c', code=44)
        audited = root / 'array-audit'
        compiled_audit = subprocess.run([
            cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
            '-I', str(Path(__file__).resolve().parent.parent / 'runtime'),
            str(root / 'ArrayMain.c'),
            str(Path(__file__).resolve().parent.parent / 'runtime/sirius_runtime.c'),
            *link_flags(cache.cc), '-o', str(audited),
        ], capture_output=True, text=True, timeout=120)
        assert compiled_audit.returncode == 0, compiled_audit.stderr
        checked_audit = subprocess.run([str(audited)], capture_output=True, timeout=60)
        assert checked_audit.returncode == 44, checked_audit

        # Length prefixes, not underscore splitting, determine a mangled member.
        write(root, 'Directions',
            'export enum Compass\n\tNorth_East\n\tSouth_West\n')
        write(root, 'Underscored',
            'import Directions alias D\n'
            'def main() -> i64\n'
            '\tlet x: D.Compass = D.North_East()\n'
            '\tmatch x\n'
            '\t\tD.North_East()\n\t\t\treturn 7\n'
            '\t\tD.South_West()\n\t\t\treturn 8\n')
        compiled = compile_module(compiler, root, 'Underscored')
        assert compiled.returncode == 0, compiled.stderr
        check_native(cache, root / 'Underscored.c', code=7)

        write(root, 'Facade',
            'import Units alias U\n'
            'export U.Meters alias Metric\n'
            'export U.Feet alias Imperial\n'
            'export U.Length alias Span\n'
            'export U.Color alias Shade\n'
            'export U.Green alias Leaf\n')
        write(root, 'Reexport',
            'import Facade alias F\n'
            'def classify(x: F.Span) -> i64\n'
            '\tmatch x\n\t\tF.Metric(n)\n\t\t\treturn n + 1\n'
            '\t\tF.Imperial(n)\n\t\t\treturn n + 2\n'
            'def main() -> i64\n'
            '\tlet a: F.Span = F.Metric(10)\n'
            '\tlet b: F.Span = F.Imperial(20)\n'
            '\tlet c: F.Shade = F.Leaf()\n'
            '\treturn classify(a) + classify(b)\n')
        compiled = compile_module(compiler, root, 'Reexport')
        assert compiled.returncode == 0, compiled.stderr
        check_native(cache, root / 'Reexport.c', code=33)

        # A structural record variable cannot retain its constructor label.
        write(root, 'Ambiguous',
            'import Units alias U\n'
            'def main() -> i64\n'
            '\tlet raw = U.Meters(1)\n'
            '\tlet value: U.Length = raw\n'
            '\treturn 0\n')
        rejected = compile_module(compiler, root, 'Ambiguous')
        assert rejected.returncode != 0 and 'expected union, found record' in rejected.stderr, rejected
        write(root, 'ArrayAmbiguous',
            'import Units alias U\n'
            'def main() -> i64\n'
            '\tlet raw = U.Red()\n'
            '\tlet values: Array[U.Color] = [raw]\n'
            '\treturn 0\n')
        rejected = compile_module(compiler, root, 'ArrayAmbiguous')
        assert rejected.returncode != 0 and 'expected union, found record' in rejected.stderr, rejected
    print('PASS qualified labeled unions: native, arrays, re-exports and ambiguous-record rejection')


if __name__ == '__main__':
    main()
