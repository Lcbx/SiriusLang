# Sirius

Sirius is an experimental, functional-first, statically compiled language with
Python-like syntax, local imperative control flow, structural typing, immutable
value semantics and copy-on-write collections. Its compiler emits **C11**.

**Current status:** the compiler is written in Sirius and can reproduce its own
committed C bootstrap. Self-hosting covers a subset of
[the intended language](LANGUAGE_DESIGN.md), not the complete design.
The historical Python compiler is archived and is **not** used for active builds.

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

`./sirius input.sirius output.c` selects the **legacy C emitter** (the only
established self-hosting path). `./sirius --mir-c input.sirius output.c` selects
the **experimental HIR → MIR → C backend**. Unsupported MIR constructs fail
explicitly; there is no implicit fallback. The stand-alone MIR interpreter is
currently a testable library, not a source-program CLI.

## Compiler development and bootstrap maintenance

Three commands have deliberately different responsibilities:

| Command | Purpose |
| --- | --- |
| `python tools/build_compiler.py [--output PATH]` | Compile the **committed** C bootstrap and runtime into an executable; does not regenerate Sirius sources |
| `python tools/generate_bootstrap.py` | Compile Sirius sources through successive compiler generations to a byte-identical fixed point, then update `bootstrap/compiler.c`; **does not run language fixtures** |
| `python tools/generate_bootstrap.py --check` | Verify committed bootstrap matches the computed fixed point, without modifying it or running fixtures |
| `python tools/test_bootstrap.py` | Verify the fixed point and committed C, then run compiler execution and diagnostic fixtures |
| `python tools/test_bootstrap.py --fixed-point-only` | Verify self-hosting and committed C without running language fixtures |
| `python tools/test_compiler.py` | Test the default C emitter built directly from the committed C bootstrap |
| `python tools/probe_mir_selfhost.py --require-ready` | Probe MIR-only compiler generation; **not** a self-hosting fixed-point test |

Normal changes to the Sirius compiler:

```sh
python tools/test_compiler.py --jobs 4
python tools/generate_bootstrap.py
python tools/test_bootstrap.py --jobs 4
```

Commit Sirius source and generated `bootstrap/compiler.c` together. **Never edit
generated C by hand.** The *previous* committed compiler must be able to compile
the updated Sirius source; introduce new syntax in stages.

The ordered source manifests and byte-oriented concatenation live in
`tools/source_manifest.py`. Sources are concatenated because Sirius imports are
not implemented yet. `--rebuild` clears content-addressed compiled artifacts,
and test runners supporting parallel fixtures accept `--jobs N` (default up to
4). Cached objects and binaries are stored under `.build/cache/`, while fixture
executables and successive compiler generations use temporary directories.

## Tests

Run individual suites as needed:

| Command | Scope |
| --- | --- |
| `python tools/test_bootstrap_tools.py` | Bootstrap generation, stale-C rejection and CLI dispatch |
| `python tools/test_compiler.py` | 85 successful native executions and 37 expected diagnostics |
| `python tools/test_tokenizer.py` | 19 tokenizer golden fixtures |
| `python tools/test_parser.py` | 11 parser golden fixtures and smoke checks |
| `python tools/test_types.py` | Type interning, typed HIR and specialization |
| `python tools/test_hir_worklist.py` | Independent HIR specialization worklist |
| `python tools/test_mir.py` | Hand-constructed executable MIR and verifier |
| `python tools/test_mir_lower.py` | HIR → MIR lowering and interpreter execution |
| `python tools/test_mir_program.py` | Linked concrete MIR specializations |
| `python tools/test_mir_c.py` | Differential MIR-C / legacy-C cases |
| `python tools/test_mir_migration.py` | Source feature parity against legacy emission |
| `python tools/test_mir_interpreter.py` | MIR interpreter behavior |
| `tools/test_mir_*.py` | Focused MIR behavior and ownership tests (run scripts individually) |
| `python tools/test_reference_c.py` | 24 independent handwritten C references plus runtime-panic checks; not a compiler test |
| `python tools/test_build_cache.py` | Content-addressed build-cache invalidation |
| `python tools/test_tool_utils.py` | Process result and newline test helpers |
| `python tools/test_python_bootstrap.py --command 'python archive/python_bootstrap/sirius.py'` | **Archival** Python compiler tests only; not part of active bootstrap |

The 85/37 counts describe the current tests, not a full language conformance
claim. The fixture-generation scripts are **manual authoring tools**:
`tools/generate_core_fixtures.py` and
`tools/generate_extended_fixtures.py`; they overwrite checked-in fixtures.

## Repository structure

```text
src/                    Sirius compiler modules, entry points and MIR interpreter
bootstrap/compiler.c    Committed, reproducible C compiler bootstrap
runtime/                C11 runtime implementation and header
archive/python_bootstrap/  Historical, frozen Python implementation
experiments/target_ir.py  Illustrative Python IR dataclasses (not used in builds)
tests/compiler/         Compiler feature and diagnostic fixtures
tests/fixtures/         Earlier language examples
tests/reference_c/      Independent handwritten C examples
tests/mir/              Sirius MIR test drivers
tools/                  Build/generation commands, shared helpers and test runners
```

`src/cli.sirius` is the default compiler's entry point and
`src/mir_cli.sirius` is the MIR-only entry point. `src/emit_c.sirius` is
still required for the self-hosted compiler, while `src/mir_emit_c.sirius`
implements the experimental replacement backend.

## Implementation boundaries

The legacy backend covers a practical subset of scalars, recursion, records,
constructor functions, strings, rank-1 arrays, dictionaries and optionals.
Canonical type interning and typed HIR exist alongside legacy emitter-local
type and specialization handling.

The MIR path implements typed CFG instructions, an interpreter, a C emitter,
specialization lowering, explicit ownership operations, guard insertion, liveness,
last-use release insertion and exit cleanup. It supports numerous source-level
scalar, string, record, array, dictionary and optional operations. However,
some nested projection updates, record spreads and inference cases still prevent
complete MIR-only compilation of the compiler. **No MIR-only self-hosting fixed
point has been demonstrated.** Arrays use CoW for consuming updates; some
updates (including `Array.append`) still use conservative copying instead of
unique-owner reuse. MIR interpreter file-I/O parity is incomplete.

Modules/imports, multidimensional arrays, broadcasting, full effect/purity
checking, universal runtime type descriptors and concurrent map are future work.
Negative indexing remains deferred.

See [ROADMAP.md](ROADMAP.md) for priorities, [IR_DESIGN.md](IR_DESIGN.md)
for IR architecture, [INTERPRETER.md](INTERPRETER.md) for interpreter details
and [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md) for intended semantics.
