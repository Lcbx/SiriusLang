# Build, run, and use Sirius

Sirius compiles `source.sirius` through typed HIR and MIR to C11; the separate
MIR runner can interpret source or saved, optimized MIR. Commands below run
from the repository root. See [Development](DEVELOPMENT.md) for rebuilding the
compiler and testing its own source.

## Native compilation

Requirements: a C11 compiler and runtime sources in `runtime/`. Python 3.10+
is needed for build helper scripts, but not for the bootstrapped compiler itself.

```sh
python tools/build_compiler.py
./sirius examples/fibonacci.sirius fibonacci.c
cc -std=c11 -I runtime fibonacci.c runtime/sirius_runtime.c -lm -o fibonacci
./fibonacci
```

Alternatively, compile the committed seed directly:

```sh
cc -std=c11 -I runtime bootstrap/compiler.c runtime/sirius_runtime.c -lm -o sirius
```

Windows programs use `.exe`; use the C/linker flags appropriate for MinGW or
MSVC-targeting Clang. `tools/build_compiler.py` detects the native toolchain.
The historical AST-to-C backend is gone; there is no implicit fallback.

## MIR interpreter and artifacts

```sh
python tools/build_mir_runner.py
./sirius-run examples/fibonacci.sirius
./sirius-run --emit-mir fibonacci.smir examples/fibonacci.sirius
./sirius-run --load-mir fibonacci.smir
```

The runner executes the same lowered, ownership-optimized MIR as the C emitter.
`--max-steps N` and `--max-depth N` guard interpreted execution only. With
source execution, `arguments()[0]` is the source path; when loading `.smir`, it
is the artifact path; compiled C uses its executable name. These formats are
versioned and experimental, **not** stable portable bytecode/ABIs across
compiler versions. See [Interpreter](INTERPRETER.md) for wire-format details,
limits and parity tests.

## Syntax and values

The [examples](../examples/README.md) give runnable illustrations of `let`,
`var`, `def`, `rec`, `if`, `while`, `for`, structural records, arrays and CoW.

Checked indexing is `a[!i]`; optional indexing is `a[?i]`. Plain `a[i]`
requires the compiler to prove bounds statically (for supported immutable
array shapes). Negative indices in checked and optional array indexing count
from the end (`a[!-1]` is the last element); an empty array or an index below
`-len(a)` is out of bounds. Checked writes accept negative indices and nested
updates preserve CoW snapshots:

```sirius
var state = {items=[{value=1}]}
let snapshot = state
state.items[!0].value = 2
```

A typed `Fn` can specialize a generic named function, locally or through a
qualified imported name:

```sirius
def double(x)
	return x * 2

def apply(f: Fn[i64,i64], x: i64) -> i64
	return f(x)

def main() -> i64
	let callback: Fn[i64,i64] = double
	return apply(callback, 21)
```

Unannotated generic function values require an expected `Fn` signature.

Scalar `i64` addition, subtraction, multiplication and negation wrap modulo
2^64, including `-(-9223372036854775807 - 1)`. Integer division truncates
toward zero, and `%` follows the dividend's sign (`-7 % 3` is `-1`).
Division or remainder by zero, and the minimum `i64` value with divisor `-1`,
trap. Numeric conversion rules remain under design.

Functions may share a name when their annotated **parameter types differ**:

```sirius
def twice(x: i64) -> i64
	return x * 2

def twice(x: f64) -> f64
	return x * 2.0

def main() -> i64
	let integer: Fn[i64,i64] = twice
	return integer(21)
```

Calls choose an exact argument-type match. An overloaded function used as a
value needs an expected `Fn` type; neither return type alone nor implicit
coercion distinguishes overloads. Duplicate signatures and unmatched calls
are compile-time errors. Overloading generic, extern or constructor declarations
is not yet supported. All overloads in a source module must share export visibility.
Expression-bodied lambdas are noncapturing by design. For tagged unions and exhaustive matching, see the
[language design](../LANGUAGE_DESIGN.md#constructor-based-tagged-unions).

## Standard library: initial numerical matrices

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

## Standard library: callbacks

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

Overload resolution uses parameter types, including the callback's full `Fn`
signature. Generic named callbacks and noncapturing lambdas receive an
expected `Fn` type when one is available. Generic helpers can also infer
callback **parameter** types from their own concrete inputs and uses of the
callback (for example, an array element in `map` or an index in `generate`).
Independently typable arguments may appear before or after a callback, including
nested calls such as `map(values, fn(x) x + 1)`. A lambda cannot capture
outer locals: `map(values, fn(x) box.run(x))` is unsupported even with `x`
annotated; pass `box.run` directly when no other state is needed, or use
`map_with_context` to pass a runtime context explicitly:

```sirius
import Std.Functional alias F

def apply_box(box, values)
	return F.map_with_context(values, box, fn(x, ctx) ctx.run(x))
```

This passes `box` as an ordinary argument on each
callback invocation. It works with managed record fields and source-free generic
packages, and returns correctly typed empty arrays. Context is immutable from
the callback's perspective; existing CoW semantics still apply. A callback
parameter whose type cannot be inferred requires an annotation.

The compiler inspects callback argument types ahead of time without changing
runtime evaluation order. The callback's
return type is then inferred from its body. Multi-stage callback-result chains
are also supported when an independently typed argument seeds the chain,
for example `pipeline(fn(s) len(s), fn(x) str(x), [7, 12])` when `pipeline`
calls the first callback on the second callback's result. A callback passed
through another generic function can also infer its inputs from later arguments
(e.g. `relay(fn(x) x * 2, [7, 14])` where `relay` forwards to a generic
`consume(callback, values)`). This works across multiple forwarding functions
and branch bodies. Local callback aliases, record fields holding callbacks, and
mutable reassignments also carry constraints. Nested paths such as
`box.inner.run` are followed through record copies; overwriting `box.inner`
invalidates aliases beneath that field. After conditional assignments,
an alias contributes constraints only when preserved on every continuing path;
exhaustive `match` arms are joined similarly. Branches ending with `return`
do not weaken the aliases of paths that continue, and unreachable statements
do not add constraints. Loops do not assume a body executes. For example,
`let box = {run = callback}` can forward `box.run` to `consume`, even when
`callback` is still generic. Conversely, a **record literal argument** can infer
an unannotated callback field from a generic callee that invokes that field:

```sirius
def sum_box(box, values) -> i64
	var total = 0
	for value in values
		total += box.run(value)
	return total

let answer = sum_box({run = fn(x) x * 2}, [7, 14])
```

Nested literal fields (`box.inner.run`) and independently constrained fields
work as well, including with `.smirg` generic declarations. The inference is
limited to record literals with identifiable callback fields; it does not
infer arbitrary record transformations or types without a concrete seed.
Record function fields support direct indirect calls (`box.run(value)`),
including nested projections; extracting a field first (`let f = box.run;
f(value)`) also works. Dependency cycles without a concrete type seed
still require explicit `Fn` annotations. Compilation diagnostics distinguish
**conflicting** inferred signatures, **cyclic** forwarding dependencies without
a type seed, and **underdetermined** callback parameter types. Conflicts include
the first mismatching type or arity and callback-call lines in the defining
body; cycles include the forwarding chain. For unresolved or cyclic cases,
annotate callback parameters explicitly.
`map` can change the element type, and `generate` can return records, booleans
or nested arrays. Empty results retain those types. `fold` specializes its
return type from the initial value and callback, including heterogeneous arrays.
When the callback's type cannot be inferred, supply a typed `Fn` value.
Source-free `.smirg` packages preserve generic inferred-result definitions.

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

## Standard library: filesystem

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

## Source modules and linking

For `import`, `export`, `.smirl` libraries, source-free `.smirg` specialization,
and bundling, see [Modules and packages](MODULES.md).
