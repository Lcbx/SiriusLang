#!/usr/bin/env python3
"""Contextual labeled-union injection through direct, imported and indirect calls."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc


UNITS = '''export rec Meters(value: i64)
\tpub value
export rec Feet(value: i64)
\tpub value
export let Length = Meters | Feet
export enum Color
\tRed
\tGreen
export def score(x: Length) -> i64
\tmatch x
\t\tMeters(v)
\t\t\treturn v + 2
\t\tFeet(v)
\t\t\treturn v + 4
'''

MAIN = '''import Units alias U

rec Wrapper(item: U.Length)
	pub item

def classify(x: U.Length) -> i64
\tmatch x
\t\tU.Meters(v)
\t\t\treturn v
\t\tU.Feet(v)
\t\t\treturn v + 10

def pass_through(x: U.Length) -> U.Length
\treturn x

def display(x: U.Color) -> i64
\tmatch x
\t\tU.Red()
\t\t\treturn 20
\t\tU.Green()
\t\t\treturn 30

def sum_values(values: Array[U.Length]) -> i64
\tvar total = 0
\tfor value in values
\t\ttotal += classify(value)
\treturn total

def main() -> i64
\tlet a = classify(U.Meters(2))
\tlet b = classify(pass_through(U.Feet(3)))
\tlet c = display(U.Green())
\tlet d = U.score(U.Feet(5))
\tlet e = sum_values([U.Meters(1), U.Feet(2)])
\tlet f: Fn[U.Length, i64] = classify
\tlet g = f(U.Meters(4))
\tlet h = Wrapper(U.Feet(3))
\treturn a + b + c + d + e + g + classify(h.item)
'''


def compile_module(compiler: Path, root: Path, name: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run([str(compiler), '--module-root', str(root), name,
                           str(root / f'{name}.c')], capture_output=True,
                          text=True, timeout=120)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path,
                        help='existing compiler binary (otherwise build from current sources)')
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        (root / 'Units.sirius').write_text(UNITS, encoding='utf-8')
        (root / 'Main.sirius').write_text(MAIN, encoding='utf-8')
        generated = compile_module(compiler, root, 'Main')
        assert generated.returncode == 0, generated.stderr
        check_native(cache, root / 'Main.c', code=84)

        audit = root / 'audit'
        built = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function',
            '-DSIRIUS_ARC_AUDIT', '-I', str(Path(__file__).resolve().parent.parent / 'runtime'),
            str(root / 'Main.c'),
            str(Path(__file__).resolve().parent.parent / 'runtime/sirius_runtime.c'),
            *link_flags(cache.cc), '-o', str(audit)], capture_output=True, text=True, timeout=120)
        assert built.returncode == 0, built.stderr
        observed = subprocess.run([str(audit)], capture_output=True, timeout=60)
        assert observed.returncode == 84, observed

        # Constructor names do not survive an intermediate structural record.
        for name, body in (
            ('LocalRaw', 'let raw = U.Meters(1)\n\treturn classify(raw)'),
            ('ImportedRaw', 'let raw = U.Feet(1)\n\treturn U.score(raw)'),
            ('IndirectRaw', 'let f: Fn[U.Length, i64] = classify\n\tlet raw = U.Meters(1)\n\treturn f(raw)'),
        ):
            (root / f'{name}.sirius').write_text(
                MAIN.split('def main() -> i64')[0] +
                'def main() -> i64\n\t' + body + '\n', encoding='utf-8')
            failed = compile_module(compiler, root, name)
            assert failed.returncode != 0 and ('expected union, found record' in failed.stderr or 'indirect call argument type mismatch' in failed.stderr), failed

        # A constructor outside the expected union must not be silently selected.
        (root / 'Wrong.sirius').write_text(
            'import Units alias U\n'
            'rec Other(value: i64)\n\tpub value\n'
            'def consume(x: U.Length) -> i64\n\treturn 0\n'
            'def main() -> i64\n\treturn consume(Other(1))\n', encoding='utf-8')
        failed = compile_module(compiler, root, 'Wrong')
        assert failed.returncode != 0 and 'expected union, found record' in failed.stderr, failed

    print('PASS labeled union arguments: native, ARC, nested, imported, indirect, negative')


if __name__ == '__main__':
    main()
