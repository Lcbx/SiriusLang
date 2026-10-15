# Sirius

Sirius is an experimental, functional-first language with Python-like syntax,
structural typing, immutable value semantics, copy-on-write (CoW) collections,
and local imperative control flow. Its self-hosted compiler emits C11; the same
lowered MIR can also run in a Sirius-written interpreter.

**Status:** The compiler is self-hosted on a **subset** of the planned language.
The default native path is parser → typed/specialized HIR → CFG MIR → C11.
The checked-in `bootstrap/compiler.c` is generated from the Sirius sources.
A separate legacy **Sirius** emitter remains available for comparisons, but is
not part of the default compiler or bootstrap. Python is used for build and test
orchestration, **not** for compiling Sirius source.

## Build and run

Requires a C11 compiler (`cc` or `gcc`). Python 3.10+ is needed for the build/test
scripts, but not to compile the committed C bootstrap directly. Run from the
repository root:

```sh
python tools/build_compiler.py
./sirius tests/fixtures/01_scalars.sirius output.c
cc -std=c11 -I runtime output.c runtime/sirius_runtime.c -lm -o output
./output
```

On Windows use `sirius.exe` and `output.exe`. Without Python, build the compiler
from the committed C source:

```sh
cc -std=c11 -I runtime bootstrap/compiler.c runtime/sirius_runtime.c -lm -o sirius
```

The default compiler accepts `sirius input.sirius output.c`. There is no silent
fallback to legacy emission. To compare the retained emitter:

```sh
python tools/build_legacy_compiler.py
./sirius-legacy input.sirius output.c
```

`src/cli.sirius` is the default entry point; `src/legacy_cli.sirius` builds the
legacy executable. The latter also accepts `--mir-c` for differential tests.

### Interpret source or saved MIR

```sh
python tools/build_mir_runner.py
./sirius-run input.sirius [program arguments...]
./sirius-run --emit-mir program.smir input.sirius
./sirius-run --load-mir program.smir [program arguments...]
```

The runner compiles source to the same optimized MIR but **does not generate C
for the input**. It can optionally enforce `--max-steps N` and `--max-depth N`.
Serialized executables are versioned concrete programs, not a stable external
ABI. See [INTERPRETER.md](INTERPRETER.md).

### Concrete and generic MIR libraries

```sh
./sirius-run --emit-lib utils.smirl --export first --export second utils.sirius
./sirius-run --list-lib utils.smirl
./sirius-run --entry second --load-lib utils.smirl
./sirius-run --link-lib combined.smirl utils.smirl other.smirl
```

Libraries contain named **concrete zero-argument roots**, their reachable
specializations and structural signatures. Independently compiled artifacts
can be linked with canonical type remapping and call relocation. Direct CLI
execution requires a selected `()->i64` entry; other concrete functions can
be exposed and inspected.

Cross-library concrete calls use `SIRIUS-MIR-LIB/2` import metadata. A consumer
can declare a function from another library without a dummy implementation:

```sirius
# consumer.sirius
extern def twice(x: i64) -> i64
def consumer() -> i64
	return twice(5)
```

```sirius
# provider.sirius
def twice(x: i64) -> i64
	return x * 2
def provider() -> i64
	return twice(3)
```

```sh
./sirius-run --emit-lib consumer.smirl --export consumer consumer.sirius
./sirius-run --emit-lib provider.smirl --export provider --expose twice provider.sirius
./sirius-run --link-lib combined.smirl consumer.smirl provider.smirl
./sirius-run --entry consumer --load-lib combined.smirl  # exits with 10
./sirius-run --emit-c-lib combined.c --entry consumer combined.smirl
```

Imports are inferred from reachable `extern def` declarations, and linking
rejects missing or incompatible structural signatures. The earlier
`--import NAME` placeholder option remains for compatibility. `--emit-c-lib`
allows native compilation of the *same linked concrete MIR*; mixed calls
between native and interpreted function implementations are not yet supported.

A first link-time generic specialization path is available using template-bearing
`SIRIUS-MIR-LIB/6` artifacts (versions 3–5 remain readable). A generic provider may contain only templates:

```sirius
# generic.sirius
def identity(x)
	return x
```

```sh
./sirius-run --emit-lib generic.smirl --template identity generic.sirius
./sirius-run --emit-lib consumer.smirl --export consumer consumer.sirius
./sirius-run --link-lib combined.smirl consumer.smirl generic.smirl
```

The consumer uses a concrete `extern def identity(x: T) -> T` declaration.
The linker specializes the saved syntax arena against the imported structural
argument types and verifies the complete signature. Multiple consumers can
request different specializations; equal requests share one implementation.
Concrete exports take precedence over templates. Linked output is still a
concrete version-1 library and works in the interpreter or native C emitter.

Version-6 libraries persist the syntax arena, resolved module declarations,
canonical types, constants, and a **limited parameterized HIR** representation
for generic bodies that return a single expression built from parameters,
literals, and scalar unary/binary operations. Type parameters are bound at link
time and operators checked for the concrete types; other bodies use the existing
concrete analyzer. The loader validates stored HIR against syntax. Version-3
(source), version-4 (syntax), and version-5 (declarations) remain readable.
Full parameterized HIR for control flow, calls, collections, and constructors
remains to be implemented.
Specialized functions can call typed external functions supplied by other
libraries, including other generic templates. The linker expands transitive
specialization requests, checks structural signatures and rejects missing or
incompatible dependencies. Generic imports still require **concrete** `extern def`
signatures in the provider syntax; generic external declarations, source-level
modules, and mixed native/interpreted function calls remain unimplemented. See [INTERPRETER.md](INTERPRETER.md)
and [MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md).

## Implemented language subset

- Scalars (`i64`, `f64`, `bool`), strings, structural records, rank-1 arrays,
  dictionaries, optionals, generic specializations, constructors (`rec`),
  recursion/`recur`, conditionals, sequential loops, and file/CLI operations.
- Managed values have explicit MIR ownership, CFG release/cleanup passes, CoW
  updates, and optimization of last-use transfers into calls.
- Record spreads/deletions, local `let` record destructuring, and nested
  record/checked-array assignments (including supported compound updates).
- `a[?i]` returns an optional; `a[!i]` checks and panics; plain `a[i]` is
  currently proven safe only for nonnegative constant indices into array
  literals (including nested literal chains). General static bounds inference
  and negative indexing are deferred.
- Module-level compile-time `let` constants support `i64`, `bool`, `string`,
  homogeneous arrays, nested records, earlier constant references, constant
  field/index access, `len` of constant strings/arrays, record spread/delete,
  and supported pure operators. General function evaluation and `comptime`
  are **not** implemented; module array type annotations remain limited.
- MIR and interpreted execution share scalar, collection, and file I/O
  operations, with representative differential tests. Implicit-failure
  unwinding and exact decimal-to-`f64` rounding in all edge cases remain open.

**Not yet implemented:** source imports/modules; multidimensional numerical
arrays, broadcasting, general vectorized operations, transitive purity/effects,
first-class higher-order functional APIs, runtime `Any`, persistent typed generic
artifacts, native library registration, and compile-time MIR execution.
The intended semantics are in [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md), not
inferred from current compiler limitations.

## Development workflow

`tools/source_manifest.py` defines ordered source groups; files are currently
concatenated because source imports are not implemented. Build scripts cache
artifacts under `.build/cache/`.

| Command | Role |
| --- | --- |
| `python tools/build_compiler.py` | Build from **committed** C bootstrap; no regeneration |
| `python tools/test_compiler.py --jobs 4` | Native MIR compiler positive/negative fixtures |
| `python tools/test_legacy_compiler.py --jobs 4` | Separate legacy regression suite |
| `python tools/generate_bootstrap.py` | Generate and replace C bootstrap at a self-hosting fixed point; no fixtures |
| `python tools/generate_bootstrap.py --check` | Verify checked-in C matches generated fixed point |
| `python tools/test_bootstrap.py --jobs 4` | Check fixed point, committed C, and compiler fixtures |
| `python tools/test_bootstrap.py --fixed-point-only` | Check self-hosting without language fixtures |
| `python tools/test_mir_selfhost.py` | Independently check native generations and MIR runtime cases |
| `python tools/test_mir_run.py --all-fixtures` | Native vs interpreted source execution |
| `python tools/test_mir_wire.py --all-fixtures` | MIR serialization and validation |
| `python tools/test_mir_library.py` | Named concrete-library behavior and invalid metadata |
| `python tools/test_mir_link.py` | Merge independent concrete libraries |
| `python tools/test_mir_import.py` | Resolve and type-check concrete imports |
| `python tools/test_mir_generic.py` | Cross-library generic and transitive external dependencies |

After editing `src/`, run focused tests, regenerate the bootstrap, then run
`test_bootstrap.py`. Commit Sirius changes and generated `bootstrap/compiler.c`
together; **do not edit the generated C**. New syntax used inside the compiler
must first be understood by the previously committed seed.

Additional focused tests include `tools/test_tokenizer.py`, `test_parser.py`,
`test_types.py`, `test_hir_worklist.py`, `test_mir_*.py`,
`test_runtime_collections.py`, `test_reference_c.py`, `test_build_cache.py`,
and `test_bootstrap_tools.py`. `test_reference_c.py` checks independent
handwritten C behavior; it is not a source-compiler test. Fixture generators
(`generate_core_fixtures.py`, `generate_extended_fixtures.py`) overwrite
checked-in fixtures and should only be run deliberately. The regression suites
cover a tested subset, not complete language conformance or memory safety.

## Repository and documentation

| Path | Purpose |
| --- | --- |
| `src/` | Lexer, parser, type/HIR/MIR pipeline, CLI entries and interpreter |
| `bootstrap/compiler.c` | Committed reproducible C bootstrap |
| `runtime/` | C11 runtime and API |
| `tests/` | Language, IR, runtime, diagnostics and reference C fixtures |
| `tools/` | Build, test, fixture-generation and profiling scripts |
| `benchmarks/` | Isolated C microbenchmarks |

- [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md): intended semantics.
- [ROADMAP.md](ROADMAP.md): remaining tasks and development priorities.
- [IR_DESIGN.md](IR_DESIGN.md): actual compiler pipeline and IR ownership model.
- [INTERPRETER.md](INTERPRETER.md): dynamic execution and serialized MIR.
- [MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md): proposed modules, generic
  libraries, compile-time execution and native interop.
- [RUNTIME.md](RUNTIME.md): current storage and CoW implementation.
- [MIR_PROFILE.md](MIR_PROFILE.md): profiling methods and historical benchmarks.
