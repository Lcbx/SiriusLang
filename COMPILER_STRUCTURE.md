# Compiler source organization

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
and independently tested ownership proofs. Similarly, avoid converting a
historical serialization compatibility fixture into compiler source.

## Checks

`python tools/test_compiler_layout.py` checks imports and source reachability,
including the explicit test-only exception. `python tools/test_docs.py`
checks documentation links and rooted source/test file references. After any
production compiler change, run `python tools/test_bootstrap.py` and regenerate
`bootstrap/compiler.c` with `python tools/generate_bootstrap.py` only once
self-hosting reaches a fixed point. Use `python tools/test_all.py` for the full
suite; targeted MIR/dataflow tests are useful during small refactors.
