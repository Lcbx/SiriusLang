# Sirius roadmap

This tracks **implementation work**, not language semantics. See
[LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) for the target language,
[README.md](README.md) for usage and current support, and
[MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md) for the longer-term module/ABI
proposal. A checked box indicates tested implementation, not a public stability
or completeness guarantee.

## Priorities

1. **Runtime and storage:** continue profiling parser, ownership dataflow and
   MIR-generated collections. Improve allocations and CoW reuse where measured
   bottlenecks justify complexity; preserve value semantics and the C ABI.
2. **Simplification:** remove redundant analyses, stale interfaces and duplicate
   paths without destabilizing the committed bootstrap. Prefer small changes
   with both interpreter and native tests.
3. **Language features:** add missing semantic constructs in vertical slices
   (HIR → MIR → native/interpreter → tests), while preserving self-hosting.
4. **Modules/libraries:** expand persistent parameterized HIR and exercise
   final source/value semantics in regressions; follow with mixed native/interpreted calls. Compile-time MIR execution is postponed.

**Completed foundation:** MIR is the default C backend; the compiler produces a
byte-identical native self-hosting fixed point. The legacy Sirius C emitter is
retained for comparisons. The dynamic MIR runner, concrete MIR executables,
multi-entry libraries, cross-library merging and signature-checked concrete
imports are implemented. There is no Python compiler in the active workflow.

## Active work and acceptance criteria

- [ ] Strengthen ownership and runtime-failure handling: verify path-dependent
  ownership credits and define cleanup on implicit panics, allocation failure,
  and interpreter budget termination. Preserve explicit guard/exit cleanup.
- [ ] Expand interpreted/native parity for managed nested values, failures,
  numeric boundaries and CLI/file behavior. Specify overflow/conversion and
  IEEE-754 edge behavior before claiming complete equivalence.
- [x] Cache concrete HIR bodies by declaration and canonical argument TypeIds,
  reusing inferred typed bodies when the specialization worklist processes a
  callee; memoize unannotated return inference within a body analysis with
  recursion-sensitive keys. The MIR verifier still checks CFG and ownership.
  Baseline self-compilation timing is essentially unchanged; keep profiling
  before attempting broader HIR/MIR storage changes.
- [x] Have MIR consume resolved HIR symbol identities for local reads,
  assignment roots, nested-update roots and record spreads. Constructor
  shorthand `pub` reads its own parameters, not caller locals. Specialization
  worklists carry call IDs solely in `HIR_TARGET`, without a duplicate call
  array; keep compatibility fallbacks for standalone MIR tests. Verify shadowed
  scopes, CoW and a byte-identical self-hosted bootstrap.
- [ ] Address remaining HIR/legacy-type coupling and source span diagnostics.
  Use canonical `TypeId`s for new code; avoid expanding legacy emitter-local
  type arithmetic. Retain the legacy emitter only for differential checks.
- [ ] Investigate high-cost parser arena CoW and dense CFG register-state
  copies with `tools/profile_ir_storage.py`; consider packed states or a
  mutable builder only if a measured end-to-end improvement survives tests.
- [x] Support typed source-level `extern def` declarations with automatic
  concrete MIR import metadata, keeping `--import` for compatibility.
- [ ] Introduce separate-source, path-derived module resolution and namespace
  imports. Retain deterministic structural record layouts and specialization
  identity.
- [x] Introduce source-backed generic exports (`SIRIUS-MIR-LIB/3`) and
  link-time specialization from concrete imported structural signatures,
  with reuse, validation and both interpreter/native output paths.
- [x] Resolve external calls introduced by generic specialization, including
  transitive concrete/template dependencies and structural types across libraries.
- [x] Replace retained provider text with version-4 persistent syntax arenas
  (token spellings, AST nodes/edges), avoiding source reparsing at link time;
  continue to load version-3 source-backed libraries.
- [x] Persist resolved module declarations, constants and canonical type tables
  in version-5 generic artifacts, validating them against syntax at load and
  reusing a provider context for all link-time specializations.
- [x] Persist a first parameterized HIR subset in version-6 libraries: symbolic
  argument types for expression returns and immutable `let` bindings (names,
  literals, scalar unary/binary expressions), validated during loading and
  instantiated with concrete operator/local-type checks. Unsupported bodies
  fall back to concrete HIR analysis.
- [x] Persist simple immutable `let` bindings followed by a return in parameterized
  HIR; concretely validate inferred local types, operators and name resolution.
  Keep ordinary body analysis for unsupported constructs.
- [x] Support parameterized record/array literals, record field selection and
  value-returning builtins (`len`, `str`, `sqrt`) with concrete type interning,
  operator checks and source-level native/interpreted tests.
- [x] Handle named user-function calls and a single early-return `if` arm
  followed by a final return in persistent parameterized HIR. Reuse semantic
  signature validation and specialization requests, check branch/return types
  after concrete substitution, and accept historical v6 empty fallback plans.
- [ ] Generalize persisted parameterized HIR to multi-arm control flow,
  mutation, collection operations, semantic imports and constructors.
- [x] Allow a resolved concrete library to emit native C (`--emit-c-lib`),
  enabling native/interpreter differential execution of the same linked MIR.
- [ ] Specify native registration, ownership and error ABIs for actual mixed
  native/interpreted function-call dispatch; emitting whole-program C is not
  sufficient.
- [ ] (Postponed) Compile-time MIR execution and its staging/syntax. Do not
  require purity inference or mandatory effect restrictions initially.
- [ ] Extend arrays to multidimensional shapes, broadcasting and numerical
  kernels. Transitive purity checks are required before unordered/parallel
  functional transformations, not for initial compile-time interpretation.
- [ ] Add sanitizer and Windows CI, broader error-path tests, and checks for
  compatibility across successive compiler generations.

## Recent implemented slices

- [x] Canonical typed HIR worklist and specialization IDs shared with MIR;
  CFG lowering, verifier, guards, lifetime insertion, edge/exit cleanup,
  balance checks and last-use move optimizations.
- [x] Native/interpreted `i64`, `bool`, `f64`, strings, records, rank-1 arrays,
  dictionaries, optionals and managed nested values; CoW record/index updates,
  `recur`, record destructuring and augmented nested assignments.
- [x] Compiler-side typed constant evaluator for `i64`, `bool`, strings,
  homogeneous arrays and records, including earlier constants, spreads,
  deletes, field/index access and selected pure operations. Arbitrary
  function calls, user-defined `comptime` and runtime imports are deferred.
- [x] Reuse generic runtime `S_Dict` in MIR-generated dictionaries, with
  structural hashes for supported keys and CoW on shared updates.
- [x] Optimize unique-owner record updates, direct record-array append and
  multi-argument last-use ownership transfers into calls.
- [x] Versioned concrete MIR executable round-trip, named multi-root
  library loading/validation, merging of independently serialized libraries,
  and name/signature resolution of concrete imports (`SIRIUS-MIR-EXEC/1`,
  `SIRIUS-MIR-LIB/1` and `SIRIUS-MIR-LIB/2`).
- [x] Keep default and legacy compiler entry points separate; commit the
  MIR-generated C bootstrap and verify a native fixed point.

## Bootstrap and testing policy

The checked-in C bootstrap is the seed for all ongoing compiler development.
Python scripts are **tooling**, not a replacement compiler.

1. Edit Sirius source and focused positive, negative and (where applicable)
   ownership/aliasing tests. Compiler source must remain compilable by the
   *previous* seed; introduce new syntax in stages.
2. Run `python tools/test_compiler.py`, the relevant `tools/test_mir_*.py`
   suites, and the separate legacy comparison suite if shared semantics change.
3. Run `python tools/generate_bootstrap.py`; it generates a fixed point but
   **does not** run language fixtures.
4. Run `python tools/test_bootstrap.py` and
   `python tools/generate_bootstrap.py --check`, then commit Sirius source,
   tests and generated C together. Never hand-edit `bootstrap/compiler.c`.

Use `tools/source_manifest.py` for source inclusion, not hardcoded copies of
ordered module names in scripts. The C runtime must also remain buildable by
itself. `tools/test_reference_c.py` protects the older public C API.

## Design constraints and deferred decisions

- Preserve structural, lexicographically ordered record fields and observable
  immutable-value/CoW behavior, regardless of uniqueness optimizations.
- Keep the compiled and interpreted MIR paths semantically aligned; direct
  MIR emission retains defensive checks, while compiler-verified MIR may omit
  redundant runtime register checks.
- Compiler self-hosting validates a subset, **not** all intended semantics or
  ownership safety. Tests are evidence for a particular set of programs.
- Numeric overflow/division/conversion, portable type identity/ABI, foreign
  ownership and compile-time staging still need precise contracts.
- Negative indexing, multidimensional numerical operations, `Any`, purity
  analysis and parallel map are deferred language work.

Detailed implementation mechanics belong in [IR_DESIGN.md](IR_DESIGN.md),
[INTERPRETER.md](INTERPRETER.md), and [RUNTIME.md](RUNTIME.md). Historical
performance measurements are in [MIR_PROFILE.md](MIR_PROFILE.md); update this
roadmap at milestones, not after every micro-optimization.
