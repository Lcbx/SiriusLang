#!/usr/bin/env python3
"""Check first row-major matrix utilities, shape errors, CoW and MIR parity."""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, compile_native, link_flags, require_cc
from project_paths import ROOT


PROGRAM = '''import Std.Numeric alias N

def expect_error(value: N.MatrixResult, expected: string) -> bool
	match value
		N.MatrixOk(matrix)
			return false
		N.ShapeError(message)
			return message == expected
	return false

def main() -> i64
	let data = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
	if not N.valid_shape(2, 3, data) or N.valid_shape(3, 3, data)
		return 1
	if N.valid_shape(-1, 0, Array[f64]()) or N.valid_shape(1, -1, Array[f64]())
		return 2
	# Product checks must not overflow even with huge user-provided dimensions.
	if N.valid_shape(9223372036854775807, 2, [1.0])
		return 3
	if not expect_error(N.make(2, 4, data), "matrix data does not match shape")
		return 4
	match N.make(2, 3, data)
		N.ShapeError(message)
			return 5
		N.MatrixOk(matrix)
			if matrix.rows != 2 or matrix.cols != 3 or matrix.data[!5] != 6.0
				return 6
			if not expect_error(N.reshape(matrix, 4, 2), "matrix data does not match shape")
				return 7
			match N.reshape(matrix, 3, 2)
				N.ShapeError(message)
					return 8
				N.MatrixOk(reshaped)
					if reshaped.rows != 3 or reshaped.cols != 2 or reshaped.data[!3] != 4.0
						return 9
					var edited = reshaped.data
					edited[!0] = 99.0
					if matrix.data[!0] != 1.0 or reshaped.data[!0] != 1.0 or edited[!0] != 99.0
						return 10
			match N.transpose(matrix)
				N.ShapeError(message)
					return 11
				N.MatrixOk(transposed)
					if transposed.rows != 3 or transposed.cols != 2
						return 12
					if transposed.data[!0] != 1.0 or transposed.data[!1] != 4.0 or transposed.data[!2] != 2.0 or transposed.data[!3] != 5.0 or transposed.data[!4] != 3.0 or transposed.data[!5] != 6.0
						return 13
					if matrix.data[!1] != 2.0
						return 14
			let right = N.Matrix(3, 2, [7.0, 8.0, 9.0, 10.0, 11.0, 12.0])
			match N.matmul(matrix, right)
				N.ShapeError(message)
					return 15
				N.MatrixOk(product)
					if product.rows != 2 or product.cols != 2
						return 16
					if product.data[!0] != 58.0 or product.data[!1] != 64.0 or product.data[!2] != 139.0 or product.data[!3] != 154.0
						return 17
			if not expect_error(N.matmul(matrix, matrix), "incompatible matrix dimensions")
				return 18
			# A zero inner dimension yields zeros, not an empty result.
			match N.matmul(N.Matrix(2, 0, Array[f64]()), N.Matrix(0, 3, Array[f64]()))
				N.ShapeError(message)
					return 27
				N.MatrixOk(zeros)
					if zeros.rows != 2 or zeros.cols != 3 or len(zeros.data) != 6 or zeros.data[!5] != 0.0
						return 28
			# Overflow must be rejected before computing or allocating the result.
			if not expect_error(N.matmul(N.Matrix(9223372036854775807, 0, Array[f64]()), N.Matrix(0, 2, Array[f64]())), "matrix result is too large")
				return 29
			if not expect_error(N.transpose(N.Matrix(2, 2, data)), "invalid source matrix shape")
				return 19
			if not expect_error(N.reshape(N.Matrix(2, 2, data), 3, 2), "invalid source matrix shape")
				return 20
			if not expect_error(N.matmul(N.Matrix(2, 2, data), right), "invalid source matrix shape")
				return 21
	# Same-shape addition, singleton axes, scalar-shaped 1x1 inputs and CoW.
	let base = N.Matrix(2, 3, [1.0, 2.0, 3.0, 4.0, 5.0, 6.0])
	match N.add(base, N.Matrix(1, 3, [10.0, 20.0, 30.0]))
		N.ShapeError(message)
			return 34
		N.MatrixOk(added)
			if added.rows != 2 or added.cols != 3 or added.data[!0] != 11.0 or added.data[!3] != 14.0 or added.data[!5] != 36.0
				return 35
			var changed = added.data
			changed[!0] = 888.0
			if base.data[!0] != 1.0 or added.data[!0] != 11.0
				return 36
	match N.add(N.Matrix(2, 1, [10.0, 20.0]), N.Matrix(1, 3, [1.0, 2.0, 3.0]))
		N.ShapeError(message)
			return 37
		N.MatrixOk(outer)
			if outer.rows != 2 or outer.cols != 3 or outer.data[!0] != 11.0 or outer.data[!2] != 13.0 or outer.data[!3] != 21.0 or outer.data[!5] != 23.0
				return 38
	match N.add(N.Matrix(1, 1, [2.5]), base)
		N.ShapeError(message)
			return 39
		N.MatrixOk(shifted)
			if shifted.rows != 2 or shifted.cols != 3 or shifted.data[!0] != 3.5 or shifted.data[!5] != 8.5
				return 40
	if not expect_error(N.add(base, N.Matrix(3, 3, [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0])), "incompatible broadcast dimensions")
		return 41
	if not expect_error(N.add(base, N.Matrix(2, 2, [1.0, 2.0, 3.0, 4.0])), "incompatible broadcast dimensions")
		return 42
	if not expect_error(N.add(base, N.Matrix(1, 2, [1.0])), "invalid source matrix shape")
		return 43
	match N.add(N.Matrix(0, 3, Array[f64]()), N.Matrix(1, 3, [4.0, 5.0, 6.0]))
		N.ShapeError(message)
			return 44
		N.MatrixOk(empty)
			if empty.rows != 0 or empty.cols != 3 or len(empty.data) != 0
				return 45
	match N.add(N.Matrix(9223372036854775807, 0, Array[f64]()), N.Matrix(1, 0, Array[f64]()))
		N.ShapeError(message)
			return 46
		N.MatrixOk(empty)
			if empty.rows != 9223372036854775807 or empty.cols != 0 or len(empty.data) != 0
				return 47
	# Degenerate matrices must not iterate over enormous empty axes.
	match N.transpose(N.Matrix(0, 9223372036854775807, Array[f64]()))
		N.ShapeError(message)
			return 30
		N.MatrixOk(t)
			if t.rows != 9223372036854775807 or t.cols != 0 or len(t.data) != 0
				return 31
	match N.matmul(N.Matrix(0, 0, Array[f64]()), N.Matrix(0, 9223372036854775807, Array[f64]()))
		N.ShapeError(message)
			return 32
		N.MatrixOk(t)
			if t.rows != 0 or t.cols != 9223372036854775807 or len(t.data) != 0
				return 33
	# Shapes with zero-size axes are legal and retain both dimension counts.
	match N.make(0, 3, Array[f64]())
		N.ShapeError(message)
			return 22
		N.MatrixOk(empty)
			match N.transpose(empty)
				N.ShapeError(message)
					return 23
				N.MatrixOk(t)
					if t.rows != 3 or t.cols != 0 or len(t.data) != 0
						return 24
			match N.matmul(empty, N.Matrix(3, 0, Array[f64]()))
				N.ShapeError(message)
					return 25
				N.MatrixOk(product)
					if product.rows != 0 or product.cols != 0 or len(product.data) != 0
						return 26
	return 0
'''


def run(*command: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in command], capture_output=True, timeout=90)


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path, help='existing compatible MIR runner')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-stdlib-numeric-') as temporary:
		root = Path(temporary)
		(root / 'Std').mkdir()
		shutil.copyfile(ROOT / 'stdlib/Std/Numeric.sirius', root / 'Std/Numeric.sirius')
		(root / 'App.sirius').write_text(PROGRAM)
		generated = root / 'App.c'
		compiled = run(cache.seed(), '--module-root', root, 'App', generated)
		assert compiled.returncode == 0, compiled
		native = run(compile_native(cache, generated))
		assert (native.returncode, native.stdout, native.stderr) == (0, b'', b''), native
		binary = root / 'audited'
		audit = run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
			'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
			*link_flags(cache.cc), '-o', binary)
		assert audit.returncode == 0, audit
		audited = run(binary)
		assert (audited.returncode, audited.stdout, audited.stderr) == (0, b'', b''), audited
		runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
		library = root / 'App.smirl'
		built = run(runner, '--build-modules', root, 'App', library)
		assert built.returncode == 0, built
		interpreted = run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
		assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (0, b'', b''), interpreted
		# Linked MIR must emit the same native behavior as direct module compilation.
		linked_c = root / 'linked.c'
		emitted = run(runner, '--emit-linked-c', library, 'M_3_App_4_main', linked_c)
		assert emitted.returncode == 0, emitted
		linked = run(compile_native(cache, linked_c))
		assert (linked.returncode, linked.stdout, linked.stderr) == (0, b'', b''), linked
	print('PASS Std.Numeric: rank-2 row-major helpers, singleton-axis broadcasting, overflow-aware shape checks, CoW, native/MIR and ARC')


if __name__ == '__main__':
	main()
