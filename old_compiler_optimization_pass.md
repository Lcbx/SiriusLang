# Sirius — Reference-counting overhead: findings and improvement plan

## 1. Summary of findings

The analysis of `bootstrap/compiler.c` reveals substantial generated-code duplication, primarily caused by ownership cleanup.

Generated C

# 9.52 MB

311,038 lines

Lines containing releases

# 95,847

Including runtime code

Cleanup-only blocks

# 46.9%

Of generated C basic blocks

The LZW-style analysis identified several major sources of repetition:

- Repeated cleanup sequences. Identical groups of up to 15 releases appear dozens or hundreds of times, frequently at different control-flow exits.
- Duplicate cleanup blocks. Approximately 1,200 additional occurrences of identical complete C block bodies were identified.
- Redundant releases. There are 990 occurrences of immediately repeated identical release statements. Some result from physical register-slot reuse.
- ARC audit instrumentation. One audit checkpoint per emitted basic block contributes substantially to source size, although these calls are compiled away when auditing is disabled.

These measurements concern generated C, not necessarily executable size or execution time. The file compresses from 9.52 MB to about 604 KB, confirming extensive textual repetition.

### Comparison with other languages

Swift and Nim demonstrate that reference counting benefits greatly from compiler-level ownership optimization.

Swift uses ownership-aware intermediate representation, borrowed values, move semantics and ARC optimization. Nim's ARC/ORC implementation similarly uses move and cursor inference to reduce unnecessary reference-count changes.

Go and D primarily use tracing garbage collection, so they have different memory-management overheads and avoid explicit releases at most control-flow exits.

Conclusion: Sirius's cleanup duplication is not an inevitable cost of reference counting. Much of it should be reducible without changing the language's memory model.

## 2. Existing Sirius infrastructure

Inspection of the current sources confirms that much of the necessary infrastructure already exists.

| Component              | Existing capability                  | Main opportunity                                             |
| ---------------------- | ------------------------------------ | ------------------------------------------------------------ |
| `mir_ownership.sirius` | Must/may ownership dataflow          | More precise elimination of unnecessary ownership operations |
| `mir_lifetime.sirius`  | Liveness and last-use releases       | Improve release placement                                    |
| `mir_cleanup.sirius`   | Path-sensitive edge and exit cleanup | Reduce duplicated cleanup sequences                          |
| `mir_opt.sirius`       | Move folding and CFG compaction      | Extend CFG deduplication                                     |
| `mir_emit_c.sirius`    | Physical register-slot reuse         | Eliminate redundant emitted operations                       |

There is one especially clear limitation in `mir_compact_cfg`: it only attempts equivalent-block merging when blocks contain two or fewer instructions.

The largest repeated cleanup sequences therefore escape the existing optimization.

The pass order is already appropriate:

Guard insertion and lifetime analysis

Edge and exit cleanup

MIR optimization and CFG compaction

First optimization target

Ownership verification → C generation

Sirius also already implements last-use moves, consuming calls, consuming constructors, and ownership-aware physical slot reuse. Those should be improved rather than replaced.

The most significant missing capability is inferred temporary borrowing, particularly for function arguments and array/record projections.

## 3. Proposed improvement plan

| Phase | Work                                   | Priority  | Risk        |
| ----- | -------------------------------------- | --------- | ----------- |
| 0     | Establish reproducible baseline        | Essential | Low         |
| 1     | Generalize CFG block deduplication     | High      | Low         |
| 2     | Eliminate redundant releases           | High      | Low–medium  |
| 3     | Optimize cleanup placement and sharing | High      | Medium–high |
| 4     | Improve move and borrow inference      | Medium    | High        |
| 5     | Optimize compiler string construction  | Lower     | Low–medium  |

### Phase 0 — Establish the baseline

Before modifying the compiler, record:

- Generated C size, instructions, releases, and basic-block counts.
- Optimized executable size and native C compilation time.
- Sirius compiler execution time, allocation counts, and peak memory.
- Actual runtime retain/release counts on representative programs.

Use several workloads, including the self-hosted compiler, array-intensive programs, nested records, CoW updates, and branch-heavy functions.

This prevents confusing reductions in C source size with actual runtime improvements.

### Phase 1 — Generalize CFG deduplication

Modify `mir_compact_cfg` to identify identical blocks regardless of instruction count.

Use hashing to avoid quadratic comparison, with exact structural equality checking on matches.

Blocks may be merged when their instructions and control-flow destinations are semantically identical. Identical cleanup prefixes with different destinations require separate treatment.

Expected benefit: smaller CFG, fewer emitted instructions, and potentially faster C compilation.

This is the best first implementation because the limitation is already identified and the transformation is conceptually simple.

### Phase 2 — Eliminate redundant releases

Implement a local pass that tracks definitely empty registers or slots.

For example:

```
s_mir_record_release(r15); r15 = NULL;
s_mir_record_release(r15); r15 = NULL;
```

The second operation can disappear if no intervening operation reinitializes the slot.

Perform logically safe simplifications in MIR, while handling duplicates introduced by physical register reuse separately in the C backend.

Extend to inter-block reasoning only after the local version is validated.

Expected benefit: smaller code and fewer unnecessary runtime operations.

### Phase 3 — Reduce duplicated cleanup paths

Currently, `mir_cleanup_function` generates releases independently for CFG edges based on ownership and liveness.

Investigate three improvements:

1. Avoid generating releases for values already provably absent on the relevant path.
2. Group common cleanup tails where the same ownership obligations and release timing can be preserved.
3. Move releases to shared control-flow points when safe, instead of duplicating them on many branches.

This could eliminate far more code than exact block deduplication.

However, cleanup placement must remain compatible with CoW uniqueness checks, ownership transfers, failure paths and deterministic destruction.

Expected benefit: substantial reduction in cleanup CFG complexity, with possible runtime improvements.

### Phase 4 — Improve ownership optimization and borrowing

Build on the existing move-aware MIR operations.

The next useful capabilities are temporary read-only borrowing for call parameters, then borrowed projections from arrays and records.

For example, a short-lived read-only use should ideally avoid a retain followed immediately by a release when the owning root is guaranteed to stay alive.

Keep borrowing as an internal optimization. Sirius should preserve the semantic rule that values are owned and reads behave as copies, without requiring user-visible borrow annotations.

Start with intra-block borrowing and a conservative safety verifier before considering cross-block or interprocedural borrowing.

Expected benefit: fewer genuine ARC operations, potentially improving runtime performance more than code-size-only optimizations.

### Phase 5 — Improve string generation

The bootstrap contains extensive string-concatenation machinery, particularly in compiler code generation.

Investigate wider use of fragment accumulation or a string builder instead of repeatedly allocating intermediate strings.

Measure allocation counts and compiler runtime before adopting changes.

This is independent of ARC cleanup and should remain a separate optimization effort.

## 4. Validation and success criteria

Every phase should be an independent patch with before/after measurements.

The principal checks are `test_mir_opt.py`, `test_mir_exit_cleanup.py`, the ownership/balance tests, call/constructor move tests, native ARC auditing, CoW and value-slot tests, and `test_bootstrap.py`.

Validation must include native versus interpreted behavior, failure paths, self-hosting fixed points and Windows GCC compatibility.

A change should be retained when it provides a measurable benefit without regressions. In particular:

- A code-size optimization succeeds if it materially reduces generated or executable code without excessive compiler overhead.
- An ARC optimization succeeds if it reduces actual ownership work, allocations or execution time, not merely textual release counts.
- A complex transformation with negligible benefits should be reverted or deferred.

## 5. Recommended starting point

I recommend implementing phases 1 and 2 first, as separate patches.

They address concrete inefficiencies revealed by the analysis, require no new language semantics, and should produce useful measurements for deciding whether the more ambitious cleanup restructuring is worthwhile.

The broader strategic direction is to make MIR ownership analysis precise enough to avoid generating unnecessary ownership operations, rather than relying on the C compiler to simplify them afterward.

I would not change Sirius's reference-counting memory model, introduce a tracing GC, or adopt Swift's full ownership architecture at this stage. The existing register-level ownership design is a reasonable foundation; the immediate work is to make better use of it.