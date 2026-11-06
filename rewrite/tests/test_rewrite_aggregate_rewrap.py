#!/usr/bin/env python3
"""Rewrap already built typed aggregates without losing value/ARC semantics."""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines
from rewrite_paths import ENTRY, MODULES, DRIVERS


def run(*arguments):
    return subprocess.run([str(item) for item in arguments], capture_output=True, timeout=90)


def sirius(program):
    return '\n'.join('\t' * ((len(line) - len(line.lstrip(' '))) // 4) + line.lstrip(' ')
                     for line in program.splitlines()) + '\n'


VALID = [
    ('existing optional values, present/absent and managed snapshots', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def make(raw: Optional[Word]) -> Optional[U]
    return raw
def main() -> i64
    let old = Word?(Word("hello"))
    let missing = Word?()
    let new: Optional[U] = old
    let none: Optional[U] = missing
    let from_call = make(old)
    print(U.index(new!))
    print(U.index(from_call!))
    print(U.index(none or U.N(N(9))))
    return 0
''', b'1\n1\n0\n'),
    ('existing array conversion preserves original and snapshots', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def count(values: Array[U]) -> i64
    return len(values)
def main() -> i64
    var raw = [Word("one"), Word("two")]
    let source_copy = raw
    let values: Array[U] = raw
    raw[!0] = Word("modified")
    print(U.index(values[!0]))
    print(U.index(values[!1]))
    print(values[!0] == U.Word(Word("one")))
    print(source_copy[!0].value)
    print(count(raw))
    return 0
''', b'1\n1\ntrue\none\n2\n'),
    ('existing dict with managed keys and snapshot overwrites', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def main() -> i64
    var original = Dict[string,Word]().insert("a", Word("alpha")).insert("b", Word("beta"))
    let wrapped: Dict[string,U] = original
    original = original.insert("a", Word("changed"))
    print(U.index(wrapped[!"a"]))
    print(U.index(wrapped[!"b"]))
    match wrapped[!"a"]
        N(n)
            print(0)
        Word(s)
            print(s)
    print(original[!"a"].value)
    return 0
''', b'1\n1\nalpha\nchanged\n'),
    ('nested array conversion and indirect return', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def build(input: Array[Array[N]]) -> Array[Array[U]]
    return input
def main() -> i64
    let nested = [[N(1), N(2)], [N(3)]]
    let converted = build(nested)
    print(U.index(converted[!0][!1]))
    print(U.index(converted[!1][!0]))
    print(nested[!0][!1].value)
    return 0
''', b'0\n0\n2\n'),
    ('dictionary containing arrays and multi-level optional payloads', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def main() -> i64
    let inner = [Word("hello"), Word("world")]
    let lookup = Dict[i64,Array[Word]]().insert(2, inner)
    let converted: Dict[i64,Array[U]] = lookup
    let once: Optional[Word] = Word?(Word("found"))
    let twice: Optional[Optional[Word]] = Optional[Word]?(once)
    let nested: Optional[Optional[U]] = twice
    print(U.index(converted[!2][!0]))
    print(U.index(nested!!))
    print(lookup[!2][!0].value)
    return 0
''', b'1\n1\nhello\n'),
    ('record constructor and field assignment with aggregate conversions', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
rec Holder(items: Array[U], maybe: Optional[U], lookup: Dict[i64,U])
def main() -> i64
    let raw = [Word("text")]
    var holder = Holder(raw, N?(N(7)), Dict[i64,N]().insert(1, N(9)))
    let snapshot = holder
    holder.items = [N(1)]
    holder.maybe = Word?(Word("new"))
    print(U.index(snapshot.items[!0]))
    print(U.index(snapshot.maybe!))
    print(U.index(snapshot.lookup[!1]))
    print(U.index(holder.maybe!))
    return 0
''', b'1\n0\n0\n1\n'),
    ('empty arrays, missing optional, and empty dictionary', '''
rec N(value: i64)
rec Word(value: string)
let U = N | Word
def main() -> i64
    let xs = Array[N]()
    let ys: Array[U] = xs
    let missing = N?()
    let absent: Optional[U] = missing
    let lookup = Dict[string,N]()
    let output: Dict[string,U] = lookup
    print(len(ys))
    print(len(output))
    print(U.index(absent or U.Word(Word("none"))))
    return 0
''', b'0\n0\n1\n'),
]

INVALID = [
    ('ambiguous array payload', '''
rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let xs = [A(1)]
    let ys: Array[U] = xs
    return len(ys)
''', b'binding type mismatch'),
    ('ambiguous optional payload', '''
rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let value = A?(A(1))
    let new: Optional[U] = value
    return 0
''', b'binding type mismatch'),
    ('ambiguous dictionary payload', '''
rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let raw = Dict[i64,A]().insert(0, A(1))
    let new: Dict[i64,U] = raw
    return 0
''', b'binding type mismatch'),
    ('mismatched keys', '''
rec A(n: i64)
rec B(s: string)
let U = A | B
def main() -> i64
    let raw = Dict[i64,A]()
    let new: Dict[string,U] = raw
    return 0
''', b'binding type mismatch'),
    ('existing tagged alternatives cannot be relabeled', '''
rec A(n: i64)
rec B(s: string)
rec C(flag: bool)
let U = A | B
let V = A | C
def main() -> i64
    let raw: Array[U] = [A(1)]
    let new: Array[V] = raw
    return 0
''', b'binding type mismatch'),
    ('reverse conversion is rejected', '''
rec A(n: i64)
rec B(s: string)
let U = A | B
def main() -> i64
    let raw: Array[U] = [A(1)]
    let new: Array[A] = raw
    return 0
''', b'binding type mismatch'),
    ('nested mismatched structural payload', '''
rec A(n: i64)
rec B(s: string)
rec C(flag: bool)
let U = A | B
def main() -> i64
    let xs = [[C(true)]]
    let converted: Array[Array[U]] = xs
    return 0
''', b'binding type mismatch'),
]


def main():
    cache = BuildCache(require_cc())
    override = os.environ.get('SIRIUS_REWRITE_COMPILER')
    compiler = Path(override) if override else cache.native_driver('rewrite-aggregate-rewrap', ENTRY, extra_modules=MODULES)
    if not override:
        verifier = cache.native_driver('rewrite-aggregate-rewrap-ir', DRIVERS / 'rewrap_ir_test_driver.sirius', extra_modules=MODULES)
        result = run(verifier)
        assert result.returncode == 0 and not result.stdout and not result.stderr, ('verifier', result)
        print('PASS rewrap IR verifier', flush=True)
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, output, binary = (root / name for name in ('example.sirius', 'compiled.c', 'example'))
        for name, program, expected in VALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, output)
            assert result.returncode == 0, (name, 'compile', result.stderr)
            compile_c(cache.cc, output, binary, runtime)
            result = run(binary)
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, 'run', result)
        print(f'PASS aggregate rewrap native/ARC: {len(VALID)} cases', flush=True)
        for name, program, message in INVALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, output)
            assert result.returncode == 1 and message in result.stderr, (name, result)
        print(f'PASS aggregate rewrap invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
