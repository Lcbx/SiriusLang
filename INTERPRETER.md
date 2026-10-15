# MIR interpreter and artifacts

Sirius runs the **same linked, typed CFG MIR** in its C11 backend and the
Sirius-written interpreter (`src/mir_interpreter.sirius`). `src/run_cli.sirius`
parses, specializes, lowers, verifies, optimizes and executes source without
emitting C for that input; the runner itself is a native program. There is no
separate Python evaluator or silent legacy-emitter fallback.

## Dynamic execution

```sh
python tools/build_mir_runner.py
./sirius-run input.sirius [args...]
./sirius-run --max-steps 100000 --max-depth 64 input.sirius
python tools/test_mir_run.py --all-fixtures
```

Like native compilation, direct execution expects `main() -> i64`.
`arguments()[0]` is the input source path, while a native compiled program
sees its executable path. The test harness adjusts this difference when
comparing output. Positional program arguments follow the source path; `--`
separates runner flags from a source filename beginning with `-`.

Default limits are **10 million** interpreter steps and **256** nested calls.
Limits must be positive decimal integers up to one billion. Explicit panics
and budget exhaustion report to stderr; invalid CLI usage exits with status 2.
`print` and `eprint` write to stdout and stderr respectively. Missing files
are distinct from existing empty files.

## Serialized concrete executables

```sh
./sirius-run --emit-mir main.smir input.sirius
./sirius-run --load-mir main.smir [args...]
python tools/test_mir_wire.py --all-fixtures
```

`--emit-mir` saves the optimized, verified concrete program without executing
it. `--load-mir` reconstructs descriptors and CFG from the artifact and runs
it without source parsing. `arguments()[0]` becomes the artifact path.

`SIRIUS-MIR-EXEC/1` stores the entry index, canonical structural types,
concrete functions, register types, blocks, instructions and string payloads.
The loader checks format/version, bounds, types, structure and ownership before
execution; malformed artifacts are rejected. There are file-size (64 MiB) and
item-count limits. This is a versioned internal interchange format, **not** a
long-term stable ABI or a generic IR for new specializations.

## Named concrete libraries

```sh
./sirius-run --emit-lib utils.smirl --export first --export second utils.sirius
./sirius-run --list-lib utils.smirl
./sirius-run --entry second --load-lib utils.smirl
python tools/test_mir_library.py
```

`SIRIUS-MIR-LIB/1` stores named **concrete, zero-argument** root functions and
all their reachable concrete specializations. The signatures use canonical
structural type descriptions, independent of local `TypeId` ordering. Library
loading checks duplicate names, root indices, signature consistency, MIR
structure and ownership. `--entry` precedes `--load-lib`; a selected entry
must return `i64` for direct execution. Other concrete signatures can be
exported and inspected.

### Linking and concrete imports

```sh
./sirius-run --link-lib combined.smirl a.smirl b.smirl
./sirius-run --list-lib combined.smirl
./sirius-run --entry first --load-lib combined.smirl
python tools/test_mir_link.py
python tools/test_mir_import.py
```

The linker merges structural types, relocates local function references and
checks duplicate exports. `SIRIUS-MIR-LIB/2` additionally stores unresolved
**concrete** imports with canonical function signatures. On linking, imports
are resolved by symbol name and signature against exported implementations;
missing or incompatible symbols are errors. An unresolved library can be
inspected but not executed. Linking successfully produces a resolved version-1
library, and older version-1 libraries remain valid inputs.

Consumer source can now declare a typed external function without a body:

```sirius
# consumer.sirius
extern def helper(x: i64) -> i64
def consumer() -> i64
	return helper(5)
```

The compiler checks argument and return annotations and automatically emits
`SIRIUS-MIR-LIB/2` import metadata for reachable external calls. No `--import`
option or dummy implementation is needed:

```sh
./sirius-run --emit-lib consumer.smirl --export consumer consumer.sirius
./sirius-run --emit-lib provider.smirl --export provider --expose helper provider.sirius
./sirius-run --link-lib combined.smirl consumer.smirl provider.smirl
./sirius-run --entry consumer --load-lib combined.smirl
python tools/test_mir_extern.py
```

The provider's `helper` must already be reachable from a concrete root; `--expose`
selects that specialization. `extern def` requires explicit parameter and return
types. It cannot have a body and is never an executable root. Direct source
execution or native source compilation rejects unresolved reachable externals.
The old `--import NAME` flag remains available for compatibility with prior
typed-placeholder sources, but is no longer needed for new code.

### Link-time generic specializations

```sh
./sirius-run --emit-lib generic.smirl --template identity generic.sirius
./sirius-run --emit-lib consumer.smirl --export consumer consumer.sirius
./sirius-run --link-lib combined.smirl consumer.smirl generic.smirl
python tools/test_mir_generic.py
python tools/test_mir_generic_semantics.py
```

`SIRIUS-MIR-LIB/6` stores a parsed syntax snapshot, validated resolved declarations, a limited parameterized HIR subset and named templates, which need not
have concrete roots. Consumers use concrete `extern def` declarations; the
linker instantiates missing imports against the provider module, remapping
structural argument types and checking the full resulting signature. Equal
requests share one specialization, and matching concrete exports take priority.
The output is an ordinary resolved concrete library and supports both execution
backends. A specialization can import other concrete functions or templates from separate
artifacts; the linker resolves these transitive dependencies using concrete
structural signatures. Missing or mismatched providers are rejected. Version-6 templates contain module constants, canonical declaration types and symbolic-type HIR annotations for immutable `let` bindings and a final return (including record/array literals, field access and selected builtin calls), without retaining original source text. The loader checks metadata against syntax; concrete local types and operators are revalidated at specialization. Control flow, mutation, user calls and other complex bodies still use the normal analyzer. Versions 3–5 remain readable; external declarations must still be concrete.

### Native execution of linked concrete libraries

```sh
./sirius-run --emit-c-lib combined.c --entry consumer combined.smirl
cc -std=c11 -I runtime combined.c runtime/sirius_runtime.c -lm -o combined
./combined
```

The `--emit-c-lib` path uses the same verified, linked MIR as `--load-lib`, but
emits a standalone C11 program instead of interpreting it. It requires a fully
resolved artifact and a zero-argument `i64` entry; this is whole-program backend
selection, **not** per-function mixed native/interpreted dispatch.

Imports can resolve either to concrete exports or to syntax-backed generic templates.
True source modules, general parameterized HIR, native registration,
and mixed native/interpreted calls remain unimplemented. See
[MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md).

## Execution semantics and checks

The verifier checks typed instruction operands, calls, register use, CFG
successors and owned-value balance. Managed registers hold ownership credits:
`COPY`/`RETAIN` duplicate ownership, `MOVE` transfers it, `RELEASE` drops it,
and returns transfer owned results. Arrays/records use consuming CoW updates;
optionals and dictionary access use typed payloads. Calls may transfer several
**distinct, last-use** managed arguments after optimization. The interpreter
maintains logical reference counts, while native MIR C uses generated type-
directed retain/drop code and the C runtime.

The interpreter handles the tested `i64`, `f64`, `bool`, strings, structural
records, rank-1 arrays, dictionaries and optionals; recursion, CFG control
flow, print/file I/O, nested CoW updates and checked failures are supported.
Native dictionary storage uses generic hashed `S_Dict`; the interpreter keeps
simpler linear storage with structural equality. The tested semantics, not the
storage implementation, are shared.

Useful focused checks are `tools/test_mir.py`, `test_mir_lower.py`,
`test_mir_program.py`, `test_mir_ownership.py`, `test_mir_lifetime.py`,
`test_mir_exit_cleanup.py`, `test_mir_balance.py`, `test_mir_arrays.py`,
`test_mir_records.py`, `test_mir_dict.py`, `test_mir_optional.py`,
`test_mir_c.py`, and `test_mir_interpreter.py`. The shared fixture suites
exercise native vs dynamic output, diagnostics, and selected failure cases.

**Known gaps:** Complete cleanup during implicit runtime failure or budget
termination, exact decimal-to-`f64` rounding at all IEEE-754 boundaries,
integer overflow conventions, generic extern declarations and typed template
artifacts, native/IR cross-calls, and compile-time execution. General purity/effect inference can
be developed later; it is **not a prerequisite** for the intended permissive
compile-time MIR interpreter.
