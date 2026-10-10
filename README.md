# Sirius compiler — bootstrap and self-hosting work

This repository contains two compilers:

- **Python stage-0 (`bootstrap_compiler/`):** a working Sirius-to-C11 subset compiler, using SLY, a dataclass syntax AST, specialization, static checks, and ownership-aware C generation.
- **Sirius stage-1 (`src/`):** a tokenizer, indexed-AST parser, and subset type checker/C11 emitter written in Sirius, now able to compile its own concatenated sources.

**Minimal self-hosting is verified:** stage-1 compiles its own source to stage-2 C, and the stage-2 compiler produces byte-identical stage-3 C. This does **not** imply full parity with Python stage-0 or the complete language design.

## Layout

- `tests/fixtures/*.sirius` — all positive compiler fixtures, including compiler-authoring features (only the first 24 have independent C references).
- `tests/expected_c/*.c` — compilable C11 **reference lowering**, not an expected byte-for-byte compiler output.
- `tests/expected_output/*.txt` — standard output, with exit status 0.
- `tests/negative/*.sirius`, `tests/negative/expectations.json` — expected rejection cases and suggested stable diagnostic IDs.
- `tests/runtime_errors/` — checked failure examples with expected nonzero exit and panic message.
- `tests/deferred/` — planned language features with illustrative source and expected output; not yet exercised by the runner.
- `include/sirius_runtime.{h,c}` — C11 runtime with managed strings, generic 1D arrays and hash dictionaries (typed element callbacks), reference counting, CoW, and file I/O; retains the original i64-only APIs for independent C references.
- `bootstrap_compiler/sirius.py` — actual CLI compiler entry point.
- `bootstrap_compiler/syntax.py` — SLY lexer, explicit indentation tokens, dataclass AST, Pratt expression parser.
- `bootstrap_compiler/codegen.py` — specialization, semantic checks, managed lifetime tracking, C11 code generation.
- `bootstrap_compiler/sly/` — vendored SLY source with its license (from the supplied archive).
- `src/tokenizer.sirius` — Sirius-written byte-span lexer (ASCII identifiers, UTF-8 strings), indentation and lexical diagnostics.
- `src/parser.sirius` — Sirius-written recursive-descent/precedence parser with an indexed AST arena and syntax diagnostics.
- `src/emit_c.sirius` — Sirius-written type checker and C11 emitter, including specialization, managed record constructors, nested rank-1 arrays, dictionaries and partial ownership handling.
- `tests/emitter/`, `tools/test_emitter.py` — executable C-emission golden tests and expected compiler diagnostics.
- `bootstrap_c/compiler.c` — verified generated C bootstrap source; builds the Sirius compiler without Python.
- `tools/test_selfhost.py` — stage-0 → stage-1 → stage-2 → stage-3 reproducibility and executable smoke tests.
- `tests/tokenizer/{fixtures,expected_output}/`, `tests/tokenizer/driver.sirius`, `tools/test_tokenizer.py` — tokenizer golden tests.
- `tests/parser/{fixtures,expected_output}/`, `tests/parser/driver.sirius`, `tools/test_parser.py` — parser AST/diagnostic golden tests and syntax smoke tests.
- `LANGUAGE_DESIGN.md` — current Sirius language design and agreed clarifications.
- `target_ir.py`, `IR_DESIGN.md` — proposed **future** typed HIR and CFG MIR representation (not yet integrated).
- `tools/verify_c.py` — C11 compilation and execution of all reference C fixtures.
- `tools/test_compiler.py` — executable end-to-end compiler conformance suite, with optional strict C warnings, negative diagnostics, and runtime panic tests.

## Positive tests

The first 24 compiler fixtures have independent C reference programs. The 14 additional stage-0 fixtures (`tests/fixtures/25_*.sirius` through `38_*.sirius`) cover string, record, nested and union arrays; dictionaries with managed values; collection iteration; CLI arguments; a miniature token/symbol-table/C-text workflow; the rank-1 array shorthand; UTF-8 byte inspection; multiline parenthesized expressions; and module integer constants.

| File | Bootstrap behavior |
| --- | --- |
| 01_scalars | Scalar variables, annotation, arithmetic and output |
| 02_branches | if/elif/else, comparisons, lazy Boolean `and` |
| 03_loops | Half-open `for` range, `while`, `break`, `continue` |
| 04_recursion | Ordinary recursion, `recur` with simultaneous argument replacement |
| 05_records | Structural inline records, field update, value semantics |
| 06_constructor | `pub` fields and computed constructor fields |
| 07_record_spread | Spreading, removing, overriding record fields |
| 08_array_cow | Dynamic array literal, optional index, checked CoW assignment |
| 09_strings | Immutable strings, concatenation and printing |
| 10_generic | Instantiation of a generic function at two scalar types |
| 11_extension_method | Resolving dot syntax to a free function |
| 12_functions_as_values | Named function pointers and noncapturing lambdas |
| 13_optional_early_exit | `Optional` elimination via `else return` |
| 14_nested_cow | Managed array nested in record; recursive retain/drop |
| 15_array_return | Array aliasing, retain, CoW, and ownership transfer on return |
| 16_tagged_union | Constructor-based union alias and exhaustive pattern matching |
| 17_dict | Typed `Dict` and persistent CoW updates |
| 18_file_io_PROVISIONAL | Read/write UTF-8 text, Optional result, side effects |
| 19_optional_fallback | Lazy evaluation of optional fallback |
| 20_structural_layout | Canonical sorted-field record layout across declaration order |
| 21_managed_union | ARC for managed union alternatives |
| 22_constructor_override | Conditional override of previously published constructor field |
| 23_evaluation_order | Ordered argument/operand/record evaluation and repeated while tests |
| 24_pub_shorthand | `pub value.x` constructor shorthand |

`_PROVISIONAL` cases exercise **required bootstrap capabilities** whose syntax/API is not yet settled. The `16_tagged_union` and `17_dict` fixtures now use the agreed forms.

The names of C symbols, byte layout of runtime headers, function specialization mangling, and exact sequence of C statements are deliberately **not** contractual. The language semantics, compile-error classifications, exit status, and stdout are the relevant checks.

## Run reference C tests

```bash
python tools/verify_c.py
```

This invokes `cc` with C11 and strict warnings, runs the 24 reference programs in a temporary directory, and checks stdout and one expected runtime panic. It exercises the reference C, **not** either compiler's emitted C. Unlike `test_compiler.py` and `test_emitter.py`, `verify_c.py` currently requires an executable named `cc` on `PATH` (it does not fall back to `gcc`).

## Compile and run Sirius with Python

Run from this repository's root directory (requires Python 3.10+ and a C11 compiler):

```bash
python bootstrap_compiler/sirius.py compile tests/fixtures/04_recursion.sirius -o /tmp/factorial.c
cc -std=c11 -I include /tmp/factorial.c include/sirius_runtime.c -lm -o /tmp/factorial
/tmp/factorial
```

On Windows, use your available C compiler (Zig's `zig cc` is an option) rather than assuming `cc` is on `PATH`. The bundled lexer is vendored and does not require pip installation.

Run all compiler tests, including proposed file I/O, compiler errors, strict C warning checks, and the expected runtime panic:

```bash
python tools/test_compiler.py --command "python bootstrap_compiler/sirius.py" --provisional --negatives --runtime-errors --strict-c
```

### Implemented subset

- Tab-indented declarations, literals, operators, functions, branches, loops, and `recur`.
- Static i64/f64/bool/string typing, inferred returns, basic generic monomorphization, function pointers and noncapturing lambdas.
- Compile-time module-level `let` integer constants (`i64`), including references to earlier constants and `+`, `-`, `*`; no runtime storage. Local bindings may shadow constants. Union aliases remain supported.
- Structural record interning with lexicographically sorted fields, record spreads/deletions, immutable constructor publication and conditional overrides.
- Rank-1 `Array[T]` (equivalent to `Array[T,1]`) of scalar, string, record, union and nested-array elements; array literals, empty typed construction, `append`, `len`, indexed reads/updates, and sequential `for` iteration.
- `Dict[K,V]` with `i64`/`string` keys and arbitrary supported value types; `insert`, `get` and `len`. Managed fields retain/drop correctly in CoW copies.
- UTF-8 immutable strings with byte length, `byte_at(i) -> Optional[i64]` (unsigned byte, `None` out of bounds), slices, searching, prefix/suffix tests, lexical comparison, concatenation, `str(i64)`, and joining arrays of strings.
- `arguments()` exposes command-line arguments as `Array[string,1]`; provisional `read_text` / `write_text` provide basic compiler I/O.
- Local CoW update reuse (`a = a.append(...)`, `d = d.insert(...)`) transfers ownership directly, allowing amortized growth when the binding is unshared.
- Optional fallback and `else return`, constructor-based structural tagged unions and basic exhaustive matching.
- Explicit cleanup of managed locals/temporaries (arrays, records, strings, optionals and unions) on returns and control-flow exits.
- Ordered evaluation of expressions and short-circuit conditions; checked source diagnostics for the negative tests.
- Simple `print`, `sqrt`, `len`, `str`, `arguments`, and provisional `read_text` / `write_text` runtime calls.

### Not implemented yet / known limitations

- The IR design documents a typed HIR and CFG MIR; **the working compiler currently uses syntax AST -> specialization/type-check + direct C lowering**. The revised roadmap puts the durable HIR/MIR work **after the minimal Sirius compiler becomes self-hosting**, rather than implementing these stages twice.
- Multidimensional numeric operations, arbitrary-key dictionaries (beyond `i64` and `string`), general operator overloading, dynamic `Any`, modules, richer pattern matching, and parallel operations are missing.
- Indexing without `?` is rejected unless/provisionally until a static bounds-proven lowering is implemented.
- `map` itself is not implemented. A small static call scan emits the intended purity error for the known impure callback fixture, but **transitive general-purpose effect checking is not implemented**.
- Structurally identical union alternatives are intentionally rejected; constructor names alone do not create distinct variants.
- Constructors support direct publication and overrides of previously published fields. Other complex conditional publication flows are not yet supported.
- `recur` for managed parameters still requires an ownership-aware implementation. Complex nested aliasing/ownership and all platform-specific C backends need additional checks.
- Integer overflow, narrowing conversions, platform portability, and robust diagnostics need explicit policy and further tests.

## Future improvements (medium priority)

- **Python-style negative array indexing:** `a[-1]` addresses the last element; in general, a negative index `i` addresses element `len(a) + i`. The same normalization applies to `a[i?]`, `a[i?]!`, and mutable indexed assignment. For indices still out of range, optional indexing yields `None` and checked indexing panics. Plain `a[i]` continues to require a static bounds proof.
- `tests/deferred/25_negative_indexing.sirius` records the intended behavior and expected output; the working compiler/runtime do **not** support negative array indices yet.

## Compiler-authoring API additions

```python
var tokens = Array[string]()  # equivalent to Array[string,1]()
tokens = tokens.append("def")  # CoW; reuses uniquely owned storage
let token = tokens[0?]!        # checked access

var counts = Dict[string,i64]()
counts = counts.insert("def", 1)
let n = counts.get("def") or 0

for token in tokens            # sequential; immutable loop binding
    print(token)

let data = "hello".slice(0, 4)  # byte-oriented UTF-8 slice
let byte = data.byte_at(0) or -1  # Optional[i64], no character decoding
let location = data.find("ell") or -1
let text = ", ".join(tokens)   # join Array[string]
let paths = arguments()          # program name plus CLI parameters
```

`str` currently formats `i64`; dictionary keys are `i64` or `string`. Explicit constructor-name annotations such as `Array[Token,1]` and `Dict[string,Token]` resolve to the constructor's canonical structural record type. Generic managed optionals are supported. Arrays other than rank 1 and Python-style negative indices remain deferred.

For assignments of the form `a = a.append(v)` and `d = d.insert(k, v)`, the old binding's ownership is *consumed* by the update. If references are shared the runtime copies; otherwise the allocation is reused. This optimization preserves Sirius immutable-value semantics. The Python bootstrap still uses conservative retain/release elsewhere; it is not a replacement for future ownership MIR.

## Established bootstrap semantics and remaining assumptions

1. `0..n` is half-open, and the `for` variable is immutable (confirmed).
2. Integer literals without a contextual type default to `i64`; floating literals default to `f64` (confirmed). All example arithmetic stays inside representable bounds. Signed overflow, division, and conversion behavior remain unspecified by these fixtures.
3. `print` appends a newline, booleans print as `true`/`false`, and `Optional or fallback` evaluates the fallback only on absence. Prefer Python-like standard-library behavior unless it conflicts with Sirius semantics.
4. Plain records are laid out in canonical, lexicographically sorted field-name order (confirmed). Physical representation is implementation-specific; equivalent records must have the same canonical type.
5. `Dict[K,V]`, `.insert`, `.get`, constructor-based `let Result = Success | Failure`, and `match` are accepted bootstrap syntax/APIs. `read_text` and `write_text` remain provisional.
6. Read-only array parameters can be represented as borrowed C pointers; this is an implementation detail that must not alter Sirius value semantics.
7. Records with managed fields have compiler-generated retain/drop/copy helpers; trivially copyable records may copy directly.
8. Checked indexed assignment `a[i?]! = value` panics if missing; optional indexing `a[i?]` does not (confirmed).

## Union typing assumption

A union of constructors refers to their **structural result record types**. `let Result = Success | Failure` is a compile-time type-descriptor alias, not a runtime call or bitwise operation; its evaluation must be resolved during type checking. Constructor identity is not stored in ordinary records. Two constructors with exactly the same record shape therefore cannot become distinguishable alternatives in this initial union model. The accepted fixture uses `{value: i64}` versus `{code: i64}`. An alternative design with nominal variant identities would need an explicit semantic decision.

## Development roadmap

This checklist separates **implemented bootstrap capabilities** from **features needed to write the compiler in Sirius** and from the larger language goals. Checkmarks refer to demonstrated test coverage, not full language-spec completeness.

### 0. Test-first Python bootstrap

- [x] Golden `.sirius` fixtures, independent C11 references and stdout expectations (24 independent C references; 14 additional stage-0 fixtures).
- [x] Python SLY lexer, indentation-aware parser and syntax AST.
- [x] Basic static type checking, concrete function specialization and C11 emission.
- [x] Structural records with deterministic field order, constructors, unions, basic matching, function values.
- [x] Scalar expressions, sequential control flow, recursion/`recur`, managed strings, rank-1 typed arrays, typed dictionaries, basic `Optional`.
- [x] Reference/runtime test runners, negative diagnostics and checked-index panic test.
- [ ] Tighten ownership correctness for managed `recur` parameters and more complex loops; extend tests.
- [ ] Improve error reporting and verify portability with Zig C compiler and Windows toolchains.

### 1. Make the subset practical for compiler authoring (Python stage-0)

- [x] Generic `Array[T]` / `Array[T,1]` supporting strings, records, unions and compiler data structures; allocation, length, append, checked/optional indexed access/update, and sequential iteration.
- [x] Generic `Dict[K, V]` with `string`/`i64` keys and compiler data types as values, including CoW updates.
- [x] UTF-8 string/byte inspection (`byte_at`), comparison, slices, search, joining and text construction using arrays of strings (sufficient for an initial lexer/emitter).
- [x] File reading/writing, command-line arguments, `print` diagnostics (API remains provisional; richer error propagation is deferred).
- [x] Test a small compiler-shaped workload (token record array, symbol dictionary, assembled C text), plus nested CoW and managed unions.
- [x] Introduce an indexed AST arena and syntax diagnostics in Sirius; cross-file imports/compilation remain open.
- [x] Implement module-scope integer `let` constants in Python stage-0 and use named integer token/AST kinds in the Sirius tokenizer and parser.
- [ ] Freeze the minimal bootstrap subset once the Sirius compiler compiles its own replacement.

### 2. Rewrite compiler in Sirius (start as soon as phase 1 basics work)

- [x] Write and independently test a self-contained Sirius tokenizer, including indentation, bracket continuations and lexical diagnostics.
- [x] Write the Sirius indentation-aware parser using token indices and an AST arena; validate all stage-0 positive syntax fixtures.
- [x] Add stage-1 AST node/edge definitions and stable structural dump tests.
- [x] Begin stage-1 semantic resolution and C11 emission: scalar types, calls, module constants, branches, loops and tail recursion (limited coverage; see below).
- [x] Add canonical POD-record interning, field access and updates, spread/delete/override, nested record layouts and basic `pub` constructors to the Sirius emitter.
- [x] Add call-driven generic parameter specialization, inferred evaluator return types, a concrete signature cache, and specialization of POD-record functions.
- [x] Begin ownership-aware lowering: managed strings, `Array[i64]`, scope/return cleanup, checked reads/CoW writes, iteration, and append with unique-owner reuse.
- [x] Add `Array[string]` and arrays of POD records using generic runtime callbacks; support `Array[Token]()` (after its constructor declaration), checked reads/writes, iteration, append, CoW and return cleanup.
- [x] Add compiler-generated retain/drop/copy for managed record fields, nested records and arrays of managed records, with CoW and aliasing regressions.
- [x] Add nested arrays and initial typed dictionaries with CoW insert/length and integer optional lookup.
- [x] Extend `Dict.get` to supported value types, with owned typed `Optional` results and branch-lowered lazy `or` fallbacks.
- [x] Implement managed direct-`pub` constructors and element mutation through mutable record fields; test CoW and ownership.
- [x] Resolve constructor-defined parameter types, evaluate effectful binary/named-call operands left to right, and clean managed locals on tested loop exits.
- [ ] Extend these semantics to all expression forms and control-flow exits.
- [ ] Implement only the Sirius features needed by this compiler first; continue to use Python solely as the stage-0 bootstrap.
- [ ] Expand stage-0/stage-1 equivalence beyond the thirteen shared positive programs; the 54 stage-1-only positive programs and 29 diagnostic tests establish focused coverage, not full semantic equivalence.
- [ ] Use stage-1 to compile its own source (stage-2); check that stage-2 compiles the same suite successfully.
- [x] Check in verified generated C bootstrap sources (`bootstrap_c/compiler.c`) and retain the C runtime.
- [ ] Freeze the Python bootstrap after portability and broader equivalence checks.

### 3. Improve the self-hosted compiler architecture (in Sirius, not twice)

- [ ] Introduce typed/specialized HIR: `TypeId`, resolved names/fields/calls and monomorphization.
- [ ] Lower to explicit CFG MIR with blocks, temporaries, branches, returns and `recur` backedges.
- [ ] Encode ownership uses, moves, borrows, retains, releases and CoW updates in MIR; insert cleanup on every control-flow exit.
- [ ] Move C emission into a backend that only consumes typed/lowered IR.
- [ ] Build robust constructor initialization and general transitive purity/effect checks.
- [ ] Add regressions for branch-dependent lifetimes, nested CoW and failure paths.

### 4. Expand toward the language design

- [ ] Full multidimensional homogeneous arrays, broadcasting, numerical primitives and general operator resolution.
- [ ] Modules/imports, canonical descriptors, richer generics, standard-library collections and diagnostics.
- [ ] `map`, `filter`, `generate` with genuinely enforced purity; later fusion/SIMD/parallelization.
- [ ] Dynamic `Any`, deeper static bounds analysis and advanced FBIP/ARC improvements.
- [ ] Deferred medium-priority negative array indexing (`tests/deferred/25_negative_indexing.sirius`).

**Strategy:** Do not first port the Python compiler's mixed analysis/emission code wholesale, and do not implement a full typed HIR + CFG MIR in both languages. Extend Python just enough to compile a *small new compiler written in Sirius*, reach self-hosting, and then develop the durable IR architecture in Sirius. The present `target_ir.py` is a design reference rather than mandatory Python implementation work.

## Verify self-hosting

Run from the project root:

```bash
python tools/test_selfhost.py
```

This constructs one source file from `src/tokenizer.sirius`, `src/parser.sirius`, `src/emit_c.sirius`, and `tests/emitter/driver.sirius` (module imports are still missing). It uses Python stage-0 **only to seed stage-1**, then checks:

1. Stage-1 generates stage-2 C and builds a native stage-2 compiler.
2. Stage-2 regenerates stage-3 C **byte-for-byte identical** to stage-2 C.
3. This fixed-point C also matches the checked-in `bootstrap_c/compiler.c`.
4. The native stage-3 compiler builds and runs 16 representative Sirius programs.

To build the checked-in self-hosted compiler **without Python**:

```bash
cc -std=c11 -I include bootstrap_c/compiler.c include/sirius_runtime.c -lm -o sirius
./sirius tests/fixtures/01_scalars.sirius generated.c
```

Replace `cc` with `gcc` if necessary; on Windows, use an appropriate `.exe` output name. This is the minimal self-hosting milestone, not proof of full language conformance or ownership safety for every program. Continue running the tokenizer, parser, emitter, stage-0 and sanitizer tests while extending the compiler.

## Stage-1 tokenizer

Run `python tools/test_tokenizer.py`. It concatenates `src/tokenizer.sirius` and `tests/tokenizer/driver.sirius` in a temporary file because module imports are not yet supported, then compiles that file using the Python bootstrap and checks the token streams against `tests/tokenizer/expected_output/*.txt`. It does not depend on SLY at runtime.

`tokenize(source: string) -> Array[Token]` emits half-open UTF-8 byte spans (`start`, `end`), 1-based line numbers, and 1-based **byte** columns. Token kinds are named by module constants (`TOKEN_*`) at the top of `tokenizer.sirius`; negative kinds indicate a fatal lexical error and terminate the stream. Keywords remain ordinary `NAME` tokens for the future parser. NEWLINE/INDENT/DEDENT are explicit; blank/comment-only lines and bracketed continuations do not affect indentation. A final NEWLINE is synthesized when necessary.

Bootstrap lexer scope: ASCII identifiers; UTF-8 allowed inside quoted strings and comments; decimal/scientific literals; quoted strings with escapes (but no multiline/triple-quoted strings yet); common punctuation and operators. Grammar validation and semantic diagnostics belong to the parser. `byte_at` and `slice` are sufficient; neither regex nor additional runtime features were needed. Numeric character tests are annotated with ASCII spellings; parenthesized expressions (including conditions and argument lists) may span lines and contain inline comments. This is covered by `tests/fixtures/37_multiline_parentheses.sirius` and `tests/tokenizer/fixtures/19_multiline_condition.sirius`.

## Stage-1 parser

Run `python tools/test_parser.py` from the project root. The runner concatenates `src/tokenizer.sirius`, `src/parser.sirius` and its test driver (imports are still unavailable), builds with the Python stage-0 compiler and C11, then checks AST golden files and syntax-error diagnostics. It also smoke-tests parsing of the existing positive language fixtures **and the tokenizer/parser's own Sirius sources**. This is syntax coverage only, not self-hosting or semantic conformance.

`parse_module(source)` returns `ParseResult(state, node)`. `state.nodes` is an `Array[Node]` with `i64` kind IDs declared as module constants (`NODE_*`); `state.edges` is a flat array of child node IDs. Every `Node` stores `kind`, a token index, and a half-open span in the edge array (`first`, `count`). Names, literal text and source locations remain in the tokenizer's spans. Parse errors are returned as `state.error` and `state.error_line`. This avoids recursive managed record types and creates a stable representation for the upcoming name/type resolver and C emitter.

The stage-1 parser covers functions, typed/default arguments, constructors/`pub`, blocks, `if/elif/else`, `while`, `for`, `match`, `let`/`var`, assignments, `return`/`recur`, record and array literals, unary/binary precedence, calls, fields, optional/checked indexing and lambda syntax. It does **not** yet perform semantic type checking, desugar constructors, resolve overloads, lower ownership, or emit C. Module/import syntax, advanced pattern matching and richer diagnostics remain to be implemented.

## Stage-1 gap audit and next implementation steps

The Python stage-0 already compiles the compiler's own Sirius sources. The gap is in stage-1 semantics and lowering, **not** stage-0 language support. The stage-1 lexer and AST parser are reusable; `emit_c.sirius` still mixes resolution and direct C emission and accepts only a restricted executable subset.

- [x] Name resolution, typed scalar function calls, local binding checks, control flow, module integer constants, and direct self-tail recursion.
- [x] Canonical structural POD-record descriptors, by-value C structs, field reads/writes, spread/delete/override, nested records, and basic scalar `pub` constructors.
- [x] Infer omitted evaluator parameter/return types at calls; specialize and cache by concrete argument signature, including structurally equivalent POD records.
- [x] Emit reference-counted `S_String *` expressions with owned expression results and retain/release at bindings, assignments, function calls and returns; support concatenation, comparison, `len`, `str(i64)` and printing.
- [x] Specialize managed rank-1 `Array[i64]`, including literals, annotations, checked indexing, CoW assignment, append, `len`, sequential iteration and function parameters/returns. Fast-path `a = a.append(item)` transfers an exclusively owned array to the append runtime.
- [x] Add executable managed-value regressions and retain the existing tokenizer/parser/reference C tests.
- [x] Add `Array[string]` and `Array[T]` for POD-record `T`, including `Array[Token]()` once `Token` is declared, with generic C retain/drop callbacks and CoW updates.
- [x] Support managed-field record literals, field access/updates, nested record copy/drop and arrays of managed records.
- [x] Support nested arrays of the existing scalar and record element types, with per-element retain/drop and CoW.
- [x] Add typed `Dict[K,V]` with `i64`/`string` keys, CoW `insert`, `len`, and owned `Optional[V]` results from `get` for supported values.
- [x] Lower lazy `Optional or fallback` into explicit C branches; retain/drop managed optional payloads on tested bindings and exits.
- [x] Add byte-oriented string operations (`byte_at`, `slice`, `find`, `starts_with`, `ends_with`, `join`), compile-time byte-accurate C string literals, file I/O and `arguments()` using the existing runtime.
- [ ] Add nested dictionary types, optional indexing/early-return operations, and union arrays, following the existing stage-0 API.
- [ ] Support constructor overrides/branching, tagged unions, optional indexing, `else return`, methods, and non-capturing function values beyond the self-hosting subset.
- [ ] Generalize ordered lowering to array/record initializers and string methods; complete cleanup for managed `recur`, unusual aliasing and all control-flow paths.
- [x] Compile concatenated Sirius sources through stage-1/stage-2, assert byte-identical stage-2/stage-3 C and retain generated bootstrap C.
- [ ] Broaden diagnostic equivalence, sanitizer coverage and stage-3 fixture tests.
- [ ] After self-hosting, split semantic analysis from C emission into typed HIR and ownership-aware CFG MIR.

**Current limitations:** Managed strings, rank-1 scalar/record arrays, and records containing strings, arrays or nested records work in the tested ownership paths. Constructor bodies must contain direct `pub` statements without overrides or branches; managed fields are supported, but untyped constructor parameters remain unsupported. Generic functions are monomorphized when called, and unannotated recursive generic returns need explicit return types. Complete return-path checking, general left-to-right initializer evaluation and full constructor publication are still missing. These restrictions are intentional diagnostics, not claims of full language compatibility.

## Stage-1 C emitter (in progress)

Run `python tools/test_emitter.py` from the repository root. The runner combines `src/tokenizer.sirius`, `src/parser.sirius`, `src/emit_c.sirius` and `tests/emitter/driver.sirius`, compiles the combined source with Python stage-0, builds that output with C11, then compiles and executes the **C generated by the Sirius emitter**. This is separate from the tokenizer/parser AST tests and the Python compiler's conformance suite.

The current `compile_c(source)` uses the indexed AST directly. It pre-collects function declarations, resolves scalar local bindings and call targets, checks basic types and mutability, resolves module `i64` constants, and emits C11 for explicitly typed or call-site-specialized scalar functions (`i64`, `f64`, `bool`, and limited `string`), `print`, `if`/`elif`/`else`, `while`, half-open range `for`, `break`/`continue`, ordinary recursive calls and direct self-`recur`. `recur` arguments are evaluated into distinct C temporaries before updating parameters and jumping back to the function entry, preserving simultaneous replacement. The `main() -> i64` entry point is wrapped with a conforming C `main`. It additionally interns structural record types using lexicographically sorted fields, emits deterministic C structs and record-by-value operations, supports scalar/nested-POD record literals, direct field updates, record spread/delete/override, and direct `pub` constructors with typed parameters (including `sqrt(f64)`). Managed record fields now use generated recursive copy/drop and taking field-access helpers. Arrays of managed records use those callbacks for CoW, indexing, iteration and release. Direct-`pub` constructors support managed fields with sequential initialization; advanced publication/control flow is still rejected. Untyped evaluator parameters specialize by concrete argument types; return types can be inferred during each specialization. Functions are cached per concrete signature, with in-progress tracking to avoid unbounded recursive specialization. Managed strings and rank-1 `Array[i64]`/`Array[string]` and POD-record arrays are lowered using the C runtime, including retain/release on bindings, calls, returns and normal scope exit. Array updates use CoW; checked indexing panics on invalid positions, and `a = a.append(item)` can consume unique array storage. Nested arrays use the existing generic runtime and element retain/drop callbacks. `Dict[K,V]` uses the generic hash table runtime with typed copy/drop, supports `i64` or `string` keys, `insert`, `len`, and owned `Optional[V]` lookup for supported value types; self-`insert` consumes the old uniquely owned dictionary. String methods now lower using ownership-consuming wrappers; `read_text(path)` returns `Optional[string]`, `write_text(path, text)` returns `bool`, and `arguments()` returns `Array[string]`. The C entrypoint forwards its `argc`/`argv` to the runtime. String literals are emitted with explicit byte length so embedded NULs are preserved. Only basic `main() -> i64` executables are emitted.

The runner checks **67 generated-C programs** (13 shared stage-0 language fixtures and 54 stage-1-specific emitter fixtures) and **29 expected diagnostic failures**, with strict C11 compilation (`-Wall -Wextra -Werror -pedantic`). Coverage includes generic returns, managed strings, record field aliasing and replacement, nested arrays, dictionary CoW, array CoW/iteration and ownership transfer. The generated programs link with `include/sirius_runtime.c`. These passing tests do not establish complete ownership or language-spec conformance.

**Important limitations:** Self-hosting is verified for this compiler subset, but this is still a combined semantic checker and direct C emitter, **not** typed HIR/MIR. It rejects unsupported constructs rather than inventing a lowering. Constructor branching/overrides, tagged unions, optional array indexing, `else return`, managed `recur`, and full static effect checking remain unfinished. Ordered C temporaries are generated for multiple effectful binary/named-call operands, but not all nested array/record/string-method expressions. Managed loop exits and sequential constructor field initialization are supported in tested cases; complete cleanup and shadowed-local binding correctness need broader validation. Complex `elif` conditions containing optional fallbacks remain rejected pending CFG lowering. Integer overflow checks and full language conformance remain future work.

## Module integer constants (bootstrap subset)

Module-level `let` names may hold an `i64` compile-time constant (optionally annotated `: i64`).
Supported initializers are integer literals, unary `+`/`-`, `+`/`-`/`*` expressions, and references to **earlier** integer constants.
The bootstrap substitutes constant values directly during C generation; there is no global C variable or runtime initializer.
Module `var`, forward references, non-constant initializers, overflow, and duplicate module names are rejected.
Function-local declarations may shadow module constants, but assigning directly to a module constant is forbidden.
Constructor-based tagged-union aliases (`let Result = Success | Failure`) retain their separate compile-time interpretation.
Module-level record constants, function calls, general `comptime`, and module imports are not supported yet.

The regression suite now includes `tests/fixtures/38_module_constants.sirius` and six additional module-constant diagnostics.
