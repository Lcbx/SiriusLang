#!/usr/bin/env python3
"""Nested record fields: snapshots, typed ownership, errors and bounds traps."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(command):
    return subprocess.run([str(item) for item in command], capture_output=True, timeout=60)


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'interpreter.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('''rec Inner(name: string, values: Array[i64])
rec Outer(inner: Inner, label: string)
def change(x: Outer) -> Outer
\tvar next = x
\tnext.inner = Inner("new", [3, 4])
\treturn next
def main() -> i64
\tlet original = Outer(Inner("old", [1, 2]), "label")
\tlet changed = change(original)
\tprint(original.inner.name)
\tprint(changed.inner.name)
\tprint(original.inner.values[!0])
\tprint(changed.inner.values[!0])
\tlet items = [original, changed]
\tprint(items[!1].inner.name)
\treturn 0
''', b'old\nnew\n1\n3\nnew\n'),
        ('''rec Leaf(text: string)
rec Branch(leaf: Leaf)
rec Root(branch: Branch)
def main() -> i64
\tlet a = Root(Branch(Leaf("deep")))
\tlet b = a
\tprint(a.branch.leaf.text)
\tprint(b.branch.leaf.text)
\treturn 0
''', b'deep\ndeep\n'),
        ('''rec Leaf(label: string, data: Array[string])
rec Box(child: Leaf, count: i64)
def replace(box: Box, label: string) -> Box
\tvar next = box
\tnext.child = Leaf(label, ["updated"])
\treturn next
def main() -> i64
\tlet first = Box(Leaf("first", ["original"]), 1)
\tvar list = [first]
\tlet snapshot = list
\tlist[!0].child = replace(first, "second").child
\tprint(snapshot[!0].child.label)
\tprint(snapshot[!0].child.data[!0])
\tprint(list[!0].child.label)
\tprint(list[!0].child.data[!0])
\treturn 0
''', b'first\noriginal\nsecond\nupdated\n'),
        ('''rec Inner(value: string)
rec Outer(child: Inner)
def main() -> i64
\tvar x = Outer(Inner("a"))
\tlet y = x
\tx.child = x.child
\tx.child = Inner(x.child.value + "!")
\tprint(y.child.value)
\tprint(x.child.value)
\treturn 0
''', b'a\na!\n'),
        ('''rec Empty()
rec Container(empty: Empty, name: string)
def main() -> i64
\tlet value = Container(Empty(), "ok")
\tlet copy = value
\tprint(copy.name)
\treturn 0
''', b'ok\n'),
        ('''rec Child(name: string, numbers: Array[i64])
rec A(inner: Child)
rec B(inner: Child)
def identity(value: B) -> A
\treturn value
def main() -> i64
\tlet start: A = B(Child("structural", [5]))
\tlet result = identity(start)
\tprint(start.inner.name)
\tprint(result.inner.numbers[!0])
\treturn 0
''', b'structural\n5\n'),
        ('''rec Inner(text: string)
rec Outer(inner: Inner)
def main() -> i64
\tvar rows = Array[Outer]()
\tfor i in 0..10
\t\trows = rows.append(Outer(Inner(str(i))))
\tlet saved = rows
\trows[!0] = rows[!-1]
\tprint(saved[!0].inner.text)
\tprint(rows[!0].inner.text)
\tprint(len(rows))
\treturn 0
''', b'0\n9\n10\n'),
    ]
    invalid = [
        'rec A(child: B)\nrec B(x: i64)\ndef main() -> i64\n\treturn 0\n',
        'rec A(child: A)\ndef main() -> i64\n\treturn 0\n',
        'rec Inner(x: i64)\nrec Outer(child: Inner)\ndef main() -> i64\n\tlet x = Outer(3)\n\treturn 0\n',
        'rec Inner(x: i64)\nrec Outer(child: Inner)\ndef main() -> i64\n\tvar x = Outer(Inner(1))\n\tx.child = 3\n\treturn 0\n',
        'rec Inner(x: i64)\nrec Outer(child: Inner)\ndef main() -> i64\n\tlet x = Outer(Inner(1))\n\tx.child = Inner(2)\n\treturn 0\n',
        'rec Inner(x: i64)\nrec Outer(child: Inner)\ndef main() -> i64\n\tlet x = Outer(Inner(1))\n\tprint(x.child.missing)\n\treturn 0\n',
        'rec Inner(x: i64)\nrec Outer(child: Inner)\ndef main() -> i64\n\tvar x = Outer(Inner(1))\n\tx.child.x = "bad"\n\treturn 0\n',
        'rec Inner(x: string)\nrec Outer(child: Inner)\ndef main() -> i64\n\tvar xs = [Outer(Inner("a"))]\n\txs[!0].child.x = 17\n\treturn 0\n',
    ]
    bounds = [
        'rec Inner(a: Array[i64])\nrec Outer(inner: Inner)\ndef main() -> i64\n\tlet x = Outer(Inner([1]))\n\tprint(x.inner.a[!2])\n\treturn 0\n',
        'rec Inner(a: Array[string])\nrec Outer(inner: Inner)\ndef main() -> i64\n\tlet x = [Outer(Inner(["one"]))]\n\tprint(x[!0].inner.a[!-2])\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        directory = Path(directory)
        audit = directory / 'audit.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = (directory / n for n in ('input.sirius', 'output.c', 'program'))
        for i, (contents, expected) in enumerate(valid):
            source.write_text(contents, encoding='utf-8')
            result = run([compiler, '--run', source])
            assert (result.returncode, normalize_newlines(result.stdout), result.stderr) == (0, expected, b''), ('interpreter', i, result)
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert (result.returncode, normalize_newlines(result.stdout), result.stderr) == (0, expected, b''), ('native', i, result)
        print(f'PASS nested records: {len(valid)} interpreted and audited native', flush=True)
        for i, contents in enumerate(invalid):
            source.write_text(contents, encoding='utf-8')
            for command in ([compiler, '--run', source], [compiler, source, generated]):
                result = run(command)
                assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS nested records: {len(invalid)} invalid rejected', flush=True)
        for i, contents in enumerate(bounds):
            source.write_text(contents, encoding='utf-8')
            result = run([compiler, '--run', source])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('interpreter bounds', i, result)
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS nested records: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
