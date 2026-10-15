# Sirius

Sirius is an experimental, functional-first, statically compiled language with
Python-like syntax, local imperative control flow, structural typing, immutable
value semantics and copy-on-write collections. Its compiler emits **C11**.

**Current status:** the compiler is written in Sirius and can reproduce its own
committed C bootstrap. Self-hosting covers a subset of
[the intended language](LANGUAGE_DESIGN.md), not the complete design.
The historical Python compiler is archived and is **not** used for active builds.

**Design vs. implementation:** [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md)
defines language semantics; [MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md)
proposes path-derived modules, portable MIR libraries, compile-time execution,
and native/FFI interoperability. The dynamic MIR interpreter can load versioned
executables and **concrete multi-entry MIR libraries** with named exports and
canonical structural signatures. Source imports, generic MIR libraries,
compile-time execution and native linking remain unimplemented.

## Current development priorities

The runtime and MIR migration milestones are stable; feature development has
resumed. The agreed implementation order remains:

1. **Runtime:** improve `S_Array`, `S_Dict`, strings, allocation and CoW behavior,
   preserving snapshots and the existing C ABI. Start with measured bottlenecks.
2. **Simplification:** reduce duplicated runtime/compiler logic and stale build
   paths. Prefer incremental refactors with focused regression tests.
3. **MIR as the default — completed:** normal compilation and the committed
   bootstrap use MIR. **Keep** `src/emit_c.sirius` accessible in the separate
   legacy compiler for differential checks; do not delete it.
4. **Features — active:** implement missing language constructs in tested slices,
   including local `let` record destructuring, nested record/array updates,
   and statically proven plain indexing of array literals (MIR backend only).

The MIR compiler supports compile-time module `let` constants with `i64`,
`bool`, `string`, homogeneous arrays and structural records (including nested
combinations). Initializers accept earlier constant names, field access,
constant array indexing, string concatenation, integer arithmetic, Boolean
`not`, `len(string|array)`, record spreads (`{BASE.*}`) and field deletion.
Record fields have canonical sorted layouts with last-write-wins overrides.
MIR materializes independently owned CoW values on use. Runtime-dependent
initializers, arbitrary function calls and general `comptime` remain deferred.
String module constants permit explicit `: string` annotations; array type
annotations are not yet accepted at module level.

Plain `a[i]` indexing currently proves only nonnegative integer literals applied
  to array literals (including nested literal chains). For other reads use
  `a[?i]` or `a[!i]`; no general length/range inference is implemented yet.

See [ROADMAP.md](ROADMAP.md#current-priorities-and-decisions) for the migration
criteria, and [RUNTIME.md](RUNTIME.md) for the data-structure review. MIR emits
its own typed records and arrays, but now delegates dictionary storage to
`S_Dict` through a managed wrapper; record keys use structural hashing.

## Build and use

Requires a C11 compiler (`cc` or `gcc`). Python 3.10+ is required for development
tools, not for executing the C bootstrap. Run these commands from the project root.

```sh
python tools/build_compiler.py
./sirius tests/fixtures/01_scalars.sirius output.c
cc -std=c11 -I runtime output.c runtime/sirius_runtime.c -lm -o output
./output
```

On Windows use `sirius.exe` / `output.exe`; the build script selects the executable
suffix. Building without Python is also possible:

```sh
cc -std=c11 -I runtime bootstrap/compiler.c runtime/sirius_runtime.c -lm -o sirius
```

`./sirius input.sirius output.c` uses **HIR → MIR → C**, the default and
committed self-hosted bootstrap. The legacy emitter is preserved in a separate
compiler so the default executable does not link two backends:

```sh
python tools/build_legacy_compiler.py
./sirius-legacy input.sirius output.c
```

Record destructuring is available through the MIR compiler:

```sirius
def pick(record)
	let {first = .a, second = .b} = record
	return first
```

The source expression is evaluated once. Missing fields, duplicate bindings and
non-record sources are compile-time errors; only local `let` patterns are
supported. The retained legacy compiler rejects this newer syntax explicitly.

Nested assignments also work through the MIR backend:

```sirius
var state = {items=[{value=1}]}
let snapshot = state
state.items[!0].value = 2
```

The compiler evaluates checked indices once, then reconstructs modified values
from the leaf to the mutable root using CoW. Record-field and checked-index compound assignments also work on nested paths
for supported numeric and string operators. The
legacy emitter still supports only its previously implemented direct updates.

The legacy executable also accepts `--mir-c` for comparisons; the regular
`./sirius` always uses MIR. Unsupported MIR constructs fail
explicitly; there is no implicit fallback. The normal MIR compiler omits
runtime register-initialization bookkeeping after ownership verification;
direct MIR-to-C emission retains defensive checks for unverified input.
The verified emitter also trusts explicit MIR exit releases instead of emitting a
second function-wide cleanup sweep.
Managed destination replacement in verified MIR C uses shared, typed
store helpers (the incoming reference is evaluated before the old one is
released); direct defensive MIR emission retains the explicit checked form.
The MIR interpreter is also available as a **dynamic source runner**:

```sh
python tools/build_mir_runner.py
# Optional execution guards: --max-steps N --max-depth N
./sirius-run input.sirius [program arguments...]
python tools/test_mir_run.py --all-fixtures
# Save optimized, concrete MIR without compiling to C:
./sirius-run --emit-mir program.smir input.sirius
# Load and run without parsing Sirius source:
./sirius-run --load-mir program.smir [program arguments...]
python tools/test_mir_wire.py --all-fixtures
```

`src/run_cli.sirius` uses the same parser, typed HIR, specialization worklist,
MIR ownership passes and optimized linked program as the C emitter. It executes
MIR directly without generating C for the input program. `arguments()[0]`
represents the source path, followed by program arguments. Native C programs
instead see their executable name. For differential tests the harness adjusts
`argv[0]` to be equal for both backends. There is no compile-time MIR execution
or purity analysis. The serialized executable stores canonical type descriptors
and function/CFG instructions; its versioned format is not a general module
or cross-version library ABI. For `--load-mir`, `arguments()[0]` is the artifact
path. See [INTERPRETER.md](INTERPRETER.md) for current limitations.


Named concrete libraries (no source-level `export` declaration required yet):

```sh
./sirius-run --emit-lib utils.smirl --export first --export second utils.sirius
./sirius-run --list-lib utils.smirl
./sirius-run --entry second --load-lib utils.smirl
python tools/test_mir_library.py
```

A library includes all concrete specializations reachable from its zero-argument
export roots, with named function indices and structural signatures independent
of local `TypeId` ordering. The loader validates export uniqueness, signatures,
serialized MIR and ownership before execution. Direct selection requires a
`()->i64` entry; other concrete return types can be exported and inspected.
This is not yet cross-library linking: there are no imports, unresolved symbols,
exported generic bodies or native registration hooks.

For phase-level performance profiling of the native MIR compiler, see
[MIR_PROFILE.md](MIR_PROFILE.md) and `tools/profile_mir.py`.
For clone/allocation counts by generated type ID, run
`python tools/profile_ir_storage.py` (see [MIR_PROFILE.md](MIR_PROFILE.md)).
For runtime dictionary counts during self-compilation, use
`python tools/profile_runtime_dict.py --runs 3` (see [RUNTIME.md](RUNTIME.md)). MIR ownership
analysis now seeds its worklist at the CFG entry, and exit cleanup runs only
one post-edge may-ownership analysis. The MIR C backend stores typed record and
array fields in a compact union (8 bytes rather than 32 on 64-bit targets);
the parser also avoids allocating token slices when lengths cannot match.
Direct record-field updates now use consuming `MIR_RECORD_SET` instead of
reconstructing all fields. Unique records mutate in place, while shared records
clone and retain managed fields to preserve snapshots. The interpreter and C
emitter implement the same behavior; `tools/test_mir_record_set.py` tests both.
These changes preserve observable value semantics.
Direct self-field array appends (`r.items = r.items.append(x)`) now use
`MIR_RECORD_ARRAY_APPEND` to avoid retaining and cloning the old array before
replacing that same field. Shared records and nested arrays still use CoW;
`tools/test_mir_record_array_append.py` validates the interpreter and native C.
The MIR peephole optimizer transfers multiple managed arguments into a call
when their final operations are adjacent releases, avoiding unnecessary
retains and enabling callee-side CoW reuse. Each transferred register must be
unique in the call; duplicate arguments and values used later remain borrowed.
`tools/test_mir_call_move.py` tests the verifier, interpreter and native C,
including three transfers in one call.

## Compiler development and bootstrap maintenance

Three commands have deliberately different responsibilities:

| Command | Purpose |
| --- | --- |
| `python tools/build_mir_runner.py [--output PATH]` | Build `sirius-run` for source and serialized MIR execution |
| `python tools/test_mir_wire.py --all-fixtures` | Check versioned MIR serialization, malformed-input rejection and round-trip parity for 91 fixtures |
| `python tools/test_mir_run.py --all-fixtures` | Compare native/interpreted execution on 18 cases, 3 runtime failures and all 91 shared fixtures |
| `python tools/build_compiler.py [--output PATH]` | Compile the **committed** C bootstrap and runtime into an executable; does not regenerate Sirius sources |
| `python tools/generate_bootstrap.py` | Compile Sirius sources through successive compiler generations to a byte-identical fixed point, then update `bootstrap/compiler.c`; **does not run language fixtures** |
| `python tools/generate_bootstrap.py --check` | Verify committed bootstrap matches the computed fixed point, without modifying it or running fixtures |
| `python tools/test_bootstrap.py` | Verify the fixed point and committed C, then run compiler execution and diagnostic fixtures |
| `python tools/test_bootstrap.py --fixed-point-only` | Verify self-hosting and committed C without running language fixtures |
| `python tools/test_compiler.py` | Test the default MIR compiler built from the committed C bootstrap |
| `python tools/build_legacy_compiler.py [--output PATH]` | Build `sirius-legacy` separately from Sirius sources using the committed MIR bootstrap |
| `python tools/probe_mir_selfhost.py --require-ready` | Quick source-generation probe of the default MIR backend; **not** a fixed-point test |
| `python tools/test_mir_selfhost.py` | Build the standalone MIR compiler, compile successive native generations and compare their C byte-for-byte; run MIR runtime smoke cases (prefers Clang for the large generated C) |

Normal changes to the Sirius compiler:

```sh
python tools/test_compiler.py --jobs 4
python tools/test_legacy_compiler.py --jobs 4
python tools/generate_bootstrap.py
python tools/test_bootstrap.py --jobs 4
```

Commit Sirius source and generated `bootstrap/compiler.c` together. **Never edit
generated C by hand.** The *previous* committed compiler must be able to compile
the updated Sirius source; introduce new syntax in stages.

The ordered source manifests and byte-oriented concatenation live in
`tools/source_manifest.py`. `COMPILER_SOURCES` is MIR-only;
`LEGACY_COMPILER_SOURCES` retains the old emitter and its separate CLI.
Sources are concatenated because Sirius imports are
not implemented yet. `--rebuild` clears content-addressed compiled artifacts,
and test runners supporting parallel fixtures accept `--jobs N` (default up to
4). Cached objects and binaries are stored under `.build/cache/`, while fixture
executables and successive compiler generations use temporary directories.

## Tests

Run individual suites as needed:

| Command | Scope |
| --- | --- |
| `python tools/test_bootstrap_tools.py` | Bootstrap generation, stale-C rejection and CLI dispatch |
| `python tools/test_runtime_collections.py` | Runtime generic arrays/dictionaries, aliasing, CoW and managed values |
| `python tools/test_compiler.py` | Default MIR backend: 91 native executions and 46 expected diagnostics |
| `python tools/test_legacy_compiler.py` | Retained legacy emitter: 85 native executions and 37 expected diagnostics |
| `python tools/test_mir_compiler.py` | Default MIR backend: 91 native executions, 46 expected diagnostics, and two additional positive cases; `--compiler PATH` reuses a native compiler for regression testing |
| `python tools/test_tokenizer.py` | 19 tokenizer golden fixtures |
| `python tools/test_mir_record_set.py` | MIR record updates, snapshot CoW and native C |
| `python tools/test_mir_record_array_append.py` | Fused record-array append, snapshot CoW, verifier and interpreter/native parity |
| `python tools/test_mir_call_move.py` | Last-use managed call argument transfers, duplicate-argument safety, use-after-move rejection and native/interpreter parity; optional `--compiler PATH` |
| `python tools/test_parser.py` | 11 parser golden fixtures and smoke checks |
| `python tools/test_types.py` | Type interning, typed HIR and specialization |
| `python tools/test_hir_worklist.py` | Independent HIR specialization worklist |
| `python tools/test_mir.py` | Hand-constructed executable MIR and verifier |
| `python tools/test_mir_lower.py` | HIR → MIR lowering and interpreter execution |
| `python tools/test_mir_program.py` | Linked concrete MIR specializations |
| `python tools/test_mir_c.py` | Differential MIR-C / legacy-C cases, including compound assignments |
| `python tools/test_mir_selfhost.py` | Native MIR stage-2/3/4 fixed point and targeted runtime regressions |
| `python tools/test_mir_opt.py` | Managed copy-to-move optimization, interpreter, native C and ownership balance |
| `python tools/test_mir_migration.py` | Source feature parity against legacy emission |
| `python tools/test_mir_interpreter.py` | MIR interpreter behavior |
| `tools/test_mir_*.py` | Focused MIR behavior and ownership tests (run scripts individually) |
| `python tools/test_reference_c.py` | 24 independent handwritten C references plus runtime-panic checks; not a compiler test |
| `python tools/test_build_cache.py` | Content-addressed build-cache invalidation |
| `python tools/test_tool_utils.py` | Process result and newline test helpers |
| `python tools/test_python_bootstrap.py --command 'python archive/python_bootstrap/sirius.py'` | **Archival** Python compiler tests only; not part of active bootstrap |

The fixture counts describe tested behavior, not a full language conformance
claim. Two legacy negative fixtures (`f64` arrays and optional fallback in
`elif`) are valid programs accepted by MIR; the MIR runner tests them as
positives instead of treating them as diagnostic failures. The fixture-generation scripts are **manual authoring tools**:
`tools/generate_core_fixtures.py` and
`tools/generate_extended_fixtures.py`; they overwrite checked-in fixtures.

## Repository structure

```text
src/                    Sirius compiler modules, entry points and MIR interpreter
bootstrap/compiler.c    Committed, reproducible C compiler bootstrap
runtime/                C11 runtime implementation and header
archive/python_bootstrap/  Historical, frozen Python implementation
experiments/target_ir.py  Illustrative Python IR dataclasses (not used in builds)
tests/compiler/         Compiler feature and diagnostic fixtures
tests/fixtures/         Earlier language examples
tests/reference_c/      Independent handwritten C examples
tests/mir/              Sirius MIR test drivers
tools/                  Build/generation commands, shared helpers and test runners
benchmarks/             Standalone C collection microbenchmarks
```

`src/cli.sirius` is the MIR-default compiler entry point, and
`src/legacy_cli.sirius` is the separately buildable legacy entry point.
`src/emit_c.sirius` is retained for legacy differential tests but is **not**
part of the default compiler or its self-hosted bootstrap.

## Implementation boundaries

The legacy backend covers a practical subset of scalars, recursion, records,
constructor functions, strings, rank-1 arrays, dictionaries and optionals.
Canonical type interning and typed HIR exist alongside legacy emitter-local
type and specialization handling.

The MIR path implements typed CFG instructions, an interpreter, a C emitter,
specialization lowering, explicit ownership operations, guard insertion, liveness,
last-use release insertion, exit cleanup, and post-cleanup owned-copy/call-argument move elimination. It runs all 85 shared legacy-positive executable fixtures plus the newer MIR-only cases, including `f64` arithmetic,
`recur`, record spread/delete, and nonrecursive generic return inference.
Nested checked-array and record assignment paths support both simple and
supported augmented assignments. The dynamic MIR interpreter executes floating-point operations, file I/O, and source-level programs through `sirius-run`. **MIR-only self-hosting reaches a native C
fixed point:** generations 2, 3 and 4 emit byte-identical C with no legacy
emitter in their sources. Arrays use CoW for consuming updates and amortized
in-place append for unique owners; string join allocates once. The MIR C emitter
shares managed-value cleanup across return paths to reduce generated C size.
The native and interpreted paths pass their differential execution and runtime-failure cases, as well as all 91 shared positive output fixtures. The serialized MIR runner also passes 91 source-versus-loaded comparisons. Exact decimal-to-`f64` rounding for every literal and complete abnormal-exit unwinding remain open.

Modules/imports, multidimensional arrays, broadcasting, full effect/purity
checking, universal runtime type descriptors and concurrent map are future work.
Negative indexing remains deferred.

See [ROADMAP.md](ROADMAP.md) for priorities, [IR_DESIGN.md](IR_DESIGN.md)
for IR architecture, [INTERPRETER.md](INTERPRETER.md) for interpreter details
and [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) for intended semantics.

**Runtime storage:** the MIR C emitter now shares a single type-directed CoW
clone path for generated records and arrays. MIR dictionaries now reuse the
public generic `S_Dict` table, while the retained legacy layout stays separate
(see `RUNTIME.md`). To reproduce dictionary crossover/update measurements,
run `python tools/benchmark_dictionaries.py` (`--runs N` for repetition).
These C microbenchmarks are not representative of full compiler throughput.

**Compiler storage profiling:** `python tools/profile_ir_storage.py` counts
MIR-backed record/array allocation requests during self-compilation. The MIR
liveness worklist shares its initially empty register mask between blocks,
reducing allocation calls and measured compile time without changing the IR
format. See [MIR_PROFILE.md](MIR_PROFILE.md) for comparative measurements.
