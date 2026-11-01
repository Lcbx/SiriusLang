#!/usr/bin/env python3
"""Array[Array[i64]] native execution, ARC, CoW and errors."""
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
        ('''def main() -> i64
	let rows = [[1, 2], [3, 4, 5]]
	print(len(rows))
	print(len(rows[!1]))
	print(rows[!-1][!-1])
	return 0
''', b'2\n3\n5\n'),
        ('''def main() -> i64
	let empty: Array[Array[i64]] = Array[Array[i64]]()
	var rows = empty.append(Array[i64]()).append([7])
	print(len(empty))
	print(len(rows[!0]))
	print(rows[!1][!0])
	return 0
''', b'0\n0\n7\n'),
        ('''def main() -> i64
	var rows = [[1, 2], [3]]
	let snapshot = rows
	rows[!0][!1] += 10
	rows[!1] = [9]
	rows = rows.append([4])
	print(snapshot[!0][!1])
	print(snapshot[!1][!0])
	print(rows[!0][!1])
	print(rows[!1][!0])
	print(rows[!2][!0])
	return 0
''', b'2\n3\n12\n9\n4\n'),
        ('''def grow(rows: Array[Array[i64]]) -> Array[Array[i64]]
	var result = rows
	result[!0] = result[!0].append(8)
	return result.append([4, 5])
def main() -> i64
	let rows = [[1]]
	let copy = grow(rows)
	print(len(rows[!0]))
	print(len(copy))
	print(copy[!0][!1])
	print(copy[!1][!1])
	return 0
''', b'1\n2\n8\n5\n'),
        ('''rec Matrix(rows: Array[Array[i64]])
def main() -> i64
	var m = Matrix([[1, 2], [3]])
	let old = m
	m.rows[!0][!1] = 9
	m.rows = m.rows.append([5])
	print(old.rows[!0][!1])
	print(len(old.rows))
	print(m.rows[!0][!1])
	print(m.rows[!2][!0])
	return 0
''', b'2\n2\n9\n5\n'),
        ('''rec Matrix(rows: Array[Array[i64]])
def main() -> i64
	var boxes = [Matrix([[1, 2]])]
	let snapshot = boxes
	boxes[!0].rows[!0][!1] = 99
	print(snapshot[!0].rows[!0][!1])
	print(boxes[!0].rows[!0][!1])
	return 0
''', b'2\n99\n'),
        ('''def main() -> i64
	var rows = [[2], [3]]
	let before = rows
	for row in rows
		print(row[!0])
		rows = rows.append([9])
	print(len(before))
	print(len(rows))
	return 0
''', b'2\n3\n2\n4\n'),
        ('''def index() -> i64
	print(6)
	return 0
def main() -> i64
	var matrix = [[4, 5]]
	let old = matrix
	matrix[!index()][!1] *= 2
	matrix[!0] = matrix[!0]
	print(old[!0][!1])
	print(matrix[!0][!1])
	return 0
''', b'6\n5\n10\n'),
    ]
    invalid = [
        'def main() -> i64\n\tlet a = [[1], ["x"]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a: Array[Array[i64]] = [1]\n\treturn 0\n',
        'def main() -> i64\n\tlet a: Array[Array[string]] = [[1]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a: Array[Array[i64]] = [["x"]]\n\treturn 0\n',
        'def main() -> i64\n\tlet a = Array[Array[Optional[i64]]]()\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [[1]].append(["x"])\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [[1]]\n\ta[!0] = ["x"]\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [[1]]\n\ta[!0][!0] = "x"\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [[1]]\n\ta[!0][!0] = 3\n\treturn 0\n',
        'rec Matrix(rows: Array[Array[Optional[i64]]])\ndef main() -> i64\n\treturn 0\n',
    ]
    bounds = [
        'def main() -> i64\n\tlet a = [[1]]\n\tprint(a[!1][!0])\n\treturn 0\n',
        'def main() -> i64\n\tlet a = [[1]]\n\tprint(a[!0][!1])\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [[1]]\n\ta[!-2][!0] = 3\n\treturn 0\n',
        'def main() -> i64\n\tvar a = [[1]]\n\ta[!0][!-2] = 3\n\treturn 0\n',
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
        print(f'PASS nested arrays: {len(valid)} audited native C programs', flush=True)
        for i, content in enumerate(invalid):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, ('invalid', i, result)
        print(f'PASS nested arrays: {len(invalid)} invalid programs', flush=True)
        for i, content in enumerate(bounds):
            source.write_text(content, encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, ('compile bounds', i, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode != 0 and b'index out of bounds' in result.stderr, ('native bounds', i, result)
        print(f'PASS nested arrays: {len(bounds)} bounds traps', flush=True)


if __name__ == '__main__':
    main()
