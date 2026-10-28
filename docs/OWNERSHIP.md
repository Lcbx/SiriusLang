# Ownership and memory management

> Paths and commands are relative to the repository root. **Current** below
> describes the Phase 12 implementation (October 2026); **target** is intended
> behavior, not a claim that every value already has that representation.
> Trust the compiler, runtime and tests for current behavior, and
> `LANGUAGE_DESIGN.md` for intended language semantics.

## 1. Intended model and current storage

### Target memory-management strategy

Sirius has immutable **value semantics** for records and collections. A local
`var` can replace a value, and the implementation can modify storage that it
proves exclusively owned, without exposing shared mutation. The intended
model is:

1. **Value slots own values.** An initialized MIR register, record field,
   array element or dictionary entry owns the value stored there. Reading a
   field/index produces an independently owned *copy* at the language/MIR
   boundary. It need not deep-copy nested buffers. A compiler-proven temporary
   borrow or last-use move is an unobservable implementation optimization.
2. **Reference counts belong to shared heap allocations, not to values as
   such.** Inline scalars and inline record/union payloads need no object
   refcount of their own. A value containing strings or other managed buffers
   owns references to those buffers, which retain their own counts. A slot's
   ownership obligation is not necessarily one heap pointer or one credit.
3. **Type-directed `copy`, `move`, and `drop`.** Copy inline bits and retain
   each owned managed child; move transfers a slot's entire ownership and
   invalidates the source; drop recursively releases all resources belonging
   to that value. Calls, container storage and CoW use the same contracts.
4. **CoW and structural sharing.** Copying a container can retain its buffer.
   Mutating a value consumes/replaces an owned root; if its allocation is
   uniquely owned, reuse is permitted, otherwise copy the modified path while
   preserving independent values and nested sharing.
5. **Acyclic, deterministic lifetime management.** Statically typed immutable
   graphs and no capturing closures permit non-atomic ARC without a tracing
   collector in the initial single-threaded runtime. No user-facing borrow
   checker, mutable shared object graph, or required source-level lifetime
   annotations are planned. Interpreted and native MIR must agree on visible
   semantics despite different physical layouts.

The target is **not** "a refcount in every register", "a refcount in every
array element", or mandatory SoA metadata arrays. Array/dictionary slots
logically hold complete values. Whether a particular value is inline or boxed
is an internal layout choice, to be validated for both copy costs and memory
usage. Inline record and union storage in containers remains a future
step; current heap slots store boxed records.

### Current abstract ownership contract

* MIR marks scalar `i64`, `f64`, `bool`, function references and payload-free
  enum tags as unowned. `string`, records, arrays, dictionaries, optionals and
  payload-bearing tagged unions are **managed MIR types**. "Managed" means
  the value has an ownership/drop obligation; it does not always mean the
  native register contains one refcounted pointer.
* Every initialized managed MIR register owns **one logical value**. The
  interpreter models ownership with owned heap handles; the native emitter
  may represent an eligible scalar-only record as an inline C aggregate.
  The native emitter currently uses owned call parameters and owned projections;
  it does not infer temporary borrows.
* Each populated managed field/element/entry owns its child value. A boxed
  managed child normally contributes one separate reference, *per slot*.
  Repeating one reference in two elements means two owning edges; sharing a
  parent adds only a reference to the parent, not to all its children.
  Future inline values with managed fields would own those fields directly.

For example, copying an `Array[Record]` handle normally increments **the
array buffer's** count, not every element's count. Indexing an ordinary
boxed-record element copies its fields into an inline scalar-only
register where eligible, or retains its boxed record handle otherwise. Even
scalar-only record elements currently occupy separate boxes in arrays. If a
future inline managed-record element is copied, its inline bytes and managed
child handles would be copied/retained instead. In all cases the
original array remains unchanged and the extracted value may outlive it.

* When the last reference to an allocation is released, its children are
  dropped using the type descriptor. CoW cloning retains every managed child
  edge copied into the new allocation. The incoming replacement is retained
  **before** releasing the displaced value, preserving self-aliasing.
* Canonical types are interned by structure and record fields are sorted by
  field name. Child descriptors precede parents; well-typed heap ownership
  graphs are acyclic. Type IDs are descriptor indices, **not** encoded runtime
  pointer/ownership flags. `Types.runtime_storage`, `runtime_ownership` and
  `runtime_layout` describe generic MIR storage; the C emitter can specialize
  the actual layout further.

### Current native representation (generated `mir_emit_c.sirius`)

| Value or location | Current physical storage and refcount |
| --- | --- |
| `i64`, `f64`, `bool`, `Fn`, payload-free tagged union (enum) | Inline scalars (enum carries an `i64` alternative tag); no ARC |
| `string` | Pointer to `S_String` with its own non-atomic refcount and UTF-8 bytes |
| Eligible scalar-only record in a native register | C aggregate `S_MirInline<TypeId>`; **no register-level record allocation/refcount** |
| Other record registers, boxed records in heap slots | `S_MirRecord*`, with `type_id`, `refs`, `length`, `capacity`, and `S_MirField[]` |
| `Array[T]` (ordinary) | Refcounted `S_MirRecord` array buffer; one `S_MirField` slot per element; managed elements hold owned handles (records often separately boxed) |
| `Array[T]` of scalar-only records | One refcounted array buffer containing separate refcounted record objects; array elements are **not** packed inline |
| `Dict[K,V]` | Refcounted MIR wrapper owning a separate refcounted `S_Dict` hash table; table entries have type-directed key/value copy/drop hooks |
| `Optional[T]` | Managed MIR wrapper with zero or one owned payload slot (even for scalar payloads at present) |
| Tagged union with payload | Managed tag-plus-owned-record wrapper; heterogeneous alternative payload sizes are supported through separate record objects |

The current *inline-register* eligibility test is intentionally narrow:
records with **1–4 fields** of `i64`, `f64`, or `bool` only. Union
alternatives and record types serving as dictionary keys/values are excluded.
Eligible records are boxed when stored in heap containers, including arrays;
indexed reads unbox them into inline C aggregates. This is an
**implementation-specific specialization**, not a universal record ABI:
current eligibility also depends on other types appearing in the compiled
program.

The default payload-bearing union layout has a separately refcounted wrapper
and record payload. `MIR_RECORD_NEW_FUSED` expresses a constructor hint;
`SIRIUS_EXPERIMENTAL_UNION_COALLOC` can put wrapper and payload in one
allocation **with two independent ARC identities**, but is opt-in and not the
default layout. The opt-in `SIRIUS_EXPERIMENTAL_UNION_HYBRID`
variant coallocates only fresh scalar-record alternatives with at most three
fields and leaves wider/managed alternatives separately boxed. It retains
two ARC identities, the existing union pointer ABI and the current array
element storage; it is not a true inline-array representation. All-empty-record unions avoid the wrapper entirely (although extracting
an empty record payload currently materializes a record). `Fn`
values are noncapturing references and need no closure environment or ARC.
The runtime also has generic `S_Array` and scalar-specialized helpers; those
runtime layouts should not be conflated with the generated MIR-record arrays.
See `runtime/sirius_runtime.c` and `runtime/sirius_runtime.h`.

## 2. MIR instruction contracts

| Operation | Source(s) | Destination / child edges |
| --- | --- | --- |
| `MIR_COPY`, `MIR_RETAIN` | Borrow | Copy into independently owned destination, retaining managed allocation(s) as required before replacing its old value |
| `MIR_MOVE` | Consume one managed source | Transfer owned value without retaining; invalidate source, drop prior destination value |
| `MIR_RELEASE`, `MIR_RELEASE_IF_INIT` | Consume one (or conditionally owned) register | Drop owned value (all its ARC obligations) and invalidate register |
| `MIR_RECORD_NEW`, `MIR_ARRAY_NEW` | Borrow each input, including repeated registers | Construct an owned value: inline eligible record or allocated root; copy/retain inputs **per managed child slot** |
| `MIR_RECORD_NEW_MOVE`, `MIR_ARRAY_NEW_MOVE` | Borrow other fields; consume selected distinct managed inputs | Move selected child ownership into slots; copy/retain others |
| `MIR_RECORD_NEW_FUSED`, `MIR_RECORD_NEW_FUSED_MOVE` | Same ownership as corresponding record constructors | Hint for optional native wrapper/payload coallocation; the two objects retain independent ARC identities when enabled |
| `MIR_RECORD_GET`, `MIR_ARRAY_GET`, optional/dictionary reads | Borrow container | Copy selected value into an owned destination: normally retain its managed storage, or copy inline fields plus their credits; source keeps its ownership |
| `MIR_DICT_KEY`, `MIR_DICT_VALUE` | Borrow dictionary at a valid read-only cursor | Retain projected managed key/value; source and snapshots retain their credits |
| `MIR_UNION_NEW`, `MIR_UNION_TAG`, `MIR_UNION_GET` | Constructor borrows a payload, tag read borrows; payload projection borrows wrapper | Managed union owns its selected record payload; reading the payload produces a new owned result; payload-free enum remains an unowned tag |
| `MIR_UNION_NEW_MOVE` | Consume a last-use owned record payload | Transfer the payload credit into a non-enum wrapper without a retain/release pair; invalidate the payload register |
| `MIR_FUNCTION_REF`, `MIR_CALL_INDIRECT` | Function reference is a non-owning scalar; indirect arguments are borrowed | Callee receives retained managed arguments and returns an owned value, like a direct call |
| `MIR_OPTIONAL_SOME` / `NONE` | Borrow payload / no payload | Create owned optional; `SOME` retains managed payload |
| `MIR_RECORD_SET`, `MIR_ARRAY_SET`, append, `MIR_DICT_INSERT` | Consume parent; borrow replacement / key / value | Retain incoming managed child; perform CoW if shared; release displaced child on replacement; return owned parent |
| `MIR_CALL` | Borrow each argument | Callee receives an owned copy per managed argument position; native emission retains these arguments |
| `MIR_CALL_MOVE` | Consume selected distinct managed argument registers; borrow the rest | Transfer selected values into callee parameters; copy/retain other managed arguments |
| `MIR_RETURN` | Transfer result | Returned owned register becomes the caller's/entry's one owned result; all other managed registers must be unowned |
| `MIR_PANIC` | No result | Explicit CFG cleanup releases managed registers in the current MIR frame; a process-fatal trap does not unwind caller frames |

`MIR_CALL_MOVE` and constructor moves use `first` plus a position mask in
`second`; no duplicate or destination-register move is allowed. The verifier
checks type signatures and transfer positions, and the ownership analysis
checks that every moved source was initialized. An argument used multiple
positions must be borrowed/retained instead. The runtime must retain an
incoming aliased child **before** dropping a replaced field, and evaluate
call arguments before changing caller slots. The table states logical MIR
semantics; the current native emitter does not perform borrow elision.

## 3. Analysis and compiler pass sequence

1. Structural/type verification (`mir_verify`) validates the type table,
   register operands, CFG targets, call signatures, and legal move masks.
2. Forward must-initialization and may-ownership propagation uses 63-bit
   packed masks. Entry parameters start owned. At CFG joins, **must** uses
   intersection; **may** uses union. Fixed-point worklists handle loops.
3. Backward CFG liveness (`mir_lifetime`) adds last-use releases; the guard
   pass gives checked operations explicit failure successors; exit cleanup
   (`mir_cleanup`) handles branch-dependent owners with conditional releases.
4. `mir_check_balance` rejects uninitialized reads, use-after-move, double
   release, inconsistent register-credit transitions, and any managed owner
   potentially remaining at a reachable `RETURN` or `PANIC` except the
   returned value. Checks run on both must/may states.
5. The new **type-directed nested-credit contracts** (`mir_credit_shapes`,
   `mir_credit_heap_function`) account for child edges in constructors and CoW
   updates, projected child retains, and interprocedural argument ownership.
   Each function is checked; a call assumes the verified callee consumes its
   input-frame credits and transfers exactly one owned result on return.
6. Verified MIR C emission may elide redundant runtime initialization checks
   and frame-exit sweeps; move-aware MIR can avoid ARC pairs (see below).
   Defensive checked C emission retains initialization checks and exit
   cleanup; the MIR interpreter follows the original owned-value credit
   contracts with its model heap.

Register-slot reuse is a *physical storage* optimization. Separate MIR
registers may share C storage only when their possible managed ownership
intervals cannot overlap, including across CFG joins and moved values.

### Existing ownership and memory optimizations

These are implemented, not merely proposed:

* **Move-aware MIR:** local last-use `COPY`/`RETAIN` + `RELEASE` folding;
  `MIR_MOVE`, `MIR_CALL_MOVE`, moving record/array/union constructors, consuming
  CoW updates, and register-slot reuse based on non-overlapping ownership
  intervals. The ownership verifier still checks the resulting MIR.
* **Unique-root CoW:** native records/arrays and dictionaries reuse owned
  storage when its refcount is one. Shared versions clone the modified
  parent/path and retain the child edges copied into the new storage.
  Scalar-only record array elements are still boxed and refcounted.
* **Inline scalar records:** eligible scalar-only records use native
  aggregate registers, with boxing/unboxing at heap-container boundaries.
  Managed-field records and array elements remain boxed.
* **Type-directed native slot helpers:** `mir_c_slot_copy`,
  `mir_c_slot_retain`, `mir_c_slot_drop`, plus generated scalar-record
  box/unbox routines, connect logical value operations to current C layouts.

**Not yet implemented:** inferred read-only call parameters, borrowed array
projections, cross-block borrow plans, and deferred owner releases. Native
`MIR_CALL` retains managed arguments; `MIR_CALL_MOVE` transfers selected owned
arguments; `MIR_ARRAY_GET` and `MIR_RECORD_GET` return independent owned values.
HIR callback-alias inference supports higher-order specialization; it is not
a MIR ARC/lifetime borrow analysis.

Before introducing native borrow elision, keep the heuristic planner separate
from a conservative checker over verified MIR. A checked plan should identify
the owning root and projection path, the borrowed use interval (initially one
basic block), conflicts with moves, releases and CoW updates, any callee
read-only assumptions, and the release sites on all relevant exits. Reject
unsupported CFG shapes rather than approximating them. Emit only checked plans
after lifetime/cleanup/slot-layout decisions, or invalidate and recheck plans
after later transformations. Retain the existing owned call ABI across module
and compiled-library boundaries until an explicit borrowing ABI is designed.
An internal borrow need not change the serialized `.smirg` interface when
there is no externally observable borrowed calling convention.

## 4. Failure and result ownership

Failure handling is divided by its **control-flow effect**, not by whether an
operation can fail:

1. **In-band outcomes:** `Optional`/status values and floating-point nonfinite
   results use the ordinary return/branch path. An executable constructor-based
   `Result = Success | Failure` union uses the same contract: the selected
   alternative owns its managed payload, returning transfers the union's one
   root credit, and matching/projection borrows or transfers children by normal
   MIR rules. An exhaustive `match` is checked by HIR when unions exist; no
   exceptional unwind is required. A call may also fail fatally during
   allocation even when it returns an in-band status for ordinary failures.
2. **Explicit fatal traps:** integer division/remainder invalid operands,
   invalid shifts, unchecked bounds and forced optional unwrap lower to
   `MIR_PANIC` CFG edges with cleanup in the current function. An in-band
   alternative, such as `array.get(i)` returning `Optional`, has no panic edge
   for absence. Fatal traps may leave live caller frames because they terminate
   the process; ownership is not promised after that process ends.
3. **Implicit fatal aborts:** out-of-memory, invalid runtime invariants, and
   execution budgets can terminate without a MIR cleanup edge. They are not
   recoverable language exceptions. Dynamic execution may report an execution
   failure to its host, but Sirius code cannot catch it as a value.

`MirFailureInfo` and `MirFailureAudit` report explicit guards, possible fatal
aborts and in-band outcomes independently. A result-producing operation is
never assumed infallible merely because it encodes an ordinary failure as a
value. The native shadow-credit audit checks retain/drop consistency until
`s_panic`; its exit-time **leak check applies only on normal termination**.
This prevents process-fatal frames from being misreported as leaks while
still catching real leaks on successful runs.

IEEE-754 `0.0 / 0.0` produces NaN; finite nonzero division by zero normally
produces signed infinity. Integer division does not have a NaN/sentinel
representation under the current language semantics.

## 5. The boundary of the static proof

The static proof is **compositional over MIR instruction contracts**. It
establishes that each register/frame and typed child edge has a well-defined
acquire/transfer/drop obligation on every *reachable CFG path*, conditional
on the interpreter/C runtime implementing the instruction contracts. The
nested-credit shape is symbolic: arrays can hold arbitrarily many child
edges, optionals zero or one, and dictionaries one key and value per entry.
Shared children are counted per edge, not per distinct allocation. Call
arguments are passed as one credit each; the callee is verified separately.

It is **not** an independently verified theorem about generated C or the ARC
runtime. Native C now has an **opt-in dynamic shadow-credit audit**:

* Build **both** generated C and `runtime/sirius_runtime.c` with
  `-DSIRIUS_ARC_AUDIT` (the normal runtime and compiler are unaffected).
* Every managed allocation in the MIR-record emitter and built-in string,
  array, and dictionary runtime registers one shadow credit. Retains and
  releases update both the actual counter and the shadow ledger; reallocations
  update the object's address. The auditor rejects credit mismatches.
* The emitted MIR C registers a **type-directed scanner** for managed
  records, arrays, optionals, payload-bearing union wrappers and dictionary
  wrappers. Scalar-only record elements are boxed and scanned as record
  child edges; payload-free enums have no ARC root. Generic MIR dictionaries
  register key/value
  managedness with the native hash table, and CoW clones preserve these
  scanner settings. Each occupied dictionary entry and repeated child is
  counted separately, independently of retain/release hooks.
* Each generated MIR function registers its **owned** managed C register
  slots in an audit-only stack frame. Eligible inline scalar-only records
  contain no managed field slots, while boxed records have owned roots.
  Physical slot reuse is handled naturally: each live physical slot is
  enumerated once, including suspended caller frames. Debug scans read current slot contents, rather
  than stale cached pointers after a move or CoW.
* On the ordinary owned ABI, `MIR_CALL_MOVE` evaluates arguments into C
  temporaries and clears transferred caller slots **before** entering the
  callee; each credit appears once at stable checkpoints. Ordinary
  calls retain managed arguments, while moved calls transfer selected credits.
  The frame is removed on normal return.
* At the end of each generated C basic block, `s_arc_audit_check_frames()`
  independently scans heap-child edges plus all registered MIR frame roots
  and known native global roots, comparing **exact** ownership credits.
  It catches dangling children, missing credits, surplus root credits, and
  duplicated or omitted roots. The command-line argument cache is a registered
  native global root with a string-element edge scanner.
* `s_arc_audit_check_roots(roots, count)` remains available for isolated
  explicit-root snapshots in tests or non-MIR integrations. Its root list must
  include every outstanding root; it does not implicitly combine frame roots.
* On **normal exit**, an exit handler checks for outstanding managed objects
  **after** command-line argument cleanup; fatal `s_panic` deliberately skips
  the final leak check.
* `python tools/test_mir_native_arc.py` checks allocations, retains, releases,
  reallocations and exit leaks in native generated MIR.
* `python tools/test_mir_heap_edges.py` checks exact nested credits for returned
  record/array and dictionary/optional graphs, plus injected missing-credit,
  dangling-child and surplus-root-credit faults.

This is execution-specific verification of the C ARC operations, **not** a
proof that every heap edge is properly retained in all possible executions.
In particular:

* The MIR type-directed scanners reconstruct heap edges only for supported
  generated containers. The CLI argument array is registered, but other
  ordinary native containers and arbitrary FFI objects are not yet
  exhaustively scanner-registered. Exact frame-wide snapshots require every
  long-lived native/global root and managed child edge to be registered; those
  integrations remain outside the automatic guarantee.
* `mir_audit_return_credits` dynamically checks reference counts in the MIR
  interpreter heap reachable from a **successful** returned root, including
  repeated nested child edges. The native scanner reconstructs supported heap
  edges at observed block boundaries; neither is a static proof over all paths.
* Allocation failures, runtime aborts, recursion/step-limit failures,
  and other **non-unwinding** terminations bypass cleanup of live caller
  frames. This is permitted for process-fatal failures, not recoverable errors.
* C runtime, dictionary callbacks, and CoW helper implementations remain
  trusted. Native/FFI objects and their roots are not automatically covered;
  generic dynamic `Any` boxing and future library ABIs need their own
  ownership contracts. Current tagged unions and noncapturing `Fn` values
  are supported by their existing contracts, not a general dynamic-object ABI.
* No threading, concurrency, cycle collector, or general source-level
  borrowed views are covered by the current model.

A dedicated normal-return regression (`tools/test_mir_inband_ownership.py`) exercises
`Optional.Some` and `Optional.None` through borrowed and moved calls. A shared
record/array graph carries duplicate child credits into the callee; both
branches are checked by the static verifier, interpreter heap-credit audit and
native `SIRIUS_ARC_AUDIT`. Negative cases reject missing callee-frame cleanup
and reads after a moved argument. This is **path coverage**, not a proof that
all possible active heap edges are correct in native code.

Remaining boundaries: register any additional native/FFI roots and edge
scanners; expand nested managed-value and union/CoW stress coverage; and
define and check any future native borrow plans before using them at all.
Container-inline managed values, fully inline heterogeneous union payloads, and representation
across compiled-library boundaries still need layout/ABI design and measured
validation. Process-fatal unwinding is deliberately not required. A fully
heap-wide static alias/reference-count proof would need a stronger heap model.

## 6. Regression entry points

- `python tools/test_mir_ownership.py`, `test_mir_balance.py`,
  `test_mir_ownership_combined.py`: reads, joins, loops, return cleanup.
- `python tools/test_mir_call_move.py`, `test_mir_constructor_move.py`:
  move eligibility and native/interpreter parity.
- `python tools/test_mir_credits.py`: nested returned graphs, duplicated
  edges, CoW, CFG cleanup and optional interpreter refcount audit.
- `python tools/test_mir_nested_credits.py`: shape metadata, nested
  borrowed/moved call frames, lost callee credit and use-after-transfer.
- `python tools/test_mir_frame_roots.py`: exact frame/global credits across
  nested borrowed and moved calls, both Optional paths, and injected faults.
- `python tools/test_mir_native_arc.py`: instrumented native ARC across
  nested containers, CoW, call frames and injected leaks/credit mismatches.
- `python tools/test_mir_guard.py`, `test_mir_failure_contract.py`:
  guarded failures and known non-unwinding failures.
- `python tools/test_mir_scalar_slots.py`, `test_mir_value_slot_reads.py`:
  inline record registers, owned reads and CoW behavior.
- `python tools/test_union_scalar_enums.py`, `test_union_coallocation_header.py`:
  scalar enums and optional coallocated wrapper/payload layout.
- `python tools/test_bootstrap.py`: self-hosting and compiler fixtures.
