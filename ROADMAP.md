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
- `Std.Functional` provides sequential callbacks; purity is not enforced.
- Tagged unions, exhaustive `match`, filesystem `Result` wrappers, CoW
  snapshots, and compiler-side ownership analysis have working subsets.

The legacy AST-to-C emitter, Python compiler, and flat source manifest are
retired. MIR is the sole C backend.

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
   benchmark wins on tiny microcases.
4. **Expand the language and standard library gradually.** Generalized array
   transformations and numerical operations, runtime descriptors, effect/purity
   checking, and a richer module/library interface remain larger pieces of
   planned work.

## Deferred architecture

- **MIR compile-time execution:** reuse the existing MIR interpreter to
  evaluate at compile time; define eligibility and diagnostics separately.
- **Native registration and FFI:** type representation, specialization across
  native/interpreted boundaries, and host ABI adapters require an explicit
  contract. The current `.smirg` format is not that ABI.
- **Numerical execution:** multidimensional shapes, broadcasting, slicing,
  SIMD and eventually parallel pure transforms need further design and tests.
- **Concurrency:** no capturing closures, global-mutability model, or
  concurrent-map implementation is currently promised.

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
