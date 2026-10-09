# Sirius bootstrap compiler — executable language examples

This repository now contains an **executable Python-to-C11 bootstrap compiler** as well as the original test-first specification. It uses the SLY lexer and dataclass syntax AST, specializes functions on concrete types, checks a bootstrap subset of static semantics, and generates ownership-aware C. The compiler is an experimental subset implementation, **not yet a self-hosting compiler** or a complete implementation of the language design.

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
- `src/parser.sirius` — Sirius-written recursive-descent/precedence parser with an indexed AST arena and syntax diagnostics (no semantic analysis yet).
- `tests/tokenizer/{fixtures,expected_output}/`, `tests/tokenizer/driver.sirius`, `tools/test_tokenizer.py` — tokenizer golden tests.
- `tests/parser/{fixtures,expected_output}/`, `tests/parser/driver.sirius`, `tools/test_parser.py` — parser AST/diagnostic golden tests and syntax smoke tests.
- `LANGUAGE_DESIGN.md` — current Sirius language design and agreed clarifications.
- `target_ir.py`, `IR_DESIGN.md` — proposed **future** typed HIR and CFG MIR representation (not yet integrated).
- `tools/verify_c.py` — C11 compilation and execution of all reference C fixtures.
- `tools/test_compiler.py` — executable end-to-end compiler conformance suite, with optional strict C warnings, negative diagnostics, and runtime panic tests.

## Positive tests

The first 24 compiler fixtures have independent C reference programs. Additional compiler-only feature fixtures (`tests/fixtures/25_*.sirius` through `37_*.sirius`) cover string, record, nested and union arrays; dictionaries with managed values; collection iteration; CLI arguments; a miniature token/symbol-table/C-text workflow; the rank-1 array shorthand; and UTF-8 byte inspection.

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

This invokes `cc` with C11 and strict warnings, runs each program in a temporary directory, and checks stdout and one expected runtime panic. It exercises the reference C, **not** the future Sirius compiler.

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

- [x] Golden `.sirius` fixtures, independent C11 references and stdout expectations (24 independent C references; 13 additional compiler-authored fixtures).
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
- [ ] Freeze the minimal bootstrap subset once the Sirius compiler compiles its own replacement.

### 2. Rewrite compiler in Sirius (start as soon as phase 1 basics work)

- [x] Write and independently test a self-contained Sirius tokenizer, including indentation, bracket continuations and lexical diagnostics.
- [x] Write the Sirius indentation-aware parser using token indices and an AST arena; validate all stage-0 positive syntax fixtures.
- [x] Add stage-1 AST node/edge definitions and stable structural dump tests.
- [ ] Resolve names, types and call targets against the AST, then implement a small stage-1 C emitter.
- [ ] Implement only the Sirius features needed by this compiler first; continue to use Python solely as the stage-0 bootstrap.
- [ ] Compare stage-0 Python and stage-1 Sirius compilers on the shared golden/negative tests.
- [ ] Use stage-1 to compile its own source (stage-2); check that stage-2 compiles the same suite successfully.
- [ ] Commit verified generated C bootstrap code and runtime; freeze Python bootstrap thereafter.

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

## Stage-1 tokenizer

Run `python tools/test_tokenizer.py`. It concatenates `src/tokenizer.sirius` and `tests/tokenizer/driver.sirius` in a temporary file because module imports are not yet supported, then compiles that file using the Python bootstrap and checks the token streams against `tests/tokenizer/expected_output/*.txt`. It does not depend on SLY at runtime.

`tokenize(source: string) -> Array[Token]` emits half-open UTF-8 byte spans (`start`, `end`), 1-based line numbers, and 1-based **byte** columns. Token kinds are documented at the top of `tokenizer.sirius`; negative kinds indicate a fatal lexical error and terminate the stream. Keywords remain ordinary `NAME` tokens for the future parser. NEWLINE/INDENT/DEDENT are explicit; blank/comment-only lines and bracketed continuations do not affect indentation. A final NEWLINE is synthesized when necessary.

Bootstrap lexer scope: ASCII identifiers; UTF-8 allowed inside quoted strings and comments; decimal/scientific literals; quoted strings with escapes (but no multiline/triple-quoted strings yet); common punctuation and operators. Grammar validation and semantic diagnostics belong to the parser. `byte_at` and `slice` are sufficient; neither regex nor additional runtime features were needed. Numeric character tests are annotated with ASCII spellings; parenthesized expressions (including conditions and argument lists) may span lines and contain inline comments. This is covered by `tests/fixtures/37_multiline_parentheses.sirius` and `tests/tokenizer/fixtures/19_multiline_condition.sirius`.

## Stage-1 parser

Run `python tools/test_parser.py` from the project root. The runner concatenates `src/tokenizer.sirius`, `src/parser.sirius` and its test driver (imports are still unavailable), builds with the Python stage-0 compiler and C11, then checks AST golden files and syntax-error diagnostics. It also smoke-tests parsing of the 37 existing positive language fixtures **and the tokenizer/parser's own Sirius sources**. This is syntax coverage only, not self-hosting or semantic conformance.

`parse_module(source)` returns `ParseResult(state, node)`. `state.nodes` is an `Array[Node]`; `state.edges` is a flat array of child node IDs. Every `Node` stores `kind`, a token index, and a half-open span in the edge array (`first`, `count`). Names, literal text and source locations remain in the tokenizer's spans. Parse errors are returned as `state.error` and `state.error_line`. This avoids recursive managed record types and creates a stable representation for the upcoming name/type resolver and C emitter.

The stage-1 parser covers functions, typed/default arguments, constructors/`pub`, blocks, `if/elif/else`, `while`, `for`, `match`, `let`/`var`, assignments, `return`/`recur`, record and array literals, unary/binary precedence, calls, fields, optional/checked indexing and lambda syntax. It does **not** yet perform semantic type checking, desugar constructors, resolve overloads, lower ownership, or emit C. Module/import syntax, advanced pattern matching and richer diagnostics remain to be implemented.
