# Compiler source organization

> Paths and commands in this document are relative to the repository root.

The production compiler is a Sirius module graph rooted at
`src/modular_cli.sirius`. The MIR execution/tooling CLI is
`src/run_cli.sirius`; the standalone source scanner uses
`src/module_scan_cli.sirius`. The generated `bootstrap/compiler.c` is an
artifact, not an independent source implementation. The historical flat-source
manifest and AST-to-C emitter are retired.

## Modules and responsibilities

| Area | Files | Responsibility |
| --- | --- | --- |
| Syntax | `tokenizer.sirius`, `parser.sirius` | Lexing and syntax trees |
| Types | `types.sirius`, `module.sirius` | Canonical descriptors and structural signatures |
| HIR | `hir*.sirius` | Typed declarations, body analysis, resolution and specialization |
| MIR representation | `mir.sirius`, `mir_cfg.sirius`, `mir_common.sirius` | Typed MIR, CFG facts and shared execution semantics |
| MIR construction | `mir_lower.sirius`, `mir_program.sirius` | HIR-to-MIR lowering and program assembly |
| MIR passes | `mir_verify.sirius`, `mir_ownership.sirius`, `mir_lifetime.sirius`, `mir_guard.sirius`, `mir_cleanup.sirius`, `mir_opt.sirius` | Verification, ownership/dataflow, explicit failure edges, cleanup and optimization |
| MIR execution | `mir_interpreter.sirius`, `mir_float.sirius` | Dynamic execution and decimal literal decoding |
| C backend | `mir_emit_c.sirius`, `compile_common.sirius` | Runtime/code emission and compile result type |
| Modules and packages | `module_*.sirius`, `mir_{wire,semantic,generic,template,library,link,bundle}.sirius` | Module discovery, serialization, generic packages and library linking |

Only `hir_parametric.sirius` is currently test-only: its symbolic HIR plan is
exercised by `tests/hir/analysis_module_driver.sirius`, but the production
compiler uses the concrete HIR specialization path. The former
`mir_parametric.sirius` was a disconnected legacy flat-snapshot experiment;
its declarations did not resolve as a native module and no active tool imported
it, so it was removed. Historical versioned artifacts are retained as fixtures.

`mir_cfg_state_equal` now contains the common dense `Array[i64]` comparison.
The existing `mir_live_equal` and `mir_ownership_equal` exports delegate to it,
so native test drivers and clients keep their existing names. All managed-type tests in verification and both execution backends now use
the canonical `Types.runtime_ownership()` classification through
`mir_managed_type`. The isolated floating-point literal parser has moved out of the interpreter without
changing its parsing algorithm or the MIR wire format.

## Token-kind union boundary

`tokenizer.TokenKind` is an exhaustive, labeled union used when classifying
lexical kinds and reporting errors. The tokenizer retains `Token.kind: i64`
for now: its signed codes are part of the existing token and generic-AST
serialization contract, and numeric field access remains pervasive in parser,
module scanning and HIR lowering. `token_kind_from_code` converts numeric
codes into the tagged view, while `token_kind_code` maps back explicitly. An
unrecognized code maps to `TkUnknown`, not to a valid source token.
`token_is_error` and `token_label` use exhaustive matches.

The zero-based `TokenKind.index(TkName)` discriminant is **not** the old
`TOKEN_NAME` numeric code. Do not serialize a union discriminant in place of
an existing token code. Converting `Token.kind`, AST `Node.kind`, HIR operations
or MIR operations to tagged storage must be a measured, separately verified
schema change; all of those fields currently retain their numeric layout.

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

## Refactoring boundaries

Keep the following files separate, despite small sizes: `compile_common.sirius`
is the shared compile-result type; `mir_common.sirius` shares interpreter/emitter
checks; and the three CLI roots have different build and execution contracts.
Merging them would complicate independent native imports and entry points.

The largest remaining candidates for *future*, separately tested extraction are:

- `mir_emit_c.sirius`: generated runtime/ARC helpers versus per-instruction C
  emission; first define a narrow string-generation interface and verify the
  generated bootstrap fixed point.
- `mir_lower.sirius`: expression and statement lowering; avoid splitting
  mutable builder state across modules without first defining its interface.
- `mir_interpreter.sirius`: heap operations versus instruction dispatch;
  preserve owned-call frame and return-credit invariants when separating them.
- `parser.sirius`: expression versus statement parsing; avoid changes until
  parser-state ownership is explicit.

Do not combine HIR analysis, MIR verification and C emission solely to reduce
file count: their distinct responsibilities allow interpreter/native parity
and independently tested ownership proofs.

## Checks

`python tools/test_compiler_layout.py` checks imports and source reachability,
including the explicit test-only exception. `python tools/test_docs.py`
checks documentation links and rooted source/test file references. After any
production compiler change, run `python tools/test_bootstrap.py` and regenerate
`bootstrap/compiler.c` with `python tools/generate_bootstrap.py` only once
self-hosting reaches a fixed point. Use `python tools/test_all.py` for the full
suite; `python tools/test_hir_overload_outcomes.py` exercises the overload
outcome union directly; targeted MIR/dataflow tests are useful during small
refactors.
