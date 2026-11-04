#!/usr/bin/env python3
"""Labeled record unions: tags, managed payload ownership, native C, and errors."""
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


def run(*items):
    return subprocess.run([str(item) for item in items], capture_output=True, timeout=90)


VALID = [
    ('same-shaped alternatives have distinct tags', '''rec Meters(value: i64)
rec Feet(value: i64)
let Length = Meters | Feet
def identify(value: Length) -> i64
    return Length.index(value)
def main() -> i64
    let first: Length = Length.Meters(Meters(3))
    let second = Length.Feet(Feet(9))
    let copy = second
    print(Length.index(first))
    print(Length.index(second))
    print(identify(copy))
    print(Length.index(Meters))
    print(Length.index(Feet))
    return 0
''', b'0\n1\n1\n0\n1\n'),
    ('heterogeneous managed payloads and function returns', '''rec Number(value: i64)
rec Text(value: string)
rec Tokens(values: Array[string])
let Outcome = Number | Text | Tokens
def choose(x: bool) -> Outcome
    if x
        return Outcome.Text(Text("kept"))
    return Outcome.Tokens(Tokens(["a", "b"]))
def main() -> i64
    let one = choose(true)
    let two = choose(false)
    let three = Outcome.Number(Number(9))
    print(Outcome.index(one))
    print(Outcome.index(two))
    print(Outcome.index(three))
    return 0
''', b'1\n2\n0\n'),
    ('union array snapshot, optional, nested dictionaries', '''rec Number(value: i64)
rec Word(value: string)
let Value = Number | Word
def main() -> i64
    var row = [Value.Word(Word("original")), Value.Number(Number(5))]
    let before = row
    row[!0] = Value.Word(Word("after"))
    let first = before[!0]
    let second = row[!0]
    let absent = Value?()
    let present = Value?(first)
    let mapping = Dict[i64, Value]().insert(1, first).insert(2, second)
    print(Value.index(first))
    print(Value.index(second))
    print(Value.index(present!))
    print(Value.index(absent or Value.Number(Number(0))))
    print(Value.index(mapping[!2]))
    print(Value.index(mapping.get(1)!))
    return 0
''', b'1\n1\n1\n0\n1\n1\n'),
    ('empty and enum fields with managed optional payload', '''enum State
    Start
    Stop
rec Nothing()
rec Event(state: State, note: Optional[string])
let Message = Nothing | Event
def main() -> i64
    let a = Message.Nothing(Nothing())
    let b = Message.Event(Event(State.Stop(), string?("hello")))
    let snapshot = b
    print(Message.index(a))
    print(Message.index(b))
    print(Message.index(snapshot))
    return 0
''', b'0\n1\n1\n'),
    ('structural aliases preserve declared tag order', '''rec A(value: i64)
rec B(value: string)
let First = A | B
let Second = A | B
def index_of(x: First) -> i64
    return First.index(x)
def main() -> i64
    let value = Second.B(B("text"))
    print(index_of(value))
    return 0
''', b'1\n'),
]

INVALID = [
    ('single record is not union', 'rec A(x: i64)\nlet U = A\ndef main() -> i64\n    return 0\n', b'union requires two'),
    ('same label twice', 'rec A(x: i64)\nlet U = A | A\ndef main() -> i64\n    return 0\n', b'union requires two'),
    ('unknown variant record', 'rec A(x: i64)\nlet U = A | Missing\ndef main() -> i64\n    return 0\n', b'union alternative must be a record'),
    ('wrong payload', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    let value = U.A(B("wrong"))\n    return 0\n', b'union payload type mismatch'),
    ('non-record payload', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    let value = U.A(2)\n    return 0\n', b'union payload type mismatch'),
    ('unknown alternative', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    let value = U.Unknown(A(2))\n    return 0\n', b'unknown union alternative'),
    ('wrong union index argument', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    print(U.index(99))\n    return 0\n', b'index requires matching'),
    ('union not integer', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    return U.A(A(2))\n', b'return type mismatch'),
    ('no implicit relabel', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef main() -> i64\n    let value = U.A(U.B(B("x")))\n    return 0\n', b'union payload type mismatch'),
    ('distinct order is a distinct type', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\nlet V = B | A\ndef accept(value: U) -> i64\n    return 0\ndef main() -> i64\n    return accept(V.A(A(3)))\n', b'argument type mismatch'),
    ('reserved alias name', 'rec A(x: i64)\nrec B(x: string)\nlet i64 = A | B\ndef main() -> i64\n    return 0\n', b'duplicate or reserved union'),
    ('no implicit record-to-union', 'rec A(x: i64)\nrec B(x: string)\nlet U = A | B\ndef take(x: U) -> i64\n    return 0\ndef main() -> i64\n    return take(A(2))\n', b'argument type mismatch'),
]


def sirius(program):
    # Fixtures use four-space indentation for readability; lexer uses tabs.
    lines = program.splitlines()
    converted = []
    for line in lines:
        spaces = len(line) - len(line.lstrip(' '))
        converted.append('\t' * (spaces // 4) + line.lstrip(' '))
    return '\n'.join(converted) + '\n'


def main():
    # An override avoids rebuilding the large original compiler when available.
    override = os.environ.get('SIRIUS_REWRITE_COMPILER')
    cache = BuildCache(require_cc())
    compiler = Path(override) if override else cache.native_driver('rewrite-unions', ROOT / 'rewrite/main.sirius', extra_modules=MODULES)
    if not override:
        verifier = cache.native_driver('rewrite-union-ir', ROOT / 'rewrite/union_ir_test_driver.sirius', extra_modules=MODULES)
        output = run(verifier)
        assert output.returncode == 0 and not output.stdout and not output.stderr, ('union verifier', output)
        print('PASS union typed IR verification', flush=True)
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, c_file, binary = (root / name for name in ('input.sirius', 'generated.c', 'program'))
        for name, program, expected in VALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 0, (name, 'compile', result)
            compile_c(cache.cc, c_file, binary, runtime)
            result = run(binary)
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, result)
        print(f'PASS union native + ARC: {len(VALID)} cases', flush=True)
        for name, program, message in INVALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 1 and message in result.stderr, (name, result)
        print(f'PASS union invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
