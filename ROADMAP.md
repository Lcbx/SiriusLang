# Sirius implementation roadmap

This is the **active task list**, not a history of completed work. The
[language design](LANGUAGE_DESIGN.md) defines intended semantics, and the
[examples](examples/README.md) list exercised features and restrictions.
For the detailed milestones, benchmarks, and previous implementation decisions,
see the [October 25, 2026 roadmap snapshot](docs/history/ROADMAP_20261025.md).

## Current baseline (October 2026)

- Self-hosted Sirius compiler with native module imports, typed HIR, verified
  CFG MIR, MIR-to-C11 emission, and a checked-in C bootstrap.
- Dynamic MIR interpretation, serialized executables (`.smir`), linked concrete
  libraries (`.smirl`), and source-free generic declaration packages (`.smirg`).
- Generic function specializations and first-class `Fn` values, including
  contextual specialization from expected function signatures. Local functions,
  qualified imports, named re-exports, and packaged generics are covered by
  targeted regression tests.
- Typed named-function overloads use exact argument matching and deterministic
  export symbol mangling; imported and re-exported overloads are supported.
- Exported generic results are inferred per concrete specialization, including
  arrays, structural records, named re-exports and source-free `.smirg`
  packages. Recursive inferred results still require annotations.
- `Std.Functional.map`, `map_indexed`, `filter`, `generate`, `fold`, `zip` and `zip_with` have generic
  implementations. `empty_array_like` infers empty-array element types from
  an array or a callback. Concrete generic callee arguments can supply
  omitted callback parameter types, including independently typable later
  arguments, nested generic results, and multi-stage callback-result chains. `enumerate` returns structural indexed
  records for arbitrary element types while keeping legacy record names.
  Purity is not enforced.
- Tagged unions, exhaustive `match`, filesystem `Result` wrappers, CoW
  snapshots, and compiler-side ownership analysis have working subsets.

The legacy AST-to-C emitter, Python compiler, and flat source manifest are
retired. MIR is the sole C backend.

Serialized generic packages now use only `SIRIUS-MIR-GENERIC/3`, including
packages without dependencies. Historical `/1` and `/2` readers and
compatibility fixtures have been removed; old packages must be rebuilt.
Concrete MIR libraries, executable MIR, bundles and internal snapshots retain
their current independent formats. No cross-version artifact compatibility is
promised while the language is experimental.

## Priorities

1. **Complete language features in tested slices.** Improve generic and
   higher-order type inference, especially generic array-producing callbacks
   and callbacks across module boundaries. Extend error diagnostics and tagged
   union ergonomics where needed. Keep MIR/native/interpreter parity.
2. **Maintain correctness and cross-platform portability.** Preserve the
   self-hosting bootstrap fixed point, CoW semantics, error-path behavior, ARC
   balance, Windows GCC support, and deterministic serialization. Add focused
   positive, negative, and aliasing tests to each change.
3. **Simplify compiler and runtime where justified.** Remove duplication
   when a shared interface is clearer. Profile allocations and compiler stages
   before changing storage or ownership passes; don't optimize solely for
   benchmark wins on tiny microcases. Investigate allocation lifetime and
   region-based scratch storage for nonescaping temporary data, without a
   universal implicit context or changes to visible value semantics.
4. **Expand the language and standard library gradually.** Generalized array
   transformations and numerical operations, runtime descriptors, effect/purity
   checking, and a richer module/library interface remain larger pieces of
   planned work.

## Next implementation slice

**Higher-order constraint diagnostics and inference:** generic callbacks can now
flow through local aliases, copied nested record fields, and mutable
assignments. Invalidating aliases on reassignment prevents stale constraints;
lexical slots prevent shadowed names from creating false constraints. Source
imports and source-free `.smirg` packages retain these generic definitions.
Conditional alias joins now retain only identities preserved by continuing
`if`/`elif`/`else` paths; exhaustive `match` arms use the same rule, with
pattern-bound fields available to local inference. Return and guard exits do
not contaminate the continuing path; loop joins allow zero iterations.
The compiler distinguishes conflicting callback signatures, forwarding
cycles without type seeds, and underdetermined callbacks, naming the relevant
function and recommending explicit annotations. Conflicts now identify the
first incompatible callback parameter types or arities and the defining-body
call-site lines; forwarding preserves that evidence. Cycles report the
bounded, human-readable forwarding chain, including for `.smirg` imports.
Imported generic callback failures now name the defining module as well as
the relevant callback body lines (including source-free packages); a complete
source-file-aware explanation of all constraints remains future work.
Direct calls through function-valued record fields use
existing MIR indirect calls, including nested field projections. Defining-module `match` constructors in imported generic
bodies now resolve correctly, including from source-free packages. Truly underdetermined callbacks still require an explicit `Fn` annotation.

`enumerate` now uses `map_indexed(values, callback)` to build indexed records
in one pass without array-element rewrites. No compiler or runtime changes
were required. See the measured comparison below.

## Resolved module regression (October 26, 2026)

Imported generic bodies and computed constructors now preserve the defining
module's imported namespaces under hygienic aliases. Their qualified calls,
function values, type annotations, and match patterns resolve against the
original dependency, not identically named imports in the consumer. Unqualified
constructor calls are likewise attributed to the defining module. Existing
module bundle, incremental discovery, package dependency, re-export, union,
source module and contextual callback regressions now pass in the focused
native runner tests. `test_module_capture_imports.py` also checks collisions,
source-free `.smirg` dependencies, deterministic linking, and missing aliases.

Generic calls now infer unannotated callback fields in record literal arguments,
including nested literal paths, when the defining generic body invokes that
specific field with known argument types. Each field is inferred independently;
conflicting and unrelated fields cannot silently supply a type. Imported generic
bodies and source-free `.smirg` packages use the same rules. This does not
infer callback fields through arbitrary record transformations or through
values with no usable type seed.

The independent two-generation bootstrap check and the record-literal
callback interpreter/source-free package regression now pass. Sequential
`zip_with` exercises binary callback inference with heterogeneous array inputs,
while `zip` builds generic structural pairs on top of it; both truncate to the
shorter array. The two-array helpers require no new compiler inference rules.

The previously proposed case `map(values, fn(x) box.apply(x))` is
not a callback-inference gap: it attempts a lexical capture, which Sirius
intentionally excludes. The equivalent `map(values, box.apply)` works with
function-valued fields. Do not pursue lexical capture support as a roadmap
task; use explicit callback context where needed.

`Std.Functional.partition` additionally splits arrays in one traversal, calling
its predicate once per element and preserving both groups' order and CoW
semantics. It is a library-only change, not a step toward lexical captures.

`Std.Functional.enumerate` now delegates to one-pass `map_indexed`: in a
repeated 2,048-element workload (3,000 runs), Clang `-O2` improved from
248 ms to 120 ms median, and GCC `-O2` from 190 ms to 95 ms (six timed
executions after warm-up, same executable workload and array runtime).
These are workload-specific measurements, not proof of a general 2x benefit.
The new helper is tested through native C, MIR interpretation, source-free
`.smirg` packaging, linked C and ARC auditing. No native array type is needed.

`Std.Functional.map_with_context(values, context, callback)` passes state
explicitly to noncapturing callbacks, including state with managed values and
function-valued fields. Tests cover native, MIR, ARC and source-free packages.
The MIR-only test in `tools/test_mir_closure_environment.py` independently
exercises an owned record holding a function reference and explicit
state across C, interpreter and serialization. These are supported value
composition techniques, not work toward capturing closures.

## Negative array indexing (October 27, 2026)

Checked and optional array indexes now accept negative values, counting back
from the end (`-1` is last, `-len(a)` is first). Out-of-range optional reads
return absence; out-of-range checked reads and writes panic. The interpreter
and C emitter use the same bounds semantics for MIR array operations, including
negative `i64` minimum without overflow. Source tests cover CoW snapshots,
nested writes, managed elements, and empty arrays. Plain unguarded `a[i]`
remains subject to compile-time bounds proof.

## Numerical-array groundwork (October 27, 2026)

`Std.Numeric` now exposes an initial **rank-2 `f64` matrix wrapper** backed by
flat, row-major `Array[f64]` storage. Checked `make`, `reshape`, `transpose`,
`matmul`, and singleton-axis broadcast `add` return tagged shape results. Storage sharing follows existing CoW
semantics; transpose and multiplication create fresh arrays. Shape checks avoid
signed product overflow, and multiplication and addition check result size.
Focused native/MIR, emitted-library and ARC-audited tests cover valid/invalid
shapes, zero dimensions and independent array snapshots.

This is intentionally **library-first**, using `Array[f64]` and record-based
shape metadata. `Std.Numeric.add` validates broadcast compatibility and
zero-axis behavior. No native
`Array[T, 2]` representation, generic element types, vectorized kernels or
strided views are introduced.

Initial compiled Sirius benchmarks (`tools/benchmark_stdlib_numeric.py`)
compared the library against shape-specialized flat-array loops using the same
array runtime. Clang `-O2` before/after measurements (five-run medians, 128x128
addition at 512 repetitions and 64x64 multiplication at 64 repetitions) were:

| Operation | Original matrix | Hoisted input arrays | Flat baseline |
| --- | ---: | ---: | ---: |
| Elementwise add | 59.9 ms | 27.0 ms | 27.6 ms |
| Broadcast add | 57.9 ms | 30.7 ms | 28.9 ms |
| Matmul | 68.0 ms | 16.6 ms | 15.2 ms |

Repeated `matrix.data` projections inside loops induced per-element ownership
traffic in emitted C. Binding the input arrays locally once after shape checks
removed most of that cost without compiler or runtime changes. GCC `-O2` and
Clang `-O0` also improved. The CoW snapshot comparison remained close to the
flat baseline. Timings are local workload measurements, not general speedup
claims: the flat loops specialize shapes at source level, and the benchmark
includes allocation, checksums, and process startup, but not compilation time
(which is reported separately). No evidence yet justifies native ranked arrays,
new MIR kernels, or an allocation-lifetime redesign for these operations.

Next, extend the standard library only for concrete numerical use cases and
measure any future kernel change against both the library and an equivalent
flat-array baseline.

## Allocation-lifetime investigation (experimental; October 26, 2026)

Jai-style scratch arenas may benefit temporary numerical buffers and compiler
analysis passes. Sirius should keep semantic dependencies explicit; an implicit,
mutable context for every function is not a current goal. Allocation placement
should preserve CoW value semantics, ARC cleanup, and native/MIR parity.

The existing `tools/profile_mir_allocations.py` records 19,078,159 generated
MIR record allocations (including 2,725,320 CoW clones) during a local
self-hosting run. This count alone does **not** establish which allocations
are nonescaping or suitable for a region. The profiler now ignores LF/CRLF
differences when comparing generated C with the checked-in bootstrap.

A separate C11, **POD-only** scratch prototype in
`tests/runtime/scratch_arena.c` has been validated for alignment, overflow,
reset/reuse, and comparison with `malloc`/`free` on two synthetic phase-local
workloads. `tools/test_runtime_scratch_arena.py --benchmark` reports timings
and allocation counts; the results are not compiler performance measurements.
No production allocator, emitted C, language syntax, or ownership behavior
has changed.

Next: identify real pass-local allocations and prove that no pointers or
managed references escape a reset boundary; then compare memory peaks,
end-to-end timings, and ARC effects in both execution backends. A region for
arbitrary ARC-managed values must account for child release and CoW aliasing;
do **not** reset it as if it contained plain bytes. Require a real workload
improvement before exposing any allocator-selection API.

## Design commitments and decision gates

The [language design](LANGUAGE_DESIGN.md) is authoritative for semantics;
its long-term feature list is **not** the implementation queue. In particular:

- **Already chosen:** no capturing closures or mutable shared objects;
  local mutation preserves CoW value semantics. Errors use ordinary results or
  fatal process traps, not exception unwinding. Built-in arrays remain rank-1;
  multidimensional shapes are library values. Noncapturing function values and
  explicit callback contexts are sufficient for current higher-order APIs.
- **Modules:** private-by-default, qualified, acyclic imports, and
  path-derived names are the design. Module-level `let` is intended for
  compile-time evaluation, not runtime initialization; general compile-time
  expression execution and native registration remain separate milestones.
- **Before widening numerical semantics:** decide signed overflow, division,
  conversions, and floating-point edge behavior, then cover native and
  interpreted execution. Do not silently infer such guarantees from the C
  backend's implementation details.
- **Before broad equality/hashing or nominal variants:** decide structural
  equality for aggregate/numerical types and whether same-shaped constructors
  can ever be distinct union alternatives. Preserve today's structural union
  restriction in the meantime.
- **Before operator overloading across modules:** specify how imported
  operators enter scope and how ambiguity is reported. Existing exact-typed
  named-function overloads do not settle this.
- **Before parallel functional operations:** implement and verify transitive
  effect/purity checking, including indirect callbacks; specify deterministic
  output order and floating-point reduction behavior. Atomic RC, task
  scheduling, and concurrency primitives are exploratory, not promises.
- **Before native/interpreted mixed libraries:** specify exported type identity,
  registration, specialization, ownership, and host/target ABI adaptation.
  `.smirg` packages are not a stable native ABI.

Possible language conveniences such as `var` record destructuring,
`a[i, j]` indexing syntax, static rank parameters, and optional namespaces
remain proposals, not required dependencies of the current roadmap.

## Deferred architecture

- **MIR compile-time execution:** reuse the existing MIR interpreter to
  evaluate at compile time; define eligibility and diagnostics separately.
- **Native registration and FFI:** type representation, specialization across
  native/interpreted boundaries, and host ABI adapters require an explicit
  contract. The current `.smirg` format is not that ABI.
- **Numerical execution:** library-defined multidimensional arrays and
  broadcasting first; evaluate generic ranks, slicing and higher-level APIs.
  SIMD, fusion and parallel pure transforms may require compiler-recognized
  operations, subject to measured benefits. Native ranked descriptors are not
  an implementation priority.
- **Concurrency:** a global-mutability model, sharing rules, and a
  concurrent-map implementation are not currently promised.

See [modules/FFI architecture](docs/MIR_MODULES_AND_FFI.md) and the
[IR design](docs/IR_DESIGN.md) for detailed constraints. Historical unchecked
phase lists live only in the archived [roadmap snapshot](docs/history/ROADMAP_20261025.md).

## Work rules

- Implement compiler behavior in Sirius; Python is build/test orchestration.
- Keep each feature small enough for focused positive, negative, and
  interpreter/native parity tests. Check managed types for aliasing and ARC.
- Preserve source-order evaluation, value semantics and stable structural
  signature matching; don't add implicit backend fallbacks.
- Regenerate `bootstrap/compiler.c` from Sirius source, never hand-edit it.
  Keep the modular fixed-point test passing.
- Maintain the distinction between **implemented**, **experimental**, and
  **designed** behavior in documentation; update
  [usage](docs/USAGE.md) only when the interface exists.

See [Development](docs/DEVELOPMENT.md) for test and bootstrap commands.
