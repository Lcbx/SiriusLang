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
canonical structural signatures. A first path-based source-module workflow is available via `sirius-run --build-modules` (also exposed by `tools/build_modules.py`):
static namespaced function/constant imports, exported record constructors and qualified type names, separate compilation
and existing MIR linking. The parser recognizes `import`, `export def`, `export rec` and `export let`; the frontend resolves
qualified function calls and constants using typed HIR declarations. A Sirius-native scanner now discovers the source dependency graph and
extracts concrete export signatures. Sirius orchestrates per-module compilation
and in-memory MIR linking; Python provides an optional launcher. Source-module builds now specialize exported generic functions on demand for concrete imported argument types (with explicit return annotations). These concrete specializations are linked by canonical structural signatures. Source-independent generic declaration packages (`.smirg`) can be specialized explicitly into concrete `.smirl` libraries, or discovered and specialized automatically by the native module builder when an imported source is absent. Compile-time execution and native linking are also future work. Typed `extern def` declarations are still supported.

## Portable generic packages (experimental)

An exported generic function can be stored as a versioned, validated
syntax/typed-HIR snapshot and specialized **without the provider source**.
Explicit `extern def` dependencies survive as named MIR imports, while
ordinary Sirius imports are supported by the module builder when their
source or `.smirg` dependencies remain available.

```sh
./sirius-run --emit-generic Ops ops.smirg Ops.sirius
./sirius-run --specialize-generic ops.smirg twice twice.smirl i64
./sirius-run --list-lib twice.smirl
```

For a package containing `import Ops alias O`, supply the source root at
packaging time so the dependency's typed public interface can be pinned:

```sh
./sirius-run --emit-generic Adapter Adapter.smirg Adapter.sirius .
```

Package imports are resolved transitively during `--build-modules`; both
`Adapter.sirius` and its imported provider sources may be removed if the
corresponding `.smirg` packages are present. Interface changes (including
exported constant values) are rejected. Standalone `--specialize-generic`
remains limited to dependency-free packages.

Accepted type arguments include canonical scalar and structural type signatures
(e.g. `Array[i64]`, `Dict[string,i64]`, `Record{1:x=i64;}`). Use
`--link-lib` to link the resulting concrete `.smirl` with a caller. The
`.smirg` format is **not** executable MIR and has no stable cross-version ABI.
For automatic specialization, place `Ops.smirg` under the source root at the
same path as `Ops.sirius`, remove the provider source, and build an importing
module normally:

```sirius
import Ops alias O

def main() -> i64
	return O.twice(21)
```

```sh
./sirius-run --build-modules . App app.smirl
./sirius-run --entry M_3_App_4_main --load-lib app.smirl
```

The builder prefers `.sirius` when both exist. Package fallback may rescan
the source dependency graph for each newly discovered package; caching that
work remains an optimization opportunity. It validates package identity,
recovers the exported typed interface, and specializes requested signatures in
memory before linking. Version 1 packages remain supported; version 2
packages contain pinned interfaces for direct imports, recursively checked
against their available transitive dependencies. Dependencies are referenced,
not bundled into one file. The standalone source scanner
(`sirius-module-scan`) still only scans `.sirius` files. Test with
`python tools/test_mir_generic_packages.py`, `python tools/test_module_packages.py`
and `python tools/test_module_package_dependencies.py`.

## Current development priorities

The runtime and MIR migration milestones are stable; feature development has
resumed. Bitwise `i64` operators (`&`, `|`, `<<`, `>>`) and packed 63-register-word
MIR ownership states are implemented. A controlled dense-vs-packed ownership
self-compilation comparison found a 46% lower native execution time, but
end-to-end cold-build gains are not established. The implementation order remains:

1. **Runtime:** improve `S_Array`, `S_Dict`, strings, allocation and CoW behavior,
   preserving snapshots and the existing C ABI. Start with measured bottlenecks.
2. **Simplification:** reduce duplicated runtime/compiler logic and stale build
   paths. Prefer incremental refactors with focused regression tests.
3. **MIR as the sole backend — completed:** normal compilation and the
   committed C bootstrap use MIR. The retired AST-to-C emitter and its
   legacy-only tooling have been removed.
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

`./sirius input.sirius output.c` uses **HIR → MIR → C**. The retired
AST-to-C backend has no fallback path; direct MIR-to-C emission is tested
separately.

Record destructuring is available through the MIR compiler:

```sirius
def pick(record)
	let {first = .a, second = .b} = record
	return first
```

The source expression is evaluated once. Missing fields, duplicate bindings and
non-record sources are compile-time errors; only local `let` patterns are
supported.

Nested assignments also work through the MIR backend:

```sirius
var state = {items=[{value=1}]}
let snapshot = state
state.items[!0].value = 2
```

The compiler evaluates checked indices once, then reconstructs modified values
from the leaf to the mutable root using CoW. Record-field and checked-index
compound assignments also work on nested paths for supported numeric and
string operators.

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

# Combine independently generated concrete libraries:
./sirius-run --link-lib combined.smirl first.smirl second.smirl
./sirius-run --list-lib combined.smirl
./sirius-run --entry first --load-lib combined.smirl
python tools/test_mir_link.py
python tools/test_mir_import.py
```

A library contains the concrete specializations reachable from its exported
roots. Exported functions may have explicitly annotated parameters, while CLI
execution requires a zero-argument function returning `i64`. Export signatures
use canonical structural types independent of local `TypeId` values.

### Filesystem traversal (MIR backends)

`list_dir(path) -> Optional[Array[string]]` enumerates immediate entry **names**
(sorted, excluding `.` and `..`); absence means the directory cannot be opened
or read, distinct from an empty array. `is_dir(path) -> bool` checks directories
without following directory symlinks, avoiding cycles during a recursive walk.
Paths use the host filesystem's conventions, with `/` usable as a separator on
supported platforms. The Windows implementation converts UTF-8 paths to wide
Win32 paths; POSIX treats path strings as bytes.

Reusable `walk_files(root) -> Array[string]` lives in
`stdlib/Std/Filesystem.sirius` (use `Std.Filesystem` within a source root).
It performs a breadth-first walk and skips unreadable subdirectories; it does
not stream results or include directories in the returned list.
`examples/filesystem_walk.sirius` shows standalone use.

```bash
python tools/test_filesystem.py
# Optionally check source-level native/interpreted parity:
python tools/test_filesystem.py --runner ./sirius-run
```

The file APIs are supported by the MIR emitter and interpreter; the retained
retired AST-to-C backend is no longer present.

### Path-based source modules (initial subset)

File `Math/Stats.sirius` under a source root:

```sirius
export def increment(x: i64) -> i64
	return x + 1
```

File `App.sirius` in the same source root:

```sirius
import Math.Stats alias Stats

def main() -> i64
	print(Stats.increment(41))
	return 0
```

```sh
python tools/build_mir_runner.py
python tools/build_module_scanner.py
python tools/build_modules.py --root examples --entry App --output app.smirl --run
# Native Sirius build (Python is not the build orchestrator):
./sirius-run --build-modules examples App app.smirl
python tools/test_module_scanner.py
python tools/test_source_modules.py
python tools/test_compiler_source_modules.py
```

`src/module_scan.sirius` checks source dependency cycles,
and retains the AST, HIR declarations, and canonical typed export signatures.
`src/module_interface.sirius` re-interns imported types and compile-time values.
The scanner and `src/module_build.sirius` share HIR interface injection, including
export signatures that reference imported public record types. The builder
compiles the original parsed modules,
and links the MIR libraries in memory via `mir_link_libraries`. It does not
serialize interfaces or reparse synthetic `extern def` source. The standalone
scanner CLI still emits `SIRIUS-MODULES/1` for inspection, and the runner still
accepts `--module-interface path` for explicitly supplied source declarations.
`tools/build_modules.py` is an optional launcher; namespace resolution happens
in `src/module_resolve.sirius`. Without `--run`, it prints the
encoded `main` entry name; the resulting `.smirl` can also be loaded directly.

To run the discovery step alone, use `./sirius-module-scan source-root App`;
The separate scanner can be built with `tools/build_module_scanner.py`. Explicit imports map directly to paths; a directory walk is not
needed for dependency resolution.

Current restrictions: concrete exported functions require annotated
parameters and return types; generic exports may leave parameters unannotated
when their return types are explicit. `export rec Point(x: i64, y: i64)` makes both
`Alias.Point(...)` construction and the `Alias.Point` type annotation available
to importers, including inside `Array[Alias.Point]`. Public constructors must
have annotated parameters and direct `pub` fields (implicit parameter publication
is supported); computed `pub` expressions cannot yet be exported. Construction
is inlined in the importing MIR, without a library function symbol. All record
types remain structural; their field descriptors are re-interned in each consumer.
`export let`
shares compile-time values (`i64`, `bool`, strings, records and arrays) through
qualified names such as `Settings.CONFIG.size`; the consuming HIR receives a
copy of their typed value arena, without a runtime getter. Constant-only modules
are supported. Exported functions and direct-publication constructors may use
imported public record types (including nested `Array`, `Optional` and `Dict`
annotations); their resolved structural descriptors are passed to consumers.
Cross-module type aliases, operator overloads, re-exports,
and runtime module objects are not yet supported.
`import`, `export def`, `export rec` and `export let` are parsed by the Sirius frontend. Imports require the module
builder (or a typed interface supplied through `sirius-run --module-interface`);
standalone native compilation cannot resolve module dependencies yet.

Cross-library calls use *declarations without bodies* in the consuming source:

```sirius
extern def plus_two(x: i64) -> i64

def main() -> i64
	return plus_two(40)
```

The provider can define `def plus_two(x: i64) -> i64` in a separate source.
Export the provider's `plus_two` and the consumer's `main` with `--emit-lib`,
then combine the artifacts using `--link-lib`. `--list-lib` displays imports
and exports. The linker matches names and structural function signatures,
remaps types, and redirects calls; unresolved or incompatible imports are
errors. Libraries with unresolved imports cannot execute directly. The
library format is `SIRIUS-MIR-LIB/2`; the format is experimental and not
backward-compatible with `/1`.

This is concrete function linking. The source-module tool above builds on
it; full compiler-integrated modules and cross-library generic specialization
are still pending, as are native registration and mixed native/interpreted dispatch.

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

The build, fixed-point and regression commands have different responsibilities.
`python tools/test_all.py` runs all active regression scripts. Use
`--list` to inspect the selected scripts. Full runs include expensive native builds and self-hosting checks.

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
| `python tools/test_all.py` | Run all active test scripts, reporting every failure |
| `python tools/probe_mir_selfhost.py --require-ready` | Quick source-generation probe of the default MIR backend; **not** a fixed-point test |
| `python tools/test_mir_selfhost.py` | Build the standalone MIR compiler, compile successive native generations and compare their C byte-for-byte; run MIR runtime smoke cases (prefers Clang for the large generated C) |

Normal changes to the Sirius compiler:

```sh
python tools/test_compiler.py --jobs 4
python tools/generate_bootstrap.py
python tools/test_bootstrap.py --jobs 4
```

Commit Sirius source and generated `bootstrap/compiler.c` together. **Never edit
generated C by hand.** The *previous* committed compiler must be able to compile
the updated Sirius source; introduce new syntax in stages.

The bootstrap compiler sources are still concatenated in dependency order using
`tools/source_manifest.py`. The first compiler utility source, `src/module.sirius`,
now exports the symbol mangler and module-name-to-filepath conversion as typed
Sirius functions. `tools/test_compiler_source_modules.py` compiles that *same*
source as an independent module and invokes it via `import module alias Names`.
This is a migration compatibility check, **not** a modular self-hosted compiler:
HIR/MIR still use flat cross-file references. Concrete record signatures, public constructors/type names, and constant values
now cross module boundaries. Exported function and constructor signatures may
also reference imported public record types. Generic specialization, named
re-exports, and mutual source dependencies still prevent a general
compiler-module migration.
`--rebuild` clears content-addressed artifacts; test runners supporting parallel
fixtures accept `--jobs N` (default up to 4). Cached objects and binaries are stored under `.build/cache/`, while fixture
executables and successive compiler generations use temporary directories.

## Tests

Run individual suites as needed:

| Command | Scope |
| --- | --- |
| `python tools/test_bootstrap_tools.py` | Bootstrap generation, stale-C rejection and CLI dispatch |
| `python tools/test_runtime_collections.py` | Runtime generic arrays/dictionaries, aliasing, CoW and managed values |
| `python tools/test_compiler.py` | Default MIR backend: 96 native executions and 50 expected diagnostics |
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
| `python tools/test_mir_direct_c.py` | Direct MIR-to-C printing, panic escaping, and uninitialized-register checks |
| `python tools/test_mir_selfhost.py` | Native MIR stage-2/3/4 fixed point and targeted runtime regressions |
| `python tools/test_mir_opt.py` | Managed copy-to-move optimization, interpreter, native C and ownership balance |
| `python tools/test_mir_interpreter.py` | MIR interpreter behavior |
| `tools/test_mir_*.py` | Focused MIR behavior and ownership tests (run scripts individually) |
| `python tools/test_reference_c.py` | 24 independent handwritten C references plus runtime-panic checks; not a compiler test |
| `python tools/test_build_cache.py` | Content-addressed build-cache invalidation |
| `python tools/test_tool_utils.py` | Process result and newline test helpers |

The fixture counts describe tested behavior, not a full language conformance
claim. The old emitter's diagnostic fixtures (`f64` arrays and optional
fallback in `elif`) are accepted by MIR and tested as positive programs. The fixture-generation scripts are **manual authoring tools**:
`tools/generate_core_fixtures.py` and
`tools/generate_extended_fixtures.py`; they overwrite checked-in fixtures.

## Repository structure

```text
src/                    Sirius compiler modules, entry points and MIR interpreter
bootstrap/compiler.c    Committed, reproducible C compiler bootstrap
runtime/                C11 runtime implementation and header
tests/compiler/         Compiler feature and diagnostic fixtures
tests/fixtures/         Earlier language examples
tests/reference_c/      Independent handwritten C examples
tests/mir/              Sirius MIR test drivers
tools/                  Build/generation commands, shared helpers and test runners
benchmarks/             Standalone C collection microbenchmarks
```

`src/cli.sirius` is the compiler entry point; `src/mir_emit_c.sirius` is
the sole C emitter. `tools/test_mir_direct_c.py` checks direct emission
independently of source lowering.

## Implementation boundaries

The MIR path implements typed CFG instructions, an interpreter, a C emitter,
specialization lowering, explicit ownership operations, guard insertion, liveness,
last-use release insertion, exit cleanup, and post-cleanup owned-copy/call-argument move elimination. It runs all 96 current compiler-positive fixtures, including `f64` arithmetic,
`recur`, record spread/delete, and nonrecursive generic return inference.
Nested checked-array and record assignment paths support both simple and
supported augmented assignments. The dynamic MIR interpreter executes floating-point operations, file I/O, and source-level programs through `sirius-run`. **MIR-only self-hosting reaches a native C
fixed point:** generations 2, 3 and 4 emit byte-identical C with no legacy
emitter in their sources. Arrays use CoW for consuming updates and amortized
in-place append for unique owners; string join allocates once. The MIR C emitter
shares managed-value cleanup across return paths to reduce generated C size.
The native and interpreted paths pass their differential execution and runtime-failure cases, as well as all 91 shared positive output fixtures. The serialized MIR runner also passes 91 source-versus-loaded comparisons. Exact decimal-to-`f64` rounding for every literal and complete abnormal-exit unwinding remain open.

Generic cross-module specialization, native/FFI linking, multidimensional
arrays, broadcasting, full effect/purity checking, universal runtime type
descriptors and concurrent map are future work.
Negative indexing remains deferred.

See [ROADMAP.md](ROADMAP.md) for priorities, [IR_DESIGN.md](IR_DESIGN.md)
for IR architecture, [INTERPRETER.md](INTERPRETER.md) for interpreter details
and [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) for intended semantics.

**Runtime storage:** the MIR C emitter now shares a single type-directed CoW
clone path for generated records and arrays. MIR dictionaries now reuse the
public generic `S_Dict` table, while older specialized C layouts remain separate
(see `RUNTIME.md`). To reproduce dictionary crossover/update measurements,
run `python tools/benchmark_dictionaries.py` (`--runs N` for repetition).
These C microbenchmarks are not representative of full compiler throughput.

**Compiler storage profiling:** `python tools/profile_ir_storage.py` counts
MIR-backed record/array allocation requests during self-compilation. The MIR
liveness worklist shares its initially empty register mask between blocks,
reducing allocation calls and measured compile time without changing the IR
format. See [MIR_PROFILE.md](MIR_PROFILE.md) for comparative measurements.
