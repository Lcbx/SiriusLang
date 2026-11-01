#!/usr/bin/env python3
"""Nested string arrays: native execution, ARC, CoW, invalid types and bounds."""
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
	let rows = [["a", "b"], ["c"]]
	print(len(rows))
	print(len(rows[!0]))
	print(rows[!1][!0])
	return 0
''', b'2\n2\nc\n'),
        ('''def main() -> i64
	let original: Array[Array[string]] = Array[Array[string]]()
	let changed = original.append(Array[string]()).append(["hello"])
	print(len(original))
	print(len(changed[!0]))
	print(changed[!1][!0])
	return 0
''', b'0\n0\nhello\n'),
        ('''def main() -> i64
	var rows = [["a", "b"], ["c"]]
	let old = rows
	rows[!0][!1] = "new"
	rows[!1] = ["d"]
	rows = rows.append(["e"])
	print(old[!0][!1])
	print(old[!1][!0])
	print(rows[!0][!1])
	print(rows[!2][!0])
	return 0
''', b'b\nc\nnew\ne\n'),
        ('''def grow(rows: Array[Array[string]]) -> Array[Array[string]]
	var result = rows
	result[!0] = result[!0].append("tail")
	return result.append(["end"])
def main() -> i64
	let before = [["start"]]
	let after = grow(before)
	print(len(before[!0]))
	print(after[!0][!1])
	print(after[!1][!0])
	return 0
''', b'1\ntail\nend\n'),
        ('''rec Grid(rows: Array[Array[string]])
def main() -> i64
	var state = Grid([["left", "right"]])
	let snapshot = state
	state.rows[!0][!1] = "updated"
	state.rows = state.rows.append(["last"])
	print(snapshot.rows[!0][!1])
	print(state.rows[!0][!1])
	print(state.rows[!1][!0])
	return 0
''', b'right\nupdated\nlast\n'),
        ('''rec Grid(rows: Array[Array[string]])
def main() -> i64
	var grids = [Grid([["old"]])]
	let snapshot = grids
	grids[!0].rows[!0][!0] += "!"
	print(snapshot[!0].rows[!0][!0])
	print(grids[!0].rows[!0][!0])
	return 0
''', b'old\nold!\n'),
        ('''def main() -> i64
	var rows = [["a"], ["b"]]
	for row in rows
		print(row[!0])
		rows = rows.append(["more"])
	print(len(rows))
	return 0
''', b'a\nb\n4\n'),
        ('''def index() -> i64
	print(7)
	return 0
def main() -> i64
	var rows = [["x", "y"]]
	let copy = rows
	rows[!index()][!-1] = "changed"
	rows[!0] = rows[!0]
	print(copy[!0][!1])
	print(rows[!0][!1])
	return 0
''', b'7\ny\nchanged\n'),
    ]
    invalid = [
        'def main() -> i64\n\tlet a = [["a"], [1]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a: Array[Array[string]] = [[1]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a: Array[Array[i64]] = [["x"]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a = Array[Array[bool]]()\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [["a"]].append([1])\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [["a"]]\n\ta[!0] = [1]\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [["a"]]\n\ta[!0][!0] = 1\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [["a"]]\n\ta[!0][!0] = "b"\n\treturn 0\n',
        'rec Grid(rows: Array[Array[bool]])\ndef main() -> i64\n\treturn 0\n',
        'def bad(a: Array[Array[string]]) -> Array[Array[i64]]\n\treturn a\ndef main() -> i64\n\treturn 0\n',
    ]
    bounds = [
        'def main() -> i64\n\tlet a = [["x"]]\n\tprint(a[!1][!0])\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [["x"]]\n\tprint(a[!0][!1])\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [["x"]]\n\ta[!-2][!0] = "y"\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [["x"]]\n\ta[!0][!-2] = "y"\n\treturn 0\n',
    ]
    with tempfile.TemporaryDirectory() as directory:
        directory = Path(directory)
        runtime = directory / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, generated, binary = (directory / item for item in ('source.sirius', 'code.c', 'program'))
        for i, (content, expected) in enumerate(valid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile', i, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, ('native', i, result)
        print(f'PASS nested string arrays: {len(valid)} audited native C programs', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS nested string arrays: {len(invalid)} invalid programs', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS nested string arrays: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
