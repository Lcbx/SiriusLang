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

Use `gcc` instead of `cc` where needed. On Windows, use `sirius.exe` / `output.exe` and launch them as appropriate. The self-hosted CLI is `sirius <input.sirius> <output.c>`.

**Regenerating the C bootstrap after changing Sirius compiler sources:**

```sh
python tools/test_emitter.py
python tools/test_selfhost.py --update-bootstrap
python tools/test_selfhost.py
```

The update command compiles from the **existing** `bootstrap_c/compiler.c`, checks the self-reproducing fixed point and smoke cases, then overwrites the committed C source. Commit that generated C along with the Sirius changes. New compiler source must remain compilable by the prior C seed; add support for new syntax before using it in `src/`.

HIR now runs an independent pre-emission body traversal after declaration indexing. It resolves lexical scopes, local/parameter/module-constant names, and statically known scalar expression types, recording typed source nodes and rejecting definite local binding errors. Record and collection literals, explicit collection constructors, field/index access, optional unboxing and selected builtin/method calls now receive canonical types when inputs are known. Generic specialization and ownership remain in the emitter; this is not yet a complete semantic pass.

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
| `python tools/test_selfhost.py` | C bootstrap → self-reproducing generated C, committed bootstrap comparison, 82 executable cases and 36 expected diagnostics |
| `python tools/test_emitter.py` | Sirius-written emitter: 82 positive C executions, 36 expected diagnostics |
| `python tools/test_parser.py` | Sirius parser: 11 golden cases and syntax smoke tests |
| `python tools/test_types.py` | Canonical type interning, compiler bindings, and generic cache identity |
| `python tools/test_tokenizer.py` | Sirius tokenizer: 19 golden cases |
| `python tools/test_selfhost.py --update-bootstrap` | Explicitly replace the committed C bootstrap after fixed-point and full emitter checks |
| `python tools/verify_c.py` | 24 independent C references plus runtime panic; **does not test generated compiler output** |

Indexing uses `a[?i]` for optional reads, `a[!i]` for checked reads/updates, and postfix `!` to unbox an optional. Boolean guards use `condition else return value`, `condition else break`, or `condition else continue`.

The counts describe the current fixtures, not full conformance. The active tokenizer, parser, emitter and bootstrap tests use **only the committed C compiler** as their compiler seed. Python orchestrates tests but does not compile Sirius. The historical `bootstrap_python/` tests may still be run manually for archival comparison.

## Repository map

```text
src/                  Sirius lexer, parser, type table, initial typed HIR, and C emitter
bootstrap_python/     Frozen historical Python bootstrap (not used by active builds)
bootstrap_c/          Checked-in, reproducible generated compiler.c
include/              Shared C11 runtime (ARC, CoW collections, strings, I/O)
tests/                Language, lexer, parser, emitter and failure fixtures
tools/                Tests and bootstrap/build helpers
LANGUAGE_DESIGN.md    Intended language semantics (normative direction)
IR_DESIGN.md          Proposed typed HIR and CFG MIR lowering
ROADMAP.md            Technical debt, design assessment, staged implementation plan
target_ir.py          Illustrative Python IR dataclasses; not used by the compiler
```

Source modules in `src/` are currently **concatenated in a fixed order**, along with `tests/emitter/driver.sirius` using `tools/sirius_sources.py`, to build the compiler. Sirius imports are not implemented yet. `tools/sirius_sources.py` centralizes the ordered source manifest; `tools/emitter_cases.py` contains the shared stage-1/fixed-point regression inventory.

## Current scope and limitations

The self-hosted compiler supports a tested subset of scalar control flow, function specialization, structural records and `rec` constructors (parameters automatically published; `def` constructors rejected), managed strings, rank-1 arrays (including nested/record elements), dictionaries, partial optionals, and C emission with conservative ownership handling. Missing constructs include tagged unions and richer optional/constructor forms.

The main outstanding work is **a robust typed representation and ownership-aware control-flow lowering**. `src/emit_c.sirius` still combines name/type resolution, specialization, ownership handling, and textual C emission. A canonical type-descriptor table now interns scalar, record, array, dictionary, and optional types. Generic specialization caches, bound symbols and record-field lookup now use canonical `TypeId`s. Canonical descriptors now drive record field layout and C retain/drop helper selection for records, arrays and dictionaries. The incremental typed HIR (`src/hir.sirius`) resolves literals, local names, record fields, unary/binary expressions, assignment compatibility, builtin and user-function call signatures, string methods, collection method types, and record/constructor field composition using canonical `TypeId`s. A separate pre-emission HIR traversal now handles lexical scopes, bindings, known scalar/record/collection expressions, checked/optional indexing, optional fallback and selected call results. The C emitter still handles deferred expression types, argument evaluation, generic specialization, constructors, control flow and ownership; no complete typed-function HIR exists. Legacy arithmetic type IDs still name generated C types and occur in the remaining lowering paths; a complete HIR-only semantic pass remains Phase B work. Memory management and evaluation order have focused tests, but they are not yet established correct for every nested expression or early exit.

Other design features—including modules, general operator overloads, universal descriptors and `Any`, pure `map`/`filter`/`generate`, multidimensional arrays, SIMD and concurrency—remain unimplemented. Python-style negative array indices are explicitly deferred.

See [ROADMAP.md](ROADMAP.md) for **prioritized debt, incremental implementation steps, acceptance criteria, and design alignment**. See [IR_DESIGN.md](IR_DESIGN.md) for the intended intermediate representations. Regenerate changed bootstrap C with `python tools/test_selfhost.py --update-bootstrap`, then verify with `python tools/test_selfhost.py`; never edit generated C manually.
