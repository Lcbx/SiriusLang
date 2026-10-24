# Ownership in the current Sirius implementation

This describes the **implemented** MIR/ARC contract (October 2026), not the
whole language's proposed ownership semantics. Main sources: `src/mir.sirius`,
`mir_cfg.sirius`, `mir_ownership.sirius`, `mir_lifetime.sirius`,
`mir_cleanup.sirius`, `mir_verify.sirius`, `mir_interpreter.sirius`,
`mir_emit_c.sirius`, `types.sirius`, and `runtime/sirius_runtime.c`.

## 1. Values and ownership units

* `i64`, `f64`, and `bool` are scalar values without ARC credits. Managed
  `string`, `Record`, `Array`, `Dict`, and `Optional` have reference ownership.
* Each **initialized managed MIR register** holds exactly **one owned root
  reference**, not a borrowed pointer. There is no independent, long-lived
  borrow-register kind; reading an operand normally borrows its register.
* Each **populated managed field/element/entry** holds another credit on its
  child, independent of any register referring to the same object. Repeating
  the same child in two array slots creates two edges, not one.
* A composite owns its children; releasing its final reference recursively
  drops each owned child edge. Sharing the root retains **only the root**.
  Cloning a shared composite for CoW retains its managed children before
  replacing an edge. Record fields are sorted according to the canonical type.
* `TypeId` indexes canonical descriptors. `runtime_ownership` distinguishes
  scalar, string, and composite storage; `runtime_layout` provides the
  target-neutral payload-slot shape. The C ABI determines physical layouts.
* Canonical type descriptors are acyclic: composite child types are interned
  before their parents. Therefore well-typed managed child edges cannot form
  a type-preserving cycle. Sharing and repeated references are allowed.
  Malformed foreign/runtime heap objects are outside this guarantee.

## 2. MIR instruction contracts

| Operation | Source(s) | Destination / child edges |
| --- | --- | --- |
| `MIR_COPY`, `MIR_RETAIN` | Borrow | Retain managed source before replacing owned destination |
| `MIR_MOVE` | Consume one managed source | Transfer credit; invalidate source, drop prior destination credit |
| `MIR_RELEASE`, `MIR_RELEASE_IF_INIT` | Consume one (or conditionally owned) register | Drop credit and invalidate register |
| `MIR_RECORD_NEW`, `MIR_ARRAY_NEW` | Borrow each input, including repeated registers | Allocate one owned root and retain once **per managed child slot** |
| `MIR_RECORD_NEW_MOVE`, `MIR_ARRAY_NEW_MOVE` | Borrow other fields; consume selected distinct managed inputs | Move selected child credits into slots; retain others |
| `MIR_RECORD_GET`, `MIR_ARRAY_GET`, optional/dictionary reads | Borrow container | Retain selected managed result; container keeps its child credit |
| `MIR_OPTIONAL_SOME` / `NONE` | Borrow payload / no payload | Create owned optional; `SOME` retains managed payload |
| `MIR_RECORD_SET`, `MIR_ARRAY_SET`, append, `MIR_DICT_INSERT` | Consume parent; borrow replacement / key / value | Retain incoming managed child; perform CoW if shared; release displaced child on replacement; return owned parent |
| `MIR_CALL` | Borrow each argument | Caller retains once **per managed argument position** for callee frame; callee returns an owned result |
| `MIR_CALL_MOVE` | Consume selected distinct managed argument registers; borrow the rest | Transfer selected root credits into callee parameters; borrow-retain others; transfer callee return credit into caller destination |
| `MIR_RETURN` | Transfer result | Returned owned register becomes the caller's/entry's one owned result; all other managed registers must be unowned |
| `MIR_PANIC` | No result | Explicit CFG cleanup releases managed registers in the current MIR frame; a process-fatal trap does not unwind caller frames |

`MIR_CALL_MOVE` and constructor moves use `first` plus a position mask in
`second`; no duplicate or destination-register move is allowed. The verifier
checks type signatures and transfer positions, and the ownership analysis
checks that every moved source was initialized. An argument used multiple
positions must be borrowed/retained instead. The runtime must retain an
incoming aliased child **before** dropping a replaced field, and evaluate
call arguments before changing caller slots.

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
   and frame-exit sweeps, but **does not** elide ARC retain/drop operations.
   Defensive C emission retains its checks and exit cleanup. The interpreter
   follows corresponding credit rules with a model heap.

Register-slot reuse is a *physical storage* optimization. Separate MIR
registers may share C storage only when their possible managed ownership
intervals cannot overlap, including across CFG joins and moved values.

## 4. Failure and result ownership

Failure handling is divided by its **control-flow effect**, not by whether an
operation can fail:

1. **In-band outcomes:** `Optional`/status values and floating-point nonfinite
   results use the ordinary return/branch path. A future constructor-based
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
* The emitted MIR C registers a **type-directed scanner** for every managed
  record, array, optional and dictionary wrapper. Generic MIR dictionaries
  register their key/value managedness with the native hash table; CoW clones
  preserve these scanner settings. Each occupied dictionary entry and repeated
  child reference is counted separately, independently of retain/release hooks.
* Each generated MIR function registers the addresses of its managed C register
  slots in an audit-only stack frame. Physical register-slot reuse is handled
  naturally: each live physical slot is enumerated once, including slots in
  suspended caller frames. Debug-only root scans read current slot contents,
  rather than caching pointer values that could become stale after a move or CoW.
* `MIR_CALL_MOVE` evaluates arguments into C temporaries, then clears moved
  caller slots **before** entering the callee. Retained (borrowed) arguments
  and moved credits appear exactly once in the active callee frame at the next
  stable checkpoint. The frame is removed on normal return.
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
  trusted. Native/FFI objects and runtime registration are not yet in this
  ownership proof; nor are future union/function/`Any` representations.
* No threading, concurrency, cycle collector, or general borrowed views are
  covered by the current model.

A dedicated normal-return regression (`tools/test_mir_inband_ownership.py`) exercises
`Optional.Some` and `Optional.None` through borrowed and moved calls. A shared
record/array graph carries duplicate child credits into the callee; both
branches are checked by the static verifier, interpreter heap-credit audit and
native `SIRIUS_ARC_AUDIT`. Negative cases reject missing callee-frame cleanup
and reads after a moved argument. This is **path coverage**, not a proof that
all possible active heap edges are correct in native code.

Remaining P0 work: register additional native/FFI container roots and scanners; verify the future union injection/match/drop paths when implemented. Process-fatal
unwinding is deliberately not required. A heap-wide static alias/reference-count
proof would require a stronger heap abstraction.

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
- `python tools/test_bootstrap.py`: self-hosting and compiler fixtures.
