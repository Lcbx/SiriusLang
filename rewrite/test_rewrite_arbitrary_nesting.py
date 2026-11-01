#!/usr/bin/env python3
"""Arbitrary array nesting with native ARC/CoW, errors and bounds."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(cmd):
    return subprocess.run([str(x) for x in cmd], capture_output=True, timeout=60)


def sirius(code):
    # Keep source fixtures readable without relying on spaces for indentation.
    return code.replace('\n    ', '\n\t')


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('restart-compiler', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid = [('def main() -> i64\n'
      '    let rows = [[[1, 2]], [[3]]]\n'
      '    print(len(rows))\n'
      '    print(rows[!0][!0][!1])\n'
      '    return 0\n',
      '2\n2\n'),
     ('def main() -> i64\n'
      '    let empty: Array[Array[Array[string]]] = Array[Array[Array[string]]]()\n'
      '    var rows = empty.append([["a"]])\n'
      '    let old = rows\n'
      '    rows[!0][!0][!0] = "z"\n'
      '    print(len(empty))\n'
      '    print(old[!0][!0][!0])\n'
      '    print(rows[!0][!0][!0])\n'
      '    return 0\n',
      '0\na\nz\n'),
     ('rec Leaf(name: string)\n'
      'rec Forest(trees: Array[Array[Leaf]])\n'
      'def main() -> i64\n'
      '    var f = Forest([[Leaf("a")], [Leaf("b")]])\n'
      '    let snapshot = f\n'
      '    f.trees[!0][!0].name = "new"\n'
      '    print(snapshot.trees[!0][!0].name)\n'
      '    print(f.trees[!0][!0].name)\n'
      '    return 0\n',
      'a\nnew\n'),
     ('rec Leaf(nums: Array[Array[i64]])\n'
      'rec Node(children: Array[Array[Leaf]])\n'
      'def main() -> i64\n'
      '    var x = Node([[Leaf([[10], [20]])]])\n'
      '    let old = x\n'
      '    x.children[!0][!0].nums[!1][!0] += 5\n'
      '    print(old.children[!0][!0].nums[!1][!0])\n'
      '    print(x.children[!0][!0].nums[!1][!0])\n'
      '    return 0\n',
      '20\n25\n'),
     ('def main() -> i64\n'
      '    var flags = [true, false]\n'
      '    let old = flags\n'
      '    flags[!0] = false\n'
      '    flags = flags.append(true)\n'
      '    print(old[!0])\n'
      '    print(flags[!0])\n'
      '    print(flags[!2])\n'
      '    return 0\n',
      'true\nfalse\ntrue\n'),
     ('def main() -> i64\n'
      '    let old: Array[Array[bool]] = [[true, false]]\n'
      '    var now = old\n'
      '    now[!0][!1] = true\n'
      '    print(old[!0][!1])\n'
      '    print(now[!0][!1])\n'
      '    return 0\n',
      'false\ntrue\n'),
     ('def add_row(values: Array[Array[Array[string]]]) -> Array[Array[Array[string]]]\n'
      '    return values.append([["more"]])\n'
      'def main() -> i64\n'
      '    let a = [[ ["start"] ]]\n'
      '    let b = add_row(a)\n'
      '    print(len(a))\n'
      '    print(len(b))\n'
      '    print(b[!1][!0][!0])\n'
      '    return 0\n',
      '1\n2\nmore\n'),
     ('rec Item(value: string)\n'
      'def main() -> i64\n'
      '    var a: Array[Array[Array[Item]]] = Array[Array[Array[Item]]]()\n'
      '    a = a.append([[Item("hello")]])\n'
      '    print(a[!0][!0][!0].value)\n'
      '    return 0\n',
      'hello\n')]
    invalid = ['def main() -> i64\n    let a: Array[Array[Array[i64]]] = [["x"]]\n    return 0\n',
     'def main() -> i64\n    let a = Array[Array[Unknown]]()\n    return 0\n',
     'def main() -> i64\n    let a = Array[Optional[i64]]()\n    return 0\n',
     'def main() -> i64\n    let a: Array[Array[bool]] = [[1]]\n    return 0\n',
     'def main() -> i64\n    var a = [[[1]]]\n    a[!0][!0] = ["x"]\n    return 0\n']
    bounds = [('def main() -> i64\n    let items = [[[1]]]\n    print(items[!2][!0][!0])\n    return 0\n',
      'index out of bounds'),
     ('rec Item(value: string)\n'
      'def main() -> i64\n'
      '    var items = [[[Item("x")]]]\n'
      '    items[!0][!0][!1].value = "changed"\n'
      '    return 0\n',
      'index out of bounds'),
     ('def main() -> i64\n    var a = [[["a"]]]\n    a[!0][!-2][!0] = "x"\n    return 0\n',
      'index out of bounds')]
    with tempfile.TemporaryDirectory() as td:
        root = Path(td)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, generated, binary = (root / item for item in ('source.sirius', 'generated.c', 'program'))
        for i, (content, expected) in enumerate(valid):
            source.write_text(sirius(content), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected.encode() and not result.stderr, ('native', i, result)
        print(f'PASS arbitrary nesting: {len(valid)} ARC-audited native programs', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(sirius(content), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS arbitrary nesting: {len(invalid)} invalid programs', flush=True)
        for i, (content, error) in enumerate(bounds):
            source.write_text(sirius(content), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode != 0 and error.encode() in result.stderr, ('native bounds', i, result)
        print(f'PASS arbitrary nesting: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
