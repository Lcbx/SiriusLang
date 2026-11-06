#!/usr/bin/env python3
"""Qualified imported enum/union constructors and matches, native ARC/visibility."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from project_paths import ROOT, executable_path, normalize_newlines
from rewrite_paths import ENTRY, MODULES


def run(*args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def indent_source(source):
    lines = []
    for line in source.splitlines():
        spaces = len(line) - len(line.lstrip(' '))
        assert spaces % 4 == 0
        lines.append('\t' * (spaces // 4) + line[spaces:])
    return '\n'.join(lines) + '\n'


VALID = [
    ({'Colors': '''export enum Color
    Red
    Blue
export def blue() -> Color
    return Color.Blue()
''',
      'Main': '''import Colors alias C
def main() -> i64
    let red: C.Color = C.Color.Red()
    print(C.Color.index(red))
    print(C.Color.index(C.Color.Blue()))
    print(C.Color.index(Red))
    match C.blue()
        C.Color.Red
            print(90)
        C.Color.Blue
            print(91)
    return 0
'''}, b'0\n1\n0\n91\n'),
    ({'Values': '''export rec A(code: i64, message: string)
export rec B(number: i64, text: string)
export let Value = A | B
export def make() -> Value
    return Value.B(B(4, "from library"))
''',
      'Main': '''import Values alias V
def main() -> i64
    let a: V.Value = V.Value.A(V.A(3, "from main"))
    let b = V.make()
    print(V.Value.index(a))
    print(V.Value.index(b))
    print(V.Value.index(V.B(8, "direct")))
    match a
        (V.Value.A | V.Value.B)(count, message)
            print(count)
            print(message)
    match b
        V.Value.A(count, message)
            print(count)
        V.Value.B(count, message)
            print(count)
            print(message)
    return 0
'''}, b'0\n1\n1\n3\nfrom main\n4\nfrom library\n'),
    ({'Hidden': '''rec PrivateA(count: i64, words: Array[string])
rec PrivateB(index: i64, names: Array[string])
export let Payload = PrivateA | PrivateB
export def make(values: Array[string]) -> Payload
    return Payload.PrivateB(PrivateB(7, values))
''',
      'Main': '''import Hidden alias H
def main() -> i64
    var words = ["old"]
    let value = H.make(words)
    words[!0] = "new"
    match value
        (H.Payload.PrivateA | H.Payload.PrivateB)(n, snapshot)
            print(n)
            print(snapshot[!0])
    return 0
'''}, b'7\nold\n'),
    ({'Base': '''export enum Status
    Ready
    Busy
export rec Ok(value: i64)
export rec Error(text: string)
export let Result = Ok | Error
export def status() -> Status
    return Busy()
export def result() -> Result
    return Error("bad")
''',
      'Mid': '''import Base alias B
export def fetch() -> B.Result
    return B.result()
export def state() -> B.Status
    return B.Status.Ready()
''',
      'Main': '''import Mid alias M
import Base alias B
def main() -> i64
    print(B.Status.index(M.state()))
    match M.fetch()
        B.Result.Ok(value)
            print(value)
        B.Result.Error(text)
            print(text)
    return 0
'''}, b'0\nbad\n'),
    ({'Colors': '''export enum Color
    Red
    Blue
''',
      'Main': '''import Colors alias C
enum Copy
    Red
    Blue
def main() -> i64
    let x: Copy = C.Color.Red()
    match x
        C.Color.Red
            print(1)
        C.Color.Blue
            print(2)
    return 0
'''}, b'1\n'),
]

INVALID = [
    ({'Lib': '''enum Secret
    Yes
    No
export enum Public
    Good
    Bad
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Secret.Yes()
    return 0
'''}, b'unknown enum: L.Secret'),
    ({'Lib': '''export enum Color
    Red
    Blue
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Color.Green()
    return 0
'''}, b'unknown enum label'),
    ({'Lib': '''export enum Color
    Red
    Blue
''',
      'Main': '''import Lib alias L
def main() -> i64
    match L.Color.Red()
        L.Color.Red
            print(1)
        L.Color.Green
            print(2)
    return 0
'''}, b'unknown match alternative'),
    ({'Lib': '''export enum Color
    Red
    Blue
''',
      'Main': '''import Lib alias L
enum Other
    Yes
    No
def main() -> i64
    match Other.Yes()
        L.Color.Red
            print(1)
        L.Color.Blue
            print(2)
    return 0
'''}, b'match qualifier type mismatch'),
    ({'Lib': '''export rec A(i: i64)
export rec B(s: string)
export let Value = A | B
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Value.A(L.A(7))
    match x
        L.Value.A(first, second)
            print(first)
        L.Value.B(s)
            print(s)
    return 0
'''}, b'match payload binding count mismatch'),
    ({'Lib': '''export rec A(i: i64)
export rec B(s: string)
export let Value = A | B
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Value.A(L.A(7))
    match x
        (L.Value.A | L.Value.B)(x)
            print(x)
    return 0
'''}, b'grouped match binding types must agree'),
    ({'Lib': '''export rec A(i: i64)
export rec B(s: string)
export let Value = A | B
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Value.A(L.A(7))
    match x
        L.Value.A
            print(1)
        A
            print(2)
        L.Value.B
            print(3)
    return 0
'''}, b'duplicate match alternative'),
    ({'Lib': '''export rec A(i: i64)
export rec B(s: string)
export let Value = A | B
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Value.B(L.A(7))
    return 0
'''}, b'union payload type mismatch'),
    ({'Lib': '''rec Private(i: i64)
export rec Other(s: string)
export let Value = Private | Other
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Private(7)
    return 0
'''}, b'unknown or ambiguous function'),
    ({'Lib': '''export enum Color
    Red
    Blue
''',
      'Main': '''import Lib alias L
def main() -> i64
    let x = L.Color.Nope(1)
    return 0
'''}, b'unknown union'),    ({'Lib': '''rec A(i: i64)
rec B(s: string)
let Hidden = A | B
export def choose() -> Hidden
    return A(1)
''',
      'Main': '''import Lib alias L
def main() -> i64
    match L.choose()
        L.Hidden.A(n)
            print(n)
        L.Hidden.B(s)
            print(s)
    return 0
'''}, b'unknown match qualifier: L.Hidden'),
    ({'Lib': '''export rec A(i: i64)
export rec B(s: string)
export let U = A | B
''',
      'Main': '''import Lib alias L
def main() -> i64
    match L.U.A(L.A(1))
        L.U.A(n)
            print(n)
    return 0
'''}, b'non-exhaustive match'),
]


def main():
    cache = BuildCache(require_cc())
    compiler = cache.native_driver('rewrite-imported-unions-v2', ENTRY, extra_modules=MODULES)
    runtime = ROOT / 'runtime' / 'sirius_runtime.c'
    with tempfile.TemporaryDirectory(prefix='rewrite-imported-unions-') as tmp:
        root = Path(tmp)
        generated = root / 'generated.c'
        for n, (sources, expected) in enumerate(VALID):
            for module_name, source in sources.items():
                (root / (module_name + '.sirius')).write_text(indent_source(source))
            output = run(compiler, '--compile-modules', root, 'Main', generated)
            assert output.returncode == 0, (n, output)
            original = generated.read_bytes()
            repeat = run(compiler, '--compile-modules', root, 'Main', generated)
            assert repeat.returncode == 0 and generated.read_bytes() == original, (n, repeat)
            binary = executable_path(root / f'valid-{n}')
            execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                '-I', str(ROOT / 'runtime'), str(generated), str(runtime),
                *link_flags(cache.cc), '-o', str(binary)])
            result = run(binary)
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (n, result)
            generated.unlink()
        for n, (sources, expected) in enumerate(INVALID):
            for module_name, source in sources.items():
                (root / (module_name + '.sirius')).write_text(indent_source(source))
            if generated.exists():
                generated.unlink()
            result = run(compiler, '--compile-modules', root, 'Main', generated)
            assert result.returncode == 1 and expected in result.stderr and not generated.exists(), (n, result)
    print(f'PASS imported unions/enums: {len(VALID)} native ARC-audited, {len(INVALID)} invalid, deterministic C')


if __name__ == '__main__':
    main()
