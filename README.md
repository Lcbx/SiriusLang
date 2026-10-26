# Sirius

Sirius is an **experimental, functional-first, statically compiled language**
with Python-like syntax, structural types, immutable value semantics, local
imperative control flow, and copy-on-write (CoW) collections.

The self-hosted compiler lowers source through typed HIR and control-flow MIR to
**C11**. The same MIR can run in an interpreter. Sirius is a research compiler,
not a finished implementation of its [language design](LANGUAGE_DESIGN.md).

## Try it

Requires a C11 compiler (`cc`, GCC, or compatible Clang). Python 3.10+ runs
build and test scripts; it is **not** the compiler implementation.

From the repository root:

```sh
python tools/build_compiler.py
./sirius examples/game_of_life.sirius game_of_life.c
cc -std=c11 -I runtime game_of_life.c runtime/sirius_runtime.c -lm -o game_of_life
./game_of_life
```

On Windows, use `sirius.exe` / `game_of_life.exe` and compiler-appropriate
linker flags. Alternatively, build from the committed C bootstrap without
Python:

```sh
cc -std=c11 -I runtime bootstrap/compiler.c runtime/sirius_runtime.c -lm -o sirius
```

To execute Sirius source without generating C:

```sh
python tools/build_mir_runner.py
./sirius-run examples/game_of_life.sirius
```

See [Usage](docs/USAGE.md) for compiled and interpreted execution, filesystem
and callback libraries, and MIR artifacts. [Modules and packages](docs/MODULES.md)
covers imports, linking, `.smirl` and source-free `.smirg` packages.

## A small program

```sirius
def double(x)
	return x * 2

def apply(f: Fn[i64,i64], x: i64) -> i64
	return f(x)

def main() -> i64
	let callback: Fn[i64,i64] = double
	print(apply(callback, 21))
	return 0
```

Generic named functions specialize from an expected `Fn` signature. The
compiler supports named callbacks (including imported functions) and
noncapturing, expression-bodied lambdas. Capturing closures and transitive
purity checking are **not yet implemented**.

## What's implemented?

- `i64`, `f64`, `bool`, `string`, structural records, homogeneous arrays,
  dictionaries, optionals, and tagged unions with exhaustive `match`.
- Inferred and annotated functions, concrete generic specialization,
  noncapturing function values, recursion / `recur`, loops, and local mutation.
- CoW array and record updates, checked indexing, and some statically proven
  plain indexing; filesystem operations and opt-in `Result` wrappers.
- Path-based source imports, public constructors/constants, named re-exports,
  MIR libraries, portable generic declaration packages, and a MIR interpreter.

**Not implemented:** general compile-time MIR execution, multidimensional
vectorized operations/broadcasting, capturing closures, general effect/purity
inference, parallel mapping, and native/interpreted FFI registration. Features
and restrictions are listed with runnable examples in
[examples/README.md](examples/README.md).

## Develop and test

```sh
python tools/test_all.py --group smoke --fail-fast
python tools/test_all.py --group modules --fail-fast
python tools/test_all.py --list
python tools/test_examples.py
python tools/generate_bootstrap.py --check
```

The source compiler is in `src/`; `bootstrap/compiler.c` is a generated,
checked-in bootstrap artifact. The old Python compiler, flat manifest, and
legacy AST-to-C emitter have been retired. See [Development](docs/DEVELOPMENT.md) and
[Test organization](tests/README.md) for builds, test groups, fixtures,
and bootstrap regeneration.

## Documentation

| Start here | Details |
| --- | --- |
| [Language design](LANGUAGE_DESIGN.md) | Intended semantics; some are future goals |
| [Examples](examples/README.md) | Runnable programs and feature restrictions |
| [Usage](docs/USAGE.md) | CLI, standard library and MIR execution |
| [Modules and packages](docs/MODULES.md) | Source imports, `.smirl`, `.smirg`, and bundles |
| [Development](docs/DEVELOPMENT.md) | Build, tests, bootstrap workflow |
| [Roadmap](ROADMAP.md) | Open priorities, not a changelog |
| [Technical documentation](docs/README.md) | IR, ownership, runtime, interpreter, architecture, historical profiles |

Sirius currently targets a single compiler implementation and experimental
artifact formats; `.smir`, `.smirl` and `.smirg` are **not stable cross-version
ABIs**.
