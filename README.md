# Sirius

Sirius is an experimental statically compiled, functional-first language with Python-like syntax, immutable values, structural typing, local imperative control flow, and planned NumPy-like arrays. The compiler currently targets **C11**.

**Status: minimally self-hosted.** The Sirius compiler can compile its own combined source from the committed C bootstrap until the generated C reproduces itself. This demonstrates a working bootstrap subset, **not** complete implementation of [the language design](LANGUAGE_DESIGN.md). The historical Python compiler is frozen.

## Quick start

Requirements: a C11 compiler (`cc` or `gcc`); Python 3.10+ is needed for the test/build **scripts**, not for the compiler. Commands below run from the repository root.

**Build the self-hosted compiler from committed C (no Python required):**

```sh
cc -std=c11 -I include bootstrap_c/compiler.c include/sirius_runtime.c -lm -o sirius
./sirius tests/fixtures/01_scalars.sirius output.c
cc -std=c11 -I include output.c include/sirius_runtime.c -lm -o output
./output
```

Use `gcc` instead of `cc` where needed. On Windows, use `sirius.exe` / `output.exe` and launch them as appropriate. The self-hosted CLI is `sirius <input.sirius> <output.c>` (legacy C emitter). For the experimental scalar/string/record/array/dictionary HIR→MIR→C path, use `sirius --mir-c <input.sirius> <output.c>`; unsupported constructs are rejected without fallback.

**Regenerating the C bootstrap after changing Sirius compiler sources:**

```sh
python tools/test_emitter.py
python tools/test_selfhost.py --update-bootstrap
python tools/test_selfhost.py --check
```

The update command compiles from the **existing** `bootstrap_c/compiler.c`, checks the self-reproducing fixed point, and overwrites the committed C source **without running fixture tests**. The verification command (`--check`, also the default when no mode is specified) checks the fixed point, compares committed C, and runs the full emitter suite. Commit generated C together with the Sirius changes. New compiler source must remain compilable by the prior C seed; add support for new syntax before using it in `src/`.

HIR now runs an independent pre-emission body traversal after declaration indexing. It resolves lexical scopes, local/parameter/module-constant names, and statically known scalar expression types, recording typed source nodes and rejecting definite local binding errors. Record and collection literals, explicit collection constructors, field/index access, optional unboxing and selected builtin/method calls now receive canonical types when inputs are known. Concrete user-function calls and record constructors now also resolve through the declaration table; HIR checks annotated arguments, infers nested nonrecursive generic results, and records callee identities. HIR now defines the canonical specialization registry, keyed by declaration identity and argument TypeIds with in-progress tracking. `src/hir_specialize.sirius` uses a backend-independent worklist to type each concrete function body once (including mutually recursive and repeated signatures). `src/mir_program.sirius` lowers supported scalar/string/record specializations to MIR functions with matching indices. The experimental `--mir-c` path emits C from those linked MIR functions, with HIR-owned specialization identities; the legacy C emitter remains the default for the full bootstrap subset.

## MIR-only migration status

The experimental `--mir-c` backend additionally lowers named structural `rec` annotations, non-generic `rec` constructor calls (including direct `pub` expressions), module `i64` constants, empty typed arrays, `Array.append`, array and half-open-range `for` loops, `break`/`continue`, expression statements and `print`. The native C backend also supports `arguments`, `str(i64)`, `read_text` and `write_text`. `Array.append` currently copies its storage, including for a unique owner; correctness takes precedence over optimization. The MIR interpreter does not yet execute `read_text`.

**The legacy `emit_c.sirius` remains necessary**: the MIR path still lacks language constructs used by the compiler itself (notably deeper projection updates, some return-type inference, record spreads, and other complex lowering). A MIR self-hosting fixed point has not been demonstrated. Continue running the default compiler through the legacy emitter until `tools/check_mir_bootstrap.py --require-ready`, full differential tests and independent MIR self-hosting all pass.

## Tests

Test runners use `-O0` for disposable C binaries and compile the runtime once
per input fingerprint. The shared compiled bootstrap, runtime object, and Sirius
test drivers are cached in Git-ignored `.build/cache/`. A change to Sirius/C
sources, runtime headers, compiler flags, or the selected C compiler automatically
produces a new cache entry; GCC does not perform this invalidation itself.

Individual fixtures run concurrently using Python threads to launch independent
C compilers and executables. All active runners accept `--jobs N` (default up to
4; use `--jobs 1` to debug) and `--rebuild` to discard cached build artifacts.
The historical Python compiler test runner accepts these options too.

```sh
python tools/test_emitter.py --jobs 4
python tools/test_parser.py --jobs 4
python tools/test_selfhost.py --jobs 1
python tools/test_emitter.py --rebuild
```

Self-hosting still **generates each compiler generation afresh** for its
fixed-point check. Only the seed compiler and shared C runtime are cached.
Fixture outputs remain in isolated temporary directories, not in `.build/`.


| Command | What it checks |
| --- | --- |
| `python tools/test_selfhost.py --check` (or no mode flag) | C bootstrap fixed point, committed C comparison, 85 executable cases and 37 expected diagnostics |
| `python tools/test_selfhost_modes.py` | Unit checks for update/check dispatch, stale bootstrap and missing fixed point |
| `python tools/test_emitter.py` | Sirius-written emitter: 85 positive C executions, 37 expected diagnostics |
| `python tools/test_parser.py` | Sirius parser: 11 golden cases and syntax smoke tests |
| `python tools/test_types.py` | Canonical type interning, compiler bindings, and generic cache identity |
| `python tools/test_mir.py` | Sirius-written typed CFG MIR interpreter and verifier (hand-built MIR) |
| `python tools/test_mir_lower.py` | Checked HIR → scalar CFG MIR lowering; branching, loops, short-circuit evaluation, and execution |
| `python tools/test_hir_worklist.py` | Backend-independent concrete HIR specialization worklist, including canonical array type signatures |
| `python tools/test_mir_program.py` | HIR worklist → linked scalar MIR program; nested, repeated and recursive calls |
| `python tools/test_mir_ownership.py` | Forward CFG definite-initialization and move/release validation |
| `python tools/test_mir_lifetime.py` | Backward CFG liveness and last-use release insertion |
| `python tools/test_mir_exit_cleanup.py` | Edge-specific and exit cleanup, unused arguments, managed returns, loops and panic terminators |
| `python tools/test_mir_guard.py` | Explicit panic guards for division, checked arrays, and optional unwrap (interpreter/native) |
| `python tools/test_mir_failure_contract.py` | Failure metadata, post-lowering guard audit, and non-unwinding operation classification |
| `python tools/test_mir_optional.py` | Managed optional values and missing-value control flow in Sirius interpreter and native C |
| `python tools/test_mir_c.py` | Opt-in HIR→MIR→C execution against legacy C (including managed strings and dictionaries), plus direct MIR checks |
| `python tools/test_mir_migration.py` | New MIR coverage against legacy native C: named records/constructors, module constants, CLI/file builtins, append, array/range iteration, loop control |
| `python tools/check_mir_bootstrap.py --require-ready` | Probe the MIR-only compiler source manifest (excludes `emit_c.sirius`) with no legacy fallback; a full fixed point is still required |
| `python tools/test_mir_managed.py` | Shared interpreted/native managed-string MIR, retain/move/release, calls and invalidated-register checks |
| `python tools/test_mir_dict.py` | MIR dictionaries: CoW insertion, string keys, nested arrays and consuming updates (interpreter/native) |
| `python tools/test_mir_records.py` | Nested structural records, canonical fields, recursive release, self-copy, calls, and use-after-release |
| `python tools/test_tokenizer.py` | Sirius tokenizer: 19 golden cases |
| `python tools/test_selfhost.py --update-bootstrap` | Update committed C after verifying the fixed point; skip fixture tests |
| `python tools/verify_c.py` | 24 independent C references plus runtime panic; **does not test generated compiler output** |

Indexing uses `a[?i]` for optional reads, `a[!i]` for checked reads/updates, and postfix `!` to unbox an optional. Boolean guards use `condition else return value`, `condition else break`, or `condition else continue`.

The counts describe the current fixtures, not full conformance. The active tokenizer, parser, emitter and bootstrap tests use **only the committed C compiler** as their compiler seed. Python orchestrates tests but does not compile Sirius. The historical `bootstrap_python/` tests may still be run manually for archival comparison.

## Repository map

```text
src/                  Sirius lexer, parser, typed HIR/worklist, C emitter, MIR lowering and interpreter
bootstrap_python/     Frozen historical Python bootstrap (not used by active builds)
bootstrap_c/          Checked-in, reproducible generated compiler.c
include/              Shared C11 runtime (ARC, CoW collections, strings, I/O)
tests/                Language, lexer, parser, emitter and failure fixtures
tools/                Tests and bootstrap/build helpers
LANGUAGE_DESIGN.md    Intended language semantics (normative direction)
IR_DESIGN.md          Typed IR design and proposed MIR lowering
INTERPRETER.md        MIR interpreter progress and backend testing policy
ROADMAP.md            Technical debt, design assessment, staged implementation plan
target_ir.py          Illustrative Python IR dataclasses; not used by the compiler
```

Source modules in `src/` are currently **concatenated in a fixed order**, along with `tests/emitter/driver.sirius` using `tools/sirius_sources.py`, to build the compiler. Sirius imports are not implemented yet. `tools/sirius_sources.py` centralizes the ordered source manifest; `tools/emitter_cases.py` contains the shared stage-1/fixed-point regression inventory.

## MIR backend migration (in progress)

`compile_common.sirius` contains `CompileResult`, and `mir_common.sirius` contains
helpers shared by the MIR interpreter and C emitter. The **legacy C emitter
remains the default** until the replacement compiler self-hosts and passes the
full regression suite.

`MIR_ONLY_COMPILER_SOURCES` excludes `src/emit_c.sirius` and uses
`tests/emitter/mir_driver.sirius`. Run `python tools/check_mir_bootstrap.py
--require-ready` to probe the replacement source through MIR, without legacy
fallback. Successful C emission alone would not establish a self-hosting fixed
point. That probe is not yet verified to complete successfully.

MIR now handles string methods (`byte_at`, `slice`, `find`, `starts_with`,
`ends_with`, `join`), string comparisons, direct record-field updates,
record-owned array updates, guard statements, and calls published by record
constructors. Record updates rebuild canonical values rather than mutate
shared aliases. The regression coverage is in `tools/test_mir_migration.py`.

## Current scope and limitations

The self-hosted compiler supports a tested subset of scalar control flow, function specialization, structural records and `rec` constructors (parameters automatically published; `def` constructors rejected), managed strings, rank-1 arrays (including nested/record elements), dictionaries, partial optionals, and C emission with conservative ownership handling. Missing constructs include tagged unions and richer optional/constructor forms.

The new `src/mir.sirius` and `src/mir_interpreter.sirius` provide a verified, executable CFG MIR subset for `i64`, `bool`, strings, canonical structural records and rank-1 arrays and an unoptimized interpreter written in Sirius. `src/mir_lower.sirius` lowers **checked scalar/string/record/array HIR** (bindings, arithmetic, string literals/concatenation/length, record literals/field reads, array literals/checked indexing/length/CoW updates, short-circuiting, branches, loops, returns, and specialized function calls). `src/hir_specialize.sirius` resolves a backend-independent worklist of concrete signatures and checked bodies, while `src/mir_program.sirius` assembles their corresponding MIR functions. The opt-in `src/mir_emit_c.sirius` translates those verified functions to C11 (using the same specialization indices). Tests compare native executions against the legacy backend, and separate interpreter tests execute linked scalar programs without C generation. Record literals, field access, and rank-1 arrays now work in MIR (including nested managed elements). Arrays use a consuming update instruction with type-directed copy-on-write; record spreads/deletions, arbitrary nested projections, and some unannotated generic callees remain unsupported; the source-level interpreter CLI is not wired. String, record and array registers own references, with retain/copy, move and release operations. Copies share managed storage; array updates copy shared backing storage and reuse uniquely owned backing storage. Dropping the last collection reference recursively releases nested managed values; normal-return cleanup is implemented in C. See [INTERPRETER.md](INTERPRETER.md) for scope and the planned shared `--backend` test flag.

The main outstanding work is **a robust typed representation and ownership-aware control-flow lowering**. `src/emit_c.sirius` still combines name/type resolution, specialization, ownership handling, and textual C emission. A canonical type-descriptor table now interns scalar, record, array, dictionary, and optional types. The HIR-owned specialization registry now keys cache entries by declaration identity and canonical argument `TypeId`s, tracking in-progress/completed entries. The C emitter still initiates specialization and generates C; instance code storage no longer owns the specialization key or status. Bound symbols and record-field lookup use canonical `TypeId`s. Canonical descriptors now drive record field layout and C retain/drop helper selection for records, arrays and dictionaries. The incremental typed HIR (`src/hir.sirius`) resolves literals, local names, record fields, unary/binary expressions, assignment compatibility, builtin and user-function call signatures, string methods, collection method types, and record/constructor field composition using canonical `TypeId`s. A separate pre-emission HIR traversal now handles lexical scopes, bindings, known scalar/record/collection expressions, checked/optional indexing, optional fallback, and concrete user-function/constructor call results (including nested nonrecursive generic inference). The C emitter still handles unresolved expression types, argument evaluation, initiating specializations, constructor lowering, control flow and ownership; no complete typed-function HIR exists. Legacy arithmetic type IDs still name generated C types and occur in the remaining lowering paths; a complete HIR-only semantic pass remains Phase B work. Memory management and evaluation order have focused tests, but they are not yet established correct for every nested expression or early exit.

Other design features—including modules, general operator overloads, universal descriptors and `Any`, pure `map`/`filter`/`generate`, multidimensional arrays, SIMD and concurrency—remain unimplemented. Python-style negative array indices are explicitly deferred.

See [ROADMAP.md](ROADMAP.md) for **prioritized debt, incremental implementation steps, acceptance criteria, and design alignment**. See [IR_DESIGN.md](IR_DESIGN.md) for the intended intermediate representations. Regenerate changed bootstrap C with `python tools/test_selfhost.py --update-bootstrap`, then verify with `python tools/test_selfhost.py --check`; never edit generated C manually.

The experimental MIR backend now includes `Dict[i64 | string, T]` storage (keys are separately typed as either `i64` or `string`), with empty construction, `insert`, and `len` lowered from supported Sirius source. Direct MIR additionally provides an eager `MIR_DICT_GET_OR` primitive. Source-level `.get(...)` and lazy optional fallback now lower through typed Optional and CFG branches. MIR dictionaries currently use a simple linear key scan; this is a correctness-oriented baseline, not a production hash-table implementation.

The experimental `--mir-c` backend supports optional rank-1 array indexing (`a[?i]`), dictionary `.get(key)`, postfix unwrap (`!`), lazy `or` fallback, and `let x = optional else return expr`. The legacy C backend remains the default.

The experimental HIR→MIR pipeline first converts division/remainder, checked array access/mutation and optional unwrap failures into explicit guarded CFG `Panic` exits (`src/mir_guard.sirius`), then runs backward CFG last-use release insertion (`src/mir_lifetime.sirius`), forward definite-initialization/move verification (`src/mir_ownership.sirius`), then path-sensitive exit cleanup (`src/mir_cleanup.sirius`). The cleanup pass releases dead managed values along CFG edges and at explicit `Return`/`Panic` terminators, including unused arguments, while preserving returned references. A forward may-initialization analysis and `MIR_RELEASE_IF_INIT` now handle values present on only some paths; `mir_check_balance` verifies that all managed registers other than a returned value are released at reachable explicit exits. The analyses share register-read definitions; existing backend return cleanup remains defensive. These covered failures now pass through MIR cleanup before panicking; allocation failures, other runtime checks, and arbitrary host panics remain non-unwinding. See `tools/test_mir_lifetime.py`, `tools/test_mir_ownership.py`, and `tools/test_mir_exit_cleanup.py`, and `tools/test_mir_balance.py`.

The experimental MIR pipeline now audits its runtime failure contract: checked division/remainder, bounds-checked array operations and optional unwrap must be marked as CFG-guarded before code generation. `mir_failure_info` additionally identifies operations that can terminate outside the local CFG (allocations, I/O and calls whose callees fail). These **do not unwind caller-owned values**; the audit reports them but cannot make them recoverable. Integer overflow outside division remains unspecified by the language design.
