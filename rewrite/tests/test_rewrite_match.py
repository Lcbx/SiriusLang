#!/usr/bin/env python3
"""Exhaustive native match over labeled unions and enums, with ARC auditing."""
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

VALID = [
    ('same-shaped alternatives, all branches return', '''rec Meters(value: i64)
rec Feet(value: i64)
let Distance = Meters | Feet
def convert(value: Distance) -> i64
    match value
        Meters(n)
            return n
        Feet(n)
            return n * 2
def main() -> i64
    print(convert(Distance.Meters(Meters(3))))
    print(convert(Distance.Feet(Feet(4))))
    return 0
''', b'3\n8\n'),
    ('constructor order maps to canonical field order', '''rec Right(text: string, index: i64)
rec Left(index: i64)
let Either = Right | Left
def display(value: Either) -> i64
    match value
        Right(text, index)
            print(text)
            return index
        Left(index)
            return index

def main() -> i64
    print(display(Either.Right(Right("hello", 15))))
    return 0
''', b'hello\n15\n'),
    ('managed payloads preserve CoW snapshots', '''rec Names(values: Array[string])
rec Note(text: string)
let Update = Names | Note
def main() -> i64
    var names = ["before"]
    let before = Update.Names(Names(names))
    let text = Update.Note(Note("message"))
    names[!0] = "after"
    match before
        Names(values)
            print(values[!0])
        Note(value)
            print(value)
    match text
        Names(values)
            print(values[!0])
        Note(value)
            print(value)
    return 0
''', b'before\nmessage\n'),
    ('enum cases with no binding', '''enum State
    Ready
    Stopped
    Busy
def status(state: State) -> i64
    match state
        Ready
            return 1
        Stopped
            return 2
        Busy
            return 3
def main() -> i64
    print(status(State.Ready()))
    print(status(State.Busy()))
    return 0
''', b'1\n3\n'),
    ('bare tag ignores managed payload and arms can shadow local names', '''rec StringVal(text: string)
rec Number(value: i64)
let Value = StringVal | Number
def main() -> i64
    let text = "outer"
    let value = Value.StringVal(StringVal("inner"))
    match value
        StringVal
            let text = "shadow"
            print(text)
        Number(i)
            print(i)
    print(text)
    return 0
''', b'shadow\nouter\n'),
    ('nested matches on separate subjects', '''rec A(x: i64)
rec B(y: string)
let U = A | B
def main() -> i64
    let outer = U.A(A(4))
    let inner = U.B(B("ok"))
    match outer
        A(x)
            match inner
                A(y)
                    print(y)
                B(y)
                    print(y)
            print(x)
        B(z)
            print(z)
    return 0
''', b'ok\n4\n'),
    ('break and continue inside match arm', '''rec Number(n: i64)
rec Label(s: string)
let Value = Number | Label
def main() -> i64
    var total = 0
    for i in 0..5
        let value = Value.Number(Number(i))
        match value
            Number(n)
                if n == 2
                    continue
                if n == 4
                    break
                total += n
            Label(s)
                print(s)
    print(total)
    return 0
''', b'4\n'),
    ('single evaluation of discriminant', '''rec A(x: i64)
rec B(y: i64)
let U = A | B
def make() -> U
    print(77)
    return U.B(B(19))
def main() -> i64
    match make()
        A(x)
            print(x)
        B(y)
            print(y)
    return 0
''', b'77\n19\n'),
    ('optional field and dictionary snapshot through binder', '''rec Left(a: Optional[string], values: Dict[string, string])
rec Right(n: i64)
let U = Left | Right
def main() -> i64
    let stored = Dict[string, string]().insert("x", "original")
    let variant = U.Left(Left(string?("value"), stored))
    var changed = stored
    changed = changed.insert("x", "new")
    match variant
        Left(a, values)
            print(a!)
            print(values[!"x"])
        Right(n)
            print(n)
    return 0
''', b'value\noriginal\n'),
    ('tag-only alternatives with heterogeneous payloads', '''rec Ok(code: i64)
rec Text(text: string)
rec Data(values: Array[i64])
let Result = Ok | Text | Data
def classify(value: Result) -> i64
    match value
        Ok | Data
            return 10
        Text
            return 20
def main() -> i64
    print(classify(Result.Ok(Ok(3))))
    print(classify(Result.Text(Text("message"))))
    print(classify(Result.Data(Data([1, 2]))))
    return 0
''', b'10\n20\n10\n'),
    ('shared binders follow each constructors declared field order', '''rec First(z: i64, a: string)
rec Second(a: i64, z: string)
let Layout = First | Second
def show(value: Layout) -> i64
    match value
        (First | Second)(code, msg)
            print(msg)
            return code
def main() -> i64
    print(show(Layout.First(First(3, "one"))))
    print(show(Layout.Second(Second(4, "two"))))
    return 0
''', b'one\n3\ntwo\n4\n'),
    ('managed grouped binders preserve snapshots and nested matching', '''rec First(values: Array[string], text: string)
rec Second(text: Array[string], values: string)
rec Other(note: string)
let Value = First | Second | Other
def show(item: Value) -> i64
    match item
        (First | Second)(words, label)
            var copy = words
            copy[!0] = "changed"
            print(words[!0])
            print(copy[!0])
            match item
                First | Second
                    print(label)
                Other
                    print("unexpected")
            return len(words)
        Other(text)
            print(text)
            return 0
def main() -> i64
    print(show(Value.First(First(["before"], "left"))))
    print(show(Value.Second(Second(["right"], "right label"))))
    return 0
''', b'before\nchanged\nleft\n1\nright\nchanged\nright label\n1\n'),
    ('grouped enum patterns and single parenthesized patterns', '''enum Color
    Red
    Green
    Blue
def classify(color: Color) -> i64
    match color
        (Red | Green)()
            return 1
        (Blue)
            return 2
def main() -> i64
    print(classify(Color.Red()))
    print(classify(Color.Green()))
    print(classify(Color.Blue()))
    return 0
''', b'1\n1\n2\n'),
    ('single evaluation with grouped branch and shared field', '''rec A(value: i64)
rec B(value: i64)
rec C(value: i64)
let U = A | B | C
def make() -> U
    print(9)
    return U.B(B(5))
def main() -> i64
    match make()
        (A | B)(value)
            print(value)
        C(value)
            print(value * 2)
    return 0
''', b'9\n5\n'),
    ('grouped arms nested in a loop with break', '''rec One(value: i64)
rec Two(value: i64)
rec Other(value: i64)
let U = One | Two | Other
def main() -> i64
    var total = 0
    for i in 0..5
        let value = U.Two(Two(i))
        match value
            (One | Two)(n)
                if n == 4
                    break
                total += n
            Other(n)
                total += n
    print(total)
    return 0
''', b'6\n'),

]

INVALID = [
    ('nonexhaustive union', '''rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let value = U.A(A(1))
    match value
        A(n)
            print(n)
    return 0
''', b'non-exhaustive match'),
    ('duplicate union label', '''rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let v = U.A(A(1))
    match v
        A
            print(1)
        B
            print(2)
        A
            print(3)
    return 0
''', b'duplicate match alternative'),
    ('unknown label for union', '''rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    let v = U.A(A(1))
    match v
        A
            print(1)
        Missing
            print(2)
    return 0
''', b'unknown match alternative'),
    ('scalar subject', '''def main() -> i64
    match 3
        A
            return 0
''', b'match requires enum or labeled union'),
    ('wrong union binding arity', '''rec A(n: i64, value: string)
rec B()
let U = A | B
def main() -> i64
    let v = U.A(A(1, "x"))
    match v
        A(n)
            print(n)
        B
            print(0)
    return 0
''', b'match payload binding count mismatch'),
    ('enum field binding', '''enum C
    Red
    Blue
def main() -> i64
    match C.Red()
        Red(x)
            print(x)
        Blue
            print(0)
    return 0
''', b'enum match cannot bind fields'),
    ('duplicate binder', '''rec A(x: i64, y: i64)
rec B()
let U = A | B
def main() -> i64
    match U.A(A(1, 2))
        A(x, x)
            print(x)
        B
            print(0)
    return 0
''', b'duplicate match binding'),
    ('binder cannot escape arm', '''rec A(x: i64)
rec B()
let U = A | B
def main() -> i64
    match U.A(A(1))
        A(x)
            print(x)
        B
            print(0)
    print(x)
    return 0
''', b'unknown name'),
    ('binder is immutable', '''rec A(x: i64)
rec B()
let U = A | B
def main() -> i64
    match U.A(A(1))
        A(x)
            x = 3
        B
            print(0)
    return 0
''', b'cannot assign to immutable binding'),
    ('nonexhaustive enum', '''enum C
    Red
    Blue
def main() -> i64
    match C.Red()
        Red
            print(1)
    return 0
''', b'non-exhaustive match'),
    ('duplicate tags within group', '''enum Color
    Red
    Green
def main() -> i64
    match Color.Red()
        Red | Red
            print(1)
        Green
            print(2)
    return 0
''', b'duplicate match alternative'),
    ('duplicate tags across groups', '''rec A()
rec B()
rec C()
let U = A | B | C
def main() -> i64
    match U.A(A())
        A | B
            print(1)
        B | C
            print(2)
    return 0
''', b'duplicate match alternative'),
    ('nonexhaustive grouped tags', '''enum C
    A
    B
    C
def main() -> i64
    match C.A()
        A | B
            print(0)
    return 0
''', b'non-exhaustive match'),
    ('unknown tag in group', '''enum C
    A
    B
def main() -> i64
    match C.A()
        A | Missing
            print(1)
        B
            print(2)
    return 0
''', b'unknown match alternative'),
    ('incompatible shared binder types', '''rec A(n: i64)
rec B(n: string)
let U = A | B
def main() -> i64
    match U.A(A(1))
        (A | B)(n)
            print(n)
    return 0
''', b'grouped match binding types must agree'),
    ('incompatible shared binder arity', '''rec A(a: i64)
rec B(a: i64, b: i64)
let U = A | B
def main() -> i64
    match U.A(A(1))
        (A | B)(x)
            print(x)
    return 0
''', b'match payload binding count mismatch'),
    ('duplicate grouped binder', '''rec A(a: i64, b: i64)
rec B(a: i64, b: i64)
let U = A | B
def main() -> i64
    match U.A(A(1, 2))
        (A | B)(x, x)
            print(x)
    return 0
''', b'duplicate match binding'),
    ('no grouped binder leaks into outer scope', '''rec A(a: i64)
rec B(a: i64)
let U = A | B
def main() -> i64
    match U.A(A(1))
        (A | B)(x)
            print(x)
    print(x)
    return 0
''', b'unknown name'),
    ('shared binders are immutable', '''rec A(a: i64)
rec B(a: i64)
let U = A | B
def main() -> i64
    match U.A(A(1))
        (A | B)(x)
            x = 2
    return 0
''', b'cannot assign to immutable binding'),
    ('grouped enum cannot bind payload', '''enum E
    A
    B
def main() -> i64
    match E.A()
        (A | B)(n)
            print(n)
    return 0
''', b'enum match cannot bind fields'),
    ('tag-only group cannot follow with unparenthesized binders', '''rec A(n: i64)
rec B(n: i64)
let U = A | B
def main() -> i64
    match U.A(A(1))
        A | B(n)
            print(n)
    return 0
''', b'grouped match bindings require parenthesized alternatives'),

]


def sirius(program):
    return '\n'.join('\t' * ((len(line) - len(line.lstrip(' '))) // 4) + line.lstrip(' ')
                     for line in program.splitlines()) + '\n'


def run(*items):
    return subprocess.run([str(item) for item in items], capture_output=True, timeout=90)


def main():
    cache = BuildCache(require_cc())
    override = os.environ.get('SIRIUS_REWRITE_COMPILER')
    compiler = Path(override) if override else cache.native_driver('rewrite-match', ENTRY, extra_modules=MODULES)
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, c_file, binary = (root / name for name in ('test.sirius', 'generated.c', 'program'))
        for case, program, expected in VALID:
            source.write_text(sirius(program), encoding='utf-8')
            output = run(compiler, source, c_file)
            assert output.returncode == 0, (case, 'compile', output)
            compile_c(cache.cc, c_file, binary, runtime)
            output = run(binary)
            assert output.returncode == 0 and normalize_newlines(output.stdout) == expected and not output.stderr, (case, output)
        print(f'PASS match native + ARC: {len(VALID)} cases', flush=True)
        for case, program, expected in INVALID:
            source.write_text(sirius(program), encoding='utf-8')
            output = run(compiler, source, c_file)
            assert output.returncode == 1 and expected in output.stderr, (case, output)
        print(f'PASS match invalid: {len(INVALID)} cases', flush=True)


if __name__ == '__main__':
    main()
