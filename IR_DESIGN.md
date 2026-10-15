# Sirius compiler IR

**Implemented pipeline:** `src/tokenizer.sirius` → `src/parser.sirius` → typed
HIR/specializations → CFG MIR → ownership/optimization → **C11 or the MIR
interpreter**. The default self-hosted compiler uses `src/cli.sirius` and
`src/mir_emit_c.sirius`; the alternative `src/legacy_cli.sirius` and
`src/emit_c.sirius` are compiled separately. Do not confuse the retained
**Sirius** legacy emitter with the removed Python stage-0 compiler.

The intended semantics are specified in [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md).
Execution/serialization are documented in [INTERPRETER.md](INTERPRETER.md), and
outstanding work in [ROADMAP.md](ROADMAP.md).

## Representations and phase boundaries

| Layer | Current representation | Responsibility |
| --- | --- | --- |
| Syntax | Indexed `ParseState` with `Node`/child-edge arrays and source spans | Tokenization, precedence, indentation and declaration syntax |
| Types | `TypeTable` of interned, canonical structural `TypeId`s | Primitive/container descriptors and sorted record fields |
| HIR | AST-referenced typed `HirNode`s and sparse per-node annotation index | Resolve names, field indices, signatures and supported semantics |
| Specializations | `HirSpecializationRegistry` and typed worklist bodies | Discover concrete calls (including recursive/mutual references), infer supported returns |
| MIR | `MirProgram` → `MirFunction` → typed registers, `MirBlock`s, instructions and terminators | Explicit CFG evaluation, calls, values and ownership |
| Backends | Verified MIR C emitter and Sirius MIR interpreter | Execute the same lowered, linked concrete program |

HIR specialization entries and MIR functions use the **same stable array
indices**; `MIR_CALL` references those IDs. Each reachable concrete body is
analyzed once for its declaration and canonical argument-type signature.
Recursive functions with unannotated return types remain restricted. Source
annotations preserve AST identities for diagnostics; the type table travels
with `MirProgram`, rather than being copied into a separate HIR result.

Record layouts are structural, with lexicographically sorted fields. The type
registry captures semantic identity, not a stable cross-version native ABI.
Some compatibility bridges to old C kind IDs remain in the separate legacy
emitter and selected layout helpers; avoid expanding them.


## Concrete HIR reuse

Concrete HIR specialization, including return type inference and user-call
resolution, is done before MIR lowering. The specialization worklist caches
fully analyzed bodies under `(declaration, argument TypeIds)` and reuses the
same typed annotations and discovered calls when emitting a previously inferred
callee. Return inference within one body analysis is also memoized; its key
includes the active call chain to retain the recursion guard.

MIR lowering consumes this typed result and still traverses AST structure for
CFG generation. MIR validation is independent: it checks instruction types,
control flow, and ownership, not HIR-level generic inference. This does not yet
make HIR a standalone typed syntax tree, so further HIR-to-MIR simplification
remains possible.

## MIR instructions and ownership

MIR uses typed registers, explicit function arguments, basic blocks and CFG
terminators (`branch`, `jump`, `return`, `panic`). Guards for fallible divisions,
checked indexing and optional unwrapping are lowered to explicit CFG failures.
`recur` evaluates all replacement arguments before branching to the function
entry, without another native call frame.

The key managed-value rules are:

- `MIR_COPY`/`MIR_RETAIN` create another owned reference; `MIR_MOVE` transfers
  ownership and invalidates its source; `MIR_RELEASE` drops a reference.
- Calls normally borrow/retain managed arguments. `MIR_CALL_MOVE` can transfer
  **distinct last-use** owned argument registers; duplicate arguments remain
  borrowed. Returns transfer the selected owned result to the caller.
- `MIR_RECORD_SET`, `MIR_ARRAY_SET` and appends **consume** their owner. A unique
  owner can update in place; a shared one clones, retaining managed elements
  and preserving earlier snapshots. `MIR_RECORD_ARRAY_APPEND` avoids creating
  a temporary alias for the direct `r.items = r.items.append(x)` pattern.
- Nested checked lvalue updates evaluate index expressions once and rebuild the
  value path from leaf to mutable root. Field destructuring evaluates its source
  once and extracts typed `MIR_RECORD_GET`s.
- Rank-1 collections, dictionaries and `Optional[T]` use canonical element
  types. Optional access and lazy fallback branch through CFG; the internal
  eager `MIR_DICT_GET_OR` is not the implementation of source `or`.

The interpreter and native backend agree on owned-value behavior for tested
paths. The interpreter uses a logical managed heap; emitted C uses native
reference counts and type-directed copy/drop operations. See
[RUNTIME.md](RUNTIME.md) for their different container storage strategies.

## CFG transformation sequence

The production MIR build performs typed lowering and structural checks, then
inserts fallibility guards, last-use releases and edge/exit cleanup, followed
by ownership-aware peephole optimization and final verification. Relevant
components:

| Source | Responsibility |
| --- | --- |
| `mir_lower.sirius`, `mir_program.sirius` | HIR-to-MIR and linked specialization program |
| `mir_guard.sirius` | Explicit guarded trap edges |
| `mir_ownership.sirius` | Must-initialize / may-own dataflow, invalid-read/move checking, balance |
| `mir_lifetime.sirius` | Backward register liveness and last-use releases |
| `mir_cleanup.sirius` | Edge splitting and conditional/exit cleanup |
| `mir_opt.sirius` | Eliminating redundant owned copies and transfers |
| `mir_common.sirius`, `mir.sirius` | Shared classification, CFG facts, IR definitions and verification |
| `mir_emit_c.sirius`, `mir_interpreter.sirius` | Native code generation and dynamic execution |

`mir_cfg_facts` computes reachability and predecessor lists **for a fixed CFG**;
recompute after block insertion or terminator rewriting. The combined compiler
pipeline avoids repeatedly verifying unchanged intermediate MIR, but its final
balance/failure checks remain authoritative. Standalone passes and the
**defensive** `mir_emit_c` entry point still validate independently supplied
MIR. `mir_emit_c_verified` elides redundant C register-initialization checks
and function-wide exit sweeps after explicit ownership cleanup and balance
verification. This does not make implicit failure unwinding complete.

## Worked lowering examples (schematic, not serialized syntax)

**Copy-on-write array update:**

```sirius
var a = [10, 20, 30]
let snapshot = a
a[!1] = 99
```

```text
r0 = ARRAY_NEW(10, 20, 30)
snapshot = COPY(r0)       ; retains the original array
r1 = ARRAY_SET(r0, 1, 99) ; consumes r0; clones because snapshot owns it
...                      ; r1 differs from snapshot
RELEASE(snapshot)
RELEASE(r1)
```

**Optional early exit:**

```sirius
let x = values[?index] else return false
```

```text
opt = ARRAY_GET_OPTIONAL(values, index)
branch OPTIONAL_HAS(opt), found, missing
missing: return false
found: x = OPTIONAL_UNWRAP(opt)
```

**Tail recurrence:** `recur factorial(n - 1, acc * n)` evaluates `n - 1`
and `acc * n` before updating the arguments and jumping to the entry block.

## Storage choices and limitations

- The HIR annotation index avoids repeated scans of typed nodes sharing an AST
  source index. The parser still uses CoW-backed arenas.
- General-purpose MIR instructions retain operand fields and input arrays even
  when an opcode needs fewer operands. Compacting this representation would
  affect the verifier, optimizer, emitter and interpreter together; defer until
  profiling justifies it.
- Ownership CFG states are currently dense register masks with cached block
  effects and shared initial states. Their CoW cloning and parser arena appends
  are material costs in self-compilation; see [MIR_PROFILE.md](MIR_PROFILE.md).
- The default implementation covers the tested scalar/managed subset, **not**
  unions, multidimensional vector kernels, arbitrary dynamic `Any`, or complete
  abnormal-exit cleanup. Future generic libraries also require portable
  definition and specialization identities beyond process-local `TypeId`s.

Test changes with `tools/test_hir_worklist.py`, the focused
`tools/test_mir_*.py` suites, native/interpreted parity, and the bootstrap
fixed-point check before changing representation or ownership semantics.
