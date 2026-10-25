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
array shapes). Nested updates preserve CoW snapshots:

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
Noncapturing expression-bodied lambdas are supported; capturing closures are
not. For tagged unions and exhaustive matching, see the
[language design](../LANGUAGE_DESIGN.md#constructor-based-tagged-unions).

## Standard library: callbacks

Import `Std.Functional` when the source root contains `stdlib/`, or arrange
for `Std/` to be present under your chosen module root. Functions operate on
**one-dimensional arrays, sequentially**, and purity is not checked.

| Names | Supported input and output |
| --- | --- |
| `map_i64`, `filter_i64`, `generate_i64`, `fold_i64` | `i64` |
| `map_f64`, `filter_f64`, `generate_f64`, `fold_f64` | `f64` |
| `map_string`, `filter_string`, `generate_string`, `fold_string` | `string` |
| `count_if`, `any`, `all`, `find_index` | Generic array element type; predicate must resolve to `Fn[T,bool]` |

Array-producing helper signatures remain concrete until generic return-array
types can be expressed by exported library functions. Generic named callbacks
specialize from a contextual `Fn` where one is available. For a generic
predicate helper, explicitly type an otherwise ambiguous callback first:

```sirius
let predicate: Fn[string,bool] = is_nonempty
let count = Functional.count_if(values, predicate)
```

`find_index` returns `-1` if nothing matches. Pure/parallel/fused transforms
are design goals, not current behavior. Run
`python tools/test_stdlib_callbacks.py` for tested usage.

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
