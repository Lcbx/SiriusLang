# IR migration notes

Historical milestones, not the current feature inventory. Later documentation
and implementation supersede fixture counts and restrictions below. See
[IR design](../IR_DESIGN.md), [Interpreter](../INTERPRETER.md) and
[Development](../DEVELOPMENT.md) for the maintained pipeline.

### Historical source-to-MIR migration

The HIR worklist now resolves named `rec` return/parameter annotations by canonical published fields, including nested `Array`/`Optional`/`Dict` annotations. Constructor calls are lowered directly into `MIR_RECORD_NEW` in published-field evaluation order, with canonical physical ordering. Top-level integer constants are resolved independently of the C emitter. The lowering additionally supports empty typed arrays, array append (`MIR_ARRAY_APPEND`, consuming owner), array/range `for`, loop control, expression statements, and CLI/file/string conversion intrinsics. Native MIR-C initializes the arguments runtime and converts CLI strings into MIR-owned arrays; MIR interpreter file reads are deferred. MIR-only native self-hosting reaches a fixed point and all 91 executable fixtures pass. Two historical negative fixtures are accepted by MIR as valid programs (floating-point arrays and `elif` optional fallback).

## Phase B migration status

HIR generic specialization lookup and lifecycle, symbol binding metadata and structural record-field lookup now use canonical `TypeId`s. The MIR C backend selects layout, field variants and managed helpers from canonical descriptor tags; scalar identity checks in parts of HIR/MIR lowering still compare reserved primitive TypeIds. The module declaration HIR pass precedes emission and supplies function headers; pre-emission body traversal now resolves local lexical scopes, supported scalar and record/collection expressions, indexing modes and selected calls; complete typed bodies for every language feature remain future work; a typed CFG MIR subset is implemented.

