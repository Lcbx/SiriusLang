#!/usr/bin/env python3
"""Focused Array[string] native execution and ARC/CoW regression tests.

Compiles through the rewrite and checks generated C with ARC auditing.
"""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(args):
    return subprocess.run([str(a) for a in args], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('''def main() -> i64
\tlet strings = ["a", "b", "c"]
\tprint(len(strings))
\tprint(strings[!-1])
\tprint(strings[!-3])
\treturn 0
''', b'3\nc\na\n'),
        ('''def main() -> i64
\tlet strings: Array[string] = Array[string]()
\tlet grown = strings.append("abc").append("d")
\tprint(len(strings))
\tprint(grown[!0])
\tprint(grown[!1])
\treturn 0
''', b'0\nabc\nd\n'),
        ('''def id(values: Array[string]) -> Array[string]
\treturn values
\ndef main() -> i64
\tlet before = ["one", "two"]
\tvar after = id(before)
\tafter[!0] = after[!1]
\tafter = after.append("three")
\tfor value in after
\t\tprint(value)
\tprint(before[!0])
\treturn 0
''', b'two\ntwo\nthree\none\n'),
        ('''def main() -> i64
\tvar words = ["start", "end"]
\tlet saved = words
\twords[!0] = words[!0] + words[!1]
\twords[!-1] = words[!0]
\tprint(saved[!0])
\tprint(saved[!1])
\tprint(words[!0])
\tprint(words[!1])
\treturn 0
''', b'start\nend\nstartend\nstartend\n'),
        ('''def make(n: i64) -> Array[string]
\tvar result = Array[string]()
\tfor i in 0..n
\t\tresult = result.append(str(i))
\treturn result
\ndef main() -> i64
\tlet values = make(10)
\tfor s in values
\t\tprint(s)
\treturn 0
''', b''.join(str(i).encode() + b'\n' for i in range(10))),
        ('''def main() -> i64
\tvar items = ["old", "kept"]
\tfor item in items
\t\tprint(item)
\t\titems = items.append("new")
\tprint(len(items))
\treturn 0
''', b'old\nkept\n4\n'),
        ('''def main() -> i64
\tlet a = ["x\\0y", "é"]
\tlet b = a.append("tail")
\tprint(a[!0])
\tprint(b[!1])
\treturn 0
''', b'x\x00y\n\xc3\xa9\n'),
        ('''def main() -> i64
\tlet strings = ["x"]
\tlet ints = [1, 2]
\tprint(strings[!0])
\tprint(ints[!0])
\treturn 0
''', b'x\n1\n'),
    ]
    invalid = [
        '''def main() -> i64
\tlet a = ["a", 1]
\treturn 0
''',
        '''def main() -> i64
\tlet a = [1, "a"]
\treturn 0
''',
        '''def main() -> i64
\tlet a: Array[i64] = ["a"]
\treturn 0
''',
        '''def main() -> i64
\tlet a: Array[string] = [1]
\treturn 0
''',
        '''def main() -> i64
\tlet a = ["a"].append(1)
\treturn 0
''',
        '''def main() -> i64
\tlet a = [1].append("a")
\treturn 0
''',
        '''def main() -> i64
\tvar a = ["a"]
\ta[!0] = 1
\treturn 0
''',
        '''def main() -> i64
\tvar a = [1]
\ta[!0] = "a"
\treturn 0
''',
        '''def main() -> i64
\tlet a = ["x"]
\ta[!0] = "y"
\treturn 0
''',
        '''def bad(a: Array[string]) -> Array[i64]
\treturn a
\ndef main() -> i64
\treturn 0
''',
        '''def main() -> i64
\tlet a = []
\treturn 0
''',
        '''def main() -> i64
\tlet a = Array[bool]()
\treturn 0
''',
        '''def main() -> i64
\tlet a: Array[bool] = [true]
\treturn 0
''',
    ]
    bounds = [
        '''def main() -> i64
\tlet a = ["x"]
\tprint(a[!1])
\treturn 0
''',
        '''def main() -> i64
\tlet a = ["x"]
\tprint(a[!-2])
\treturn 0
''',
        '''def main() -> i64
\tvar a = ["x"]
\ta[!1] = "y"
\treturn 0
''',
        '''def main() -> i64
\tlet a = Array[string]()
\tprint(a[!0])
\treturn 0
''',
    ]
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        audit = root / 'audit_runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = root / 'source.sirius', root / 'generated.c', root / 'program'
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS {len(valid)} valid Array[string] cases (audited native C)', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS {len(invalid)} invalid Array[string] cases', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS {len(bounds)} Array[string] bounds cases', flush=True)


if __name__ == '__main__':
    main()
