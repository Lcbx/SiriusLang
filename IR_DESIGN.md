# Sirius compiler IR — target architecture

**Status:** the active, self-hosted Sirius compiler (`src/`) currently performs most semantic checks alongside direct C11 generation, with an initial typed HIR arena in `src/hir.sirius` for selected expression classes; an independent but partial function-body HIR analysis pass handles known scalar, record and collection expressions; no complete typed-function HIR exists; an isolated scalar/string/record HIR-to-MIR lowering subset is now implemented. A typed CFG MIR, HIR-to-MIR lowering subset and interpreter are implemented in Sirius and exercised with supported source-level functions, and an experimental `--mir-c` path now emits C11 from that same MIR; default compilation still uses the legacy direct emitter. `src/types.sirius` now provides canonical semantic interning for primitive, record, array, dictionary and optional types; the emitter still consumes legacy integer kind encodings via a compatibility bridge. The Python bootstrap (`archive/python_bootstrap/`) is frozen historical source. Both the legacy and MIR-only native compilers reach reproducible C fixed points. The MIR backend executes all 85 shared positive compiler fixtures; MIR is not yet the default. This document distinguishes the working subset from architectural goals. See [ROADMAP.md](ROADMAP.md), phases B–D, for incremental migration and ownership testing.

The intended pipeline is **tokens → syntax AST → typed/specialized HIR → ownership-aware CFG MIR → (interpreter | C11)**. The incremental bridge already stores `HirNode(source, operation, type_id, field_index)`, where `source` refers back to the syntax AST. Literal, name, field, unary/binary and call-signature resolution is C-independent; HIR now validates builtin and user-function calls, string methods, array/dictionary methods, and record field composition (spread, delete, replace) and constructor publication using canonical descriptors. The surrounding emitter still constructs and consumes these nodes while generating C, and still triggers specialized body generation, expression traversal, record initializer evaluation order, and ownership. HIR owns specialization cache keys and lifecycle. A pre-emission HIR declaration pass now indexes module functions and verifies declaration names, parameter syntax/uniqueness and deprecated `def` constructors before C output begins. The body pass now uses that declaration index to resolve concrete calls (including forward references), check annotated arguments, infer nonrecursive generic return types through nested calls, and type constructor-produced records. Unresolved/recursive cases remain deferred. Specialization keying and in-progress tracking now belong to HIR (`HirSpecializationRegistry` keyed by function-body AST identity plus canonical argument TypeIds); the emitter still initiates specializations and owns the resulting C bodies; the body pass is not yet a complete specialized HIR. The current type table is a semantic identity registry (dense `TypeId` indices, descriptor tags and child IDs); runtime size/alignment/field offsets are not yet stored in the semantic descriptors. The C backend now derives record field order and type-specific copy/drop operations from those descriptors, through a temporary legacy-kind naming bridge. `experiments/target_ir.py` contains illustrative Python dataclasses only. The current Sirius AST is an indexed arena (`Array[Node]` plus child edges), not the original Python syntax AST.

### Incremental source-to-MIR migration

The HIR worklist now resolves named `rec` return/parameter annotations by canonical published fields, including nested `Array`/`Optional`/`Dict` annotations. Constructor calls are lowered directly into `MIR_RECORD_NEW` in published-field evaluation order, with canonical physical ordering. Top-level integer constants are resolved independently of the C emitter. The lowering additionally supports empty typed arrays, array append (`MIR_ARRAY_APPEND`, consuming owner), array/range `for`, loop control, expression statements, and CLI/file/string conversion intrinsics. Native MIR-C initializes the arguments runtime and converts CLI strings into MIR-owned arrays; MIR interpreter file reads are deferred. MIR-only native self-hosting reaches a fixed point and all 85 executable shared fixtures pass. Two historical negative fixtures are accepted by MIR as valid programs (floating-point arrays and `elif` optional fallback).

### Concrete specialization worklist (implemented subset)

`src/hir_specialize.sirius` types each reachable concrete function once, using a HIR-owned queue keyed by `(declaration body ID, canonical argument TypeIds)`. A request is registered before callee analysis; calls to `IN_PROGRESS` entries resolve to the same specialization ID (including mutual recursion). Every `HirSpecializedFunction` records its typed nodes, resolved call targets and return type. HIR specialization analysis does not depend on either backend. Nonrecursive unannotated generic return types are inferred by analyzing the concrete callee body. Recursive unannotated returns remain rejected; richer inference and effects analysis remain future work.

`src/mir_program.sirius` consumes that worklist and lowers its supported scalar/string/record subset. The `MirProgram.functions` index is the specialization ID; `MIR_CALL` uses the same index. MIR native C supports `i64`, `bool`, `f64` (including arithmetic, comparisons and `sqrt`), owned strings, structural records, rank-1 arrays, dictionaries, optionals, record spread/delete, and tail `recur`. Deep nested projection assignments remain unsupported. The MIR interpreter does not yet implement the new floating-point instructions or file I/O. The default C emitter still drives its own instantiations. The opt-in `src/mir_emit_c.sirius` instead consumes verified linked MIR, including its worklist-owned specialization indices, and uses no emitter-local specialization cache.

## Syntax AST (already implemented in both compilers)

Both parsers preserve indentation-sensitive blocks, declarations (`let`, `var`), constructors (`pub`), record operations, loops, `match`, indexing modes, and source positions. The Sirius parser uses source byte spans and flat node/edge arrays; future HIR must retain those spans for diagnostics. Expression precedence and short-circuit behavior are semantic obligations, not properties of C emission.

The executable HIR-to-MIR bridge maintains a sparse index from AST node IDs to
HIR annotation chains. This avoids repeatedly searching the annotation list and
preserves distinct operations on a node. Canonical `TypeTable` descriptors are
cached by stable structural keys while keeping insertion-order numeric `TypeId`s.
The MIR-C emitter now accumulates generated fragments before joining sections,
function bodies and the full translation unit. Normal source compilation uses
`mir_emit_c_verified` after CFG ownership, lifetime and balance validation.
This removes redundant register-read initialization checks and uses `NULL` as
an empty ownership state for managed C pointers instead of per-register flags.
Scalar registers need no runtime initialized-state tracking. Guarded arithmetic,
index and optional operations rely on their explicit MIR CFG panic edges.
The separate `mir_emit_c` entry point retains defensive initialization checks
for arbitrary hand-constructed MIR, including tests of invalid register reads.
Both paths retain reference counting and path-sensitive cleanup semantics.
These are compiler-internal representation changes, not changes to source-language
value semantics.

## Typed HIR

* Each expression has an interned `TypeId`, source `Span`, and resolved symbol/function identity.
* Canonical record types intern by lexicographically sorted field names/types, not constructor names. Different shapes use distinct `TypeId`s. Their layout is computed deterministically (alignment/offsets).
* Generic calls are specialized by `(definition SymbolId, concrete argument TypeIds)`; resolved function calls refer to the specialization. The return type and effect signature are attached to the specialization. Avoid recursively instantiating the same in-progress specialization.
* A constructor becomes an ordinary function producing an immutable record. Validate unconditional field publication, freeze published bindings, and statically resolve field layouts.
* Extension method calls become ordinary calls with the receiver inserted as the first argument.
* Operator overloads become statically resolved function calls (or known primitive operations).
* Indexing is explicitly `PROVEN`, `OPTIONAL`, or `PANIC` at the HIR level. The type of an `OPTIONAL` index is `Optional[element_type]`.
* Structural-field lookups are replaced with integer field indices. Dynamic `Any` is not part of the first bootstrap.
* Function values have a typed signature and effect guarantees. Noncapturing lambdas become named, generated functions.
* Purity checking uses a transitive call graph; panics are permitted in otherwise pure functions, whereas I/O and impure FFI are effects.

## Lowered MIR (CFG)

`src/mir.sirius` currently defines typed `MirProgram`, `MirFunction`, `MirBlock`, `MirInstruction` and `MirTerminator` records for scalar, string, record, array, dictionary and optional values. `mir_verify` validates instruction operands, result types, signatures and CFG targets; `src/mir_interpreter.sirius` executes them using typed register slots. `MIR_CALL` uses explicit argument-register indices, with limits for total steps and call depth. They now accept **checked scalar/string/record HIR lowering output**, including calls linked by `src/mir_program.sirius` from the HIR worklist, and are tested against `mir_execute`; the opt-in MIR C emitter consumes the same `MirProgram`, while legacy compilation still bypasses MIR. The implemented managed operations include string construction/concatenation/length, record construction (`MIR_RECORD_NEW`) and field extraction (`MIR_RECORD_GET`), plus `MIR_COPY`, `MIR_RETAIN`, `MIR_MOVE` and `MIR_RELEASE`. Every initialized string/record register owns a reference. Record construction and field reads retain nested managed values; C copies share immutable reference-counted records and releases recursively drop nested strings/records. Calls retain arguments and returns transfer ownership. The interpreter maintains a record heap with equivalent logical reference credits. Managed collections, direct record updates, and CFG lifetime/exit-cleanup passes now have experimental coverage; complete ownership proofs and borrows remain future work. See [INTERPRETER.md](INTERPRETER.md).

A function is `FunctionMIR(locals, arguments, blocks, entry)`.
Each basic block holds ordered instructions and one terminator (`Jump`, `Branch`, `Switch`, `ReturnTerm`, or `PanicTerm`).

* Expressions become typed temporaries, making evaluation order explicit before the C backend sees them.
* `if`, `elif`, `and`, `or`, `while`, `for`, and `Optional` fallback become CFG branches. In particular, **`and`/`or` are not eager arithmetic instructions**.
* `for i in a..b` lowers to integer bounds and a loop (confirmed half-open).
* `recur f(a, b)` evaluates **all** new arguments first and then jumps to the function's entry block. Direct self-tail recurrence never creates another C call frame.
* `let Result = Success | Failure` defines a compile-time descriptor alias for a union of constructor return types. Resolve `|` over constructor descriptors in a compile-time type expression, not as the ordinary bitwise operator over function values. `Success` and `Failure` are ordinary record constructors. In a union-expected context, an ordinary record result is implicitly injected into its union alternative based on its canonical structural type.
* Constructor patterns such as `Success(value)` resolve statically to their record payload shape, bind fields, and become `Switch` over a union discriminant. Check exhaustiveness. The current subset requires structurally distinct alternative record types; identical payload shapes would have identical type IDs and cannot be distinguished by origin alone. Nominal constructor tags remain a possible later extension.
* Union type descriptors intern by an order-independent canonical set of alternative type IDs, with deterministic tag numbering; constructor names are used for source-level pattern matching rather than as record type identities.
* Array access becomes a bounds proof/optional result/checked panic path, depending on syntax.
* **Deferred (medium priority):** signed negative array indices normalize relative to array length before the bounds check (`-1` is last element). This applies to reads and CoW writes; optional/checked/proven access retain their current semantics. See `tests/deferred/25_negative_indexing.sirius`.
* Record spread, deletions and overrides are resolved to a new record layout; preserve left-to-right **expression evaluation** even if a field is later overwritten.

## Implemented experimental array MIR

The opt-in MIR interpreter and C backend now support canonical rank-1 `Array[T]` values using `MIR_ARRAY_NEW`, `MIR_ARRAY_GET`, `MIR_ARRAY_LENGTH`, and `MIR_ARRAY_SET`. Element types may be scalars, strings, records or previously interned array types. Index reads and writes are checked (no negative-index normalization yet). `MIR_ARRAY_SET(dst, owner, index, value)` **consumes** the owned `owner` register, invalidating it if distinct from `dst`. The array is updated in place only when uniquely owned; shared arrays are cloned, retaining managed elements and releasing the replaced element. Copies retain the collection handle; recursive release follows canonical element descriptors. A separate CFG ownership/lifetime pass remains planned.

## Memory management in MIR

Value semantics have to be visible in the compiler, not reconstructed from textual C:

* `Retain` and `Release` track managed storage (implemented for MIR strings and records); the MIR C backend uses canonical record descriptors to generate recursive field drops. Arrays/variants still require implementation.
* A plain copy of an inline record copies scalars and retains managed fields. Records may therefore share nested arrays safely.
* `CoWSet(root, path, value)` resolves nested field/index updates and assures unique ownership of any modified buffer. Reuse uniquely owned allocation; otherwise copy the path (with reference management).
* Function inputs are *logically immutable*. For a nonescaping read-only parameter C may safely borrow, but retained/returned/stored values must have well-defined ownership credits.
* Function returns transfer ownership to the caller. All control-flow exits must release live owned temporaries; the ARC pass can insert necessary releases after CFG creation.
* In the first version, insert conservative retains and releases; ownership/escape/FBIP optimization can come later.

## Worked lowering examples

### `08_array_cow`

Sirius:

```sirius
var a = [10, 20, 30]
let original = a
a[!1] = 99
```

Illustrative MIR (notation rather than exact Python serialization):

```text
a        = array_make_i64(10, 20, 30)
original = alias(a)
retain(a)                    ; one ownership credit for original
index    = const_i64(1)
value    = const_i64(99)
cow_set(a, index[PANIC], value)
...                          ; read a and original separately
release(original)
release(a)
```

### `04_recursion`

```sirius
recur factorial(n - 1, acc * n)
```

```text
next_n   = sub_i64(n, 1)
next_acc = mul_i64(acc, n)
n        = next_n
acc      = next_acc
jump factorial_entry
```

### `16_tagged_union`

```sirius
def Success(value: i64)
    pub value

def Failure(code: i64)
    pub code

let Result = Success | Failure
```

`Success(7)` first produces a record `{value: i64}`. When passed to a parameter of type `Result`, HIR inserts `UnionInject(Result, SuccessTag, record)`; in C this is a tagged union with a record payload. A match checks the tag and extracts the corresponding record before binding its fields.

### `13_optional_early_exit`

```sirius
let x = values[?index] else return false
```

```text
optional = array_get_optional(values, index)
branch(optional.present, has_value, missing)
missing: return false
has_value: x = optional.value
```

## Rationale for separating lowering from parsing

The earlier GDScript-to-C++ transpiler combined parsing, inference and emission. Sirius now has separate tokenizer and parser implementations, but still mixes semantic checks and textual C emission in `src/emit_c.sirius`. The goal is not to rewrite parsing again: introduce the HIR/MIR stages between the already working indexed AST and a smaller C backend so ownership and evaluation order are explicit.


## Phase B migration status

HIR generic specialization lookup and lifecycle, symbol binding metadata and structural record-field lookup now use canonical `TypeId`s. The C backend still accepts legacy kind numbers for layout and generated helper naming. The module declaration HIR pass precedes emission and supplies function headers; pre-emission body traversal now resolves local lexical scopes, supported scalar and record/collection expressions, indexing modes and selected calls; complete typed bodies for every language feature remain future work; a typed CFG MIR subset is implemented.

## Implemented experimental dictionary MIR

`MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, and `MIR_DICT_LENGTH` operate on canonical `Dict[K,V]` descriptors. `K` is currently `i64` or `string`; `V` can be any MIR-supported type, including managed arrays/records/dictionaries. Storage is reference-counted and unoptimized (linear key scan, insertion order). `MIR_DICT_INSERT` consumes its owner, reuses a unique allocation, or clones a shared dictionary while retaining nested values. Existing keys are replaced without changing entry count. The verifier checks key/value/result types. `MIR_DICT_GET_OR` accepts an already evaluated fallback register, **not** an `Optional` result, and must not be used as a direct lowering of lazy source `or`. Source `Dict[K,V]()`, `.insert`, and `len(dict)` are supported experimentally; source `.get` now lowers through Optional and CFG control flow.

## Implemented experimental optional MIR

Canonical `Optional[T]` values use zero/one owned payload fields, including nested managed values. `MIR_OPTIONAL_NONE`, `MIR_OPTIONAL_SOME`, `MIR_OPTIONAL_HAS`, `MIR_OPTIONAL_UNWRAP`, `MIR_ARRAY_GET_OPTIONAL` and `MIR_DICT_GET_OPTIONAL` are verified and executed by both the Sirius MIR interpreter and the MIR-C backend. The interpreter maintains reference counts; C uses the existing managed-record runtime with descriptor-directed nested release.

HIR-to-MIR lowering now handles `a[?i]`, `dict.get(key)`, postfix `!`, `optional or fallback` and `let x = optional else return value`. The latter two produce actual CFG branches, so fallback expressions execute only for absent values. The older `MIR_DICT_GET_OR` remains explicitly eager and must not be used to lower source-level `or`. The implemented CFG ownership/lifetime passes follow this lowering, as described below. See `tools/test_mir_optional.py` and differential cases in `tools/test_mir_c.py`.

## Explicit runtime failure edges (experimental)

`src/mir_guard.sirius` splits checked `MIR_DIV_I64`, `MIR_REM_I64`, `MIR_ARRAY_GET`, `MIR_ARRAY_SET`, and `MIR_OPTIONAL_UNWRAP` into validation and continuation/panic blocks. Pure `MIR_NONZERO_I64`, `MIR_DIV_NO_OVERFLOW_I64`, `MIR_ARRAY_INDEX_VALID`, and existing `MIR_OPTIONAL_HAS` predicates preserve the original panic messages, including the `INT64_MIN / -1` overflow case. This pass runs **before** lifetime and exit cleanup; the latter release owned registers on the newly explicit failure edges. Guarded instructions are marked to make repeated lowering idempotent. Both backends retain defensive checks for unguarded manually constructed MIR. This is not general exception unwinding: allocator failure and other implicit aborts remain outside the CFG model.

## Experimental CFG lifetime insertion

`src/mir_lifetime.sirius` analyzes each MIR function using backward register liveness to a fixed point across CFG backedges. At a last use or dead definition of a managed register, it inserts `MIR_RELEASE`. Explicit consuming operations (`MIR_MOVE`, `MIR_ARRAY_SET`, `MIR_DICT_INSERT`) do not release consumed sources; copies and calls retain their existing ownership semantics. The pass is run after typed MIR construction and before final verification, and is idempotent. Forward definite-initialization (`src/mir_ownership.sirius`) runs before and after lifetime insertion. It intersects initialized registers at CFG joins and loop backedges, with explicit move/release invalidation, to reject invalid uses statically. Both passes share operand-read definitions.

A second pass (`src/mir_cleanup.sirius`) performs **path-sensitive ownership cleanup**. Using one snapshot of forward may-ownership and backward liveness, it inserts conditional releases for managed registers dead on each CFG edge, splitting conditional edges as needed. This avoids repeated whole-CFG analysis after every edge rewrite; the rewritten graph is reanalyzed once for exit cleanup and verified for balance. An additional *may-initialized* forward union analysis identifies values owned on **any** path to a return or explicit panic. Exit cleanup uses `MIR_RELEASE` for definitely initialized registers and `MIR_RELEASE_IF_INIT` for conditionally initialized ones, preserving the returned value. `mir_check_balance` rejects reachable exits that may retain any other managed register. The conditional release uses runtime initialization flags (C) or register validity (interpreter); its effect is tracked by both analyses. Cleanup is idempotent. Tests cover branches, joins, nested managed arrays, explicit conditional releases, native/interpreter agreement, and prior lifetime behavior. Covered division, checked indexing/update and unwrap failures now have explicit cleanup exits; allocator failures and other implicit aborts remain non-unwinding. Ownership-credit and full unwinding proofs remain future work.

Final balance verification propagates must-initialization (intersection) and may-ownership (union) together in one forward worklist, reusing reachability and predecessors and checking register validity before exit balance. Standalone callers can still request one analysis only. Final balance verification reuses the same CFG reachability, predecessor lists and definite-initialization states to check both register validity and managed-reference balance. Scalar-only functions skip the may-ownership solver. Public standalone checks keep the same diagnostics.

MIR lowering keeps source syntax, typed HIR annotations, canonical types, and call
resolution in a read-only `MirLowerContext`; `MirBuilder` carries only mutable
registers, CFG blocks, scopes, and loop targets. The forward ownership worklist
precomputes sparse register write effects for each basic block and applies one
state transfer per block, preserving sequential move/redefinition behavior.
Must/may joins update an owned dense mask rather than allocating a new one per
predecessor. Instruction-level validation still uses ordered reads/writes.

The combined compiler path uses unchecked *internal* guard, lifetime and cleanup transformations between an initial ownership verification and final ownership-balance verification. The final failure audit reuses that verification. Individual public MIR passes still validate their inputs and outputs; intermediate checks are not omitted from standalone tests.

### MIR fallibility audit (implemented)

`mir_failure_info(op)` is the shared instruction-level contract. `guarded` marks synchronous operations that must be lowered to explicit `Panic` edges; `abort` marks operations that may terminate outside the current function's CFG (notably allocation and callee failures). `MIR_ARRAY_SET` has both properties: bounds failure is guarded, allocation failure during CoW is not unwound.

`mir_audit_failures()` runs after guard insertion and after full source-to-MIR cleanup. It rejects unchecked guarded instructions and misplaced guard markers, while counting non-unwinding operations. It deliberately **does not** prove guard dominance or recover ownership in calling frames. Handwritten MIR can use defensive backend traps without passing the compiler-produced-MIR audit. Numeric overflow semantics (apart from division) remain undecided.

## MIR migration: string methods and record updates

The experimental MIR defines typed string operations for `byte_at`, `slice`,
`find`, `starts_with`, `ends_with`, `join`, and comparisons. `byte_at`/`find`
return `Optional[i64]`. Shared operand-read and type-verification rules apply
to the interpreter and C backend. Compiler guard metadata does not reinterpret
arbitrary string-literal contents.

Direct record-field assignments and updates to arrays held in record fields
reconstruct canonical record values, preserving aliases. This is a correct
value-semantic lowering, but not an in-place CoW optimization. Arbitrarily deep
projection updates are deferred.

`MIR_ONLY_COMPILER_SOURCES` excludes the legacy emitter to make self-hosting
coverage measurable independently. The production/default C path is unchanged;
a native MIR-only fixed point is verified through generations 2, 3 and 4 by `tools/test_mir_selfhost.py`. All 85 executable fixtures pass under the standalone MIR backend (`tools/test_mir_compiler.py`); two legacy diagnostic cases are valid MIR programs.
