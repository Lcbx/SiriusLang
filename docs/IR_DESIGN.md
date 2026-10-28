# Compiler IR

Paths and commands are relative to the repository root. The self-hosted pipeline is:

```text
source -> tokens -> syntax AST -> typed/specialized HIR -> CFG MIR
                                                         |-> C11 -> native
                                                         `-> interpreter
```

Both backends consume verified, ownership-optimized MIR. Production entry points
and source responsibilities are in [Compiler structure](COMPILER_STRUCTURE.md);
current priorities are in [Roadmap](../ROADMAP.md). Historical source-to-MIR
milestones are preserved in [IR migration notes](history/IR_MIGRATION.md).

HIR performs declaration validation, lexical/type resolution, constructor
publication and specialization. Its annotations reference syntax-arena nodes;
MIR makes evaluation order, calls, control flow and ownership explicit.
Canonical `TypeId`s are descriptor-table indices. Descriptor construction and
deserialization reject malformed or cyclic types without changing the table;
invalid construction returns TypeId 0. Primitive IDs 0-5 remain reserved.
Composite operations query descriptor tags rather than numeric ID ranges.
Runtime layout metadata describes storage categories and slots, not portable
byte offsets or a stable FFI ABI. See [Ownership](OWNERSHIP.md) for current
native layouts and static-proof limits.

## Specialization worklist

`src/hir_specialize.sirius` types each reachable concrete function once, using a
HIR-owned queue keyed by `(declaration body ID, canonical argument TypeIds)`. A request
is registered before callee analysis; calls to `IN_PROGRESS` entries resolve to the same
specialization ID (including mutual recursion). Every `HirSpecializedFunction` records
its typed nodes, resolved call targets and return type. The body array index **is** the
stable specialization ID (there is no duplicate per-body ID field). HIR specialization
analysis does not depend on either backend. Nonrecursive unannotated generic return
types are inferred by analyzing the concrete callee body. Recursive unannotated returns
remain rejected; richer inference and effects analysis remain future work.

`src/mir_program.sirius` consumes that worklist and lowers the supported typed bodies.
The `MirProgram.functions` index is the specialization ID; `MIR_CALL` uses the same
index. The HIR registry, typed bodies and type table are local to the builder; the
public `MirProgramBuild` contains the MIR program, entry index, error and named
import/export tables. Resolved `HirSpecializedCall` targets pass directly to MIR
lowering, without a second call-target record or conversion array. The canonical type
table is attached to `MirProgram`, rather than retained twice at the HIR/MIR boundary.
MIR native C supports `i64`, `bool`, `f64` (including arithmetic, comparisons and
`sqrt`), owned strings, structural records, rank-1 arrays, dictionaries, optionals,
record spread/delete, and tail `recur`. Nested record and checked-array projections
support assignment and numeric/string compound updates through path reconstruction. The
MIR interpreter supports floating-point operations and file I/O.  The default
`src/mir_emit_c.sirius` consumes verified linked MIR, including its worklist-owned
specialization indices, and uses no emitter-local specialization cache.

## Syntax AST (implemented in the Sirius compiler)

The parser preserves indentation-sensitive blocks, declarations, `rec`
constructors and `pub` fields, record operations, loops, `match`, indexing
modes and source positions. It stores byte spans in flat node/edge arenas.
Expression precedence and short-circuit behavior belong to language semantics,
independent of C emission.

The executable HIR-to-MIR bridge maintains a sparse index from AST node IDs to
HIR annotation chains. This avoids repeatedly searching the annotation list and
preserves distinct operations on a node. Canonical `TypeTable` descriptors are
cached by stable structural keys while keeping insertion-order numeric `TypeId`s.

## C emission

The MIR-C emitter accumulates generated fragments before joining sections,
function bodies and the full translation unit. Normal source compilation uses
`mir_emit_c_verified` after CFG ownership, lifetime and balance validation.
This removes redundant register-read initialization checks and uses `NULL` as
an empty ownership state for managed C pointers instead of per-register flags.
Scalar registers need no runtime initialized-state tracking. Guarded arithmetic,
index and optional operations rely on their explicit MIR CFG panic edges.
For native linked MIR libraries, `mir_emit_c_reachable` traverses `MIR_CALL`
and `MIR_CALL_MOVE` from the selected entry, removes unrelated exported
functions, remaps concrete call indices and invokes the defensive C emitter.
This produces a self-contained C executable without changing the linked `.smirl`
artifact. Single-file compiler mode retains its verified MIR-C fast path.
The separate `mir_emit_c` entry point retains defensive initialization checks
for arbitrary hand-constructed MIR, including tests of invalid register reads.
The verified emitter omits its function-wide exit-release sweep: the preceding
CFG cleanup pass already emits the required explicit releases, and balance
validation proves other managed registers are dead on every return. The
defensive entry point retains the release sweep. Both paths retain reference
counting and path-sensitive cleanup semantics.
These are compiler-internal representation changes, not changes to source-language
value semantics.

## Typed HIR

* Each expression has an interned `TypeId`, source `Span`, and resolved symbol/function identity.
* Canonical record types intern by lexicographically sorted field names/types, not
  constructor names. Different shapes use distinct `TypeId`s. The emitted MIR C
  representation uses sorted field indices in `S_MirField` slots; byte offsets and
  alignment follow the target C ABI, not a portable packed-record layout.
* Generic calls are specialized by `(definition SymbolId, concrete argument TypeIds)`;
  resolved function calls refer to the specialization. The return type and effect
  signature are attached to the specialization. Avoid recursively instantiating the same
  in-progress specialization.
* A constructor becomes an ordinary function producing an immutable record. Validate
  unconditional field publication, freeze published bindings, and statically resolve
  field layouts.
* Indexing is explicitly `PROVEN`, `OPTIONAL`, or `PANIC` at the HIR level. The type of
  an `OPTIONAL` index is `Optional[element_type]`.
* Structural-field lookups are replaced with integer field indices. Dynamic `Any` is not
  part of the first bootstrap.
* Function values have typed signatures. Noncapturing lambdas become private
  generated functions; effect flags currently have no enforced guarantees.

**Design targets:** extension/operator resolution should use ordinary static
calls where applicable. Transitive purity checking is unimplemented; the target
permits panics in pure functions while treating I/O and impure FFI as effects.

## Lowered MIR (CFG)

`src/mir.sirius` currently defines typed `MirProgram`, `MirFunction`, `MirBlock`,
`MirInstruction` and `MirTerminator` records for scalar, string, record, array,
dictionary and optional values. `mir_verify` validates instruction operands, result
types, signatures and CFG targets; `src/mir_interpreter.sirius` executes them using
typed register slots. `MIR_CALL` uses explicit argument-register indices, with limits
for total steps and call depth. They now accept **checked scalar/string/record HIR
lowering output**, including calls linked by `src/mir_program.sirius` from the HIR
worklist, and are tested against `mir_execute`; the default MIR C emitter consumes the
same `MirProgram`, with no separate AST-to-C fallback. The implemented managed
operations include string construction/concatenation/length, record construction
(`MIR_RECORD_NEW`) field extraction (`MIR_RECORD_GET`), and consuming CoW field updates
(`MIR_RECORD_SET`), plus `MIR_COPY`, `MIR_RETAIN`, `MIR_MOVE` and `MIR_RELEASE`. Every
initialized string/record register owns a reference. Record construction and field reads
retain nested managed values; C copies share immutable reference-counted records and
releases recursively drop nested strings/records. Ordinary calls retain arguments and
returns transfer ownership. After lifetime insertion, managed arguments released
immediately after a call can be transferred instead: `MIR_CALL_MOVE` consumes one or
more distinct managed argument registers that each occur exactly once in the call. Its
`first` field names the first moved register and `second` holds a positional bitmask of
additional moved arguments (positions 0–61); calls with more arguments simply retain any
unrepresentable ones. The verifier and forward/backward ownership analyses invalidate
all transferred registers; interpreter and C emission transfer the same ownership
credits. Duplicated registers and values used later are never transferred, while other
distinct eligible arguments can still move. The interpreter maintains a record heap with
equivalent logical reference credits. Managed collections, direct record updates, and
CFG lifetime/exit-cleanup passes now have experimental coverage; complete ownership
proofs and borrows remain future work. See [INTERPRETER.md](INTERPRETER.md) and
[OWNERSHIP.md](OWNERSHIP.md) for
the current register, nested-heap-edge, and calling-frame credit contracts,
including precise limitations of the static proof.

A `MirFunction` contains register types, parameters, blocks and an entry index.
Each block holds ordered instructions and one terminator (jump, branch, return
or panic).

* Expressions become typed temporaries, making evaluation order explicit before the C backend sees them.
* `if`, `elif`, `and`, `or`, `while`, `for`, and `Optional` fallback become CFG
  branches. In particular, **`and`/`or` are not eager arithmetic instructions**.
* `for i in a..b` lowers to integer bounds and a loop (confirmed half-open).
* `recur f(a, b)` evaluates **all** new arguments first and then jumps to the function's
  entry block. Direct self-tail recurrence never creates another C call frame.
* `let Result = Success | Failure` defines a compile-time type alias for a union of
  **labeled** constructor alternatives. `Success` and `Failure` remain purely structural
  record constructors; the label is carried only by the union alternative. Union `|` in
  this declaration is a type operation, distinct from bitwise integer `|`.
* Constructor patterns such as `Success(value)` resolve both the constructor label and
  its structural payload shape. HIR checks label-based exhaustiveness and records the
  selected canonical tag for MIR lowering, even if two payload shapes are identical.
  `enum Color` is shorthand for empty `rec` declarations and a labeled union.
* Tag-only branches such as `Error1 | Error2` use `NODE_CASE_GROUP`, containing
  individual `NODE_CASE` tag leaves and one shared body. HIR resolves and checks
  every tag, without binding payload fields. MIR emits tag comparisons that
  branch to the shared body and performs no payload extraction for that group.
  Bare single tags use the same representation. Generic packages preserve these
  syntax nodes and defining-module constructor names.
* Canonical labeled union descriptors retain alternatives in declaration order and
  preserve them through module type translation and MIR serialization.
  `MIR_UNION_NEW`/`MIR_UNION_GET` use the existing `second` field as a nonnegative label
  index; `-1` remains available for legacy unlabeled MIR with unambiguous record shapes.
  A structural record can be injected implicitly only when its shape identifies a unique
  alternative. An immediate constructor call in an expected-union binding or return
  retains its label; more general label recovery should not be assumed without a
  contextual type. The C scalar-register compactor must never reinterpret the
  alternative index as a register.
* Unions with exclusively zero-field record alternatives use an unowned `i64` tag;
  `MIR_UNION_NEW` writes that tag and `MIR_UNION_TAG` reads it directly. `MIR_UNION_GET`
  materializes an empty record when actually needed, but match lowering skips the
  extraction when the pattern binds no fields. After lifetime cleanup, MIR optimization
  may replace an immediately released empty record followed by enum injection with
  `MIR_UNION_NEW` whose `first` is `-1` and `second` is the selected alternative; the
  verifier restricts this source-free form to valid labeled, payload-free enums. No
  record is allocated by either native execution or MIR interpretation for that form.
  Unions with any nonempty alternative still use an `S_MirRecord*` tagged wrapper
  (`fields[0]` tag, following slots hold up to three unmanaged scalar fields;
  other alternatives use a retained record pointer in `fields[1]`). Dictionary union
  entries remain pointer-valued. Native `Array[Union]` specializations with at
  least one eligible scalar alternative now use strided slots containing a
  declaration-order tag and up to three unmanaged scalar fields; large/managed
  alternatives use a boxed-wrapper fallback (`tag == -1`). Copy/drop and ARC
  scanning use the active physical representation. An emission-only escape
  analysis scalar-replaces local union slots and small scalar payloads whose
  uses are limited to array operations, matching, copies/moves and drops.
  Slot reads snapshot bytes and retain only boxed fallback edges; scalar
  construction and matching avoid wrappers and payload allocations. Escaping
  values still materialize the ordinary owned representation; MIR instructions,
  verification, serialization and interpreter behavior stay unchanged.
  The isolated POD-only C11 comparison
  in `tools/benchmark_union_storage.py` predates this implementation and is
  not an end-to-end Sirius performance result.
* Experimental native coallocation (`SIRIUS_EXPERIMENTAL_UNION_COALLOC`) preserves the
  pointer-based union ABI and independent record/wrapper ARC identities while sharing
  one allocation. Post-cleanup MIR instructions `MIR_RECORD_NEW_FUSED` and
  `MIR_RECORD_NEW_FUSED_MOVE` carry the *union destination register index* in
  `immediate` (not a type ID); the payload destination and field inputs are otherwise
  those of their ordinary constructors, including the existing move-input mask for the
  move form. MIR verification requires immediate `UNION_NEW` and `RELEASE(record)` in
  the same block. Native emission selects coallocation with a compile-time flag; MIR
  interpretation ignores the allocation hint. The experimental layout marks **only
  fixed-size** coallocated records and union wrappers using the high bit of their
  existing `capacity` word. Growable arrays and ordinary records use unmarked, normal
  capacities; the native runtime rejects attempts to resize a fused node. No extra
  record-header word is required. See `ROADMAP.md` for the experiment's measured
  benefits and limitations.
* Scalar alternatives of up to three unmanaged fields now inline in union wrappers
  by default. Fresh consuming construction reuses the payload allocation with a
  reserved tag slot. The historical `SIRIUS_EXPERIMENTAL_UNION_HYBRID` flag adds
  no coallocation for these alternatives; full coallocation remains available
  for boxed fallbacks. See [Runtime](RUNTIME.md#hybrid-tagged-unions).

* Array access becomes a bounds proof/optional result/checked panic path, depending on syntax.
* Checked and optional negative array indices normalize relative to array length
  before bounds checking (`-1` is the last element). CoW writes use the same
  normalization; plain indexing still requires a static proof. See
  `tools/test_negative_array_index.py`.
* Record spread, deletions and overrides are resolved to a new record layout; preserve
  left-to-right **expression evaluation** even if a field is later overwritten.

## Implemented experimental array MIR

The MIR interpreter and default C backend now support canonical rank-1 `Array[T]` values
using `MIR_ARRAY_NEW`, `MIR_ARRAY_GET`, `MIR_ARRAY_LENGTH`, and `MIR_ARRAY_SET`. Element
types may be scalars, strings, records or previously interned array types. Checked reads
and writes normalize negative indices before testing bounds. `MIR_ARRAY_SET(dst, owner,
index, value)` **consumes** the owned `owner` register, invalidating it if distinct from
`dst`. The array is updated in place only when uniquely owned; shared arrays are cloned,
retaining managed elements and releasing the replaced element. Copies retain the
collection handle; recursive release follows canonical element descriptors. CFG
ownership, lifetime insertion and exit cleanup run before final verification.

## Memory management in MIR

Value semantics have to be visible in the compiler, not reconstructed from textual C:

* `Retain` and `Release` track strings, records, arrays, dictionaries, optionals
  and payload-bearing unions. Type-directed drops release managed children.
  Payload-free enum tags are unowned.
* Logical record copies preserve value semantics and can share managed children.
  Current native inline records contain only scalar fields; other records are boxed.
* `CoWSet(root, path, value)` resolves nested field/index updates and assures unique
  ownership of any modified buffer. Reuse uniquely owned allocation; otherwise copy the
  path (with reference management).
* Function inputs are *logically immutable*. For a nonescaping read-only parameter C may
  safely borrow, but retained/returned/stored values must have well-defined ownership
  credits.
* Function returns transfer ownership to the caller. All control-flow exits must release
  live owned temporaries; the ARC pass can insert necessary releases after CFG creation.
* Last-use moves, consuming CoW updates and physical register-slot reuse reduce
  ownership overhead. Borrow inference and heap-wide static proofs remain future work.

## Worked lowering examples

### `08_array_cow`

Sirius:

```sirius
var a = [10, 20, 30]
let original = a
a[!1] = 99
```

Illustrative MIR (pseudocode, not serialized instructions):

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
rec Success(value: i64)
	pub value

rec Failure(code: i64)
	pub code

let Result = Success | Failure
```

`Success(7)` first produces a record `{value: i64}`. When passed to a parameter of type
`Result`, HIR inserts `UnionInject(Result, SuccessTag, record)`; in C this is a tagged
union with a record payload. A match checks the tag and extracts the corresponding
record before binding its fields.

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

Separate tokenizer, parser, HIR and MIR stages keep type resolution independent
of C generation and make ownership and evaluation order explicit.


## Current runtime type layouts (static MIR types)

`Types.runtime_storage`, `Types.runtime_ownership` and `Types.runtime_layout`
expose the representation of canonical `TypeId`s independently of their numeric
positions. `RuntimeLayout(storage, ownership, fixed_slots, variable_slots)`
describes the **current** MIR implementation, not a stable FFI or shared-library
ABI. Storage is `i64`, `f64`, `bool`, `S_String *`, `S_MirRecord *`, or void;
invalid IDs have an invalid layout. Scalar values are trivial; strings own an
`S_String` reference and other composite values generally own an `S_MirRecord`
reference. Payload-free unions instead have `i64` scalar storage and no ownership
credit.

An `S_MirRecord` contains a reference count, local numeric type index, length,
capacity and `S_MirField` union slots. Records use one fixed slot per field in
canonical name order; arrays use `length` variable, type-strided slots;
optionals use zero or one; dictionary wrappers use one slot holding `S_Dict *`. Unions with
only empty-record alternatives use zero heap slots: their value is the scalar
alternative index. Other unions use a wrapper with an alternative index and
either up to three inline scalar fields or a retained record payload pointer. Native union
arrays can mix heterogeneous alternatives: their stride is fixed for that
union specialization, with a boxed fallback for large or managed payloads.

Eligible scalar-only records use inline C aggregates in registers and padded
`S_MirArrayInline<TypeId>` slots in native arrays. Array layout metadata selects
element type, byte stride and ownership policy; reads copy fields with `memcpy`
without creating a record object. CoW copies these unmanaged slots as bytes.
Record fields, dictionary entries and optional payloads still box records;
optional array indexing boxes the selected element. Managed-field records and
union alternatives use pointer-valued wrappers outside arrays, with eligible
scalar payloads inline in the wrapper. Inside union arrays,
eligible scalar-only alternatives are packed inline and larger/managed ones
use a pointer fallback. See [Ownership](OWNERSHIP.md),
`tools/test_mir_inline_arrays.py`, and `tools/test_mir_inline_unions.py`.

The C emitter selects union members, retains and releases by the canonical descriptor.
`sizeof(S_MirField)`, alignment and header offsets remain target-ABI-dependent.
The interpreter uses its own heap representation but the same static ownership
classification. Type IDs inside `S_MirRecord` are local to the executable,
**not** portable runtime identities. Reflection, native registration, dynamic
`Any` and versioned runtime registration metadata remain
separate future work.

## First-class function MIR

Canonical `TYPE_TAG_FUNCTION` signatures record parameter types, return type and an
effect flag (currently unchecked). `Fn[parameters...,return]` annotations and named or
qualified exported function references lower to `MIR_FUNCTION_REF`; `MIR_CALL_INDIRECT`
invokes function-typed scalar IDs. Source expression-bodied, noncapturing lambdas lower
to private functions, with contextual parameter-type inference. The interpreter, native
C emitter, serialized MIR and linked libraries support these values with relocation of
referenced function IDs. Imported concrete functions must be exported and have available
bodies; source-free `.smirg` packages cannot provide concrete bodies. Function
references have no ARC credit. Contextual generic function references reuse the ordinary
HIR named-call specialization worklist when an expected `Fn` signature is available,
including source-free `.smirg` generic declarations. Generic callback combinators are
available in `Std.Functional`; source-free
concrete bodies and transitive purity/effect enforcement remain deferred.

**No implicit captures.** `Fn` is a scalar function ID with no ARC
credit. Source lambda bodies have independent lexical scopes, so attempting
to reference an enclosing local is a type-checking error. There is no hidden
environment argument in either backend, and capturing closures are not a
planned feature of Sirius.

The explicit-context model is exercised by `Std.Functional.map_with_context`.
The MIR regression in `tools/test_mir_closure_environment.py` also verifies
that an ordinary owned record `{code: Fn[Env, i64, i64], env: Env}` can cross
function boundaries, retain managed fields, preserve CoW, survive MIR wire
format and linking, and invoke its code with `env` supplied explicitly. This
uses existing record and function-reference instructions; it is not a new
`Fn` representation or lexical capture mechanism.

## Tagged union MIR

Source constructor unions use `(label, structural record type)` alternatives
**in declaration order**. The ordered sequence determines structural type
identity, discriminant values, imported type signatures, and MIR wire layout.
An equal set of labels in another order is a different union type. Empty `rec` payloads
are valid, allowing `enum`
as source sugar. `MIR_UNION_NEW` and `MIR_UNION_GET` reuse the existing `second`
instruction slot for explicit alternative indices, independent of payload
shape; unlabeled legacy MIR uses `-1` for uniquely shaped payload lookup.
The interpreter and C backend use a scalar tag for payload-free unions and a
managed wrapper for unions with payloads. HIR matches source constructor labels
and annotates declaration-order alternative indices, so equal-shaped alternatives do
not collapse during lowering.

## Dictionary MIR

`MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, `MIR_DICT_LENGTH`,
`MIR_DICT_NEXT`, `MIR_DICT_KEY`, and `MIR_DICT_VALUE` operate on canonical `Dict[K,V]`
descriptors. `K` may be `i64`, `bool`, `f64`, `string`, or a canonical record, optional,
or union recursively containing hashable payloads (not arrays/dictionaries); `V` can be
any MIR-supported type, including managed arrays/records/dictionaries. Native MIR C uses
the shared runtime's open-addressed `S_Dict` through an owned `S_MirRecord` wrapper; the
interpreter retains list-based storage. `MIR_DICT_INSERT` consumes its owner, reuses
unique storage, or clones shared dictionary entries while retaining nested values.
Existing keys are replaced without changing entry count. Structural key hashes and
equality traverse canonical record fields, optional presence, and union alternative
tags/payloads; signed floating zero hashes identically, and all NaNs have one dictionary
key equivalence class and hash. Source `==` retains IEEE NaN behavior. `MIR_EQ_VALUE`
compares record, optional and union values without consuming ownership; its
interpreter/C implementations recursively compare nested arrays, but direct source array
`==` is rejected pending elementwise semantics. The verifier checks key/value/result
types. `MIR_DICT_GET_OR` accepts an already evaluated fallback register, **not** an
`Optional` result, and must not be used as a direct lowering of lazy source `or`.
Read-only cursor operations enumerate dictionary entries for nested value coercions
without modifying the source. Source `Dict[K,V]()`, `.insert`, and `len(dict)` are
supported experimentally; source `.get` now lowers through Optional and CFG control
flow.

## Implemented experimental optional MIR

Canonical `Optional[T]` values use zero/one owned payload fields, including nested
managed values. `MIR_OPTIONAL_NONE`, `MIR_OPTIONAL_SOME`, `MIR_OPTIONAL_HAS`,
`MIR_OPTIONAL_UNWRAP`, `MIR_ARRAY_GET_OPTIONAL` and `MIR_DICT_GET_OPTIONAL` are verified
and executed by both the Sirius MIR interpreter and the MIR-C backend. The interpreter
maintains reference counts; C uses the existing managed-record runtime with
descriptor-directed nested release.

HIR-to-MIR lowering now handles `a[?i]`, `dict.get(key)`, postfix `!`, `optional or
fallback` and `let x = optional else return value`. The latter two produce actual CFG
branches, so fallback expressions execute only for absent values. The older
`MIR_DICT_GET_OR` remains explicitly eager and must not be used to lower source-level
`or`. The implemented CFG ownership/lifetime passes follow this lowering, as described
below. See `tools/test_mir_optional.py` and `tools/test_mir_run.py`.

## Explicit runtime failure edges (experimental)

`src/mir_guard.sirius` splits checked `MIR_DIV_I64`, `MIR_REM_I64`, `MIR_SHL_I64`,
`MIR_SHR_I64`, `MIR_ARRAY_GET`, `MIR_ARRAY_SET`, and `MIR_OPTIONAL_UNWRAP` into
validation and continuation/panic blocks. Pure `MIR_NONZERO_I64`,
`MIR_DIV_NO_OVERFLOW_I64`, `MIR_SHIFT_VALID`, `MIR_ARRAY_INDEX_VALID`, and existing
`MIR_OPTIONAL_HAS` predicates preserve the original panic messages, including the
`INT64_MIN / -1` overflow case. This pass runs **before** lifetime and exit cleanup; the
latter release owned registers on the newly explicit failure edges. Guarded instructions
are marked to make repeated lowering idempotent. Both backends retain defensive checks
for unguarded manually constructed MIR. This is not general exception unwinding:
allocator failure and other implicit aborts remain outside the CFG model.

## Experimental CFG lifetime insertion

`src/mir_lifetime.sirius` analyzes each MIR function using backward register liveness to
a fixed point across CFG backedges. At a last use or dead definition of a managed
register, it inserts `MIR_RELEASE`. Explicit consuming operations (`MIR_MOVE`,
`MIR_ARRAY_SET`, `MIR_DICT_INSERT`) do not release consumed sources; copies and calls
retain their existing ownership semantics. The pass is run after typed MIR construction
and before final verification, and is idempotent. Forward definite-initialization
(`src/mir_ownership.sirius`) runs before and after lifetime insertion. It intersects
initialized registers at CFG joins and loop backedges, with explicit move/release
invalidation, to reject invalid uses statically. Both passes share operand-read
definitions.

A second pass (`src/mir_cleanup.sirius`) performs **path-sensitive ownership cleanup**.
Using one snapshot of forward may-ownership and backward liveness, it inserts
conditional releases for managed registers dead on each CFG edge, splitting conditional
edges as needed. This avoids repeated whole-CFG analysis after every edge rewrite; the
rewritten graph is reanalyzed once for *may*-ownership exit cleanup and verified for
balance. An additional *may-initialized* forward union analysis identifies values owned
on **any** path to a return or explicit panic. Exit cleanup uses `MIR_RELEASE_IF_INIT`
for possibly owned registers, preserving the returned value. No post-edge
*must*-ownership pass is required for this conditional release strategy.
`mir_check_balance` rejects reachable exits that may retain any other managed register.
The conditional release uses nullable managed pointers in verified C (defensive direct C
emission retains initialization state) or register validity in the interpreter; its
effect is tracked by both analyses. Cleanup is idempotent. Tests cover branches, joins,
nested managed arrays, explicit conditional releases, native/interpreter agreement, and
prior lifetime behavior. Covered division, checked indexing/update and unwrap failures
now have explicit cleanup exits; allocator failures and other implicit aborts remain
non-unwinding. An optional `mir_audit_return_credits` interpreter check traverses the
heap reachable from the returned value and compares expected incoming reference credits
with recorded counts, also rejecting leaked unreachable objects, corrupted counts,
wrong-type or out-of-range handles. It counts each field of a shared parent once, but
every repeated child reference separately; `tools/test_mir_credits.py` checks nested
collections, CoW field updates, loops, branches and caller/callee paths against native
emission. This checks **one completed execution**, not all CFG paths. Final
`mir_check_balance` additionally audits managed source/destination credit transitions on
both must-own and may-own CFG states: borrowed operands preserve their owner,
transferred/released operands lose it, and a managed result has one owned credit even
when replacing an existing value. These are static **register-level** obligations, not a
complete proof of nested heap credits, feasible path correlations or exceptional
unwinding. `tools/test_mir_credits.py` covers conditional replacement at a CFG join and
moves into occupied destinations in addition to returned graphs. Heap-wide static
ownership-credit proofs remain future work; process-fatal failures do not require
caller-frame unwinding.

Final balance verification propagates must-initialization (intersection) and
may-ownership (union) together in one forward worklist seeded at the CFG entry. Each
reached successor is processed at least once, even if the predecessor's state is
initially unchanged. This avoids unnecessary initial processing of all reachable blocks,
reuses reachability and predecessors, and checks register validity before exit balance.
Standalone callers can still request one analysis only. Final balance verification
reuses the same CFG reachability, predecessor lists and definite-initialization states
to check both register validity and managed-reference balance. Scalar-only functions
skip the may-ownership solver. Public standalone checks keep the same diagnostics.

MIR lowering keeps source syntax, typed HIR annotations, canonical types, and call
resolution in a read-only `MirLowerContext`; `MirBuilder` carries only mutable
registers, CFG blocks, scopes, and loop targets. The forward ownership worklist
precomputes sparse register write effects for each basic block and applies one
state transfer per block, preserving sequential move/redefinition behavior.
Must/may joins update an owned packed mask rather than allocating a new one per
predecessor. Instruction-level validation still uses ordered reads/writes.

The verified C emitter centralizes managed-value destination replacement in
`static inline` string/record store helpers. The new owned value is evaluated
before replacing and releasing the old one, preserving aliasing, CoW, and
self-assignment semantics; directly supplied, unverified MIR still uses its
explicit checked stores. This reduces repeated C source without altering the
MIR ownership graph. For ownership-verified C emission, scalar register storage
is also allocated using CFG liveness and a packed interference graph. Only
non-argument `i64`, `bool`, and `f64` registers can share a physical slot;
managed values use forward CFG may-ownership and an interference graph to
share slots across disjoint ownership intervals, including cross-block ones.
Instruction reads/writes and conditional cleanup also cause interference to
prevent false releases or clobbering. Joins conservatively union may-owners;
original MIR, interpreter, and defensive emission remain unchanged.

The combined compiler path uses unchecked *internal* guard, lifetime and cleanup
transformations, followed by `src/mir_opt.sirius` peephole optimization, between an
initial ownership verification and final ownership-balance verification. An adjacent
managed `MIR_COPY`/`MIR_RETAIN` and source `MIR_RELEASE` become `MIR_MOVE` when
destination and source differ. Conditional/path-dependent releases are not fused.
Ownership and liveness state helpers skip writes that leave a CoW state array unchanged.
The final failure audit reuses that verification. Individual public MIR passes still
validate their inputs and outputs; intermediate checks are not omitted from standalone
tests.

### MIR fallibility audit (implemented)

`mir_failure_info(op)` is the shared instruction-level contract. `guarded` marks
synchronous operations that must be lowered to explicit `Panic` edges; `abort` marks
operations that may terminate outside the current function's CFG (notably allocation and
callee failures). `MIR_ARRAY_SET` has both properties: bounds failure is guarded,
allocation failure during CoW is not unwound.

`mir_audit_failures()` runs after guard insertion and after full source-to-MIR cleanup.
Its independent `in_band` counter identifies operations that encode ordinary
absence/failure as values (including `Optional`, bool status, or IEEE-754 nonfinite
results); this is *not* an exceptional cleanup path. Executable `Result` unions return
and own their active alternative normally, while exhaustive matching belongs to HIR. It
rejects unchecked guarded instructions and misplaced guard markers, while counting
non-unwinding operations. For guarded opcodes it verifies that **every incoming CFG
path** has a matching predicate with a panic failure edge, allowing intervening
cleanup-only blocks and the two required integer-division checks. This is a proof for
the guard-lowering shape, not a general-purpose dominance solver; fatal failures
terminate the process and therefore do not require calling-frame unwinding. The verified
C entry point runs this audit before omitting defensive checks. Handwritten MIR can use
defensive backend traps without passing the compiler-produced-MIR audit. `i64` addition,
subtraction, multiplication and negation wrap at 64 bits;
division/remainder truncate toward zero and trap on zero or `INT64_MIN / -1`.

## MIR migration: string methods and record updates

The experimental MIR defines typed string operations for `byte_at`, `slice`,
`find`, `starts_with`, `ends_with`, `join`, and comparisons. `byte_at`/`find`
return `Optional[i64]`. Shared operand-read and type-verification rules apply
to the interpreter and C backend. Compiler guard metadata does not reinterpret
arbitrary string-literal contents.

Direct record-field assignments use `MIR_RECORD_SET(dst, owner, field, value)`,
which consumes the owned record and modifies it in place when uniquely owned.
If another value retains the record, C and interpreter clone the record and
retain its managed fields before replacing the selected field. The incoming
value is retained before releasing the old field, including self-assignment.
For `r.items = r.items.append(x)`, `MIR_RECORD_ARRAY_APPEND` fuses the
record and array updates instead: consume the record, make the record unique
if shared, then append to its array field without extracting a temporary
retained array. If the array itself has other owners it is still cloned.
Nested record/checked-array assignment paths read each projection and index once,
then update the leaf and rebuild ancestors using existing consuming `MIR_RECORD_SET`
and `MIR_ARRAY_SET` operations. Checked-index compound assignments reuse the same path
reconstruction and CoW operations.

The committed C bootstrap comes from the native module graph rooted at
`src/modular_cli.sirius`. Verify its fixed point with `tools/test_bootstrap.py`
and `tools/test_mir_selfhost.py`; see [Development](DEVELOPMENT.md).

## Storage review (post-MIR-default)

- **HIR:** `HirNode` is a small typed annotation over source AST IDs, not a
  second syntax tree. `HirAnnotationIndex` uses a per-source dictionary head and
  an array of predecessor links for repeated annotations; lowering builds this
  index once per function. Keep it until a measured lookup bottleneck warrants
  changing the representation.
- **MIR:** `MirInstruction` stores general-purpose operands plus `inputs` and
  `text`; even many scalar operations construct empty input arrays. Blocks own
  instruction arrays and functions own block/register-type arrays. Packing
  operands or using opcode-specific layouts may save memory, but would change
  the verifier, optimizer, interpreter, and C emitter simultaneously; defer it.
- **Ownership:** CFG ownership analyses maintain packed register-state arrays and cache
  per-block instruction effects. Liveness worklists share one empty state per
  function; CoW isolates it when an instruction makes a value live. Measured
  native compilation is faster despite slightly larger cumulative allocation
  requests. The prior standalone intersect/union helpers
  were unused; the active worklist exclusively uses `mir_ownership_join`.
  A subsequent CoW callsite profile finds large cumulative copies from CFG
  joins/transfers and parser arena appends; keep the array/record value
  semantics rather than disabling cloning. See [historical MIR profiles](history/MIR_PROFILE.md).
- **Runtime boundary:** generated MIR-C records and collections have typed
  layouts distinct from the generic `S_Array`/`S_Dict` C runtime and the
  specialized legacy `i64` collections. Consolidation requires an ABI and
  ownership plan, not simply moving functions into the same source file.

For now prioritize preserving the native bootstrap fixed point and differential
behavior over compact MIR storage or speculative container unification.

### CFG facts and MIR pass boundaries

`mir_cfg_facts(function)` computes the reachability mask and predecessor lists
for a fixed CFG. Forward ownership and backward liveness may share this snapshot;
`mir_cleanup_function` does so before inserting edge cleanup, then recomputes
facts for the modified graph before analyzing exits. A snapshot must never be
reused after a pass adds blocks or changes terminators.

The combined compiler pipeline checks initial MIR ownership, then applies guard
insertion, lifetime insertion, edge/exit cleanup, and the ownership peephole
optimizer without redundant intermediate verification. Its final balance and
failure audit validate the result. Public individual pass entry points retain
their own checks for independently constructed MIR.

### Local record destructuring (MIR backend)

`let {binding = .field, ...} = expression` lowers to one evaluation of the
record expression followed by typed `MIR_RECORD_GET` operations. HIR checks
field existence and duplicate bindings and preserves ordinary lexical scope;
MIR lifetime cleanup manages the temporary owner and field values. This does
not introduce recursive patterns or mutable destructuring, and follows the same
owned-projection rules as other record reads.
