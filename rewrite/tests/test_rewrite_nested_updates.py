#!/usr/bin/env python3
"""Indexed record-field updates: value semantics, bounds and ARC parity."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines
from rewrite_paths import ENTRY, MODULES as REWRITE_MODULES, DRIVERS


def run(command):
    return subprocess.run([str(x) for x in command], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = REWRITE_MODULES
    compiler = cache.native_driver('restart-compiler', ENTRY, extra_modules=modules)
    valid = [
        ('''rec Token(kind: i64, text: string)
def main() -> i64
\tvar tokens = [Token(1, "old"), Token(2, "untouched")]
\tlet snapshot = tokens
\ttokens[!0].text = "new"
\ttokens[!0].kind += 4
\tprint(snapshot[!0].text)
\tprint(snapshot[!0].kind)
\tprint(tokens[!0].text)
\tprint(tokens[!0].kind)
\tprint(tokens[!1].text)
\treturn 0
''', b'old\n1\nnew\n5\nuntouched\n'),
        ('''rec R(text: string)
def main() -> i64
\tvar values = [R("start")]
\tlet old = values
\tvalues[!0].text += "-next"
\tvalues[!0].text = values[!0].text
\tprint(old[!0].text)
\tprint(values[!0].text)
\treturn 0
''', b'start\nstart-next\n'),
        ('''rec R(a: i64, b: i64)
def index() -> i64
\tprint(77)
\treturn -1
def rhs() -> i64
\tprint(88)
\treturn 5
def main() -> i64
\tvar rows = [R(1, 2), R(3, 4)]
\trows[!index()].b += rhs()
\tprint(rows[!1].a)
\tprint(rows[!1].b)
\treturn 0
''', b'77\n88\n3\n9\n'),
        ('''rec A(x: string, y: bool)
rec B(y: bool, x: string)
def change(values: Array[B]) -> Array[B]
\tvar result = values
\tresult[!0].x = "updated"
\tresult[!0].y = false
\treturn result
def main() -> i64
\tlet a: Array[A] = [A("saved", true)]
\tlet b = change(a)
\tprint(a[!0].x)
\tprint(a[!0].y)
\tprint(b[!0].x)
\tprint(b[!0].y)
\treturn 0
''', b'saved\ntrue\nupdated\nfalse\n'),
        ('''rec R(s: string, n: i64)
def main() -> i64
\tvar values: Array[R] = Array[R]()
\tvalues = values.append(R("first", 1))
\tlet saved = values
\tfor i in 0..20
\t\tvalues[!0].s += "x"
\t\tvalues[!0].n += 1
\tprint(saved[!0].s)
\tprint(saved[!0].n)
\tprint(values[!0].n)
\tprint(len(values[!0].s))
\treturn 0
''', b'first\n1\n21\n25\n'),
        ('''rec R(x: i64)
def main() -> i64
\tvar values = [R(12)]
\tvalues[!0].x -= 2
\tvalues[!0].x *= 3
\tvalues[!0].x /= 5
\tprint(values[!0].x)
\treturn 0
''', b'6\n'),
        ('''rec R(s: string)
def main() -> i64
\tvar values = [R("one"), R("two")]
\tlet before = values
\tvalues[!0].s = values[!1].s
\tvalues[!1].s = "x"
\tprint(before[!0].s)
\tprint(before[!1].s)
\tprint(values[!0].s)
\tprint(values[!1].s)
\treturn 0
''', b'one\ntwo\ntwo\nx\n'),
        ('''rec R(s: string, x: i64)
def main() -> i64
\tvar values = [R("a\\0é", 2)]
\tvalues[!0].s = values[!0].s + "b"
\tvalues[!0].x = values[!0].x + 9
\tprint(values[!0].s)
\tprint(values[!0].x)
\treturn 0
''', b'a\x00\xc3\xa9b\n11\n'),
    ]
    invalid = [
        'rec R(s: string)\ndef main() -> i64\n\tlet a = [R("x")]\n\ta[!0].s = "y"\n\treturn 0\n',
        'rec R(x: i64)\ndef modify(a: Array[R]) -> i64\n\ta[!0].x = 2\n\treturn 0\ndef main() -> i64\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0].x = "bad"\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0].missing = 3\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [1]\n\ta[!0].x = 2\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tvar a = [R("x")]\n\ta[!0].s *= 3\n\treturn 0\n',
        'rec R(b: bool)\ndef main() -> i64\n\tvar a = [R(true)]\n\ta[!0].b += 1\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(2)]\n\ta[!true].x = 2\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(2)]\n\ta[!0].x.y = 2\n\treturn 0\n',
    ]
    bounds = [
        'rec R(s: string)\ndef main() -> i64\n\tvar a = [R("x")]\n\ta[!1].s = "y"\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tvar a = [R("x")]\n\ta[!-2].s += "y"\n\treturn 0\n',
        'rec R(s: string)\ndef main() -> i64\n\tvar a = Array[R]()\n\ta[!0].s = "y"\n\treturn 0\n',
        'rec R(x: i64)\ndef main() -> i64\n\tvar a = [R(1)]\n\ta[!0].x = 2\n\ta[!2].x /= 2\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        audit = root / 'audit-runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = root / 'source.sirius', root / 'generated.c', root / 'program'
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('emission', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS {len(valid)} indexed-record update programs (audited native C)', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS {len(invalid)} invalid indexed-record updates', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('emission bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS {len(bounds)} indexed-record bounds traps', flush=True)


if __name__ == '__main__':
    main()
