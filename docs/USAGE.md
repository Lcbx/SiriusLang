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

Add `--call-graph` to print declaration dependencies and unused functions and
constructors while compiling:

```sh
./sirius --call-graph program.sirius program.c
./sirius --call-graph --module-root . App app.c
```

The report distinguishes calls, function references, constructors, exports and
the entry point. It includes unreachable bodies, including mutually recursive
groups. Module names and named re-exports are resolved; local bindings shadow
declaration names. Calls through function values remain dynamic. The report is
conservative: same-name overloads share possible edges, and it does not prove
branches unreachable or enumerate dynamic targets.

Code emission uses resolved HIR specializations and MIR calls/function values,
retaining `main` and concrete exports and their dependencies. Unreachable
private functions and uncalled constructor bodies produce no executable code.
Constructors are expanded at their call sites; exported generics remain
templates until specialized. Type metadata can remain even when a constructor
body is unused. Reporting does not change the generated C.

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
trap. Explicit `f64(i64)` rounds to nearest, ties to even; `i64(f64)`
truncates toward zero and traps for NaN, infinity or values outside
`[-2^63, 2^63)`. There is no implicit numeric promotion.

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

## Standard library

See [Standard library](STDLIB.md) for numerical matrices, functional array
helpers and filesystem APIs; [Callbacks](CALLBACKS.md) explains function-value
inference and explicit runtime context.

## Source modules and linking

For `import`, `export`, `.smirl` libraries, source-free `.smirg` specialization,
and bundling, see [Modules and packages](MODULES.md).
