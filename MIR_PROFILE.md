# MIR self-compilation profile (2026-10-14)

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

## Next performance targets

1. **Parser state and AST construction:** parsing now consumes roughly half of
   `-O0` self-compilation. Inspect `ParseState` CoW copies and `append_node`'s
   arena updates before attempting a structural builder change.
2. **Ownership state representation:** packed register masks or a reusable
   per-function CFG analysis may help further, but will require equivalence
   checks on joins, loops, and conditional releases. A previous naive merged
   transfer experiment regressed severely and was reverted.
3. **Native code generation:** compiling reusable native compiler binaries at
   `-O1` improves execution speed, but makes the initial C build slower.
4. **Deprioritize MIR-C text formatting:** its measured share remains under 1%.

## Original recommendations (historical baseline)

1. **Ownership dataflow: optimize representation and reuse.** Inspect and
   measure `mir_ownership_analyze_cfg` worklist iterations, per-block state
   copies, and register-mask size. Consider packed bitsets and caching analyses
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
