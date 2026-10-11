# Sirius compiler IR — target architecture

**Status:** the active, self-hosted Sirius compiler (`src/`) currently performs most semantic checks alongside direct C11 generation, with an initial typed HIR arena in `src/hir.sirius` for selected expression classes; an independent but partial function-body HIR analysis pass handles known scalar, record and collection expressions; no complete typed-function HIR exists; an isolated scalar/string/record HIR-to-MIR lowering subset is now implemented. A typed CFG MIR, HIR-to-MIR lowering subset and interpreter are implemented in Sirius and exercised with supported source-level functions, and an experimental `--mir-c` path now emits C11 from that same MIR; default compilation still uses the legacy direct emitter. `src/types.sirius` now provides canonical semantic interning for primitive, record, array, dictionary and optional types; the emitter still consumes legacy integer kind encodings via a compatibility bridge. The Python bootstrap (`bootstrap_python/`) is frozen historical source. Self-hosting and a reproducible C fixed point **are already established**; this document describes the **next architecture**, not working IR. See [ROADMAP.md](ROADMAP.md), phases B–D, for incremental migration and ownership testing.

The intended pipeline is **tokens → syntax AST → typed/specialized HIR → ownership-aware CFG MIR → (interpreter | C11)**. The incremental bridge already stores `HirNode(source, operation, type_id, field_index)`, where `source` refers back to the syntax AST. Literal, name, field, unary/binary and call-signature resolution is C-independent; HIR now validates builtin and user-function calls, string methods, array/dictionary methods, and record field composition (spread, delete, replace) and constructor publication using canonical descriptors. The surrounding emitter still constructs and consumes these nodes while generating C, and still triggers specialized body generation, expression traversal, record initializer evaluation order, and ownership. HIR owns specialization cache keys and lifecycle. A pre-emission HIR declaration pass now indexes module functions and verifies declaration names, parameter syntax/uniqueness and deprecated `def` constructors before C output begins. The body pass now uses that declaration index to resolve concrete calls (including forward references), check annotated arguments, infer nonrecursive generic return types through nested calls, and type constructor-produced records. Unresolved/recursive cases remain deferred. Specialization keying and in-progress tracking now belong to HIR (`HirSpecializationRegistry` keyed by function-body AST identity plus canonical argument TypeIds); the emitter still initiates specializations and owns the resulting C bodies; the body pass is not yet a complete specialized HIR. The current type table is a semantic identity registry (dense `TypeId` indices, descriptor tags and child IDs); runtime size/alignment/field offsets are not yet stored in the semantic descriptors. The C backend now derives record field order and type-specific copy/drop operations from those descriptors, through a temporary legacy-kind naming bridge. `target_ir.py` contains illustrative Python dataclasses only. The current Sirius AST is an indexed arena (`Array[Node]` plus child edges), not the original Python syntax AST.

### Concrete specialization worklist (implemented subset)

`src/hir_specialize.sirius` types each reachable concrete function once, using a HIR-owned queue keyed by `(declaration body ID, canonical argument TypeIds)`. A request is registered before callee analysis; calls to `IN_PROGRESS` entries resolve to the same specialization ID (including mutual recursion). Every `HirSpecializedFunction` records its typed nodes, resolved call targets and return type. HIR specialization analysis does not depend on either backend. Currently, non-constructor callees require explicit return annotations; arbitrary generic return inference remains future work.

`src/mir_program.sirius` consumes that worklist and lowers its supported scalar/string/record subset. The `MirProgram.functions` index is the specialization ID; `MIR_CALL` uses the same index. The semantic worklist can type non-scalar functions, but MIR currently supports `i64`, `bool`, owned `string` and immutable structural records (including nested managed fields). Rank-1 arrays and a narrow dictionary subset are also executable; optional dictionary lookups, record spread/delete and nested indexed updates remain outstanding. The default C emitter still drives its own instantiations. The opt-in `src/mir_emit_c.sirius` instead consumes verified linked MIR, including its worklist-owned specialization indices, and uses no emitter-local specialization cache.

## Syntax AST (already implemented in both compilers)

Both parsers preserve indentation-sensitive blocks, declarations (`let`, `var`), constructors (`pub`), record operations, loops, `match`, indexing modes, and source positions. The Sirius parser uses source byte spans and flat node/edge arrays; future HIR must retain those spans for diagnostics. Expression precedence and short-circuit behavior are semantic obligations, not properties of C emission.

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

`src/mir.sirius` currently defines typed `MirProgram`, `MirFunction`, `MirBlock`, `MirInstruction` and `MirTerminator` records for the initial `i64`/`bool`/`string`/record subset. `mir_verify` validates instruction operands, result types, signatures and CFG targets; `src/mir_interpreter.sirius` executes them using typed register slots. `MIR_CALL` uses explicit argument-register indices, with limits for total steps and call depth. They now accept **checked scalar/string/record HIR lowering output**, including calls linked by `src/mir_program.sirius` from the HIR worklist, and are tested against `mir_execute`; the opt-in MIR C emitter consumes the same `MirProgram`, while legacy compilation still bypasses MIR. The implemented managed operations include string construction/concatenation/length, record construction (`MIR_RECORD_NEW`) and field extraction (`MIR_RECORD_GET`), plus `MIR_COPY`, `MIR_RETAIN`, `MIR_MOVE` and `MIR_RELEASE`. Every initialized string/record register owns a reference. Record construction and field reads retain nested managed values; C copies share immutable reference-counted records and releases recursively drop nested strings/records. Calls retain arguments and returns transfer ownership. The interpreter maintains a record heap with equivalent logical reference credits. Full collection ownership, record mutation, a CFG lifetime pass and borrows remain future work. See [INTERPRETER.md](INTERPRETER.md).

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

HIR generic specialization lookup and lifecycle, symbol binding metadata and structural record-field lookup now use canonical `TypeId`s. The C backend still accepts legacy kind numbers for layout and generated helper naming. The module declaration HIR pass precedes emission and supplies function headers; pre-emission body traversal now resolves local lexical scopes, supported scalar and record/collection expressions, indexing modes and selected calls; full typed bodies and MIR remain future work.

## Implemented experimental dictionary MIR

`MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, and `MIR_DICT_LENGTH` operate on canonical `Dict[K,V]` descriptors. `K` is currently `i64` or `string`; `V` can be any MIR-supported type, including managed arrays/records/dictionaries. Storage is reference-counted and unoptimized (linear key scan, insertion order). `MIR_DICT_INSERT` consumes its owner, reuses a unique allocation, or clones a shared dictionary while retaining nested values. Existing keys are replaced without changing entry count. The verifier checks key/value/result types. `MIR_DICT_GET_OR` accepts an already evaluated fallback register, **not** an `Optional` result, and must not be used as a direct lowering of lazy source `or`. Source `Dict[K,V]()`, `.insert`, and `len(dict)` are supported experimentally; source `.get` remains deferred pending optional CFG lowering.

## Implemented experimental optional MIR

Canonical `Optional[T]` values use zero/one owned payload fields, including nested managed values. `MIR_OPTIONAL_NONE`, `MIR_OPTIONAL_SOME`, `MIR_OPTIONAL_HAS`, `MIR_OPTIONAL_UNWRAP`, `MIR_ARRAY_GET_OPTIONAL` and `MIR_DICT_GET_OPTIONAL` are verified and executed by both the Sirius MIR interpreter and the MIR-C backend. The interpreter maintains reference counts; C uses the existing managed-record runtime with descriptor-directed nested release.

HIR-to-MIR lowering now handles `a[?i]`, `dict.get(key)`, postfix `!`, `optional or fallback` and `let x = optional else return value`. The latter two produce actual CFG branches, so fallback expressions execute only for absent values. The older `MIR_DICT_GET_OR` remains explicitly eager and must not be used to lower source-level `or`. This does **not** replace the pending CFG ownership/lifetime pass. See `tools/test_mir_optional.py` and differential cases in `tools/test_mir_c.py`.

## Experimental CFG lifetime insertion

`src/mir_lifetime.sirius` analyzes each MIR function using backward register liveness to a fixed point across CFG backedges. At a last use or dead definition of a managed register, it inserts `MIR_RELEASE`. Explicit consuming operations (`MIR_MOVE`, `MIR_ARRAY_SET`, `MIR_DICT_INSERT`) do not release consumed sources; copies and calls retain their existing ownership semantics. The pass is run after typed MIR construction and before final verification, and is idempotent. Forward definite-initialization (`src/mir_ownership.sirius`) runs before and after lifetime insertion. It intersects initialized registers at CFG joins and loop backedges, with explicit move/release invalidation, to reject invalid uses statically. Both passes share operand-read definitions.

A second pass (`src/mir_cleanup.sirius`) performs **path-sensitive ownership cleanup**. Using fixed-point forward ownership and backward liveness states, it releases definitely owned managed registers dead on each CFG edge, splitting conditional edges when their cleanup differs. It then recomputes forward states before releasing remaining initialized registers at explicit `Return` and `Panic` terminators, preserving the returned register. It is idempotent and rechecks ownership of the resulting program. This runs after last-use insertion for linked HIR→MIR programs. Tests cover unused arguments, conditional branches, loop exits, managed returns, explicit panics and native/interpreter agreement. Backend return cleanup remains enabled as a safety net. Implicit runtime failures still abort without unwinding; full path-dependent credit proofs and all nested CoW paths remain future work.
