# Compiler refactoring notes (October 2026)

These record local representation decisions and compatibility constraints.
For the active source map, see [Compiler structure](../COMPILER_STRUCTURE.md).
Paths are relative to the repository root.

## Shared helpers

`mir_cfg_state_equal` now contains the common dense `Array[i64]` comparison.
The existing `mir_live_equal` and `mir_ownership_equal` exports delegate to it,
so native test drivers and clients keep their existing names. All managed-type tests in verification and both execution backends now use
the canonical `Types.runtime_ownership()` classification through
`mir_managed_type`. The isolated floating-point literal parser has moved out of the interpreter without
changing its parsing algorithm or the MIR wire format.

## Using current Sirius constructs

Prefer constructing records directly when all fields are available, and use
record spreads for updates that must preserve other fields. In particular,
module-qualified name rewriting now shares one AST update path: conversions
from attributes to names discard child edges, while type and `match` patterns
retain their existing children and bindings.
HIR overload selection now uses an exhaustive outcome union (`found`, `absent`,
`rejected`) instead of a negative index combined with an error string. The
resolved type table remains in its containing record because overload
annotation lookup may intern new types. This is a local representation change;
AST and MIR opcode storage remain unchanged.

HIR match exhaustiveness checks now return a tagged outcome (`complete`,
`invalid arm`, `repeated alternative`, or `missing alternative`) rather than a
string error and an optional sentinel TypeId. Both the record-TypeId and
label-index entry points share the same checking implementation. Callers
handle all outcomes exhaustively; existing source diagnostics are unchanged.

Callback inference now uses a distinct tagged result (`resolved`,
`underdetermined`, `conflicting`, or `cyclic`) at HIR call sites, handled with
exhaustive matches. The recursive callback probe retains its independent
signature, conflict and cycle facts: those may coexist while exploring
forwarding paths, so forcing them into a single traversal-state variant would
lose information. The result priority remains conflict, resolved signature,
cycle, then underdetermined. Neither HIR/MIR serialization nor callback
calling conventions change.

Additional independent success/failure results are now explicit:

- `HirRecordEdit` keeps edited fields outside a `HirRecordUpdated(index)` /
  `HirRecordEditError(message)` outcome. Callers match before using changed
  fields; the successful index may be negative for an append or whole spread.
- `HirConstantEval` retains its type/value arenas and returns either
  `HirConstantFound(index)` or `HirConstantUnavailable()`. The latter represents
  an expression that cannot be evaluated as a module constant, not a runtime
  error. Recursive evaluation propagates this result exhaustively.
- Shared HIR/MIR literal decoding returns `HirStringDecoded(value)` or
  `HirStringDecodeError(message)`; the MIR backend no longer reconstructs a
  second error-string result record.
- Module export signature formatting returns `ModuleScanTypeValid(text)` or
  `ModuleScanTypeInvalid()`. Invalid syntax remains distinguishable from an
  empty type spelling, including for nested array/dictionary/optional types.

This refactor deliberately leaves incremental analyzer state (`HirBodyState`,
`HirModuleDecl`, and specialization worklists) as records, and leaves
`HirCallCheck`, `HirResolution`, and parametric fast-path eligibility unchanged.
These carry canonical typing/deferred-inference details across many callers;
changing them requires a separate contract and regression analysis, rather
than mechanically wrapping every field in a union. It also leaves compiler
IR opcode integers unchanged.

Sirius `match` is exhaustive dispatch on **tagged union values**, not a switch
on arbitrary `i64` tags. MIR opcodes, terminator kinds and AST node kinds are
currently integer constants; rewriting their dispatch into `match` would first
require a representation change. Keep those checks explicit unless that change
has a separate demonstrated benefit. Also avoid adding record spreads to hot
interpreter heap-reference updates solely for syntax: retain/drop costs and CoW
ownership require separate validation.
