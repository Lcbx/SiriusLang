#!/usr/bin/env python3
"""Payload-free enum tags: type safety, declaration order, aggregates, and ARC."""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines

MODULES = tuple(ROOT / 'rewrite' / name for name in (
    'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
    'lower.sirius', 'emit_c.sirius'))


def run(*args):
    return subprocess.run([str(x) for x in args], capture_output=True, timeout=60)


VALID = [
    ('ordered indices and labels', '''enum Color
\tBlue
\tRed
\tGreen
def main() -> i64
\tlet red: Color = Red()
\tprint(Color.index(Blue))
\tprint(Color.index(red))
\tprint(Color.index(Color.Green()))
\tprint(Color.index(Red()))
\treturn 0
''', b'0\n1\n2\n1\n'),
    ('enum comparisons and returning function values', '''enum Color
\tRed
\tGreen
\tBlue
def choose(x: Color) -> Color
\tif x == Red()
\t\treturn Color.Blue()
\treturn x
def main() -> i64
\tlet x = choose(Color.Red())
\tprint(x == Color.Blue())
\tprint(x != Red())
\tprint(Color.index(x))
\treturn 0
''', b'true\ntrue\n2\n'),
    ('enum field array dictionary and optional ARC CoW', '''enum Color
\tRed
\tGreen
\tBlue
rec State(color: Color, names: Array[Color], selected: Optional[Color])
def main() -> i64
\tvar colors = [Color.Red(), Color.Blue()]
\tlet snapshot = colors
\tcolors[!0] = Color.Green()
\tlet state = State(colors[!0], colors, Color?(Color.Red()))
\tlet d = Dict[string, Color]().insert("first", state.color)
\tprint(Color.index(d[!"first"]))
\tprint(Color.index(snapshot[!0]))
\tprint(Color.index(state.names[!0]))
\tprint(Color.index(state.selected!))
\tprint(Color.index(colors[?9] or Color.Blue()))
\treturn 0
''', b'1\n0\n1\n0\n2\n'),
    ('same label sequence structural enum type', '''enum First
\tA
\tB
enum Second
\tA
\tB
def use(x: First) -> i64
\treturn First.index(x)
def main() -> i64
\tprint(use(Second.B()))
\treturn 0
''', b'1\n'),
    ('index prefers local values over label names', '''enum Color
\tRed
\tBlue
def main() -> i64
\tlet Red = Color.Blue()
\tprint(Color.index(Red))
\tprint(Color.index(Color.Red()))
\treturn 0
''', b'1\n0\n'),
    ('immutable array and dictionary snapshots', '''enum State
\tStart
\tFinish
def take(items: Array[State]) -> State
\treturn items[!0]
def main() -> i64
\tvar a = [State.Start()]
\tlet b = a
\ta = a.append(Finish())
\ta[!0] = Finish()
\tlet values = Dict[i64, State]().insert(2, take(b))
\tprint(State.index(values[?2] or Finish()))
\tprint(State.index(a[!0]))
\treturn 0
''', b'0\n1\n'),
]

INVALID = [
    ('single label', 'enum Color\n\tRed\ndef main() -> i64\n\treturn 0\n', b'enum requires two distinct'),
    ('duplicate label', 'enum Color\n\tRed\n\tRed\ndef main() -> i64\n\treturn 0\n', b'enum requires two distinct'),
    ('reserved enum name', 'enum main\n\tRed\n\tBlue\ndef main() -> i64\n\treturn 0\n', b'duplicate or reserved enum'),
    ('duplicate enum name', 'enum Color\n\tRed\n\tBlue\nenum Color\n\tA\n\tB\ndef main() -> i64\n\treturn 0\n', b'duplicate or reserved enum'),
    ('unknown label', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tlet x = Color.Green()\n\treturn 0\n', b'unknown enum label'),
    ('unknown enum', 'def main() -> i64\n\tlet x = Unknown.Red()\n\treturn 0\n', b'unknown enum'),
    ('constructor payload forbidden', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tlet x = Red(1)\n\treturn 0\n', b'argument count mismatch'),
    ('qualified constructor payload forbidden', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tlet x = Color.Red(1)\n\treturn 0\n', b'ERROR'),
    ('mismatched signature', 'enum Color\n\tRed\n\tBlue\ndef take(x: Color) -> i64\n\treturn 0\ndef main() -> i64\n\treturn take(1)\n', b'argument type mismatch'),
    ('wrong index source', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tprint(Color.index(1))\n\treturn 0\n', b'enum index requires matching'),
    ('wrong index label', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tprint(Color.index(Green))\n\treturn 0\n', b'unknown enum index label'),
    ('enum ordered comparison', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tprint(Red() < Blue())\n\treturn 0\n', b'invalid comparison'),
    ('different label order', 'enum Left\n\tA\n\tB\nenum Right\n\tB\n\tA\ndef take(x: Left) -> i64\n\treturn 0\ndef main() -> i64\n\treturn take(Right.A())\n', b'argument type mismatch'),
    ('not an integer', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\treturn Red()\n', b'return type mismatch'),
    ('not printable', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tprint(Red())\n\treturn 0\n', b'printing enum'),
    ('not convertible to string implicitly', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tlet text = str(Red())\n\treturn 0\n', b'str(enum)'),
    ('ambiguous bare label', 'enum One\n\tRed\n\tBlue\nenum Two\n\tRed\n\tGreen\ndef main() -> i64\n\tlet x = Red()\n\treturn 0\n', b'unknown or ambiguous'),
    ('duplicate label name function', 'enum Color\n\tRed\n\tBlue\ndef Red() -> i64\n\treturn 1\ndef main() -> i64\n\treturn Red()\n', b'function name conflicts with enum label'),
    ('record name clashes with label', 'enum Color\n\tRed\n\tBlue\nrec Red(value: i64)\ndef main() -> i64\n\treturn 0\n', b'record name conflicts with enum label'),
    ('misusing label as value', 'enum Color\n\tRed\n\tBlue\ndef main() -> i64\n\tlet x = Red\n\treturn 0\n', b'unknown name'),
]


def main():
    cache = BuildCache(require_cc())
    override = os.environ.get('SIRIUS_REWRITE_COMPILER')
    compiler = Path(override) if override else cache.native_driver(
        'rewrite-enums', ROOT / 'rewrite/main.sirius', extra_modules=MODULES)
    if not override:
        verifier = cache.native_driver('rewrite-enum-ir',
            ROOT / 'rewrite/enum_ir_test_driver.sirius', extra_modules=MODULES)
        result = run(verifier)
        assert result.returncode == 0 and not result.stdout and not result.stderr, ('enum IR', result)
        print('PASS enum typed IR verification', flush=True)
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, c_file, binary = (root / x for x in ('input.sirius', 'program.c', 'program'))
        for name, program, expected in VALID:
            source.write_text(program, encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 0, (name, 'compile', result)
            compile_c(cache.cc, c_file, binary, runtime)
            result = run(binary)
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, result)
        print(f'PASS enum native + ARC: {len(VALID)} cases', flush=True)
        for name, program, message in INVALID:
            source.write_text(program, encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 1 and (message in result.stderr or message == b'ERROR' and b'ERROR' in result.stderr), (name, result)
        print(f'PASS enum invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
