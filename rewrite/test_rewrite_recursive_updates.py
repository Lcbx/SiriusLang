#!/usr/bin/env python3
"""Deep record/array update paths: CoW snapshots, ARC, errors and bounds."""
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
        ('''rec Leaf(text: string)
rec Branch(leaf: Leaf)
rec Tree(branch: Branch)
def main() -> i64
\tvar tree = Tree(Branch(Leaf("old")))
\tlet before = tree
\ttree.branch.leaf.text = "new"
\tprint(before.branch.leaf.text)
\tprint(tree.branch.leaf.text)
\treturn 0
''', b'old\nnew\n'),
        ('''rec Inner(values: Array[i64])
rec Outer(inner: Inner)
def main() -> i64
\tvar state = Outer(Inner([10, 20]))
\tlet before = state
\tstate.inner.values[!-1] += 4
\tprint(before.inner.values[!1])
\tprint(state.inner.values[!1])
\treturn 0
''', b'20\n24\n'),
        ('''rec Leaf(text: string)
rec Mid(leaf: Leaf)
def main() -> i64
\tvar records = [Mid(Leaf("before"))]
\tlet old = records
\trecords[!0].leaf.text += "!"
\tprint(old[!0].leaf.text)
\tprint(records[!0].leaf.text)
\treturn 0
''', b'before\nbefore!\n'),
        ('''rec Inner(values: Array[string])
rec Outer(child: Inner)
def index() -> i64
\tprint(71)
\treturn 0
def value() -> string
\tprint(72)
\treturn "new"
def main() -> i64
\tvar rows = [Outer(Inner(["old"]))]
\tlet old = rows
\trows[!index()].child.values[!0] = value()
\tprint(old[!0].child.values[!0])
\tprint(rows[!0].child.values[!0])
\treturn 0
''', b'71\n72\nold\nnew\n'),
        ('''rec Item(text: string, count: i64)
rec Container(item: Item)
def main() -> i64
\tvar items = [Container(Item("s", 1))]
\tlet previous = items
\tfor i in 0..30
\t\titems[!0].item.text += "x"
\t\titems[!0].item.count += 1
\tprint(previous[!0].item.text)
\tprint(items[!0].item.count)
\tprint(len(items[!0].item.text))
\treturn 0
''', b's\n31\n31\n'),
        ('''rec Inner(values: Array[i64])
rec Outer(inner: Inner)
def main() -> i64
\tvar x = Outer(Inner([1]))
\tlet before = x
\tx.inner.values[!0] = x.inner.values[!0]
\tx.inner = x.inner
\tx.inner.values[!0] *= 9
\tprint(before.inner.values[!0])
\tprint(x.inner.values[!0])
\treturn 0
''', b'1\n9\n'),
        ('''rec Leaf(name: string)
rec Mid(leaf: Leaf)
rec Root(mid: Mid)
def edit(x: Root) -> Root
\tvar result = x
\tresult.mid.leaf.name = "changed"
\treturn result
def main() -> i64
\tlet a = Root(Mid(Leaf("original")))
\tlet b = edit(a)
\tprint(a.mid.leaf.name)
\tprint(b.mid.leaf.name)
\treturn 0
''', b'original\nchanged\n'),
        ('''rec A(n: i64, data: Array[string])
rec B(data: Array[string], n: i64)
rec Wrapper(item: A)
def main() -> i64
\tvar w = Wrapper(B(["a", "b"], 1))
\tlet saved = w
\tw.item.data[!0] += "x"
\tw.item.n -= 1
\tprint(saved.item.data[!0])
\tprint(w.item.data[!0])
\tprint(w.item.n)
\treturn 0
''', b'a\nax\n0\n'),
    ]
    invalid = [
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tlet a = Outer(Inner(1))
\ta.inner.x = 2
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def alter(a: Outer) -> i64
\ta.inner.x += 1
\treturn 0
def main() -> i64
\treturn alter(Outer(Inner(1)))
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner(1))
\ta.inner.missing = 2
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner(1))
\ta.inner.x = "bad"
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner(1))
\ta.inner.x[!0] = 1
\treturn 0
''',
        '''rec Inner(values: Array[i64])
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner([1]))
\ta.inner.values[!true] = 2
\treturn 0
''',
        '''rec Inner(values: Array[i64])
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner([1]))
\ta.inner.values[!0] = "bad"
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner(1))
\ta.inner.x[!0].bad = 4
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = [Outer(Inner(1))]
\ta[!0].inner.x = true
\treturn 0
''',
        '''rec Inner(x: i64)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = [Outer(Inner(1))]
\ta[0].inner.x = 4
\treturn 0
''',
    ]
    bounds = [
        '''rec Inner(values: Array[i64])
rec Outer(inner: Inner)
def main() -> i64
\tvar a = Outer(Inner([1]))
\ta.inner.values[!7] = 2
\treturn 0
''',
        '''rec Inner(text: string)
rec Outer(inner: Inner)
def main() -> i64
\tvar a = [Outer(Inner("old"))]
\ta[!-2].inner.text = "new"
\treturn 0
''',
        '''rec Inner(values: Array[string])
rec Outer(inner: Inner)
def main() -> i64
\tvar a = [Outer(Inner(["one"]))]
\ta[!0].inner.values[!1] += "extra"
\treturn 0
''',
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
        print(f'PASS {len(valid)} recursive updates (interpreter/audited native)', flush=True)
        for i, contents in enumerate(invalid):
            source.write_text(contents, encoding='utf-8')
            for command in ([compiler, '--run', source], [compiler, source, generated]):
                result = run(command)
                assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS {len(invalid)} invalid recursive updates', flush=True)
        for i, contents in enumerate(bounds):
            source.write_text(contents, encoding='utf-8')
            result = run([compiler, '--run', source])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('interpreter bounds', i, result)
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, audit)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS {len(bounds)} recursive update bounds traps', flush=True)


if __name__ == '__main__':
    main()
