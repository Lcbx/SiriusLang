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
]

INVALID = [
    ({'Lib': 'export def unsupported(x)\n    return x[!0]\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.unsupported([1])\n'}, 'cannot infer function result'),
    ({'Lib': 'export def unsupported(x)\n    let y = x\n    return y\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.unsupported(1)\n'}, 'cannot infer function result'),
    ({'Lib': 'export def broken(x)\n    return broken(x)\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.broken(1)\n'}, 'cannot infer function result'),
    ({'Lib': 'export def choose(x, flag: bool)\n    if flag\n        return x\n    return 4\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.choose("word", true))\n    return 0\n'}, 'return type mismatch'),
    ({'Lib': 'export def echo(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.echo("not i64")\n'}, 'return type mismatch'),
    ({'Lib': 'export def echo(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return L.echo(1, 2)\n'}, 'argument count mismatch'),
    ({'Lib': 'export def impossible(x)\n    return missing\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    return 0\n'}, 'cannot infer function result'),
    ({'Main': 'def main()\n    return 0\n'}, 'main must have signature'),
    ({'Lib': 'export def index(x)\n    return x\n', 'Main': 'import Lib alias L\ndef main() -> i64\n    print(L.index([1, "x"]))\n    return 0\n'}, 'array element type mismatch'),
]


def run(*args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=100)


def main():
    cache = BuildCache(require_cc())
    # Reuse the generic compiler cache across both suites.
    compiler = cache.native_driver('rewrite-generics-v1', ENTRY, extra_modules=MODULES)
    with tempfile.TemporaryDirectory(prefix='rewrite-inferred-generics-') as tmp:
        root = Path(tmp)
        generated = root / 'generated.c'
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
                     '-I', str(ROOT / 'runtime'), generated, cache.runtime(),
                     *link_flags(cache.cc), '-o', binary])
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
