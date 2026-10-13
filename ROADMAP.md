# Sirius implementation roadmap

This is an **engineering plan**, not a replacement for [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md). The design specifies intended semantics; this document tracks the narrower implementation, its debt, and the order in which the Sirius-written compiler can gain features **using the facilities available at each step**.

## 1. Baseline and evidence

- **Active compiler:** Sirius source in `src/`, compiled through the checked-in C bootstrap in `bootstrap/compiler.c`. The emitter still combines semantic analysis and C11 generation.
- **Historical bootstrap:** `archive/python_bootstrap/` is **frozen**. It is retained for provenance, not used to develop features, regenerate bootstrap C, or run the active test pipeline.
- **Self-hosting:** the checked-in C builds a seed compiler; that compiler recompiles the Sirius sources until their generated C reaches a byte-identical fixed point. The fixed point is compared to `bootstrap/compiler.c`, and the generated compiler runs the full shared emitter suite (85 positives and 37 diagnostics). See `tools/test_bootstrap.py`.
- **Coverage:** 85 emitter executions (13 original language cases + 72 compiler-specific fixtures), 37 emitter diagnostics, 19 tokenizer golden cases, 11 parser golden cases, and 24 independent C references. The historical Python tests cover 38 positive programs. Passing these is **not** proof of ownership safety or full language conformance.

### Rules for further work

1. Keep [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) as the semantic source of truth; document deliberate deviations. Do not treat the Python backend's incidental behavior as the language specification.
2. Implement new compiler features **only in Sirius**. `archive/python_bootstrap/` remains frozen; Python may still be used for development/test orchestration, not as a compiler dependency.
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

The Python scripts under `tools/` are test/build orchestration and remain permitted; **`archive/python_bootstrap/` itself is archival**.

### HIR/MIR performance sequence

- [x] Index typed HIR annotations by syntax-node ID per specialization (sparse lookup, preserving distinct operations on the same node).
- [x] Buffer MIR-C output through string fragments and join once per generated section/function/program.
- [x] Index canonical type descriptors by stable keys, preserving TypeIds. An analogous specialization signature cache was benchmarked and reverted because its key construction made generation slower.
- [x] Split lowering into read-only `MirLowerContext` and mutable `MirBuilder`, passed separately to expression and statement lowering.
- [x] Reduce CFG forward ownership CoW copies with cached sparse block write effects and in-place joins; further liveness/packed-bitset work depends on profiling.
- [ ] Consider compact MIR instruction layout only after the higher-priority improvements.

## 3. Technical and organizational debt

| Priority | Debt | Evidence / risk | Recommended remedy |
| --- | --- | --- | --- |
| **P0** | Mixed typing, ownership and C emission | `src/emit_c.sirius` still combines several compiler stages; some complex `elif` optional fallbacks and expression forms are rejected | Introduce typed HIR then CFG MIR incrementally, retaining the C emitter as a temporary backend |
| **P0** | Ad-hoc numeric type IDs | The new interned descriptors coexist with legacy `TYPE_*` ranges and arithmetic type IDs during migration | Intern canonical `TypeId` entries in a type table; use integer IDs as handles, not as type definitions |
| **P0** | Incomplete ownership coverage | Manual retain/drop and scope cleanup are interleaved with C generation; branch/loop exits and nested lvalues need stronger guarantees | Make ownership explicit on MIR instructions; test all terminators and paths, including errors |
| **P1** | No module/import system | `src/` including `src/cli.sirius` is concatenated in dependency order; collision and coupling risks increase with size | Split compiler components using an ordered build manifest now; later implement static `namespace`/`import` |
| **P1** | Seed-versus-current compiler compatibility | The committed C bootstrap must be able to parse and compile updated compiler sources | Introduce new language features using syntax supported by the seed compiler; regenerate C only after fixed-point tests |
| **P1** | Testing is broad but uneven | fixed-point and emitter suites were previously separate, risking divergent coverage; sanitizers and Windows are not continuous requirements | Run all supported fixtures through the fixed-point compiler; add sanitizer and platform CI where supported |
| **P1** | Limited source diagnostics | Emitter diagnostics are largely free-form strings; node locations are imperfect for some lowered constructs | Preserve spans through HIR/MIR, use stable error categories, report original source positions |
| **P2** | Runtime/C backend coupling | C-specific ownership helpers and runtime wrappers are exposed in emission logic | Backend-neutral MIR ownership operations, then a narrow C11 backend/runtime ABI |
| **P2** | Incomplete numeric and effects policy | Overflow/conversion details, transitive purity, and order-sensitive operations are not fully settled | Decide observable semantics before implementing optimizations or parallel transforms |
| **P2** | Documentation duplication | README historically mixed fixture inventory, decisions and implementation journals | README = usage/status; this document = roadmap/debt; IR_DESIGN = target IR; LANGUAGE_DESIGN = semantics |

Do **not** attempt to solve all of these in one refactor. Each phase below should leave the compiler self-hosting and regression tests runnable.

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
- [ ] Lower `if`/`elif`, `and`/`or`, `while`, `for`, optional `or` and `else return` into branches before C generation. **Started:** `src/mir_lower.sirius` lowers checked scalar functions with `if`/`elif`/`else`, `and`/`or`, `while` and linked specialized calls; optional `or`, `else return`, checked and optional indexing, and dictionary `.get()` now lower into explicit CFG and typed MIR. `src/mir_program.sirius` associates each HIR specialization with a same-index MIR function. The experimental `src/mir_emit_c.sirius` now emits native C from this MIR, selected using `--mir-c`, with differential scalar/string/record tests (`tools/test_mir_c.py`). The legacy emitter is still the default; the MIR path now lowers `for`, array append, and guard statements.
- [ ] Lower `recur` to parallel assignment into temporaries followed by a backedge, even for managed values once ownership is implemented.
- [ ] Make the **default** C backend consume MIR only; compare visible behavior to the old backend while switching constructs one by one. **Started:** opt-in `--mir-c` consumes exactly the linked HIR worklist/MIR, with native scalar/string/record/array differential cases, direct print/panic/register checks and managed-string/record ownership tests.

**Done when:** complex optional/boolean combinations and branch-local evaluation work without special-case rejection; no semantic checks depend on generated C expressions.

### Phase D — Centralize ARC and CoW correctness

**Available then:** typed MIR places, temporaries and explicit control-flow exits; existing type-specific C copy/drop callbacks and CoW runtime.

- [ ] Define ownership for every argument, result, local, field projection and call (borrow, owned value or transferred value); conservatively retain when in doubt.
- [ ] Insert `Retain`, `Release`, `Move`, `Borrow` and `CoWSet`-like MIR operations; ensure nested record/array paths are safe under aliasing.
- [ ] Add cleanup blocks for all normal/early control-flow exits, including returns, `break`, `continue` and `recur` backedges. **Partially done:** `src/mir_cleanup.sirius` inserts path-specific dead-value releases on CFG edges and explicit return/panic cleanup, including unused arguments and path-dependent initialization. `mir_check_balance` now proves no managed register remains possibly owned at a reachable explicit exit; conditional releases are represented explicitly by `MIR_RELEASE_IF_INIT`. `break` and `continue` now lower into MIR; `recur` backedges still need coverage. Division/remainder, checked array reads/writes, and optional unwrap now have explicit guarded panic CFG edges before lifetime/cleanup insertion. Other implicit aborts (including allocation failures) remain non-unwinding.
- [ ] Test managed records, nested arrays, dictionary values, optionals and unions under repeated aliasing, branch exits and sanitizers.
- [ ] **Only afterward** add ownership/uniqueness optimization and in-place storage reuse; keep these invisible at language level.

**Done when:** all currently supported programs have a defined ownership path and tests cover every CFG terminator; no cleanup is manually interspersed with C emission.

### Phase E — Complete bootstrap-language parity and modules

**Available then:** typed HIR/MIR, safe owned values and the existing standard-library building blocks.

- [ ] Generalize remaining optional constructs and nested CoW lvalues (optional indexing and `else return` have initial MIR support). Plain `a[i]` still needs a static bounds proof.
- [ ] Complete constructors (conditional overrides after unconditional publication), constructor-based tagged unions, exhaustive `match`, function values and noncapturing lambdas. Basic `rec` constructors with automatically published parameters and direct `pub` overrides are implemented.
- [ ] Implement basic static `namespace`/`import`/`export` resolution; start with separately parsed sources and a build-time dependency graph, without runtime modules.
- [ ] Expand constant evaluation from module `i64` expressions to **record constants**; consider selected pure intrinsics before general `comptime` evaluation.
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
- [x] Make both the Sirius MIR interpreter and opt-in MIR→C backend execute the same string operations; C uses ARC credits across calls and returns.
- [x] Lower string literals, concatenation and `len(string)` from typed HIR; extend differential and ownership-specific tests.
- [x] Generalize managed register ownership to immutable structural records, including nested strings/records. `MIR_RECORD_NEW`/`MIR_RECORD_GET` consume canonical layouts; interpreter and generated C handle copy/move/retain/release and recursive destruction. See `tools/test_mir_records.py` and record cases in `tools/test_mir_c.py`.
- [x] Add rank-1 MIR arrays with canonical element types, checked access, length and consuming `MIR_ARRAY_SET` with type-directed CoW. Interpreter and MIR-C handle arrays of scalars, strings, records and nested arrays, with alias-preserving differential tests (`tools/test_mir_arrays.py`, `tools/test_mir_c.py`).
- [x] Add a narrow MIR dictionary subset: canonical `Dict[i64|string, T]`, empty construction, consuming CoW insertion, length and eager checked-type `GET_OR`, with both interpreter and native C tests. Source lowering supports construction, `insert`, and `len`; source optional `get` is now supported.
- [x] Add `Optional[T]` storage, optional dictionary lookup/fallback and array indexing, unwrap, and CFG-lazy `or`/`else return` in MIR, with native differential and interpreter ownership tests.
- [ ] Complete record spreads and deep projection updates; `Array.append` and some direct record updates are already supported. Continue ownership validation for all CFG exits.
- [x] Replace repeated full-graph ownership/liveness sweeps with queued CFG worklists and precomputed predecessors; store liveness in dense 0/1 register masks. Add a branching/backedge regression (`tools/test_mir_dataflow.py`). Follow up with packed bitsets only if profiling justifies them. Edge cleanup now uses a single may-ownership/liveness snapshot and conditional releases instead of re-solving the full CFG per edge, substantially reducing MIR-only compiler generation time.
- [x] Eliminate repeated MIR ownership/structure checks between guard, lifetime and exit-cleanup transforms in the combined compiler pipeline, retaining public pass validation and final balance/failure audits. Compare MIR-only generation output byte-for-byte against the previous compiler.
- [x] Propagate must-initialization and may-ownership in one forward worklist for managed CFG balance checks, retaining must-only and may-only entry points. Test joins, backedges and unreachable blocks (`tools/test_mir_ownership_combined.py`).
- [x] Share CFG reachability/predecessors and the must-initialization result during final ownership-balance verification; skip may-ownership dataflow for scalar-only functions. Keep standalone ownership checking and invalid-read diagnostics unchanged.
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
- `experiments/target_ir.py`: exploratory Python dataclasses only, not a runtime dependency or mandatory implementation blueprint.

Update this roadmap at **phase milestones**, not with an entry for every small patch; the commit history and test fixtures provide the incremental development record.

## MIR-only compilation migration

- [x] Extract `CompileResult` from the legacy emitter.
- [x] Share MIR entry validation and managed TypeId classification in `mir_common.sirius`.
- [x] Add an explicit self-hosting coverage probe for the MIR backend, using a MIR-only source manifest and entry-point driver.
- [ ] Support all concrete return annotations and specializations needed to analyze the compiler in HIR. **Progress:** named canonical `rec` annotations (including inside collections), constructor `pub` fields/callee requests, named annotations with module-constant-dependent fields, and module `i64` constants are supported.
- [ ] Complete HIR→MIR lowering for remaining compiler syntax and builtins. **Progress:** typed empty arrays, `Array.append` with CoW-safe copying, `for` over arrays/ranges, `break`, `continue`, expression statements, `print`, CLI arguments, `str(i64)`, file read/write in native C. Added: all six string methods, string comparisons, guard statements, direct record-field updates and record-owned array updates. Remaining: deeper nested updates, unannotated result specialization, other unsupported source constructs, and MIR interpreter parity for file reads.
- [x] Achieve a native MIR-only fixed point through generations 2, 3 and 4, excluding the legacy emitter; verify byte-identical C with `tools/test_mir_selfhost.py`. Reduce C size using a shared return cleanup epilogue, and improve generated compiler performance using amortized array append and single-allocation string join. Correct compound-assignment lowering for locals and record fields.
- [ ] Run the full execution and diagnostic suites with MIR as the default.
- [ ] Remove the legacy C emitter and retire the `--mir-c` switch.
