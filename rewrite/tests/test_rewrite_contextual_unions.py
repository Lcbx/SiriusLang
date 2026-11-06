#!/usr/bin/env python3
"""Labeled record unions: tags, managed payload ownership, native C, and errors."""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines
from rewrite_paths import ENTRY, MODULES as REWRITE_MODULES, DRIVERS

MODULES = REWRITE_MODULES


def run(*items):
    return subprocess.run([str(item) for item in items], capture_output=True, timeout=90)


VALID = [
    ('union-dependent records and nested field snapshots', '''rec Name(value: string)
rec Count(value: i64)
let U = Name | Count
rec Holder(payload: U, history: Array[U])
rec Folder(nested: Array[Array[U]], maybe: Optional[U])
def build() -> Holder
    return Holder(Name("start"), [Count(9), Name("text")])
def main() -> i64
    var h = build()
    let old = h
    h.payload = Count(5)
    h.history[!0] = Name("updated")
    let folder = Folder([[Name("inner"), Count(6)]], U?(Count(1)))
    print(U.index(old.payload))
    print(U.index(old.history[!0]))
    print(U.index(h.payload))
    print(U.index(h.history[!0]))
    print(U.index(folder.nested[!0][!1]))
    print(U.index(folder.maybe!))
    return 0
''', b'0\n1\n1\n0\n1\n1\n'),

    ('named constructors retain labels across bindings, calls and returns', '''rec A(value: i64)
rec B(value: i64)
let U = A | B
def choose(flag: bool) -> U
    if flag
        return A(4)
    return B(5)
def tag(value: U) -> i64
    return U.index(value)
def main() -> i64
    let first: U = A(1)
    let second: U = B(2)
    print(tag(first))
    print(tag(second))
    print(tag(A(3)))
    print(tag(B(3)))
    print(U.index(A(10)))
    print(U.index(B(10)))
    print(U.index(choose(true)))
    print(U.index(choose(false)))
    return 0
''', b'0\n1\n0\n1\n0\n1\n0\n1\n'),
    ('unique structural payloads from stored records', '''rec Number(value: i64)
rec Word(value: string)
let U = Number | Word
def identify(value: U) -> i64
    return U.index(value)
def main() -> i64
    let stored = Number(10)
    let text = Word("hello")
    let a: U = stored
    let b: U = text
    print(identify(a))
    print(identify(b))
    print(identify(stored))
    return 0
''', b'0\n1\n0\n'),
    ('typed arrays, nested arrays, append, indexed update and function arguments', '''rec A(value: i64)
rec B(value: i64)
let U = A | B
def length(values: Array[U]) -> i64
    return len(values)
def build() -> Array[U]
    return [B(7), A(8)]
def main() -> i64
    var values: Array[U] = [A(1), B(2)]
    let original = values
    values = values.append(A(3))
    values[!0] = B(4)
    let nested: Array[Array[U]] = [[A(5)], [B(6)]]
    print(U.index(original[!0]))
    print(U.index(values[!0]))
    print(U.index(values[!1]))
    print(U.index(values[!2]))
    print(U.index(nested[!1][!0]))
    print(U.index(build()[!0]))
    print(length([B(9), A(10)]))
    return 0
''', b'0\n1\n1\n0\n1\n1\n2\n'),
    ('optional and dictionary inserts retain managed values', '''rec N(value: i64)
rec S(value: string)
let U = N | S
def main() -> i64
    let initial: Optional[U] = U?(N(7))
    let copy = initial
    let data = Dict[i64,U]().insert(1, S("managed")).insert(2, N(8))
    let value = data.get(1)!
    print(U.index(initial!))
    print(U.index(copy!))
    print(U.index(value))
    print(U.index(data[!2]))
    return 0
''', b'0\n0\n1\n0\n'),
    ('branch and loop targets across inserted union conversions', '''rec A(value: i64)
rec B(value: string)
let U = A | B
def echo(v: U) -> i64
    return U.index(v)
def main() -> i64
    var total = 0
    for i in 0..5
        if i == 3
            total += echo(B("x"))
            continue
        total += echo(A(i))
    if total == 1
        print(44)
    else
        print(33)
    return 0
''', b'44\n'),
]

INVALID = [
    ('index rejects stored record without constructor label', '''rec A(x: i64)
rec B(x: string)
let U = A | B
def main() -> i64
    let stored = A(8)
    print(U.index(stored))
    return 0
''', b'enum index requires matching enum value'),
    ('untyped mixed tagged and untagged values stay invalid', '''rec A(x: i64)
rec B(x: string)
let U = A | B
def main() -> i64
    let values = [U.A(A(1)), B("x")]
    return len(values)
''', b'array element type mismatch'),
    ('forward union reference in record disallowed', '''rec A(x: i64)
rec B(x: string)
rec Holder(value: U)
let U = A | B
def main() -> i64
    return 0
''', b'unsupported record field type'),
    ('union must follow both alternative records', '''rec A(x: i64)
let U = A | B
rec B(x: string)
def main() -> i64
    return 0
''', b'union alternative must be a record'),

    ('stored same-shaped record has no label', '''rec A(x: i64)
rec B(x: i64)
let U = A | B
def take(x: U) -> i64
    return 0
def main() -> i64
    let saved = A(3)
    return take(saved)
''', b'ambiguous union injection'),
    ('cannot inject already tagged value into a different union', '''rec A(x: i64)
rec B(x: string)
rec C(x: bool)
let U = A | B
let V = A | C
def take(x: V) -> i64
    return 0
def main() -> i64
    let tagged = U.A(A(2))
    return take(tagged)
''', b'argument type mismatch'),
    ('unrelated record not compatible', '''rec A(x: i64)
rec B(x: string)
rec C(flag: bool)
let U = A | B
def take(x: U) -> i64
    return 0
def main() -> i64
    return take(C(true))
''', b'argument type mismatch'),
    ('mixed array requires contextual type', '''rec A(x: i64)
rec B(x: string)
let U = A | B
def main() -> i64
    let xs = [A(2), B("hi")]
    return len(xs)
''', b'array element type mismatch'),
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
    compiler = Path(override) if override else cache.native_driver('rewrite-contextual-unions', ENTRY, extra_modules=MODULES)
    if not override:
        verifier = cache.native_driver('rewrite-contextual-union-ir', DRIVERS / 'union_ir_test_driver.sirius', extra_modules=MODULES)
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
        print(f'PASS contextual union native + ARC: {len(VALID)} cases', flush=True)
        for name, program, message in INVALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 1 and message in result.stderr, (name, result)
        print(f'PASS contextual union invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
