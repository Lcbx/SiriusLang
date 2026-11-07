#!/usr/bin/env python3
"""Inferred linked-function results: concrete specializations, ARC and rejection."""
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from project_paths import ROOT, executable_path, normalize_newlines
from rewrite_paths import ENTRY, MODULES


def source(value: str) -> str:
    lines = []
    for line in value.strip('\n').splitlines():
        spaces = len(line) - len(line.lstrip(' '))
        assert spaces % 4 == 0, line
        lines.append('\t' * (spaces // 4) + line[spaces:])
    return '\n'.join(lines) + '\n'


VALID = [
    ({'Lib': '''export def identity(value)
    return value
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.identity(17))
    print(L.identity("hello"))
    print(L.identity(true))
    print(len(L.identity([2, 3, 5])))
    print(L.identity(10))
    return 0
'''}, b'17\nhello\ntrue\n3\n10\n', 5),
    ({'Lib': '''export def identity(value)
    return value
''', 'Middle': '''import Lib alias L
export def forward(value)
    return L.identity(value)
''', 'Main': '''import Middle alias M
def main() -> i64
    print(M.forward(3))
    print(M.forward("word"))
    print(M.forward(true))
    return 0
'''}, b'3\nword\ntrue\n', 7),
    ({'Lib': '''export def count(values)
    return len(values)
export def greeting(value)
    return "hello"
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.count([1, 2]))
    print(L.count(["hi"]))
    print(L.greeting(true))
    return 0
'''}, b'2\n1\nhello\n', 4),
    ({'Lib': '''export def select(a, b)
    return b
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.select(true, "text"))
    print(L.select("x", 99))
    return 0
'''}, b'text\n99\n', 3),
    ({'Lib': '''export def relay(x)
    return forward(x)
def forward(y)
    return y
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.relay("ok"))
    print(L.relay(64))
    return 0
'''}, b'ok\n64\n', 5),
    ({'Lib': '''export rec Box(text: string, items: Array[string])
export def snapshot(value)
    return value
''', 'Main': '''import Lib alias L
def main() -> i64
    var first = L.Box("old", ["a", "b"])
    let second = L.snapshot(first)
    first.text = "new"
    first.items[!0] = "changed"
    print(second.text)
    print(second.items[!0])
    print(first.text)
    print(first.items[!0])
    return 0
'''}, b'old\na\nnew\nchanged\n', 2),
    ({'Lib': '''export def chosen(x, flag: bool)
    if flag
        return x
    return 4
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.chosen(9, true))
    print(L.chosen(9, false))
    return 0
'''}, b'9\n4\n', 2),
    ({'Lib': '''export def fixed(x: string)
    return x
''', 'Main': '''import Lib alias L
def local(x: i64)
    return x
def main() -> i64
    print(L.fixed("fine"))
    print(local(9))
    return 0
'''}, b'fine\n9\n', 3),
    ({'Lib': '''export def duplicate(x)
    return x + x
export def decrement(x)
    return x - 1
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.duplicate(8))
    print(L.duplicate("ab"))
    print(L.decrement(11))
    return 0
'''}, b'16\nabab\n10\n', 4),
    ({'Lib': '''export def say(v)
    return "always"
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.say("one"))
    print(L.say(2))
    return 0
'''}, b'always\nalways\n', 3),
    ({'Lib': '''export def head(values)
    return values[!0]
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.head([5, 8]))
    print(L.head(["one", "two"]))
    print(L.head([true, false]))
    print(L.head([9]))
    return 0
'''}, b'5\none\ntrue\n9\n', 4),
    ({'Types': '''export rec Item(name: string, value: i64)
export rec Holder(items: Array[Item], title: string)
''', 'Lib': '''import Types alias T
export def title(box)
    return box.title
export def inner(box)
    return box.items[!0].name
''', 'Mid': '''import Lib alias L
export def forwarded(box)
    return L.inner(box)
''', 'Main': '''import Types alias T
import Lib alias L
import Mid alias M
def main() -> i64
    var items = [T.Item("old", 4)]
    let h = T.Holder(items, "before")
    let name = M.forwarded(h)
    let title = L.title(h)
    items[!0] = T.Item("new", 5)
    print(name)
    print(title)
    print(M.forwarded(h))
    return 0
'''}, b'old\nbefore\nold\n', 4),
    ({'Lib': '''export def element(values)
    return values[!0]
export def composed(box)
    return element(box.items)
''', 'Main': '''import Lib alias L
rec Box(items: Array[string], note: i64)
def main() -> i64
    let box = Box(["part"], 1)
    print(L.composed(box))
    return 0
'''}, b'part\n', 3),
    ({'Lib': '''export def read(values)
    return values[!"key"]
''', 'Main': '''import Lib alias L
def main() -> i64
    let mapping = Dict[string, string]().insert("key", "data")
    print(L.read(mapping))
    return 0
'''}, b'data\n', 2),
    ({'Lib': '''export def head(values: Array[string])
    return values[!0]
export def choice(other, box)
    return box.field
''', 'Main': '''import Lib alias L
rec Packet(field: string)
def main() -> i64
    print(L.head(["typed"]))
    print(L.choice(true, Packet("selected")))
    return 0
'''}, b'typed\nselected\n', 3),
]

INVALID = [
    ({'Lib': 'export def broken(x)\n    return broken(x)\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.broken(1)\n'}, 'cannot infer function result'),
    ({'Lib': 'export def choose(x, flag: bool)\n    if flag\n        return x\n    return 4\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.choose("word", true))\n    return 0\n'}, 'return type mismatch'),
    ({'Lib': 'export def echo(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.echo("not i64")\n'}, 'return type mismatch'),
    ({'Lib': 'export def echo(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.echo(1, 2)\n'}, 'argument count mismatch'),
    ({'Lib': 'export def impossible(x)\n    return missing\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return 0\n'}, 'cannot infer function result'),
    ({'Main': 'def main()\n    return 0\n'}, 'main must have signature'),
    ({'Lib': 'export def index(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.index([1, "x"]))\n    return 0\n'}, 'array element type mismatch'),
    ({'Lib': 'export def head(x)\n    return x[!0]\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.head(42))\n    return 0\n'}, 'invalid inferred result projection'),
    ({'Lib': 'export def field(x)\n    return x.absent\n', 'Main': 'import Lib alias L\nrec Data(name: string)\ndef main() -> i64\n    print(L.field(Data("test")))\n    return 0\n'}, 'invalid inferred result projection'),
    ({'Lib': 'export def head(x)\n    return x[!"wrong"]\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.head([1, 2]))\n    return 0\n'}, 'checked array read requires array and i64 index'),
    ({'Lib': 'export def unsupported(x)\n    return x[?0]\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.unsupported([1])\n'}, 'cannot infer function result'),
]


# Local immutable alias inference, including nested projections, forwarding,
# managed snapshots, and names shadowed inside branches or loops.
VALID.extend([
    ({'Lib': '''export def identity(value)
    let alias = value
    return alias
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.identity(1))
    print(L.identity("hello"))
    return 0
'''}, b'1\nhello\n', 3),
    ({'Lib': '''export def name(box)
    let element = box.items[!0]
    let result = element.name
    return result
''', 'Main': '''import Lib alias L
rec Item(name: string)
rec Box(items: Array[Item])
def main() -> i64
    let box = Box([Item("alpha")])
    print(L.name(box))
    return 0
'''}, b'alpha\n', 2),
    ({'Lib': '''export def entry(mapping)
    let alias = mapping
    let value = alias[!"key"]
    return value
''', 'Main': '''import Lib alias L
def main() -> i64
    let mapping = Dict[string, string]().insert("key", "data")
    print(L.entry(mapping))
    return 0
'''}, b'data\n', 2),
    ({'Lib': '''export def part(box)
    let alias = box
    let elements = alias.items
    return elements[!0].name
export def forwarded(box)
    let value = part(box)
    return value
''', 'Main': '''import Lib alias L
rec Item(name: string)
rec Box(items: Array[Item])
def main() -> i64
    var items = [Item("old")]
    let box = Box(items)
    items[!0] = Item("new")
    print(L.forwarded(box))
    return 0
'''}, b'old\n', 3),
    ({'Lib': '''export def head(values, flag: bool)
    if flag
        let values = "inner"
        let result = values
        return result
    let result = values[!0]
    return result
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.head(["outer"], true))
    print(L.head(["outer"], false))
    return 0
'''}, b'inner\nouter\n', 2),
    ({'Lib': '''export def outer(values)
    for values in 0..2
        print(values)
    let result = values[!0]
    return result
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.outer(["outer"]))
    return 0
'''}, b'0\n1\nouter\n', 2),
    ({'Lib': '''export def first(values)
    let alias = values
    return choose(alias)
def choose(values)
    let item = values[!0]
    return item
''', 'Main': '''import Lib alias L
def main() -> i64
    print(L.first([42]))
    return 0
'''}, b'42\n', 3),
])
INVALID.extend([
    ({'Lib': '''export def mutable(values)
    var item = values[!0]
    return item
''', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.mutable([1])\n'}, 'cannot infer function result'),
    ({'Lib': '''export def escape(values)
    if true
        let item = values[!0]
    return item
''', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.escape([1])\n'}, 'cannot infer function result'),
    ({'Lib': '''export def shadow(value, flag: bool)
    if flag
        let value = 3
        return value
    return value
''', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.shadow("text", true))\n    return 0\n'}, 'return type mismatch'),
    ({'Lib': '''export def forward(value)
    let earlier = later
    let later = value
    return earlier
''', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.forward(2)\n'}, 'cannot infer function result'),
    ({'Lib': '''export def wrong(value)
    let item = value[!0]
    return item
''', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.wrong(12)\n'}, 'invalid inferred result projection'),
])


def run(*args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=100)


def main():
    cache = BuildCache(require_cc())
    # Reuse the generic compiler cache across both suites.
    compiler = cache.native_driver('rewrite-generics-v1', ENTRY, extra_modules=MODULES)
    with tempfile.TemporaryDirectory(prefix='rewrite-inferred-generics-') as tmp:
        root = Path(tmp)
        generated = root / 'generated.c'
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        for number, (files, expected, count) in enumerate(VALID):
            for name, text in files.items():
                (root / f'{name}.sirius').write_text(source(text))
            result = run(compiler, '--compile-modules', root, 'Main', generated)
            assert result.returncode == 0, (number, result.stderr)
            output = generated.read_bytes()
            result = run(compiler, '--compile-modules', root, 'Main', generated)
            assert result.returncode == 0 and generated.read_bytes() == output, (number, result.stderr)
            definitions = len(re.findall(rb'^static inline [^\n]*f\d+\([^\n]*\) \{', output, re.MULTILINE))
            assert definitions == count, (number, definitions, count)
            binary = executable_path(root / f'case_{number}')
            execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                     '-I', str(ROOT / 'runtime'), str(generated), str(runtime),
                     *link_flags(cache.cc), '-o', str(binary)])
            actual = run(binary)
            assert actual.returncode == 0 and normalize_newlines(actual.stdout) == expected and not actual.stderr, (number, actual)
        for number, (files, fragment) in enumerate(INVALID):
            for name, text in files.items():
                (root / f'{name}.sirius').write_text(source(text))
            result = run(compiler, '--compile-modules', root, 'Main', generated)
            assert result.returncode == 1 and fragment.encode() in result.stderr, (number, result)
    print(f'PASS inferred generic results: {len(VALID)} ARC-audited valid, {len(INVALID)} invalid, deterministic C')


if __name__ == '__main__':
    main()
