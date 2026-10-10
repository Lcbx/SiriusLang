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

## Tests

| Command | What it checks |
| --- | --- |
| `python tools/test_selfhost.py` | C bootstrap → self-reproducing generated C, committed bootstrap comparison, 16 executable smoke cases |
| `python tools/test_emitter.py` | Sirius-written emitter: 67 positive C executions, 29 expected diagnostics |
| `python tools/test_parser.py` | Sirius parser: 10 golden cases and syntax smoke tests |
| `python tools/test_tokenizer.py` | Sirius tokenizer: 19 golden cases |
| `python tools/test_selfhost.py --update-bootstrap` | Explicitly replace the committed C bootstrap after fixed-point and smoke checks |
| `python tools/verify_c.py` | 24 independent C references plus runtime panic; **does not test generated compiler output** |

The counts describe the current fixtures, not full conformance. The active tokenizer, parser, emitter and bootstrap tests use **only the committed C compiler** as their compiler seed. Python orchestrates tests but does not compile Sirius. The historical `bootstrap_python/` tests may still be run manually for archival comparison.

## Repository map

```text
src/                  Sirius lexer, parser, and semantic checker/C emitter
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

Source modules in `src/` are currently **concatenated in a fixed order**, along with `tests/emitter/driver.sirius`, to build the compiler. Sirius imports are not implemented yet. `tools/test_selfhost.py` documents and verifies the C-only compilation sequence.

## Current scope and limitations

The self-hosted compiler supports a tested subset of scalar control flow, function specialization, structural records and constructors, managed strings, rank-1 arrays (including nested/record elements), dictionaries, partial optionals, and C emission with conservative ownership handling. Missing constructs include tagged unions and richer optional/constructor forms.

The main outstanding work is **a robust typed representation and ownership-aware control-flow lowering**. `src/emit_c.sirius` still combines name/type resolution, specialization, ownership handling, and textual C emission. Type IDs are partly encoded using numeric ranges rather than fully interned canonical descriptors. Memory management and evaluation order have focused tests, but they are not yet established correct for every nested expression or early exit.

Other design features—including modules, general operator overloads, universal descriptors and `Any`, pure `map`/`filter`/`generate`, multidimensional arrays, SIMD and concurrency—remain unimplemented. Python-style negative array indices are explicitly deferred.

See [ROADMAP.md](ROADMAP.md) for **prioritized debt, incremental implementation steps, acceptance criteria, and design alignment**. See [IR_DESIGN.md](IR_DESIGN.md) for the intended intermediate representations. Regenerate changed bootstrap C with `python tools/test_selfhost.py --update-bootstrap`, then verify with `python tools/test_selfhost.py`; never edit generated C manually.
