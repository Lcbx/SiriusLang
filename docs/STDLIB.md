# Standard library

Commands and paths are relative to the repository root. Imports require the
module root to expose `Std/`, normally through `stdlib/`.

## Numerical matrices

`Std.Numeric` provides a **rank-2 `f64` matrix wrapper**, not built-in
`Array[T, 2]`. `Matrix(rows, cols, data)` contains a flat row-major
`Array[f64]`: entry `(row, col)` is `data[!(row * cols + col)]`.
`make` returns `MatrixResult` (`MatrixOk` or `ShapeError`) after checking
nonnegative dimensions and element count. `reshape`, `transpose`, and
`matmul` and `add` return the same result type and validate their inputs, including
matrices constructed directly without `make`. `reshape` shares its original
data under CoW; transpose and multiplication allocate new arrays. Shapes
with zero-sized axes are permitted. Products of dimensions that overflow
`i64` are rejected instead of evaluated.

```sirius
import Std.Numeric alias N

def main() -> i64
	match N.matmul(N.Matrix(1, 2, [2.0, 3.0]), N.Matrix(2, 1, [4.0, 5.0]))
		N.MatrixOk(product)
			print(product.data[!0])  # 23
		N.ShapeError(message)
			print(message)
	return 0
```

`add` supports rank-2 elementwise broadcasting: axis lengths must agree or
one must be 1. It returns `ShapeError` for incompatible axes, and permits
0 against 1, producing a zero-sized axis. For example, adding a 1-by-2 row
to a 2-by-2 matrix broadcasts that row to both rows. Results are fresh
arrays; the inputs remain unchanged.

Generic element types, general N-dimensional broadcasting, strided views
and SIMD are **not** part of this helper. A native ranked array descriptor is
not required or currently prioritized. See
[language design](../LANGUAGE_DESIGN.md#7-arrays-and-vectorization) for the
intended eventual operations and `tools/test_stdlib_numeric.py` for executable
edge cases. Imported modules must be visible under the configured module root.

## Functional array helpers

Import `Std.Functional` when the source root contains `stdlib/`, or arrange
for `Std/` to be present under your chosen module root. Functions operate on
**one-dimensional arrays, sequentially**, and purity is not checked.

| Name | Supported signatures |
| --- | --- |
| `map`, `filter` | Generic input and output element types; preserves element order |
| `map_indexed` | Generic transform with `(index: i64, element: T)` callback in one pass; index starts at zero |
| `map_with_context` | Generic transform with `(element: T, context: C)` callback; passes an explicit immutable context to every invocation |
| `generate` | Generic callback result type (including empty arrays) |
| `partition` | Single-pass predicate split into `{matched: Array[T], unmatched: Array[T]}`; stable ordering |
| `fold` | Generic over the array element and accumulator/result types |
| `scan` | Generic prefix accumulation; returns the initial state followed by each accumulated state |
| `zip_with` | Pairwise transform of two arrays of any element types; callback result determines the output type |
| `zip` | Pair two arrays into structural `{left: A, right: B}` records |
| `enumerate` | Generic input; returns an array of structural `{index: i64, value: T}` records |
| `count_if`, `any`, `all`, `find_index` | Generic input elements; predicate must resolve to `Fn[T,bool]` |

`scan(values, initial, callback)` returns `len(values) + 1` states, starting
with `initial`. Its callback takes `(accumulator, element)` and returns the
next accumulator of the same type; empty input produces `[initial]`. The
accumulator may differ from the input element type. For example,
`scan([1, 2, 3], 0, fn(a, x) a + x)` returns `[0, 1, 3, 6]`.

`zip` and `zip_with` traverse left-to-right and stop when either array runs
out (the output length is the shorter input length). They never pad or reject
mismatched lengths. They return correctly typed empty arrays when one input is
empty. For numerical computations requiring equal dimensions, explicitly
validate lengths before calling them.

```sirius
import Std.Functional alias F

let pairs = F.zip([1, 2], ["one"])
let added = F.zip_with([1, 2], [10, 20], fn(a, b) a + b)
# pairs is [{left = 1, right = "one"}]; added is [11, 22]
```

Callbacks are noncapturing. Pass runtime state explicitly with
`map_with_context`; see [Callbacks](CALLBACKS.md) for inference rules, explicit
context and diagnostics. If inference has no concrete type seed, bind the
callback to an annotated `Fn` first. Generic helpers preserve their result
element types for empty arrays.

```sirius
import Std.Functional alias F

def main() -> i64
	let count = F.fold(["sun", "moon"], 0, fn(total, word) total + len(word))
	let sizes = F.map(["sun", "moon"], fn(word) len(word))
	let indexed = F.enumerate(["sun", "moon"])
	return count + sizes[!0] + indexed[!1].index
```

`empty_array_like(array_or_callback)` is a compiler builtin returning an empty
array whose element type is taken from an existing array or the result type of
a concrete `Fn` callback. It evaluates its argument once, but does not invoke
the callback. `map`, `filter`, and `generate` use it internally; no new runtime
representation is required.

`partition(values, predicate)` evaluates the predicate once for each input value,
returning stable `matched` and `unmatched` arrays of the original element
type. Empty inputs, empty partitions, and managed element types are supported.
The source array is not mutated; ordinary CoW semantics apply.

`enumerate` accepts any array element type and returns records with `.index`
(zero-based) and `.value` fields. Empty input yields a correctly typed empty
array. The old `IndexedI64`, `IndexedF64`, and `IndexedString` record types
remain exported for source compatibility; records returned by `enumerate` are
structurally compatible with them. It uses `map_indexed` to populate indexes
in a single traversal. `map_indexed` also supports any result element type,
including correctly typed empty outputs. `find_index` returns `-1` if there is
no match; `any` delegates to it and both short-circuit. Generic named callbacks
specialize from a contextual `Fn` where one is available. If a generic
callback cannot be inferred, bind it to a typed `Fn` first. The helpers are
sequential, and callback purity is not enforced. Run
`python tools/test_stdlib_callbacks.py` for exercised usage.

## Filesystem

`Std.Filesystem` supplies `walk_files(root) -> Array[string]`: a breadth-first,
best-effort walk that skips unreadable directories, omits directories, and
doesn't follow discovered directory symlinks. `walk_files_result` instead
returns a `WalkSuccess(files)` or the first `FilesystemFailure` when listing or
checking an entry fails. Result wrappers also exist for `read_text`,
`write_text`, `list_dir`, and `path_kind`.

`FilesystemFailure(operation, path, kind, code)` reports a portable category
plus the native errno/Win32 code. `path_kind` uses non-following metadata
semantics (`0` error, `1` file, `2` directory, `3` other including symlinks).
Directory traversal is not an atomic snapshot. The earlier built-in
`Optional`/`bool` filesystem APIs remain available. The Windows runtime
uses UTF-8 paths with wide Win32 calls; embedded NUL bytes are rejected.

See `stdlib/Std/Filesystem.sirius` and `examples/filesystem_walk.sirius`.

```sh
python tools/test_filesystem.py
python tools/test_filesystem_walk_result.py
```
