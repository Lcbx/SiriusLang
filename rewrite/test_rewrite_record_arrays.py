#!/usr/bin/env python3
"""Flat record-array value semantics, native/interpreter parity, and ARC checks."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'interpreter.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('''rec Token(kind: i64, text: string)
def update(items: Array[Token]) -> Array[Token]
\tvar result = items
\tresult[!0] = Token(8, "updated")
\tresult = result.append(Token(9, "tail"))
\treturn result
def main() -> i64
\tlet original = [Token(1, "old"), Token(2, "two")]
\tlet changed = update(original)
\tprint(original[!0].text)
\tprint(changed[!0].text)
\tprint(changed[!-1].kind)
\tfor item in changed
\t\tprint(item.text)
\treturn 0
''', b'old\nupdated\n9\nupdated\ntwo\ntail\n'),
        ('''rec R(a: string, b: i64, flag: bool)
def main() -> i64
\tvar items: Array[R] = Array[R]()
\titems = items.append(R("first", 1, true))
\titems = items.append(R("second", 2, false))
\tlet saved = items
\titems[!0] = items[!1]
\titems[!-1] = items[!0]
\tprint(saved[!0].a)
\tprint(saved[!0].b)
\tprint(saved[!0].flag)
\tprint(items[!0].a)
\tprint(items[!-1].b)
\treturn 0
''', b'first\n1\ntrue\nsecond\n2\n'),
        ('''rec R(text: string)
def main() -> i64
\tvar items = [R("same")]
\tlet old = items
\titems[!0] = items[!0]
\titems[!0] = R(items[!0].text + "!")
\tprint(old[!0].text)
\tprint(items[!0].text)
\treturn 0
''', b'same\nsame!\n'),
        ('''rec Point(x: i64, y: i64)
rec Other(y: i64, x: i64)
def count(values: Array[Other]) -> i64
\treturn len(values)
def main() -> i64
\tlet values: Array[Point] = [Point(4, 7), Other(9, 2)]
\tprint(count(values))
\tprint(values[!0].x)
\tprint(values[!1].y)
\treturn 0
''', b'2\n4\n9\n'),
        ('''rec Empty()
def main() -> i64
\tlet a: Array[Empty] = Array[Empty]()
\tlet b = a.append(Empty())
\tprint(len(a))
\tprint(len(b))
\treturn 0
''', b'0\n1\n'),
        ('''rec Row(text: string, n: i64)
def make(n: i64) -> Array[Row]
\tvar result = Array[Row]()
\tfor i in 0..n
\t\tresult = result.append(Row(str(i), i))
\treturn result
def main() -> i64
\tlet rows = make(7)
\tfor row in rows
\t\tprint(row.text)
\t\tprint(row.n)
\treturn 0
''', b''.join(str(i).encode() + b'\n' + str(i).encode() + b'\n' for i in range(7))),
        ('''rec Token(text: string)
def main() -> i64
\tvar items = [Token("a"), Token("b")]
\tfor item in items
\t\tprint(item.text)
\t\titems = items.append(Token("c"))
\tprint(len(items))
\treturn 0
''', b'a\nb\n4\n'),
        ('''rec Cell(text: string)
def main() -> i64
\tlet value = Cell("a\\0é")
\tvar a = [value]
\tlet b = a
\ta[!0] = Cell("new")
\tprint(b[!0].text)
\tprint(a[!0].text)
\treturn 0
''', b'a\x00\xc3\xa9\nnew\n'),
    ]
    invalid = [
        'rec R(x: i64)\ndef main() -> i64\n\tlet a: Array[R] = [1]\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tlet a = [R(1), 2]\n\treturn 0\n',
        'rec R(x: string)\ndef main() -> i64\n\tlet a = [R("x"), "y"]\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0] = 1\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0] = "bad"\n\treturn 0\n',
        'rec R(x: string)\ndef main() -> i64\n\tlet a = [R("x")]\n\ta[!0] = R("y")\n\treturn 0\n',
        'rec R(x: string)\ndef bad(a: Array[R]) -> Array[i64]\n\treturn a\ndef main() -> i64\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tlet a: Array[Missing] = Array[Missing]()\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tlet a: Array[R] = Array[Unknown]()\n\treturn 0\n',
        'rec R(x: i64)\nrec Other(x: string)\ndef main() -> i64\n\tlet a: Array[R] = [Other("no")]\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0] += R(2)\n\treturn 0\n',
    ]
    bounds = [
        'rec R(s: string)\ndef main() -> i64\n\tlet a = [R("a")]\n\tprint(a[!1].s)\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tlet a = [R("a")]\n\tprint(a[!-2].s)\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tvar a = [R("a")]\n\ta[!1] = R("b")\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tvar a = Array[R]()\n\ta[!0] = R("b")\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        audit = root / 'audit_runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = root / 'source.sirius', root / 'generated.c', root / 'program'
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, '--run', source])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('interpreter', i, result)
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS {len(valid)} valid record arrays (interpreter/native ARC)', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            for command in ([compiler, '--run', source], [compiler, source, generated]):
                result = run(command)
                assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS {len(invalid)} invalid record arrays', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, '--run', source])
            assert result.returncode != 0 and b'ERROR: index out of bounds' in result.stderr, ('interpreter bounds', i, result)
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS {len(bounds)} record-array bounds cases', flush=True)


if __name__ == '__main__':
    main()
