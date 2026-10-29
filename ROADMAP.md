# Sirius implementation roadmap

This is the **active task list**, not a history of completed work. The
[language design](LANGUAGE_DESIGN.md) defines intended semantics, and the
[examples](examples/README.md) list exercised features and restrictions.
For the detailed milestones, benchmarks, and previous implementation decisions,
see the [October 29 snapshot](docs/history/ROADMAP_20261029.md) and
[October 25 snapshot](docs/history/ROADMAP_20261025.md). Historical claims may
be superseded by the current baseline below.

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

1. **Make existing features compose consistently.** Extend contextual typing
   and constructor-label preservation where concrete programs expose gaps.
   Keep conversion and label decisions in HIR for MIR to consume; preserve
   native/interpreted and source-free package parity.
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

## Union semantics: implemented core and remaining work

`rec` remains structural. `let Result = Success | Failure` now interns
alternatives as canonical `(constructor label, record shape)` pairs; two
alternatives may have identical payload types. `enum Name` is source shorthand
for empty-record constructors and their labeled union, **not** a new nominal
type category. A separate `variant` declaration has been dropped from the
design.

The existing union runtime, MIR tag instructions, verified explicit alternative
indices, interpreter, C emitter, and serialization are reused. Source `match`
resolves tags by label, while plain record-to-union injection stays implicit
only for an unambiguous payload shape. A direct constructor in an expected-union
`let` or `return` can select its label even when record shapes coincide.
Qualified constructors and match patterns also preserve labels after module
name mangling, including re-exports. Contextually typed arrays and function
arguments (including imported, indirect, nested, and record-constructor calls)
can contain distinct directly constructed alternatives. `Array.append` and
`Dict.insert` also accept directly constructed same-shape alternatives,
including enum constructors and qualified imports; the native and MIR paths
preserve their selected tags. Stored records remain label-free.

Explicit `Union.Label(record)` now constructs the selected alternative
from any structurally compatible record payload; an existing union cannot be
relabeled through this form. Qualified exported union aliases also resolve.
Union storage is type-directed; see the current representation below. Large
alternatives do not force every array element to reserve their payload size.

**Verified source-free path:** payload-free enum declarations, constructor
selection, matching, and `Union.Label(record)` in exported generic bodies now
survive `.smirg` deserialization, specialization, linked `.smirl`, native C
emission, MIR interpretation and ARC auditing. The generic AST reader accepts
`NODE_ENUM`, and copied bodies bind defining-module union aliases rather than
same-named caller declarations. `tools/test_module_enum_packages.py` covers
these paths, deterministic builds and corrupted package rejection. Source-free
heterogeneous unions are also covered: `tools/test_module_heterogeneous_union_packages.py`
exercises different-size and managed payloads, same-shaped labels, explicit
construction, arrays/dictionaries, CoW, native/MIR execution, ARC auditing,
and invalid relabeling/structure cases. The path passed without compiler
changes; this closes the source-free package validation gap, but does not
change the allocation-heavy representation of payload-bearing unions.

**Contextual Optional defaults (implemented):** `optional or fallback` now
checks the fallback against the optional's payload type. Direct constructors
retain their labels, including same-shaped alternatives; array literals and
callbacks also receive the expected type. Structural conversion stays in the
absent-value CFG branch. Native/MIR, ARC/CoW, lazy trapping defaults, qualified
constructors, source-free generic imports and invalid ambiguous defaults are
covered by `tools/test_union_optional_dict_coercions.py`.

**Next:** extend contextual constructor selection to remaining nested
expressions where real examples justify it. Named pattern binders and
refutable bindings remain possible ergonomic improvements. No new nominal
`variant` or special payload type is planned.

**HIR/MIR boundary (initial separation):** HIR injection annotations now carry
the target type and selected alternative, including unambiguous stored-record
shapes as well as direct constructor labels. MIR consumes that index instead
of recovering labels from call syntax and function declarations.
HIR also owns recursive structural conversion plans for records, arrays,
dictionaries and Optionals. Plans record identity steps, child conversions
and nested union alternatives; MIR emits their records, loops and branches.
The HIR-only assertions in `tests/types/union_source_driver_native.sirius`
check direct-label and stored-record selections before lowering;
`tests/types/union_driver_native.sirius` checks nested plans and rejection.
Plans are retained in specialized HIR bodies and shared by source/target type
pair within each body. MIR rejects a missing checked plan rather than invoking
HIR analysis. Constructor publications and inferred early returns retain their
plans too. HIR plans are not serialized; source-free generics rebuild them
during specialization, while serialized MIR already contains lowered operations.
Replacing remaining syntax-dependent lowering and giving conversions explicit
HIR expression identities remain follow-up work.

HIR annotation operation IDs are now distinct across the HIR modules. Record
literals and their conversion targets previously shared an ID, causing valid
record-literal arguments and collection elements to lose their conversions.
Regression coverage includes direct/indirect calls, arrays, Optional defaults,
explicit union payloads, managed CoW snapshots and source-free generic defaults.

## Current storage and regression coverage

Payload-free enums use unowned `i64` tags. In the C backend, union alternatives
with at most three unmanaged scalar fields (`i64`, `f64`, `bool`) inline their
payload in wrappers and array slots by default. Larger and managed payloads
retain boxed storage. Union registers retain their pointer ABI. See
`src/mir_emit_c.sirius` and `tools/test_mir_inline_unions.py` for eligibility
rules and native/MIR, CoW, hashing and ARC coverage.

Boxed wrapper/payload coallocation remains experimental and disabled by
default. `SIRIUS_EXPERIMENTAL_UNION_COALLOC` enables the optional path;
`SIRIUS_EXPERIMENTAL_UNION_HYBRID` remains a tested configuration. Neither
flag is needed for default small-scalar inlining. Allocation selection and
managed/large fallback checks are in `tools/test_union_hybrid_coallocation.py`.
Earlier C models and construction benchmarks are preserved in the
[historical snapshot](docs/history/ROADMAP_20261029.md); their conclusions do
not describe all current storage paths.

## Next work

- **Feature composition:** find a concrete nested expression that loses an
  expected type or constructor label, then add positive and negative tests
  across native/MIR and source-free generics before changing lowering.
- **Independent applications:** build medium-sized programs combining modules,
  collections, explicit callback contexts and failure handling. Use observed
  library and diagnostic gaps to choose further work.
- **Differential coverage:** expand combinations of contextual defaults,
  callbacks, labeled unions and collections. Include managed payloads, CoW
  snapshots and ARC auditing where supported.
- **Numerical workloads:** extend the checked rank-2 `Std.Numeric` library from
  concrete use cases. Compare changes with the existing library and flat-array
  baseline using `tools/benchmark_stdlib_numeric.py`. Establish transitive
  effect checking before parallel transformations.
- **Allocation lifetime:** identify real pass-local allocations and prove that
  references cannot escape a reset boundary before adopting regions. The POD
  scratch prototype and allocation counts are historical evidence, not a
  production allocator or proof that ARC-managed values can be reset.

Completed callback inference, module hygiene, negative indexing, numerical
scalar fixes and matrix measurements are recorded in the
[historical snapshot](docs/history/ROADMAP_20261029.md). The examples guide
and dedicated regression tests describe their current supported behavior.

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
- **Numerical semantics:** `i64` addition, subtraction, multiplication and
  negation now wrap at 64 bits; division/remainder truncate toward zero and
  trap on zero or the minimum-value / `-1` case. Continue with explicit
  conversions and floating-point edge behavior as separately decided work.
  Explicit scalar `f64(i64)` / `i64(f64)` conversions now lower to typed
  MIR opcodes; the narrowing operation has a checked CFG panic edge.
  MIR decimal parsing retains the established short-literal path and normalizes unusually long literals so compensating
  exponents do not lose finite values to intermediate overflow/underflow.
  Correctly rounded native/interpreter literal parity remains unproven.
  Keep native and interpreted execution in agreement.
- **Equality/hashing, initial slice implemented:** source `==`/`!=` compare
  records, optionals, and labeled unions recursively as boolean values.
  Direct array `==` remains reserved for elementwise comparison; direct dict
  and function equality remain unsupported. Hashable dictionary keys include
  recursive records, optionals, and unions, but exclude arrays and dictionaries.
  NaN dictionary key equivalence is canonicalized without changing IEEE `==`.
  A future operator/collection-equality proposal requires separate review.
- **Union identity:** records remain structural and alternatives carry labels,
  including same-shaped payloads. A new nominal variant category is not planned.
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
