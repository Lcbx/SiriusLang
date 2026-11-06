#!/usr/bin/env python3
"""Source-module generic specializations: identity, caching, privacy and ARC."""
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
    ({'Utility': '''export def echo(x) -> i64
    print(x)
    return 7
''', 'Main': '''import Utility alias U
def main() -> i64
    print(U.echo(42))
    print(U.echo("ok"))
    print(U.echo(true))
    print(U.echo(9))
    return 0
'''}, b'42\n7\nok\n7\ntrue\n7\n9\n7\n', 4),
    ({'Utility': '''export def size(values) -> i64
    return len(values)
''', 'Middle': '''import Utility alias U
export def relay(x) -> i64
    return U.size(x)
''', 'Main': '''import Middle alias M
def main() -> i64
    print(M.relay([3, 8, 13]))
    print(M.relay(["hi", "there"]))
    print(M.relay([9]))
    return 0
'''}, b'3\n2\n1\n', 5),
    ({'Utility': '''export def countdown(x, n: i64) -> i64
    if n > 0
        return countdown(x, n - 1)
    print(x)
    return 0
''', 'Main': '''import Utility alias U
def main() -> i64
    print(U.countdown("word", 5))
    print(U.countdown(10, 3))
    return 0
'''}, b'word\n0\n10\n0\n', 3),
    ({'Types': '''export rec Box(items: Array[string])
''', 'Utility': '''import Types alias T
export def read(value) -> i64
    print(value.items[!0])
    return len(value.items)
''', 'Main': '''import Types alias T
import Utility alias U
def main() -> i64
    var items = ["original"]
    let box = T.Box(items)
    items[!0] = "changed"
    print(U.read(box))
    print(U.read(box))
    return 0
'''}, b'original\n1\noriginal\n1\n', 2),
    ({'One': '''def secret(x: i64) -> i64
    return x + 100
export def do(x) -> i64
    return secret(2) + x
''', 'Two': '''def secret(x: i64) -> i64
    return x * 3
export def do(x) -> i64
    return secret(3) + x
''', 'Main': '''import One alias A
import Two alias B
def main() -> i64
    print(A.do(4))
    print(B.do(4))
    return 0
'''}, b'106\n13\n', 5),
    ({'Utility': '''export def unused(x) -> i64
    return x + 1
''', 'Main': '''import Utility alias U
def main() -> i64
    print(8)
    return 0
'''}, b'8\n', 1),
    ({'Utility': '''export def display(x, suffix: string) -> i64
    print(x)
    print(suffix)
    return 0
''', 'Main': '''import Utility alias U
def main() -> i64
    print(U.display("hello", "end"))
    print(U.display(true, "done"))
    return 0
'''}, b'hello\nend\n0\ntrue\ndone\n0\n', 3),
]

INVALID = [
    ({'Utility': 'export def echo(x) -> i64\n    return 1\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.echo(1, 2)\n'}, 'argument count mismatch'),
    ({'Utility': 'export def echo(x, n: i64) -> i64\n    return n\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.echo("x", "wrong")\n'}, 'argument type mismatch'),
    ({'Utility': 'def hidden(x) -> i64\n    return 1\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.hidden(3)\n'}, 'unknown or ambiguous'),
    ({'Utility': 'export def add(x) -> i64\n    return x + 1\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.add("not an integer")\n'}, 'invalid addition operands'),
    ({'Utility': 'export def as_integer(x) -> i64\n    return x\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.as_integer("not an integer")\n'}, 'return type mismatch'),
    ({'Utility': 'export def broken(x) -> i64\n    return unknown_variable\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.broken(1)\n'}, 'unknown'),
    ({'Utility': 'export def broken(x) -> i64\n    return x\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.broken(true)\n'}, 'return type mismatch'),
    ({'Utility': 'export def add(x, offset: i64) -> i64\n    return x + offset\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.add("text", 3)\n'}, 'invalid addition operands'),
    ({'Utility': 'export def show(x) -> i64\n    return 4\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.other(10)\n'}, 'unknown or ambiguous'),
    ({'Utility': 'export def recurse(x) -> i64\n    return recurse([x])\n', 'Main': 'import Utility alias U\ndef main() -> i64\n    return U.recurse(1)\n'}, 'generic specialization limit'),
]


def run(*args):
    return subprocess.run([str(item) for item in args], capture_output=True, timeout=100)


def main() -> None:
    cache = BuildCache(require_cc())
    compiler = cache.native_driver('rewrite-generics-v1', ENTRY, extra_modules=MODULES)
    with tempfile.TemporaryDirectory(prefix='rewrite-generic-') as temporary:
        directory = Path(temporary)
        generated = directory / 'program.c'
        for number, (files, expected, count) in enumerate(VALID):
            for name, content in files.items():
                (directory / f'{name}.sirius').write_text(source(content))
            result = run(compiler, '--compile-modules', directory, 'Main', generated)
            assert result.returncode == 0, (number, result.stderr)
            contents = generated.read_bytes()
            result = run(compiler, '--compile-modules', directory, 'Main', generated)
            assert result.returncode == 0 and generated.read_bytes() == contents, (number, result.stderr)
            definitions = len(re.findall(rb'^static inline [^\n]* f\d+\([^\n]*\) \{', contents, re.MULTILINE))
            assert definitions == count, (number, definitions, count)
            binary = executable_path(directory / f'valid_{number}')
            execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                     '-I', str(ROOT / 'runtime'), generated, cache.runtime(),
                     *link_flags(cache.cc), '-o', binary])
            executed = run(binary)
            assert executed.returncode == 0 and normalize_newlines(executed.stdout) == expected, (number, executed)
        for number, (files, fragment) in enumerate(INVALID):
            for name, content in files.items():
                (directory / f'{name}.sirius').write_text(source(content))
            result = run(compiler, '--compile-modules', directory, 'Main', generated)
            assert result.returncode == 1 and fragment.encode() in result.stderr, (number, result)
        standalone = directory / 'standalone.sirius'
        standalone.write_text(source('''def reuse(x) -> i64
    return 2
def main() -> i64
    return reuse(1)
'''))
        result = run(compiler, standalone, generated)
        assert result.returncode == 1 and b'generic functions require --compile-modules' in result.stderr, result
    print(f'PASS generic modules: {len(VALID)} ARC-audited native, {len(INVALID)} rejected, stable C and specialization counts')


if __name__ == '__main__':
    main()
