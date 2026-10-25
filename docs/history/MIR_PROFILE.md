# Historical MIR self-compilation and code-generation profiles

> **Historical measurements:** reproduce before applying performance claims to newer versions. See [ROADMAP.md](../../ROADMAP.md).

**Historical measured baseline (2026-10-24):** An earlier modular compiler
emitted 4,232,563 bytes of C after backward-liveness snapshot sharing and
direct ownership-block transfer. These timings and sizes are historical:
subsequent verified managed-slot reuse across CFG blocks, ownership auditing,
and native ARC instrumentation changed generated code. In particular, managed
owners crossing basic-block boundaries are no longer necessarily assigned
dedicated slots. The current committed output is `bootstrap/compiler.c`;
measure and compare it directly rather than treating the earlier byte count
as the active baseline. All figures below are dated observations, not promises
about the current build.


## Historical MIR self-compilation profile (2026-10-14)

The standalone MIR-generated compiler compiles the concatenated MIR-only Sirius
compiler source (~240 KiB; 194 specialized functions) into ~3.45 MB of C.
The profiler instruments only selected compiler functions in the *generated C*;
it does not change the Sirius compiler implementation. Timings are single-run
measurements, not a statistically controlled benchmark.

| Phase (inclusive) | Clang -O0 | Clang -O1 |
| --- | ---: | ---: |
| Full self-compilation | 22.05 s | 10.67 s |
| Parse module | 8.07 s | 4.99 s |
| — parse statements (34,508 calls) | 6.20 s | 3.59 s |
| — tokenize | 0.027 s | 0.012 s |
| HIR specialization | 1.55 s | 0.74 s |
| MIR function lowering (194 calls) | 0.33 s | 0.10 s |
| MIR lifetime insertion | 0.87 s | 0.38 s |
| MIR exit cleanup | 5.01 s | 2.04 s |
| MIR balance verification | 5.74 s | 2.19 s |
| MIR C emission | 0.18 s | 0.09 s |

**Within the MIR passes**, ownership CFG analysis
(`mir_ownership_analyze_cfg`, 970 calls) accumulated 9.61 s at -O0 or
3.73 s at -O1. Those times are *nested inside* cleanup, balance, and other
passes; do not sum them with parent rows. Most of its cost is in repeated
register-state worklists. The parser's own tokenization is negligible:
parser statement handling and AST construction dominate.

At -O1, compiling the instrumented C took ~13.5 s, versus ~2 s for -O0.
Optimized native builds are advantageous when the compiler executable is
reused for multiple compilations. Both profiled executables generated C
byte-identical to the uninstrumented baseline.

## Follow-up: entry-first ownership worklist and may-only exit cleanup

The MIR compiler now queues the CFG entry initially, rather than every reachable
block. A `visited` marker ensures each reachable successor runs at least once,
even if incoming state initially equals the lattice identity. Must states begin
at *top* and may states at *bottom*, preserving monotone fixed-point semantics
for joins and loops. Cleanup also uses only the post-edge may-ownership analysis,
with `MIR_RELEASE_IF_INIT` at exits instead of recomputing definite ownership.

The same native self-compilation workload was profiled again (Clang):

| Phase / metric | Previous `-O0` | Updated `-O0` | Previous `-O1` | Updated `-O1` |
| --- | ---: | ---: | ---: | ---: |
| Full self-compilation | 22.05 s | **16.75 s** | 10.67 s | **8.52 s** |
| Parse module | 8.07 s | 8.34 s | 4.99 s | 5.43 s |
| HIR specialization | 1.55 s | 1.63 s | 0.74 s | 0.76 s |
| MIR exit cleanup | 5.01 s | **2.34 s** | 2.04 s | **0.80 s** |
| MIR balance verification | 5.74 s | **2.56 s** | 2.19 s | **0.82 s** |
| Ownership CFG analysis, inclusive across callers | 9.61 s | **3.78 s** | 3.73 s | **1.17 s** |
| Ownership CFG analyses | 970 | **772** | 970 | **772** |

The cumulative ownership time is nested within other phases, not additional
time. New `-O0` C output is 3,453,615 bytes, approximately the previous size.
As with the original profile, these are individual runs, not controlled
multi-run benchmarks. The two independent source changes were not profiled as
separate experiments; the timings reflect their combined effect.

**Validation:** identical C from consecutive native self-hosting generations;
85/85 executable cases, 35/35 expected diagnostics, optional/`f64` cases,
and dense cyclic CFG, joint ownership and forward ownership drivers.

## Follow-up: compact generated MIR fields and token-width guard

Generated `S_MirField` previously stored an `i64`, `f64`, string pointer and
record pointer simultaneously. Its type descriptor always selects exactly one
active payload, so generated C now uses a `union` (8 bytes instead of 32 on
the measured 64-bit system). This reduces record/array allocations and CoW
copy bandwidth. Parser token comparisons also reject unequal source-span
lengths before allocating a sliced string.

| Phase (inclusive, Clang `-O0`) | Previous | Updated |
| --- | ---: | ---: |
| Native self-compilation | 16.75 s | **11.83 s** |
| Parse module | 8.34 s | **5.10 s** |
| MIR exit cleanup | 2.34 s | **1.81 s** |
| MIR balance verification | 2.56 s | **1.89 s** |
| Ownership CFG analysis (nested) | 3.78 s | **2.76 s** |
| C source bytes | 3,453,615 | 3,454,644 |

These are individual instrumented runs with byte-identical self-generated C,
not statistically controlled timings. The generated C size stays essentially
unchanged; the main gain is runtime memory traffic. A per-site string-literal
cache was also tested and **reverted**: it made C larger and slowed the native
compiler. The canonical parser still performs substantial copy-on-write arena
updates; ownership-aware record-field updates are the more promising next task.

## Packed ownership comparison (2026-10-23)

Compared the current packed-state ownership analysis with a dense `Array[i64]`
variant storing one 0/1 value per register. The comparison changed only
`mir_ownership_get`, `mir_ownership_set`, `mir_ownership_top` and
`mir_ownership_empty`; the worklist, transfer and join algorithms were
otherwise identical. Both variants passed their own byte-identical C
self-compilation checks. Same machine, GCC `-O0`, three native self-compilation
runs per variant:

| Metric | Dense states | Packed states |
| --- | ---: | ---: |
| Source-to-C generation (seed compiler) | 3.25 s | 3.30 s |
| C → native compiler build | 16.95 s | 18.67 s |
| Native self-compilation median | 6.33 s | **3.43 s** |
| Generated C | 3,742,485 bytes | 3,749,529 bytes |

The packed implementation took **0.542× the execution time**, or about 46%
less, for this workload. It does not improve native C compilation time and
is not an estimate of end-to-end cold-build acceleration. The dense comparison
is an equivalently structured benchmark variant, not a checkout of an older
compiler release. `tools/bench_mir_ownership.py --baseline-source PATH` runs
this comparison when given that dense variant.

## Next performance targets

1. **Parser state and AST construction:** parsing now consumes roughly half of
   `-O0` self-compilation. Inspect `ParseState` CoW copies and `append_node`'s
   arena updates before attempting a structural builder change.
2. **Ownership state representation:** packed 63-bit-word register masks
   have been measured against the dense-state equivalent below. Reusing
   per-function CFG analyses remains a separate possibility; a naive merged
   transfer experiment previously regressed and was reverted.
3. **Native code generation:** compiling reusable native compiler binaries at
   `-O1` improves execution speed, but makes the initial C build slower.
4. **Deprioritize MIR-C text formatting:** its measured share remains under 1%.

## Original recommendations (historical baseline)

1. **Ownership dataflow: optimize representation and reuse.** Inspect and
   measure `mir_ownership_analyze_cfg` worklist iterations, per-block state
   copies, and register-mask size. Packed bitsets were implemented after these
   historical measurements; consider caching analyses
   across passes when the CFG is unchanged. A naive merger of separate must/may
   passes was tested but made self-compilation stall; it was reverted.
2. **Parser: reduce arena/ParseState copying.** `parse_statement` runs 34,508
   times and takes ~6.2 s at -O0. Investigate record/array copy-on-write
   allocations when `append_node` extends `nodes` and `edges`; a uniquely owned
   parser builder could help. A per-byte `is_token` rewrite was slower and was
   reverted. Do not assume tokenization is the bottleneck.
3. **Cleanup: avoid unnecessary second/third ownership analyses.** The cleanup
   pass computes liveness and several ownership snapshots before and after
   inserting edge blocks. Any caching must account for CFG and ownership
   changes, and must preserve path-sensitive release safety.
4. **Enable C compiler optimization when the executable is reused.** The
   -O1 native compiler ran self-compilation roughly twice as fast in this
   measurement, at a higher one-time C build cost.
5. **Deprioritize C-emitter string formatting.** C emission is under 1% of
   execution time; reducing C source size is a separate objective.

## Reproduce

After running `python tools/test_mir_selfhost.py --workdir build/mir-selfhost`,
use its `generation_2.c` file:

```sh
python tools/profile_mir.py --generated-c build/mir-selfhost/generation_2.c --opt 0
python tools/profile_mir.py --generated-c build/mir-selfhost/generation_2.c --opt 1
```

Or use `--stage1 PATH` to generate updated C with an existing standalone
MIR-compiler executable. `--keep-dir PATH` preserves the instrumented compiler,
C files, and output for examination. The profiling script relies on the
self-hosted compiler's current specialization indices; re-check its phase map
if compiler source ordering or specialization changes.

## Direct record-field update optimization (subsequent revision)

Direct record assignments now lower to consuming `MIR_RECORD_SET`, which
reuses uniquely owned record storage and clones only for shared snapshots.
The record payload is retained before dropping the previous field. The
interpreter uses the same ownership rules as native C.

The self-hosted C shrank from 3,454,644 to 3,169,724 bytes (about 8.2%).
Native Clang `-O0` generation of the compiler from the MIR-built binary
was around 12.3 seconds in the successful fixed-point checks; this does **not**
establish a speedup against the previously measured 11.83-second run.
Further controlled benchmarking is needed to distinguish natural variation
from a runtime regression. Stages 2, 3 and 4 remain byte-identical, and
`tools/test_mir_record_set.py` checks aliasing, verifier and interpreter behavior.

Reprofiling the new native stage-2 compiler (`-O0`) on the same combined
source yielded 11.63 seconds overall, including 5.47 seconds parsing,
1.68 seconds exit cleanup, 1.79 seconds balance checking, and 2.61 seconds
cumulative ownership dataflow (the latter is included in the phase totals).
The generated C remained identical on stages 2–4. A separate same-input,
single-run comparison measured 12.13 seconds with the previous native compiler
versus 11.90 seconds with this version; treat that small difference as
inconclusive until multiple controlled runs are available.

## Follow-up: fused record-array append

The MIR lowering now recognizes direct self-field appends such as
`p.nodes = p.nodes.append(node)` and emits `MIR_RECORD_ARRAY_APPEND`.
Instead of retaining a nested array, appending into that temporary, then
replacing the record field, it consumes the outer record, clones the outer
record only if shared, and uses the nested array's ordinary CoW append.
No aliases are invalidated.

A 100,000-element `Store.values` append microbenchmark (`clang -O1`) took
6.35 s with the previous lowering versus under 0.01 s with fused lowering.
This demonstrates removal of a quadratic clone pattern **in that benchmark**;
it is not a general compiler speedup. For an identical updated Sirius compiler
source, the previous compiler emitted 3,217,815 C bytes and the updated
compiler emitted 3,200,637 (about 0.5% smaller). Native `-O0` compiler
self-compilation remained around 13 s in comparable runs, with no reliable
end-to-end speedup. Updated stages 2–4 emitted byte-identical C. The
85 executable fixtures and 35 MIR diagnostics, new direct interpreter/native
ownership checks, and snapshot tests pass.

## Follow-up: last-use ownership transfer at calls

After lifetime and exit cleanup, `mir_optimize_function` replaces a `MIR_CALL`
followed immediately by the final `MIR_RELEASE` of a unique argument register
with `MIR_CALL_MOVE`. This transfers an already owned reference to the callee
instead of retaining it at the call site and releasing it afterward. It is
limited to one transfer per call and excludes duplicated source registers or
values used after the call. Other managed arguments retain their usual behavior.

For the same concatenated MIR-only compiler source, Clang `-O0`:

| Measure | Previous fused-append snapshot | With call transfers |
| --- | ---: | ---: |
| Native self-compilation | 13.28 s | 7.65–7.96 s (two runs) |
| Generated C | 3,267,658 bytes | 3,248,899 bytes |
| Transferred call sites in generated C | 0 | 1,031 |

The native binaries were generated from their corresponding Sirius sources
with identical Clang `-O0` options; these are limited local runs, not a full
statistical benchmark. MIR-only native stages 3 and 4 produce byte-identical C.
All 85 executable fixtures, 35 diagnostics, optional/`f64` additions and the
focused call transfer ownership tests pass.

A loop passing an `Array[i64]` through a function that appends one item at a
time (`tests/mir/call_move_benchmark.sirius`, 30,000 iterations, Clang `-O1`)
took about 0.48 s before vs below 0.01 s after, because transferring the
last owner avoids a CoW array clone on each call. This is a microbenchmark of
that pattern, not an expected speedup for arbitrary programs.

## Follow-up: multi-argument call transfers

`MIR_CALL_MOVE` can now transfer multiple distinct managed arguments when their
`MIR_RELEASE` instructions immediately follow a call. The first moved register
is stored in `first`; `second` encodes additional argument *positions* in an
i64 mask (positions 0–61). Duplicated registers, scalar arguments, and later
uses are not moved. Ownership/liveness, interpreter and C emission handle every
transferred argument. C emission passes values directly to the callee and
invalidates caller slots after the call returns, avoiding temporary C variables.

With Clang `-O0`, using the same native self-compilation workload and one run
of each compiler:

| Measure | Single-transfer baseline | Multi-transfer compiler |
| --- | ---: | ---: |
| Native self-compilation | 7.93 s | 7.79–8.09 s |
| Generated C | 3,248,899 bytes | 3,235,475 bytes |
| Generated `s_mir_record_retain` occurrences | 2,638 | 2,442 |
| Generated `s_string_retain` occurrences | 856 | 644 |

Counts include runtime helper definitions, so the decrease is indicative rather
than a dynamic retain count. **No end-to-end runtime speedup is established.**
The 85 positive fixtures and 35 expected diagnostics pass; a direct test covers
three moved strings interleaved with a borrowed scalar, duplicate-register
exclusion, invalid masks, use-after-move, interpreter heap balance, and native
C agreement. Native compiler generations 3 and 4 emit byte-identical C.

## Liveness state allocation review (2026-10-14)

`tools/profile_ir_storage.py` instruments a disposable copy of the compiled C
bootstrap and reports `s_mir_record_new` calls by numeric runtime type ID.
This includes IR-related and other managed data. It measures **initial**
allocation requests, not reallocations or peak RSS; IDs depend on the exact
bootstrap. In the self-compile workload, the `Array[i64]` runtime type (ID 13)
accounts for most requested bytes, reflecting state-heavy compiler passes.

`mir_lifetime_states_cfg` previously constructed an independent register-sized
zero array for each block and each worklist iteration. It now builds one
immutable zero state per function, shared between blocks and iterations. CoW
ensures that `mir_live_add` does not mutate another block's state.

| Measure | Before | After |
| --- | ---: | ---: |
| Native `-O0` self-compilation (median, 3 runs) | 6.89 s | 5.63 s |
| Native `-O1` self-compilation (median, 4 runs) | 2.22 s | 1.92 s |
| `s_mir_record_new` calls | 4,590,011 | 4,555,316 |
| ID 13 calls | 997,199 | 962,249 |
| Cumulative initial allocation requests | 3.34 GB | 3.48 GB |

These comparisons use the same compiler sources except for the liveness
change, with separate, fixed-point C bootstraps. Time reductions were repeated
on this machine, but are not portable guarantees. The larger cumulative
requested size is an explicit tradeoff; it does not prove higher peak memory.
`python tools/test_compiler.py` (91 executions, 46 diagnostics),
`python tools/test_mir_dataflow.py` and the C bootstrap fixed-point check pass.
The standalone `test_mir_lifetime.py` driver takes unusually long to compile
on this machine with **both** old and updated seed compilers, so its full
end-to-end result was not established here.

## CoW ownership and parser allocation attribution (2026-10-14)

Extended `tools/profile_ir_storage.py` to count copies made by
`s_mir_record_unique_owned`, using a disposable C bootstrap. The profiler
reports allocations and their initial byte requests by generated runtime
TypeId; clone allocations are included in the overall allocation totals.
The following is a **single** self-compilation snapshot after adding typed
Boolean module constants, not a performance comparison:

| Category | Clone calls | Cumulative requested bytes |
| --- | ---: | ---: |
| All generated records/arrays | 641,421 | 3.33 GB |
| `Array[i64]` (runtime ID 13) | 473,741 | 2.84 GB |
| `Array[Node]` (runtime ID 12) | 2,781 | 0.42 GB |

The profiler groups by TypeId; a separate temporary `-O0` instrumented binary
attributed the dominant ID-13 clones to `mir_ownership_join` and
`mir_ownership_apply_effects`, with smaller contributions from liveness and
cleanup. ID-12 clones are made during parser `append_node`, where a growing
arena may still have live snapshots. These copies preserve value semantics.
Cumulative requested bytes do **not** measure peak resident memory, lifetime,
or time spent allocating. No compact encoding or shared mutable parser arena
was adopted without a broader ownership/ABI design.

## Modular C output size (2026-10-24)

The native module-root compiler previously routed ownership-checked and
balanced linked MIR through the *defensive* C emitter. That produced a
10,621,160-byte bootstrap containing repeated `initN` flags, read guards, and
managed-slot state updates in thousands of generated blocks.

The modular CLI now uses `mir_emit_c_reachable_verified`: it checks
`mir_check_balance` before emitting with no redundant C-side init tracking.
The direct/externally supplied MIR emitter remains defensive.

| Metric | Before | After |
| --- | ---: | ---: |
| Committed generated C | 10,621,160 bytes | 5,175,457 bytes |
| Reduction | — | 51.3% |
| Clang `-O0` native executable | 4,676,808 bytes | 2,546,976 bytes |
| Clang `-O0` C compile/link median, 3 runs | 3.74 s | 1.91 s |

The native build comparison used the same local Clang compiler and runtime,
compiling the two committed C snapshots with `-std=c11 -O0`. These timings
exclude Sirius-to-C generation and are not a cross-platform benchmark.

Both variants contain 280 generated function definitions; the improvement does
not come from removing compiler capabilities. `tools/profile_generated_c.py`
reports the largest generated definitions by C-text size; generated function
IDs are unstable across compiler changes. Tests cover the separate checked
and verified emission APIs and the modular self-hosting fixed point.

## MIR CFG and nullable-release optimization (2026-10-24)

The previous 5,175,457-byte C bootstrap had 280 generated functions. Its
largest generated function had **1,169 register declarations** and **1,215
basic blocks** (467,028 C bytes); these are total slots/blocks, **not** a peak
live-register measurement. The checked-in source uses many edge-specific
`MIR_RELEASE_IF_INIT` instructions. About 35,302 generated `if (rN)` release
guards accounted for a large share of its C text.

Two independent optimizations were combined:

1. After MIR lifetime/exit cleanup, bypass instruction-free jump chains,
   merge short blocks with identical instructions and terminators, and prune
   unreachable blocks. Compare full operands including call input vectors;
   **do not** remap/register-reuse values. Empty jump cycles are preserved.
2. On the *verified* C path only, emit an unconditional release followed by
   `rN = NULL` for `MIR_RELEASE_IF_INIT`. Both `s_string_release(NULL)` and
   `s_mir_record_release(NULL)` are no-ops. The defensive direct-MIR path
   retains `initN` checks and conditionals.

| Measure | Before | After both changes |
| --- | ---: | ---: |
| Bootstrap C bytes | 5,175,457 | 4,679,289 |
| Change | — | -9.6% |
| Largest C function | 467,028 | 403,687 bytes |
| Largest function's emitted CFG blocks | 1,215 | 1,142 |
| Largest function's register declarations | 1,169 | 1,169 |

Isolating the CFG compaction alone (with the prior emitter) gave 5,152,357
C bytes, only about 23 KB smaller; most of the combined saving is from
nullable release emission. Numeric generated-function IDs are unstable and
should not be treated as function names. This is primarily a C-text reduction;
compiler throughput and peak register pressure require separate measurements.
`tools/profile_generated_c.py` now reports per-function declarations, block
counts and guarded releases as *emitted C metrics*. `tools/test_mir_opt.py`
checks duplicate cleanup, unreachable blocks, idempotence and interpreted/native
execution, including verified versus defensive release handling and a
conditional release on a path where the register remains uninitialized.

## Conservative scalar register-slot reuse (2026-10-24)

Verified MIR-to-C emission now compacts `i64`, `bool`, and `f64` frame slots
using block-local intervals. Registers are eligible only when their first
mention is a definition, all mentions occur in one basic block, and they are
not function arguments. Reuse requires equal types and non-overlapping
intervals; slot pools can be reused across blocks. The remapping is applied
to a temporary emission-only MIR function. The original MIR register IDs,
managed slots, checked direct-MIR emitter, and interpreter are unchanged.

| Measure | Before | After |
| --- | ---: | ---: |
| Modular bootstrap C bytes | 4,679,289 | 4,388,950 |
| Difference | — | -6.2% |
| Largest emitted function register declarations | 1,169 | 178 |
| Clang `-O0` native executable | 2,248,072 bytes | 2,129,496 bytes |

The executable sizes are from one local Clang build of each committed C
variant; they are not performance measurements.

This is a conservative subset of CFG-wide interference allocation, not a
full liveness graph coloring pass. It avoids touching loop-carried registers
and values used across joins. Managed registers remain excluded because
releases and ownership transitions require additional safeguards.
`tools/test_mir_scalar_slots.py` checks interpreter/native execution through
a loop and a branch join, plus preserved defensive emission. The full
`python tools/test_all.py` run passes all 64 scripts, including 96 generated-C
executions, 50 expected diagnostics, and MIR generations 2–4 at the same
4,388,950-byte fixed point.

## CFG-wide scalar interference allocation (2026-10-24)

Verified C emission now uses existing backward MIR liveness (`mir_lifetime_states`)
to build a **packed symmetric interference graph**. Edges cover simultaneously
live registers at block entry/exit, each definition versus live-out registers,
and read/definition hazards within a single instruction. A deterministic greedy
allocator assigns same-type scalars to slots only if they have no conflicting
previous occupants. This extends reuse to values crossing multiple CFG blocks,
including loops and joins, without changing MIR or the interpreter.

| Measure | Previous | CFG-wide allocation |
| --- | ---: | ---: |
| Modular bootstrap C bytes | 4,388,950 | 4,334,410 |
| Difference | — | -1.2% |
| Largest emitted function's C register declarations | 178 | 16 |
| Largest emitted function's CFG blocks | 1,142 | 1,142 |

The largest function's register declarations measure emitted storage, not
simultaneous MIR liveness. Code size savings are smaller than declaration-count
savings because the C body still contains the same instructions. The allocator
keeps managed values and arguments in dedicated slots; direct defensive C
emission retains logical register IDs. The scalar-slot regression exercises
sequential cross-block values, joins, loops, and interpreted/native parity.
The regenerated C bootstrap reaches a byte-identical fixed point through
multiple self-hosted generations. Runtime performance is not inferred from
these code-size measurements.

## Conservative managed-slot reuse (2026-10-24)

The verified C emitter now also shares physical `S_String *` or `S_MirRecord *`
slots for **same-TypeId** managed registers with disjoint ownership intervals.
Eligibility is stricter than for scalars: all references must occur in one
basic block, the first reference must define the register, and its final
reference must be an unconditional `MIR_RELEASE`. The release clears the slot
at the block boundary, including loop backedges. Arguments, owners referenced
across blocks, conditional `MIR_RELEASE_IF_INIT`, move/transfer endings, and
simultaneously live owners remain dedicated. This excludes hazards that
ordinary backward liveness cannot detect. The original MIR and defensive C
emission are unchanged.

| Measure | Before | After |
| --- | ---: | ---: |
| Modular bootstrap C bytes | 4,334,410 | 4,250,979 |
| Difference | — | -1.9% |
| Emitted `S_String *` register declarations | 4,626 | 1,845 |
| Emitted `S_MirRecord *` register declarations | 6,622 | 5,957 |

The reduced declaration count is not a measurement of peak live owners or a
runtime speedup. `tools/test_mir_managed_slots.py` tests sequential, disjoint
blocks, overlapping values, conditional cleanup, cross-block owners and loops
through interpreter/native parity. The compiler reaches byte-identical MIR
self-hosting generations 2–4. Further cross-block managed reuse would require
a path-sensitive may-ownership interference proof; it is not assumed safe.

## MIR record-type allocation attribution (2026-10-24)

`python tools/profile_mir_allocations.py --runs 3 --opt 1 --type-names`
instruments a disposable copy of the committed C bootstrap and, optionally,
uses a native Sirius module driver to map generated TypeIds to canonical
structural signatures. New per-type counters include initial allocation
requests, empty allocations, CoW clones and requested bytes. The instrumented
self-compilation output must be byte-identical to the committed bootstrap.

In a single `-O1` self-compilation after removing unused temporary arrays from
`mir_instruction_reads`:

| Type | Allocations | CoW clones | Cumulative requested bytes |
| --- | ---: | ---: | ---: |
| `Array[i64]` | 3,225,125 | 1,949,722 | 1,256,693,664 |
| `Optional[i64]` | 1,039,772 | 0 | 41,258,192 |
| MIR ownership block (`incoming`, `outgoing`) | 856,428 | 0 | 41,108,544 |
| MIR instruction record | 402,302 | 0 | 35,402,576 |

The prior snapshot requested 9,042,739 generated record/collection allocations;
this change requests 8,423,359 (−6.8%), mostly by avoiding an unnecessary
`Array[i64]` allocation in `mir_instruction_reads`. Allocated bytes include
CoW clones and exclude `realloc`, nested runtime allocations and peak RSS.

The next substantial target is `Array[i64]` CoW copying in CFG analysis, not
another container-wide optimization. The previous generation's 1,911,091
`Array[i64]` allocations inside its generic CoW helper demonstrate that the
remaining problem is broader than creating empty read lists. Avoid changing
the ownership-state representation again without measuring full cost.

## Ownership predecessor snapshots (2026-10-24)

A sampled stack trace of `Array[i64]` CoW clones localized most writes to
`mir_ownership_join`. Previously each block's forward analysis started from
its lattice identity (`top` for must, `bottom` for may), and joining even a
single predecessor forced an unnecessary array copy. Non-entry blocks now
start from the first reachable predecessor's outgoing state; only subsequent
predecessors require bitwise merging. Entry blocks still combine the entry
argument state with loop predecessors. Value semantics and the packed 63-bit
word representation are unchanged. The public two-array join remains intact.

| Counter, one instrumented modular self-compilation (`-O0`) | Before | After |
| --- | ---: | ---: |
| Generated record/collection allocations | 8,423,359 | 7,437,274 |
| `Array[i64]` allocations | 3,225,125 | 2,241,612 |
| `Array[i64]` CoW clones | 1,949,722 | 966,353 |
| All CoW clones | 2,165,674 | 1,182,481 |
| Array element writes | 17,230,225 | 8,449,972 |

Both runs compiled the module-root compiler from their respective Sirius
source versions, so the workloads differ slightly. These counts demonstrate
removed intermediate clones, **not** a controlled timing result or a memory
footprint measurement. The ownership-combined native test verifies that a
single-predecessor state shares an immutable snapshot without mutating its
predecessor after a later write. CFG joins, backedges and exit balance are
covered by the existing ownership regressions.

## Backward-liveness successor snapshots (2026-10-24)

`mir_lifetime_states_cfg` previously built each block's `live_out` by
unioning a shared all-zero array with the first successor's `live_in`. This
copied the dense `Array[i64]` on CoW even for blocks with only one successor.
Now jump/branch blocks start from that successor's existing snapshot, and
only union the distinct alternative successor of a branch. The backward
instruction transfer still uses CoW; it cannot mutate stored successor state.

| Counter, instrumented native `-O0` modular self-compilation | Before | After |
| --- | ---: | ---: |
| Generated record/collection allocations | 7,437,326 | 7,343,095 |
| `Array[i64]` allocations | 2,241,612 | 2,146,435 |
| `Array[i64]` CoW clones | 966,353 | 870,856 |
| All CoW clones | 1,182,481 | 1,087,011 |
| Array element writes | 8,449,972 | 7,602,197 |
| Generated bootstrap C bytes | 4,250,979 | 4,243,409 |

The ~9.9% reduction in `Array[i64]` CoW clones and ~1.3% reduction in
all generated record allocations are **operation counts**, not wall-clock
performance measurements. The two instrumented compilers use slightly
different self-hosting sources. A two-run post-change profile produced
identical counters. The lifetime regression checks shared successor snapshots,
loops, branch joins and native/interpreted execution; MIR self-hosting reaches
a byte-identical generations 2–4 fixed point.

## Direct forward-ownership block transfer (2026-10-24)

The CFG ownership worklist used to allocate two `Array[i64]` effect lists for
*every* MIR block and replay their entries on each visit. Applying the same
instruction effects directly on visited blocks removes temporary lists and
array appends. The public `mir_ownership_effects` and
`mir_ownership_apply_effects` helpers remain available and equivalent.
Both must/may propagation and entry/backedge handling are unchanged.

| Instrumented `-O0` modular self-compilation | Before | After |
| --- | ---: | ---: |
| All MIR record/collection allocations | 7,343,095 | 7,016,214 |
| `Array[i64]` allocations | 2,146,435 | 1,930,998 |
| Empty generated containers | 1,516,745 | 1,298,267 |
| All CoW clones | 1,087,011 | 1,086,594 |
| Capacity growth calls | 1,332,853 | 1,016,352 |
| Bootstrap C bytes | 4,243,409 | 4,232,563 |

The totals are from separate, slightly different compiler source versions;
cloning is essentially unchanged. A two-run profile of the new compiler was
deterministic. These operation counts do **not** demonstrate a build-time
speedup: worklist revisits now traverse instructions instead of cached effects.
The ownership/CFG regression compares direct transfer with the public effect
summaries across reachable joins, loops, repeated writes, and call moves.

## Constructor ownership transfers (2026-10-24)

The post-cleanup peephole optimizer now transfers distinct, last-use managed
inputs of `MIR_RECORD_NEW` and `MIR_ARRAY_NEW` rather than emitting redundant
retain/release pairs. The new move opcodes use the call-transfer position mask
and are validated by MIR definite ownership, backward liveness, interpreter,
and C emitter. Duplicate inputs and sources read later remain borrowed.

The native modular bootstrap reached a byte-identical fixed point at
4,258,464 C bytes (previous seed: 4,232,563 bytes). This is a code-size
*increase*, not a claim of faster self-compilation. The focused constructor
tests cover mixed owned/borrowed fields, nested records and arrays, duplicates,
invalid moves, interpreter heap balance and both C emitter modes. Actual
runtime benefit remains to be profiled; CFG-aware managed-slot reuse is
documented below.

## CFG-aware managed register slots (2026-10-24)

Verified C emission extends the previous block-local managed-slot allocation
with a packed interference graph built from forward **may-ownership** states.
At every reachable block entry and relevant ownership change, every pair of
potentially owned managed values interferes. A managed destination also
interferes with any other owner present before writing or releasing it and
with its managed instruction inputs. This last rule is required for moves and
for `MIR_RELEASE_IF_INIT` on a path where its logical register is absent but
a different owner is present in the proposed physical slot.

Greedy slot reuse requires identical TypeIds and excludes arguments. The
may-ownership union at joins conservatively treats branch-exclusive owners
as potentially overlapping; it is a **safe overapproximation**, not an exact
path-sensitive ownership proof. Only the verified C frame is compacted:
original MIR, interpreted execution and checked defensive C remain unchanged.

| Measure | Before | After |
| --- | ---: | ---: |
| Modular bootstrap C bytes | 4,258,464 | 4,012,418 |
| `S_String *` register declarations | 2,049 | 516 |
| `S_MirRecord *` register declarations | 6,098 | 2,379 |

The new bootstrap reaches a byte-identical C fixed point (generation 2 from
the previous regenerated seed). The full bootstrap suite passes 96 native
programs and 50 expected diagnostics. Tests in
`tools/test_mir_managed_slots.py` exercise sequential and cross-block reuse,
join and loop interference, conditional cleanup before/after allocation,
cleanup of an absent owner on the opposite branch, moves, call transfers,
and interpreter/native parity. Counts are from slightly different compiler
source versions (the allocator adds routines); none are peak live-pressure
measurements.

An **exploratory** comparison using the checked-in C compilers, the same
Clang `-O0` flags, and three native self-compilation runs per version gave
14.15 s median before versus 11.80 s after. C build time in that run was
4.68 s versus 3.31 s. Other test processes were active concurrently, so
these are not controlled performance measurements; no repeatable runtime
speedup is claimed. Re-run with an idle host before drawing that conclusion.
