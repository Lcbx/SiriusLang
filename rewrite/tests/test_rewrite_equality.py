#!/usr/bin/env python3
"""Descriptor-driven structural equality, nested collections, and ARC safety."""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines
from rewrite_paths import ENTRY, MODULES, DRIVERS


def run(*args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=60)


VALID = [
    ('empty record fields compare equal', '''
rec Empty()
rec Count(x: i64)
let Variant = Empty | Count
def main() -> i64
    print(Empty() == Empty())
    print(Variant.Empty(Empty()) == Variant.Empty(Empty()))
    print(Variant.Empty(Empty()) != Variant.Count(Count(0)))
    return 0
''', b'true\ntrue\ntrue\n'),
    ('structural records across constructor order', '''
rec First(z: i64, a: string)
rec Second(a: string, z: i64)
def main() -> i64
    let a = First(2, "text")
    let b = Second("text", 2)
    let c = Second("text", 3)
    print(a == b)
    print(a != c)
    print(a == c)
    return 0
''', b'true\ntrue\nfalse\n'),
    ('nested records arrays and CoW snapshots', '''
rec Point(x: i64, y: bool)
rec Model(label: string, points: Array[Point], counts: Array[i64], rows: Array[Array[string]])
def main() -> i64
    var points = [Point(7, true)]
    var counts = [4, 5]
    var rows = [["a", "b"], ["c"]]
    let a = Model("embedded\\0text", points, counts, rows)
    let snapshot = a
    points[!0].x = 8
    counts[!1] = 6
    rows[!0][!1] = "d"
    let b = Model("embedded\\0text", points, counts, rows)
    let c = Model("embedded\\0text", [Point(7, true)], [4, 5], [["a", "b"], ["c"]])
    print(a == snapshot)
    print(a == c)
    print(a != b)
    print(b == snapshot)
    return 0
''', b'true\ntrue\ntrue\nfalse\n'),
    ('union labels and deep payload comparison', '''
rec Meters(value: string, numbers: Array[i64])
rec Feet(numbers: Array[i64], value: string)
let Distance = Meters | Feet
def main() -> i64
    let a = Distance.Meters(Meters("one", [1, 2]))
    let b = Distance.Meters(Meters("one", [1, 2]))
    let c = Distance.Feet(Feet([1, 2], "one"))
    let d = Distance.Meters(Meters("one", [1, 3]))
    print(a == b)
    print(a != c)
    print(a == c)
    print(a != d)
    return 0
''', b'true\ntrue\nfalse\ntrue\n'),
    ('nested optional value equality and some none', '''
rec Item(value: string)
def main() -> i64
    let absent = Item?()
    let a = Item?(Item("text"))
    let b = Item?(Item("text"))
    let c = Item?(Item("different"))
    let nested = Optional[Item]?(Item?())
    let doubly_absent = Optional[Item]?()
    print(a == b)
    print(a != c)
    print(absent == Item?())
    print(absent != a)
    print(nested != doubly_absent)
    print(nested == Optional[Item]?(Item?()))
    return 0
''', b'true\ntrue\ntrue\ntrue\ntrue\ntrue\n'),
    ('dictionary value equality independent of insertion order', '''
rec State(entries: Dict[string, Array[string]], flags: Dict[i64, bool])
def main() -> i64
    let left = State(Dict[string, Array[string]]().insert("a", ["one"]).insert("b", ["two"]), Dict[i64, bool]().insert(1, true).insert(2, false))
    let right = State(Dict[string, Array[string]]().insert("b", ["two"]).insert("a", ["one"]), Dict[i64, bool]().insert(2, false).insert(1, true))
    let different = State(Dict[string, Array[string]]().insert("b", ["three"]).insert("a", ["one"]), Dict[i64, bool]().insert(1, true).insert(2, false))
    print(left == right)
    print(left != different)
    return 0
''', b'true\ntrue\n'),
    ('array of union payload inside optional', '''
rec A(text: string)
rec B(number: i64)
let Choice = A | B
def main() -> i64
    let a = Array[Choice]?([Choice.A(A("same")), Choice.B(B(2))])
    let b = Array[Choice]?([Choice.A(A("same")), Choice.B(B(2))])
    let c = Array[Choice]?([Choice.A(A("same")), Choice.B(B(3))])
    print(a == b)
    print(a != c)
    print(a != Array[Choice]?())
    return 0
''', b'true\ntrue\ntrue\n'),
]

INVALID = [
    ('record ordered comparison', '''rec R(x: i64)
def main() -> i64
    print(R(1) < R(2))
    return 0
''', b'invalid comparison operands'),
    ('union ordered comparison', '''rec A(x: i64)
rec B(x: i64)
let U = A | B
def main() -> i64
    print(U.A(A(2)) > U.B(B(2)))
    return 0
''', b'invalid comparison operands'),
    ('optional ordered comparison', '''def main() -> i64
    print(i64?(1) < i64?(2))
    return 0
''', b'invalid comparison operands'),
    ('record scalar mismatch', '''rec R(x: i64)
def main() -> i64
    print(R(1) == 1)
    return 0
''', b'invalid comparison operands'),
    ('union mismatch', '''rec A(x: i64)
rec B(x: string)
let U = A | B
let V = B | A
def main() -> i64
    print(U.A(A(1)) == V.A(A(1)))
    return 0
''', b'invalid comparison operands'),
    ('direct arrays are elementwise reserved', '''def main() -> i64
    print([1] == [1])
    return 0
''', b'invalid comparison operands'),
    ('direct dictionaries not supported', '''def main() -> i64
    print(Dict[string, i64]() == Dict[string, i64]())
    return 0
''', b'invalid comparison operands'),
]


def sirius(source):
    return '\n'.join(line.replace('    ', '\t') if line.startswith('    ') else line
                     for line in source.lstrip('\n').splitlines()) + '\n'


def main():
    cache = BuildCache(require_cc())
    override = os.environ.get('SIRIUS_REWRITE_COMPILER')
    compiler = Path(override) if override else cache.native_driver('rewrite-equality', ENTRY, extra_modules=MODULES)
    if not override:
        verifier = cache.native_driver('rewrite-equality-ir', DRIVERS / 'equality_ir_test_driver.sirius', extra_modules=MODULES)
        result = run(verifier)
        assert result.returncode == 0 and not result.stdout and not result.stderr, ('equality IR verifier', result)
        print('PASS structural equality IR verification', flush=True)
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
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, 'native', result)
        print(f'PASS equality native + ARC: {len(VALID)} cases', flush=True)
        for name, program, message in INVALID:
            source.write_text(sirius(program), encoding='utf-8')
            result = run(compiler, source, c_file)
            assert result.returncode == 1 and message in result.stderr, (name, result)
        print(f'PASS equality invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
