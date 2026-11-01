#!/usr/bin/env python3
"""Record fields containing arrays of records: CoW, ownership and errors."""
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
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [
        ('''rec Node(right: i64, text: string)
rec State(nodes: Array[Node], label: string)
def main() -> i64
\tvar state = State([Node(1, "old"), Node(2, "second")], "a")
\tlet snapshot = state
\tstate.nodes[!0].right = 9
\tstate.nodes[!0].text = "new"
\tprint(snapshot.nodes[!0].right)
\tprint(snapshot.nodes[!0].text)
\tprint(state.nodes[!0].right)
\tprint(state.nodes[!0].text)
\treturn 0
''', b'1\nold\n9\nnew\n'),
        ('''rec Item(value: string)
rec Box(items: Array[Item])
def update(x: Box) -> Box
\tvar y = x
\ty.items = y.items.append(Item("extra"))
\ty.items[!0].value += "!"
\treturn y
def main() -> i64
\tlet a = Box([Item("first")])
\tlet b = update(a)
\tprint(len(a.items))
\tprint(len(b.items))
\tprint(a.items[!0].value)
\tprint(b.items[!0].value)
\tprint(b.items[!1].value)
\treturn 0
''', b'1\n2\nfirst\nfirst!\nextra\n'),
        ('''rec Item(value: i64)
rec Collection(items: Array[Item])
def main() -> i64
\tvar box = Collection(Array[Item]())
\tlet empty = box
\tbox.items = box.items.append(Item(2))
\tbox.items[!0].value += 3
\tprint(len(empty.items))
\tprint(box.items[!0].value)
\treturn 0
''', b'0\n5\n'),
        ('''rec Leaf(name: string, nums: Array[i64])
rec Mid(leaves: Array[Leaf])
rec Root(mid: Mid)
def main() -> i64
\tvar tree = Root(Mid([Leaf("old", [1])]))
\tlet old = tree
\ttree.mid.leaves[!0].nums[!0] += 12
\ttree.mid.leaves[!0].name = "new"
\tprint(old.mid.leaves[!0].name)
\tprint(old.mid.leaves[!0].nums[!0])
\tprint(tree.mid.leaves[!0].name)
\tprint(tree.mid.leaves[!0].nums[!0])
\treturn 0
''', b'old\n1\nnew\n13\n'),
        ('''rec Data(text: string)
rec A(items: Array[Data])
rec B(items: Array[Data])
def identity(b: B) -> A
\treturn b
def main() -> i64
\tlet a: A = B([Data("hi")])
\tlet b = identity(a)
\tprint(a.items[!0].text)
\tprint(b.items[!0].text)
\treturn 0
''', b'hi\nhi\n'),
        ('''rec Data(value: string)
rec State(items: Array[Data])
def main() -> i64
\tvar a = [State([Data("one")])]
\tlet snapshot = a
\ta[!0].items[!0].value = "two"
\ta[!0].items = a[!0].items.append(Data("three"))
\tprint(snapshot[!0].items[!0].value)
\tprint(a[!0].items[!0].value)
\tprint(a[!0].items[!1].value)
\treturn 0
''', b'one\ntwo\nthree\n'),
        ('''rec Data(value: string)
rec State(items: Array[Data])
def get_index() -> i64
\tprint(11)
\treturn 0
def main() -> i64
\tvar s = State([Data("a")])
\tlet old = s
\ts.items[!get_index()].value += "z"
\ts.items = s.items
\tprint(old.items[!0].value)
\tprint(s.items[!0].value)
\treturn 0
''', b'11\na\naz\n'),
        ('''rec Data(value: i64)
rec State(items: Array[Data])
def main() -> i64
\tvar s = State([Data(0)])
\tlet saved = s
\tfor i in 0..20
\t\ts.items[!0].value += 1
\tprint(saved.items[!0].value)
\tprint(s.items[!0].value)
\treturn 0
''', b'0\n20\n'),
    ]
    invalid = [
        'rec Data(v: i64)\nrec Box(items: Array[Missing])\ndef main() -> i64\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Box])\ndef main() -> i64\n\treturn 0\n',
        'rec Box(items: Array[Data])\nrec Data(v: i64)\ndef main() -> i64\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tlet x = Box([1])\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data(1)])\n\tx.items = [1]\n\treturn 0\n',
        'rec Data(v: string)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tlet x = Box([Data("x")])\n\tx.items[!0].v = "y"\n\treturn 0\n',
        'rec Data(v: string)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data("x")])\n\tx.items[!0].v = 1\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data(1)])\n\tx.items[!true].v = 1\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data(1)])\n\tx.items[!0].missing = 1\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data(1)])\n\tx.items[!0].v[!0] = 1\n\treturn 0\n',
    ]
    bounds = [
        'rec Data(v: string)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tlet x = Box([Data("a")])\n\tprint(x.items[!1].v)\n\treturn 0\n',
        'rec Data(v: string)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box([Data("a")])\n\tx.items[!-2].v = "y"\n\treturn 0\n',
        'rec Data(v: i64)\nrec Box(items: Array[Data])\ndef main() -> i64\n\tvar x = Box(Array[Data]())\n\tx.items[!0] = Data(3)\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        directory = Path(directory)
        audit = directory / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(audit)])
        source, generated, binary = (directory / n for n in ('input.sirius', 'output.c', 'program'))
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS record-array fields: {len(valid)} valid audited native C', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS record-array fields: {len(invalid)} invalid programs', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS record-array fields: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
