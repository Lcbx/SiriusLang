# Sirius implementation roadmap

This is an **engineering plan**, not a replacement for [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md). The design specifies intended semantics; this document tracks the narrower implementation, its debt, and the order in which the Sirius-written compiler can gain features **using the facilities available at each step**.

## Current priorities and decisions

The priority order is deliberate. Runtime and cleanup work are stable enough
to resume incremental feature implementation; avoid speculative micro-optimization.

1. **Runtime/data structures — in progress.** Preserve reference-counted value
   semantics and CoW snapshots, while reducing allocations and needless copies.
   Review array growth, dictionary layout, hash-table behavior, string storage,
   and runtime duplication; measure before replacing mature implementations.
   Generic arrays reuse aligned temporary storage and `realloc`;
   generic dictionaries share key/value allocations. Both dictionary layouts
   now clone at their existing capacity and avoid rehashing on overwrite.
   Focused CoW and aliasing tests cover these changes.
2. **Simplify the codebase — in progress.** Factor common mechanisms where they
   demonstrably reduce complexity. Keep public and bootstrap interfaces stable;
   remove unused code only with proof from manifests/callers/tests. The runtime
   dictionary probe and CoW clone paths have been simplified, and unused
   ownership intersection/union helpers were removed. HIR bodies now use their
   array positions as specialization IDs, and `MirProgramBuild` no longer
   retains HIR analyses after lowering. Avoid merging the legacy
   collection ABI with the separate MIR-emitted storage prematurely.
3. **Make MIR the default backend — completed.** `COMPILER_SOURCES` and
   `bootstrap/compiler.c` now use MIR with a lean `src/cli.sirius` entry point.
   Independent self-hosting and MIR regression checks now pass. The historical
   AST-to-C emitter and legacy-only build/test scripts have been removed.
   Direct MIR emission checks remain under `tools/test_mir_direct_c.py`.
   MIR C optional lookups now share a descriptor-directed payload helper, and
   `S_MirField` union-member selection is centralized in the emitter.
4. **Resume feature development — in progress.** Local `let` record
   destructuring is implemented. Module constants support integers, Booleans
   and nested structural records through a typed, flat compile-time value
   arena, now including strings, homogeneous nested arrays, record spreads
   and deletes. Compile-time `len`, field access, array indexing and earlier
   constant references are evaluated before MIR. Ordinary CoW applies at
   each use; general `comptime` and arbitrary pure calls remain deferred.
   Versioned executable MIR serialization is available through
   `sirius-run --emit-mir`/`--load-mir`. Concrete multi-entry libraries now
   support `--emit-lib`, `--list-lib` and `--load-lib`, with named exports,
   type signatures and validation. `--link-lib` merges independent concrete
   libraries by interning their structural types and relocating internal calls.
   Typed `extern def` declarations now create unresolved concrete imports;
   the linker resolves their names and structural signatures across libraries.
   Path-rooted source modules now retain import/export syntax in their source:
   native parsing/HIR namespace resolution now works with structured typed
   interfaces. The builder reuses scanner AST/HIR results, re-interns imported
   type descriptors and links concrete modules without interface serialization
   or synthetic source parsing; Python is an optional launcher. Generic cross-library specialization
   and native linking remain future work.
   Keep further slices isolated and protect MIR self-hosting and snapshots.
   As an initial compiler-source migration check, `src/module.sirius` is now a
   usable source module exporting its naming/path utilities. The regression
   `tools/test_compiler_source_modules.py` compiles the real source through typed
   imports and the MIR linker. The bootstrap still consumes the flat manifest;
   structural records, public constructors/type names and exported constants now
   cross module boundaries; concrete exported signatures now accept imported
   public record types. Generic specialization and named re-exports still
   block broad HIR/MIR migration.

**Validation status (2026-10-23):** `test_bootstrap.py --fixed-point-only`
reached generation 2 and verified regenerated `bootstrap/compiler.c` byte-for-byte.
`test_mir_selfhost.py` independently matched generations 2–4. Default compiler
regressions pass (96 executions, 50 diagnostic fixtures); packed/combined
ownership, HIR/types, parser, MIR linking, wire format and dynamic execution
checks pass. The direct MIR-to-C test covers former legacy differential test
cases for printing, panic escaping and uninitialized registers. Further
the exhaustive `test_all.py` run remains a separate validation task.

**Decisions:** MIR is the only compiler backend. Retain the self-hosted C
bootstrap and value/CoW semantics. Prefer correctness and simpler interfaces
over marginal compiler-speed gains. The packed-state benchmark below measures
one specific ownership-representation improvement, not end-to-end build time.

**Future architecture:** [MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md)
records the proposed module, MIR libraries, cross-library
specialization, and FFI direction. Concrete MIR executable serialization
is already implemented; the remaining parts are **not** an active-implementation
checklist; continue incremental language features before taking on its
longer-term library/ABI milestones.

**Runtime and IR reviews:** [RUNTIME.md](RUNTIME.md) and
[IR_DESIGN.md](IR_DESIGN.md#storage-review-post-mir-default). Test with
`python tools/test_runtime_collections.py` and `python tools/test_reference_c.py`.

## 1. Baseline and evidence

- **Active compiler:** Sirius source in `src/`, compiled through the checked-in MIR-generated C bootstrap in `bootstrap/compiler.c`. Normal source compilation goes through HIR and MIR before C11 generation.
- **Historical bootstrap:** the archived Python bootstrap was removed; Sirius-written compiler sources and committed C are the supported bootstrap chain.
- **Self-hosting:** `tools/test_bootstrap.py --fixed-point-only` independently compiles successive Sirius compiler generations and compares the stabilized output against committed `bootstrap/compiler.c`. `tools/test_bootstrap_tools.py` checks only the Python orchestration with mocks; it is not evidence of a real fixed point. The retired AST-to-C emitter is no longer in the source tree.
- **Coverage:** current default MIR suite: 96 native executions and 50 expected diagnostics (2026-10-23). The retired emitter's historical suite is no longer run. Also 19 tokenizer golden cases, 11 parser golden cases, and 24 independent C references. The historical Python tests cover 38 positive programs. Passing these is **not** proof of ownership safety or full language conformance.

### Rules for further work

1. Keep [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) as the semantic source of truth; document deliberate deviations. Do not treat the Python backend's incidental behavior as the language specification.
2. Implement new compiler features **only in Sirius**. Python is used for development/test orchestration, not as a compiler dependency.
3. Each feature starts with positive, negative and (for managed values) aliasing/early-exit tests. Test the Sirius compiler built from the C bootstrap, not the historical Python compiler.
4. Preserve the C11 bootstrap fixed point. Regenerate `bootstrap/compiler.c` from Sirius sources; never edit generated C manually.
5. Separate correctness and optimization. Conservative ARC comes before moves, borrowed values, FBIP reuse or SIMD.

## 2. Active bootstrap workflow

The checked-in C compiler is the **sole seed for ongoing development**. The Python implementation is not part of the loop:

1. Implement a compiler change in `src/` and add tests. Keep the compiler's own source within the *previous* seed compiler's accepted Sirius subset.
2. Run `python tools/test_compiler.py` and the relevant tokenizer/parser tests (these use the committed C compiler). The committed-C comparison in `test_bootstrap.py` may intentionally fail until the generated bootstrap has been refreshed.
3. Run `python tools/generate_bootstrap.py` to compile the updated compiler with the **current C bootstrap**, iterate until the generated C is self-reproducing, and replace `bootstrap/compiler.c` without running the fixture suite.
4. Run `python tools/test_bootstrap.py` for the fixed-point/committed-C comparison and full emitter fixture suite, then commit the Sirius source, tests, and regenerated C together. Review the C diff rather than editing generated C.

When the new compiler adds syntax or semantics that the previous seed cannot compile, implement the compiler support first using **old-subset syntax**, refresh the C bootstrap, and only then adopt the new feature inside the compiler source. Otherwise the bootstrap chain cannot advance. A source change that cannot be compiled by the seed is a bootstrapping blocker, not a reason to unfreeze Python.

The Python scripts under `tools/` provide build/test orchestration; the historical Python compiler has been removed.

### HIR/MIR performance sequence

- [x] Index typed HIR annotations by syntax-node ID per specialization (sparse lookup, preserving distinct operations on the same node).
- [x] Buffer MIR-C output through string fragments and join once per generated section/function/program.
- [x] Eliminate redundant register-initialization checks and state flags from ownership-verified MIR C; preserve defensive checks for directly supplied MIR. Avoid duplicate index/division/unwrap checks when explicit CFG guards exist.
- [x] Index canonical type descriptors by stable keys, preserving TypeIds. An analogous specialization signature cache was benchmarked and reverted because its key construction made generation slower.
- [x] Split lowering into read-only `MirLowerContext` and mutable `MirBuilder`, passed separately to expression and statement lowering.
- [x] Keep HIR specialization registry and typed bodies within the HIR→MIR build phase; expose only MIR program, entry and error. Specialization IDs are the body/function array indices; MIR lowering consumes the HIR call targets without copying them into a duplicate record type. Verify distinct and reused signatures in HIR tests.
- [x] Reduce CFG forward ownership CoW copies with cached sparse block write effects and in-place joins; avoid unchanged writes to CoW-owned forward/backward state arrays.
- [x] Fuse owned `MIR_COPY`/`MIR_RETAIN` followed by final `MIR_RELEASE` into `MIR_MOVE`; remove verified-only redundant C exit cleanup, preserving defensive direct emission.
- [x] Centralize verified MIR C managed-slot replacement into typed helpers; preserve evaluation-before-release and defensive direct MIR checks.
- [x] Seed the ownership worklist at the entry rather than all reachable blocks, visiting successors at least once; compute post-edge may-ownership without a redundant must-ownership pass for conditional exit cleanup. Profiled native MIR self-compilation improves from 22.05 s to 16.75 s at `-O0` (single runs).
- [x] Pack generated MIR fields (`S_MirField`) into a C union, selected by canonical type descriptors; guard parser token comparisons by source-span width. Single-run `-O0` profiling improves native self-compilation from 16.75 s to 11.83 s, primarily in parsing and ownership dataflow. Full MIR fixtures and self-hosting fixed point pass.
- [x] Replace full record reconstruction on direct field assignment with consuming `MIR_RECORD_SET`. C and interpreter use in-place updates for unique owners and copy-on-write for shared owners. Verified on 85 native fixtures, snapshot/alias tests and three self-hosting generations; generated C is ~3.17 MB versus ~3.45 MB previously. Runtime speed improvement is not yet established.
- [x] Fuse `r.array = r.array.append(x)` into `MIR_RECORD_ARRAY_APPEND` to eliminate temporary CoW aliases. Native microbenchmark: 100,000 appends fall from 6.35 s to <0.01 s (`-O1`); self-compilation speed remains approximately unchanged.
- [x] Transfer multiple distinct, last-use managed arguments in one call without retain/release; preserve duplicate-argument and use-after-move safety in MIR verification, interpreter and C emission.
- [ ] Investigate constructor ownership transfers and CFG-aware register-slot reuse after profiling.
- [x] Represent forward must/may ownership with packed 63-bit `i64` words; cover register boundaries, CoW snapshots, and joins in `tools/test_mir_ownership_packed.py`.
- [x] Benchmark packed ownership against an otherwise identical dense-state implementation: three native `-O0` self-compilation runs per variant on the same toolchain give 6.33 s (dense) vs 3.43 s (packed) median, or 0.542× time. This measures execution only, not C build time; see `MIR_PROFILE.md`.
- [x] Evaluate constructor call arguments in the caller scope before introducing constructor parameter bindings; regression fixture `95_constructor_argument_scope.sirius` now passes.
- [x] Implement bitwise syntax (`&`, `|`, `<<`, `>>`) and packed ownership, regenerate the C seed and verify the real bootstrap fixed point with `test_bootstrap.py --fixed-point-only`.
- [ ] Consider compact MIR instruction layout only after the higher-priority improvements.

## 3. Technical and organizational debt

| Priority | Debt | Evidence / risk | Recommended remedy |
| --- | --- | --- | --- |
| **Done** | Legacy backend retirement | Independent MIR bootstrap and regression checks verified the replacement | Old emitter and legacy-only CLI/scripts removed; `test_mir_direct_c.py` retains direct-emission coverage |
| **P0** | Ad-hoc numeric type IDs | The new interned descriptors coexist with legacy `TYPE_*` ranges and arithmetic type IDs during migration | Intern canonical `TypeId` entries in a type table; use integer IDs as handles, not as type definitions |
| **P0** | Incomplete ownership coverage | Manual retain/drop and scope cleanup are interleaved with C generation; branch/loop exits and nested lvalues need stronger guarantees | Make ownership explicit on MIR instructions; test all terminators and paths, including errors |
| **P1** | Incomplete module integration | Structural signatures, constants and direct-publication constructors now cross module boundaries; the compiler still uses the ordered source manifest | Support generic exported signatures, cross-module specialization, and named re-exports; migrate a dependency-closed compiler component before HIR/MIR |
| **P1** | Seed-versus-current compiler compatibility | The committed C bootstrap must be able to parse and compile updated compiler sources | Introduce new language features using syntax supported by the seed compiler; regenerate C only after fixed-point tests |
| **P1** | Testing is broad but uneven | fixed-point and emitter suites were previously separate, risking divergent coverage; sanitizers and Windows are not continuous requirements | Run all supported fixtures through the fixed-point compiler; add sanitizer and platform CI where supported |
| **P1** | Limited source diagnostics | Emitter diagnostics are largely free-form strings; node locations are imperfect for some lowered constructs | Preserve spans through HIR/MIR, use stable error categories, report original source positions |
| **P2** | Runtime/C backend coupling | C-specific ownership helpers and runtime wrappers are exposed in emission logic | Backend-neutral MIR ownership operations, then a narrow C11 backend/runtime ABI |
| **P2** | Incomplete numeric and effects policy | Overflow/conversion details, transitive purity, and order-sensitive operations are not fully settled | Decide observable semantics before implementing optimizations or parallel transforms |
| **P2** | Documentation duplication | README historically mixed fixture inventory, decisions and implementation journals | README = usage/status; this document = roadmap/debt; IR_DESIGN = target IR; LANGUAGE_DESIGN = semantics |

Do **not** attempt to solve all of these in one refactor. Each phase below should leave the compiler self-hosting and regression tests runnable.

## Filesystem groundwork for compiler-owned modules (2026-10-16)

- [x] Add portable `list_dir` and `is_dir` filesystem intrinsics to C and MIR
  interpretation (sorted names, missing-vs-empty directory behavior, ownership).
- [x] Add a Sirius `walk_files` helper and native/MIR-focused tests.
- [x] Move dependency traversal and exported-signature extraction out
  of Python into a Sirius-native scanner (`src/module_scan_cli.sirius`). An explicit dotted
  import only needs path resolution, not a recursive directory scan; `walk_files`
  is for discovery tools and other reusable workflows.
- [x] Integrate module building/linking in `sirius-run --build-modules`; Python
  remains an optional thin launcher.

## Current module-linking milestone (2026-10-16)

- [x] Concrete MIR libraries, typed `extern def` imports, name/signature linking,
  and structural type remapping.
- [x] Path-rooted source-module **build tool** (`tools/build_modules.py`):
  dependency graph, cycles, explicit concrete exports, namespace-safe symbols,
  aliases, transitive links and source-module integration tests.
- [x] Parse `import` and `export def` natively; collect module metadata in HIR,
  and resolve qualified calls to typed externs without rewriting source bodies.
- [x] Retain each scanned module's parsed AST, HIR declarations, and typed export
  signatures. Re-intern imported collection types into each module's TypeTable
  and add structured external declarations directly to HIR; native builds no
  longer generate textual interfaces or reparse modules. Manifest serialization
  remains available only for the standalone module-scanner CLI.
- [x] Remove duplicated Python regex parsing/validation of source declarations;
  add scanner-level positive/negative tests and build orchestration regression.
- [x] Verify native module compilation, transitive links, reproducible outputs,
  nested collection signatures, and type/arity diagnostics with a built runner
  (`tools/test_source_modules.py`).
- [x] Re-intern canonical structural record descriptors in exported function
  signatures, including nested array/dictionary/optional shapes; reject
  incompatible record arguments (`tools/test_source_modules.py`).
- [x] Support `export let` for compile-time constants, including typed nested
  records and arrays, copying their value arenas into importing HIR modules.
  Keep constant-only modules out of the MIR function linker.
- [x] Import direct-publication `export rec` constructors and qualified record
  type names (`Module.Point`) in annotations and nested collection types;
  lower imported constructors directly to MIR record construction.
- [x] Resolve exported function and direct-publication constructor signatures
  referencing imported public record types, including nested collection types;
  share typed interface injection between scanner and builder.
- [ ] Support computed `pub` exports, generic declarations, named re-exports,
  and cross-library specialization.
- [ ] Mixed native/interpreted dispatch and native registration ABI.
- [ ] Compile-time MIR execution and purity inference (deferred by design).

## 4. Implementation order

### Phase A — Stabilize the bootstrap and test boundary

**Available now:** Python test runners, C11, Sirius scalar/record/array/dictionary support, byte-oriented strings, the existing indexed AST and self-hosted emitter.

- [x] Centralize the ordered `src/` source manifest in `tools/source_manifest.py`, retaining byte-exact historical concatenation for the bootstrap fixed point.
- [x] Run tokenizer, parser, emitter and self-hosting tests using the committed C bootstrap; keep Python-stage-0 tests explicitly archival.
- [x] Share expected output, exit codes and diagnostic substrings for the stage-1 and fixed-point compiler suites.
- [ ] Compare output/diagnostics between successive compiler generations for new language semantics.
- [x] Run the complete 85-positive/37-negative emitter suite against the fixed-point compiler using `tools/compiler_cases.py`.
- [ ] Add sanitizer builds (ASan/UBSan where supported) and Windows GCC checks. The C-bootstrap fixed-point/currentness check is already implemented in `tools/test_bootstrap.py`.
- [ ] Extend ownership regression coverage to managed `recur`, nested short-circuit branches, and conditional failures. Three new focused cases cover managed `return`, `break`, and `continue` (55–57); these also passed UBSan.

**Done when:** the committed C bootstrap alone (plus a native C compiler) is sufficient to build the active compiler and run its full supported-subset tests on development platforms. No Sirius language additions needed.

### Phase B — Separate semantic resolution into typed HIR

**Available then:** `Array[Record]`, `Dict[string,i64]`, generic calls, module integer constants, owned strings, and indexed AST arenas. No imports, runtime reflection, or recursive managed types are required.

- [x] Begin a typed HIR arena with AST source-node IDs, canonical `TypeId`s and resolved field/binding indices (`src/hir.sirius`). This covers basic expressions only; the separate full-program HIR pass is still pending.
- [x] Introduce a canonical `TypeTable` for primitive, record, `Array`, `Dict`, and `Optional` descriptors; intern by structure with sorted record fields. Register record and bound-value types through the compatibility bridge.
- [ ] Complete the migration from legacy arithmetic type IDs to canonical `TypeId`s. **Done:** generic specialization cache keys, binding metadata and record-field name lookup now use canonical IDs. **Done in part:** C record-field layout and managed helper selection now derive from canonical descriptor tags and fields. **Remaining:** C naming still uses legacy kinds, as do static type checks and some expression-level ownership operations. Add function/union descriptors when implemented.
- [ ] Move all lexical scope/name resolution, field lookup and type checking out of the C emitter into HIR construction. **Started:** module declarations and parameter syntax/uniqueness are checked in a pre-emission HIR indexing pass; name lookup, record fields, primitive literals, unary/binary operations and assignment compatibility use C-independent HIR resolvers; call signatures, collection methods, and record/constructor field composition are checked in HIR. A pre-emission traversal now visits evaluator bodies, tracks lexical scopes, resolves scalar types, known record/array/dictionary construction, field and indexing operations, optionals and selected calls, and checks definite binding/assignment errors. Known-argument generic evaluator calls and record constructors now resolve result types pre-emission, including forward and nested calls; unresolvable or recursive cases defer to the existing emitter. Monomorphization caching, full control-flow semantics, ownership, and consumption of HIR by the backend still need conversion. HIR nodes preserve syntax AST IDs for source spans.
- [x] Add an HIR-owned specialization registry keyed by function declaration identity and canonical argument `TypeId`s, tracking in-progress and completed entries. C emission now delegates cache identity/lifecycle to HIR but still initiates specialization and generates bodies. **Remaining:** schedule/type-check specialized HIR functions before backend emission, and use the shared registry for MIR function identities.
- [x] Add backend-independent concrete-specialization analysis in `src/hir_specialize.sirius`: a worklist of canonical signatures, typed HIR bodies, stable call targets, and in-progress recursion tracking. The worklist is tested without MIR/C. **Remaining:** unannotated callee result inference, constructors, effect signatures and redirecting production C emission to the analyzed specializations.
- [ ] Lower constructors into typed record-producing operations; make extension calls and built-in operations explicitly resolved.
- [ ] Temporarily generate C from typed HIR through a compatibility layer; do not require a finished MIR to land this phase.

**Done when:** existing emitter tests and bootstrap fixed point pass with semantic checks independent of C text generation. All semantic resolution uses interned type IDs, rather than reserved numeric ranges or arithmetically encoded nested type combinations. Current HIR is an incremental bridge, not yet a complete semantic pass.

### Phase C — Introduce CFG MIR and deterministic evaluation

- [x] Implement a first executable scalar CFG MIR, structural/type verifier, and unoptimized Sirius-written interpreter, independently tested with hand-built MIR (`src/mir.sirius`, `src/mir_interpreter.sirius`, `tools/test_mir.py`). Now also connected to the HIR specialization worklist and experimental C backend for scalar/string operations.
- [ ] After shared source-to-MIR lowering and C emission exist, expose shared behavioral tests with `--backend=c|both` (`INTERPRETER.md`).

**Available then:** typed HIR, indexed records/arrays, scalar indices, and normal imperative loops. Represent MIR as flat arrays of blocks and instructions; no generalized pattern matching is required; the interpreter now has a small acyclic record heap.

- [ ] Give each MIR instruction typed operands and a destination; represent `Branch`, `Jump`, `Switch`, `Return` and `Panic` as terminators.
- [ ] Evaluate expressions into explicit temporaries in source order, including calls, record fields, nested updates, and constructor publications.
- [ ] Lower `if`/`elif`, `and`/`or`, `while`, `for`, optional `or` and `else return` into branches before C generation. **Started:** `src/mir_lower.sirius` lowers checked scalar functions with `if`/`elif`/`else`, `and`/`or`, `while` and linked specialized calls; optional `or`, `else return`, checked and optional indexing, and dictionary `.get()` now lower into explicit CFG and typed MIR. `src/mir_program.sirius` associates each HIR specialization with a same-index MIR function. The default `src/mir_emit_c.sirius` emits native C from MIR; direct MIR-C validation lives in `tools/test_mir_direct_c.py`. MIR lowers `for`, `recur`, record spreads/deletes, `f64` expressions, array append and guards.
- [ ] Lower `recur` to parallel assignment into temporaries followed by a backedge, even for managed values once ownership is implemented.
- [x] Make MIR the only compiler C backend. Move direct C emission checks into `tools/test_mir_direct_c.py` and retain managed-value/ownership tests.

**Done when:** complex optional/boolean combinations and branch-local evaluation work without special-case rejection; no semantic checks depend on generated C expressions.

### Phase D — Centralize ARC and CoW correctness

**Available then:** typed MIR places, temporaries and explicit control-flow exits; existing type-specific C copy/drop callbacks and CoW runtime.

- [ ] Define ownership for every argument, result, local, field projection and call (borrow, owned value or transferred value); conservatively retain when in doubt.
- [ ] Insert `Retain`, `Release`, `Move`, `Borrow` and `CoWSet`-like MIR operations; ensure nested record/array paths are safe under aliasing.
- [ ] Add cleanup blocks for all normal/early control-flow exits, including returns, `break`, `continue` and `recur` backedges. **Partially done:** `src/mir_cleanup.sirius` inserts path-specific dead-value releases on CFG edges and explicit return/panic cleanup, including unused arguments and path-dependent initialization. `mir_check_balance` now proves no managed register remains possibly owned at a reachable explicit exit; conditional releases are represented explicitly by `MIR_RELEASE_IF_INIT`. `break`, `continue` and `recur` now lower into MIR; the latter snapshots new arguments before its entry-block backedge. Division/remainder, checked array reads/writes, and optional unwrap now have explicit guarded panic CFG edges before lifetime/cleanup insertion. Other implicit aborts (including allocation failures) remain non-unwinding.
- [ ] Test managed records, nested arrays, dictionary values, optionals and unions under repeated aliasing, branch exits and sanitizers.
- [ ] **Only afterward** add ownership/uniqueness optimization and in-place storage reuse; keep these invisible at language level.

**Done when:** all currently supported programs have a defined ownership path and tests cover every CFG terminator; no cleanup is manually interspersed with C emission.

### Phase E — Complete bootstrap-language parity and modules

**Available then:** typed HIR/MIR, safe owned values and the existing standard-library building blocks.

- [x] Implement local `let` record destructuring with explicit name/selector pairs, single source evaluation and typed field extraction. MIR only; `var` and recursive patterns remain deferred.
- [ ] Generalize remaining optional constructs and nested CoW lvalues. Optional indexing, `else return` and nested CoW updates are implemented. **Started:** plain `a[i]` is accepted for literal arrays with compile-time in-range integer indices (including nested literal chains). Unknown lengths/indices remain unproven and require `[?]` or `[!]`; extend range/length analysis later.
- [ ] Complete constructors (conditional overrides after unconditional publication), constructor-based tagged unions, exhaustive `match`, function values and noncapturing lambdas. Basic `rec` constructors with automatically published parameters and direct `pub` overrides are implemented.
- [ ] Implement basic static `namespace`/`import`/`export` resolution; start with separately parsed sources and a build-time dependency graph, without runtime modules.
- [x] Expand module constant evaluation from `i64` and `bool` expressions to nested **structural record constants**, including references to earlier constants and field access. Store values in a typed arena and instantiate records with ordinary MIR instructions, preserving CoW and sorted field layout. Test overrides, shadowing, invalid initializers and snapshots (`tests/mir/module_record_constants.sirius`).
- [x] Extend constant evaluation to decoded strings/concatenation, homogeneous arrays (including nested), constant array indexing, named record spreads/deletes, and pure `len` of constant strings/arrays. MIR materializes values with existing instructions. See `tests/mir/module_composite_constants.sirius`.
- [ ] Generalize compile-time expression evaluation to other pure intrinsics/functions, record constructor calls, and ultimately MIR-interpreted `comptime`; keep execution policy permissive initially.
- [ ] Define stable diagnostic IDs and explicitly reject unsupported semantics rather than silently translating them.
- [ ] Add language-semantic tests for each new construct and verify the C bootstrap can rebuild the compiler without invoking the frozen Python implementation.

**Done when:** the Sirius-written compiler supports the intended initial language subset and builds from separate source files without concatenation.

### Phase F — Numerical language and future extensions

**Available then:** a tested typed IR, ownership semantics, module system, and an established C backend.

- [ ] Canonical type descriptors (kind, layout, ownership operations) before dynamic `Any`; ordinary static values need no runtime type tag.
- [ ] Rank-`N` dense arrays, shape/broadcast validation, scalar and elementwise operations, reductions and linear algebra primitives.
- [ ] `map`, `filter`, `generate` with **transitive purity/effect checking** before fusion, SIMD, or parallel execution; callback order is intentionally unspecified.
- [ ] Improve FBIP/ARC and compiler optimizations after semantics and numerical determinism are pinned down.
- [ ] Add FFI contracts, concurrency/atomic RC and optional LLVM/backend work only when motivated by performance or actual applications.
- [ ] **Medium priority:** Python-style negative indexing as specified by `tests/deferred/25_negative_indexing.sirius`.

These are **language-completeness goals**, not prerequisites for preserving the existing self-hosting milestone.

### MIR string-ownership slice (experimental, implemented)

- [x] Add managed `string` registers and explicit MIR retain/copy, move and release operations in Sirius, with runtime invalidation checks.
- [x] Make both the Sirius MIR interpreter and MIR→C backend execute the same string operations; C uses ARC credits across calls and returns.
- [x] Lower string literals, concatenation and `len(string)` from typed HIR; extend differential and ownership-specific tests.
- [x] Generalize managed register ownership to immutable structural records, including nested strings/records. `MIR_RECORD_NEW`/`MIR_RECORD_GET` consume canonical layouts; interpreter and generated C handle copy/move/retain/release and recursive destruction. See `tools/test_mir_records.py` and compiler record fixtures.
- [x] Add rank-1 MIR arrays with canonical element types, checked access, length and consuming `MIR_ARRAY_SET` with type-directed CoW. Interpreter and MIR-C handle arrays of scalars, strings, records and nested arrays, with alias-preserving differential tests (`tools/test_mir_arrays.py`, `tools/test_mir_run.py`).
- [x] Add a narrow MIR dictionary subset: canonical `Dict[i64|string, T]`, empty construction, consuming CoW insertion, length and eager checked-type `GET_OR`, with both interpreter and native C tests. Source lowering supports construction, `insert`, and `len`; source optional `get` is now supported.
- [x] Add `Optional[T]` storage, optional dictionary lookup/fallback and array indexing, unwrap, and CFG-lazy `or`/`else return` in MIR, with native differential and interpreter ownership tests.
- [x] Support nested record and checked-array assignment paths, including supported indexed augmented assignments, with existing CoW operations. Cover shared snapshots, branch/loop exits, managed `recur`, and invalid nested index failures; continue ownership-credit verification for all CFG exits.
- [x] Replace repeated full-graph ownership/liveness sweeps with queued CFG worklists and precomputed predecessors; store liveness in dense 0/1 register masks. Add a branching/backedge regression (`tools/test_mir_dataflow.py`). Packed ownership states were implemented later; benchmark against this dense baseline. Edge cleanup now uses a single may-ownership/liveness snapshot and conditional releases instead of re-solving the full CFG per edge, substantially reducing MIR-only compiler generation time.
- [x] Eliminate repeated MIR ownership/structure checks between guard, lifetime and exit-cleanup transforms in the combined compiler pipeline, retaining public pass validation and final balance/failure audits. Compare MIR-only generation output byte-for-byte against the previous compiler.
- [x] Propagate must-initialization and may-ownership in one forward worklist for managed CFG balance checks, retaining must-only and may-only entry points. Test joins, backedges and unreachable blocks (`tools/test_mir_ownership_combined.py`).
- [x] Share CFG reachability/predecessors and the must-initialization result during final ownership-balance verification; skip may-ownership dataflow for scalar-only functions. Keep standalone ownership checking and invalid-read diagnostics unchanged.
- [x] Move CFG reachability into the MIR layer and reuse immutable CFG facts across cleanup may-ownership and backward liveness; recompute after CFG edge rewriting. Preserve standalone analysis entry points and the native fixed point.
- [x] Add a conservative backward CFG liveness pass, integrated into HIR→MIR construction. It inserts last-use `MIR_RELEASE` for managed registers across branches and loops, with idempotency and interpreted/native tests (`tools/test_mir_lifetime.py`).
- [x] Lower checked division/remainder, array access/CoW updates, and optional unwrap into explicit guarded panic CFG edges before lifetime insertion; exercise both backends and ownership cleanup (`tools/test_mir_guard.py`).
- [x] Add forward definite-initialization analysis across CFG joins/backedges (`src/mir_ownership.sirius`), rejecting uninitialized reads, use-after-move and double release. Validate both original and lifetime-instrumented MIR, with negative tests (`tools/test_mir_ownership.py`).
- [x] Centralize MIR fallibility metadata (`mir_failure_info`) and audit compiler-produced MIR for missing explicit traps (`mir_audit_failures`); enumerate still-non-unwinding operations (`tools/test_mir_failure_contract.py`).
- [ ] Complete ownership verification: remaining runtime failures and path-specific ownership-credit proofs. Explicit division/bounds/unwrap exits and conditional releases are implemented; retain defensive return cleanup until this analysis is complete.
- [ ] Expand interpreter/native differential testing to every managed construct and migrate the default C backend only after coverage is sufficient.

## 5. Consistency with the language design

**Aligned:** statically compiled C target; tab-indented syntax; imperative loops and functional-first API direction; structural records with deterministic, lexicographically sorted fields; immutable value semantics with CoW; simple noncapturing functions; generics via specialization; byte-oriented tokenizer; explicit `Optional`; constructor-based union distinction by structural result type; single-threaded ARC bootstrap; generated C retained for independent builds.

**Intentional incomplete implementations:**

- **Self-hosting does not mean full semantics.** The current compiler is a bootstrap subset; modules, type descriptors, array broadcasting, vector operations, effect analysis, first-class function coverage, dynamic `Any` and parallel transforms remain outstanding.
- **Effects/purity:** no general transitive effect checker yet. Do not expose an unrestricted parallelizable `map` that silently accepts side effects.
- **Type identity:** semantic interning has begun in `src/types.sirius`, but legacy numeric kinds still drive code generation. Canonical runtime layout/introspection descriptors remain future work.
- **Ownership:** manual emitter-level ARC is an implementation bridge, not a durable proof of value semantics; phases C/D address this.
- **Backend:** C11 via `cc`/`gcc` is the current implementation. The design mentions Zig's C toolchain, which remains optional; portability should be measured rather than assumed.

**Semantics still needing decisions/tests:** numeric overflow, integer division and explicit conversions; descriptor identity/layout stability across independently compiled modules; file I/O error representation (`read_text`/`write_text` are provisional); static bounds proofs; exact permitted FFI effects; how structurally identical constructor results should behave as union alternatives (current choice: they are not distinguishable). These should be resolved before optimizing or exposing them as stable user-facing contracts.

## 6. Documentation responsibilities

- [README.md](README.md): how to build/test, what works, where to look next.
- [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md): the intended public language, independent of backend shortcuts.
- [IR_DESIGN.md](IR_DESIGN.md): target HIR/MIR representations and lowering examples; refer to phases B–D here for the implementation sequence.
- [INTERPRETER.md](INTERPRETER.md): interpreter status, validation policy, shared backend testing plan.
- The old `experiments/target_ir.py` prototype has been removed and is not a compiler dependency.

Update this roadmap at **phase milestones**, not with an entry for every small patch; the commit history and test fixtures provide the incremental development record.

## Dynamic MIR execution (active)

- [x] Add independent `sirius-run` source CLI and builder. Reuse the normal
  parse → typed HIR → MIR lowering/ownership/optimization path; execute the
  resulting linked MIR in Sirius without generating C for input programs.
- [x] Make CLI `arguments()` independent of compiler-host arguments, including
  through recursive calls. Implement `MIR_READ_TEXT` with a real optional
  presence test (missing vs empty files). Fail explicitly on unknown opcodes
  instead of returning bogus values.
- [x] Add source-level native-vs-dynamic test harness: 18 independently expected
  cases, compile-diagnostic parity, and all 91 positive shared fixtures.
- [x] Cover all current `f64` MIR operations with a real-valued register field,
  nested containers, `sqrt`, and decimal/scientific literal tests. Exact
  decimal rounding across all IEEE-754 edge cases still needs a shared parser.
- [ ] Normalize runtime errors and cleanup/ownership on abnormal termination.
  Four runtime failures (including nested managed arrays) now match exit code,
  exact stdout, and stderr via shared `eprint`/`MIR_EPRINT` support. The runner
  accepts `--max-steps` and `--max-depth` independently of program arguments.
  Unwinding on implicit allocation failures and budget aborts remains pending.
- [x] Serialize and load versioned concrete MIR executables, including canonical
  type descriptors, validation and round-trip execution tests (91 fixtures).
- [x] Add named concrete exports, typed `extern def` imports, versioned library
  serialization and structural signature-checked linking, with negative tests.
- [ ] Add generic library specialization and native/interpreted boundary
  dispatch. Compile-time MIR evaluation and purity inference are deferred.

## MIR-only compilation migration

- [x] Extract `CompileResult` from the legacy emitter.
- [x] Share MIR entry validation and managed TypeId classification in `mir_common.sirius`.
- [x] Add an explicit self-hosting coverage probe for the MIR backend, using a MIR-only source manifest and entry-point driver.
- [x] Support the concrete return annotations and specializations needed for MIR-only native self-hosting, including canonical `rec` annotations, constructor `pub` fields, module constants and nonrecursive unannotated generic return inference. Recursive inference without annotations remains intentionally rejected.
- [x] Lower all current compiler-source constructs and the 85 shared executable fixtures to MIR, including `recur`, record spreads/deletes, inferred nonrecursive generic returns and basic `f64` operations. Nested record and checked-array projection updates are supported; floating-point operations and file I/O now execute through both MIR backends.
- [x] Achieve a native MIR-only fixed point through generations 2, 3 and 4, excluding the legacy emitter; verify byte-identical C with `tools/test_mir_selfhost.py`. Reduce C size using a shared return cleanup epilogue, and improve generated compiler performance using amortized array append and single-allocation string join. Correct compound-assignment lowering for locals and record fields.
- [x] Run the shared executable fixture suite through the standalone MIR backend (91/91). Check all 46 MIR-invalid diagnostic fixtures; test the other two legacy-negative fixtures as MIR-positive (`tools/test_mir_compiler.py`).
- [x] Switch the default build manifest/CLI and committed C bootstrap to MIR; verify the new bootstrap fixed point and the default compiler regression suite.
- [x] Retire the legacy C emitter, build CLI and legacy-only differential tests after real bootstrap fixed-point, MIR regression and direct-emitter checks passed.

### Latest runtime simplification (MIR-generated containers)

- [x] Consolidate C-emitted CoW cloning of records, optionals, arrays and
  dictionaries into one type-directed helper; maintain managed-field retention.
- [x] Guard emitted record allocation against overflow.
- [x] Validate managed nested snapshots and native MIR self-hosting.
- [x] Reuse the generic `S_Dict` open-addressed hash table for MIR-generated
  dictionaries, with typed retain/drop callbacks, CoW snapshots, and structural
  hashes/equality for recursively hashable record keys. Arrays/dicts/optionals
  are not hashable keys.
- [x] Add repeatable C dictionary benchmarks (small/large linear vs hashed,
  including managed-key overwrites), plus a unique-owner overwrite fast path
  which copies only the replacement value. Verify aliasing, large values and CoW.
- [x] Profile actual Sirius dictionary workloads using an instrumented
  runtime (`tools/profile_runtime_dict.py`) while the MIR compiler recompiles
  itself. Discovered redundant `TypeTable` construction in `HirBodyState`:
  passing the existing type table directly reduced measured dictionary
  creations from 61,200 to 211 and insertions from 390,976 to 25,034.
  Sample `-O0` self-compilation timings overlapped (roughly 6.7–7.3 s
  in this environment), so there is no robust wall-time speedup claim.
  Increasing dictionary initial capacity from 8 to 16 and inline bucket
  experiments showed no convincing speedup, so neither change was retained.
- [x] Profile MIR-backed allocations with `tools/profile_ir_storage.py` and
  retain a measured liveness-storage improvement: share one immutable empty
  register state across CFG blocks and iterations, cloning on CoW writes.
  On the same MIR compiler self-compilation workload, initial record/array
  allocation calls fell from 4,590,011 to 4,555,316; median native `-O0`
  time fell from ~6.9 to ~5.6 s (three runs each), and `-O1` from ~2.22 to
  ~1.92 s (four runs each). Cumulative initial allocation requests *in bytes*
  rose ~4%; the improvement reduces initialization/allocation overhead, not
  total requested bytes. No MIR opcode/layout change was warranted.
- [x] Attribute ownership and parser arena CoW allocations before considering
  MIR instruction packing. `tools/profile_ir_storage.py` now counts CoW clones
  as well as initial allocations. A self-compilation snapshot found ~3.33 GB
  of cumulative clone requests, dominated by dense `Array[i64]` ownership
  states (~2.84 GB) and the parser's growing `Array[Node]` arena (~0.42 GB).
  These are **cumulative allocations**, not resident memory. Direct caller
  tracing identifies CFG join/transfer and parser `append_node` as the hot
  operations. Packed ownership states were subsequently implemented; these
  older profiling measurements predate that change. Parser API refactoring
  remains a measured candidate, not an automatic next step.
- [x] Extend compile-time module constants from only `i64` to also `bool`:
  typed names, literal values, compile-time `not`, and explicit annotation
  mismatch rejection. Extend this typed representation to records later.
