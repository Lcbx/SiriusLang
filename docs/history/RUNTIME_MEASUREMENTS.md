# Runtime measurements and experiments

Historical observations collected in October 2026. Counts, timings and claims
about remaining work describe the recorded revision, not current behavior.
For current storage and contracts, see [Runtime](../RUNTIME.md) and
[Ownership](../OWNERSHIP.md). Commands run from the repository root.

## First runtime improvement

Generic array append and set formerly allocated and freed a temporary heap
buffer on **every** operation, even for an `i64`. The temporary is required
because the source element may alias the array being reallocated or modified.

Now small values use a 128-byte, `max_align_t`-aligned temporary on the stack;
larger values still use allocated storage. Unique-array growth uses `realloc`
with overflow checking rather than allocating/copying a second buffer on every
capacity expansion. Managed copy/drop callbacks and source-alias behavior are
unchanged. `s_string_from_utf8` now delegates allocation to `s_string_from_bytes`.

Generic dictionaries now allocate an aligned key/value payload once per occupied
slot (previously two separate allocations); insertion still snapshots borrowed
operands before any CoW clone, overwrite or table rehash. This leaves dictionary
entry pointers stable until replacement, as before, and does not change the
public `S_Dict` API. A synthetic 500,000-key insertion benchmark at `-O2`
changed from 0.174 s to 0.137 s median over five local runs; no meaningful
array-append speedup was measured in a 3-million-element benchmark (both about
0.07 s). Neither benchmark measures actual self-compilation performance.

Validation: `python tools/test_runtime_collections.py` tests growth, same-array
append/set, snapshot CoW, dictionary overwrite/rehashing, managed strings,
borrowed dictionary values and the large-element fallback. It also passes
ASan/UBSan on the tested toolchain. `python tools/test_reference_c.py` checks older C users.

## Experimental scratch lifetime study (October 26, 2026)

A proof-of-concept **local** arena exists only in
`tests/runtime/scratch_arena.c`; it is not linked into Sirius programs.
It accepts explicitly scoped, aligned, nonescaping POD buffers and retains
blocks for reuse across phase resets. It does not retain or release ARC
children and therefore must never own arbitrary Sirius records or arrays.
The test covers alignment, overflow rejection, multiblock reuse, and output
parity with individually allocated buffers; run:

```sh
python tools/test_runtime_scratch_arena.py
python tools/test_runtime_scratch_arena.py --sanitize    # if supported
python tools/test_runtime_scratch_arena.py --benchmark   # local CPU timings
```

In one `-O2` local microbenchmark, 600 phases of 512 64-byte scratch nodes
reduced individual heap allocation calls from 307,200 to 8 (retaining 32 KiB).
For 200 phases of 64 4-KiB temporary buffers, calls fell from 12,800 to 64
(retaining 256 KiB). Timings were lower in these **synthetic** cases, but the
retained peak memory and absence of escaping values are intentional trade-offs.
No speedup of the actual compiler, runtime, matrix example, or interpreter is
claimed. Treat this as a safe-scoping experiment, not a proposed general
allocator interface.

The existing MIR allocation profiler found 19,078,159 generated record
allocations in a local self-hosting run (2,725,320 CoW clones). Counts do not
encode escape lifetimes, and persistent or ARC-managed records cannot simply
be moved to scratch storage. Its output comparison now normalizes LF/CRLF
line endings; source text is otherwise compared exactly.

## Dictionary lookup and overwrite measurements (October 2026)

`python tools/benchmark_dictionaries.py --runs 3` benchmarks the generic
`S_Dict` against a **contiguous, optimistic linear-search proxy** for the old
MIR representation; it is not a benchmark of the previous MIR emitter. Tests
cover hits, misses and overwrites for 1–8192 scalar keys, plus managed string
key/value overwrites. The benchmark compiler uses `-O2`; results depend on
hardware, compiler and workloads.

In local runs, contiguous linear search was competitive at 1–16 keys and
especially for misses; hashed lookups became increasingly advantageous from
roughly 32–128 keys. At 1024 keys, hits took ~13 ns with hashing versus
~167 ns for linear lookup; misses ~18 ns versus ~324 ns. At 8192 keys the
gap was far larger. These results support a single hashed implementation
rather than adding a special small-dictionary mode now.

`S_Dict` formerly allocated and copied a complete key/value entry even when
replacing an existing key in a uniquely owned dictionary. It now preserves
that key and copies only the replacement value into a temporary before
dropping the old value. This avoids per-update allocation and key copy/drop,
including for structural record keys. Shared dictionaries continue cloning
as before, and new keys still allocate entries. The temporary uses inline
aligned storage or a heap fallback for larger value types; borrowed source
values may alias the dictionary. `tests/runtime/collections.c` covers these
cases and key copy/drop counts, including snapshot CoW.

Managed string-key overwrites improved from roughly 73–84 ns/update to
64–76 ns/update in single-run `-O2` comparisons of the original and modified
runtime. Integer overwrites improved less consistently. Compiling the Sirius
compiler with either runtime produced **identical C**, and `-O0` self-compilation
stayed around 6.6–7.0 seconds in four alternating sample runs. Consequently
no overall compiler speedup is claimed. Avoid further dictionary-specific
complexity until a real workload exposes a meaningful bottleneck.

## Self-compilation dictionary profiling (October 2026)

`python tools/profile_runtime_dict.py --runs 3` builds an instrumented copy of
`bootstrap/compiler.c` and the runtime, runs self-compilation, verifies that the
emitted C remains byte-identical and reports allocations, inserts, lookups,
CoW clones, rehashes and probe counts. Instrumentation is controlled by
`S_DICT_PROFILE`; the normal runtime builds remain uninstrumented. To profile
an updated generation before committing its C bootstrap, pass
`--generated-c PATH`.

An initial profile found 61,200 dictionary constructions and 390,976 inserts,
with only 87 clones. These mostly came from initializing `HirBodyState.types`
with `new_type_table()` and subsequently replacing it with an existing table:
each discarded table inserted six built-in type descriptors. Passing the table
through the `HirBodyState` constructor reduced counts to 211 constructions
and 25,034 inserts, without changing compiled program output or typing rules.
`-O0` self-compilation times ranged roughly 6.7–7.3 seconds across these
local runs, without a reliable whole-program speedup; the dictionary operation
count reduction is deterministic.

Experiments with inline dictionary buckets and an initial capacity of 16
reduced rehashes but did not consistently improve self-compilation time;
they were reverted. Further runtime container changes should be justified by
workload profiling rather than insertion-only C microbenchmarks.

## MIR collection-allocation profile and shared append (October 2026)

`python tools/profile_mir_allocations.py --runs 3` instruments a **temporary**
copy of generated compiler C (not the committed bootstrap or runtime) to count
MIR record allocations, CoW clones, array appends, capacity growth, and
per-type allocations during module-root self-compilation. It verifies that the
compiled result remains byte-identical to the input C seed, and asserts
identical counter results across runs. Use `--opt` to select the native C
optimization level or `--source` to inspect a smaller Sirius input. Type IDs
are local to each generated compiler; the tool does not count allocations
inside generic `S_Dict`, `S_String`, or C library functions. Instrumented
execution times are **not** comparable performance benchmarks.

Before the optimization, self-compilation performed approximately 9.04 million
MIR record allocations, 2.13 million CoW clones, 7.04 million appends, and
1.99 million record-capacity growth operations. About 246,000 appends first
cloned a shared array, then immediately reallocated the exact-size clone.

`src/mir_emit_c.sirius` now lets a shared-array clone reserve one additional
field before copying its elements. Unique arrays retain their existing growth
policy; other CoW operations still make exact-length clones. The snapshot's
length and reference counts are unchanged, and the append checks capacity
limits before incrementing length. In a subsequent self-hosting profile,
capacity-growth operations fell to approximately 1.77 million (about 11%
fewer); total allocation counts changed slightly as the compiler's emitted C
changed. The focused `test_mir_array_alloc.py` test confirms that cloning and
appending scalar or nested managed arrays do not perform the redundant
`realloc`, and verifies retained snapshots. No end-to-end speedup is claimed.

## MIR operand-read allocation profile (2026-10-24)

The MIR C-emitted runtime was instrumented while the native modular compiler
self-compiled. `tools/profile_mir_allocations.py --runs 3 --type-names` reports
allocation counts by structural type, including CoW clone counts, empty
containers and requested bytes. These are **cumulative requests**, not peak
memory, and do not account for all allocations inside `S_String` or `S_Dict`.

`mir_cfg.mir_instruction_reads` previously constructed a temporary empty
`Array[i64]` before branches that either returned no register operands or
returned `ins.inputs` directly. It now performs those returns first and builds
an initial one-element array when needed. This preserves read ordering and
value semantics, including malformed unused input metadata. The native
instruction-read tests cover constants, calls, writes, duplicate operands and
multi-register instructions. No runtime ABI or MIR storage layout changed.

| Profile (single `-O1` self-compilation) | Before | After |
| --- | ---: | ---: |
| Generated MIR record/collection allocations | 9,042,739 | 8,423,359 |
| `Array[i64]` allocations | 3,845,067 | 3,225,125 |
| Empty generated containers | 2,739,174 | 1,516,617 |
| Capacity growth operations | 1,769,984 | 1,333,101 |

The reduction in `Array[i64]` allocation calls is about 16%; total generated
record/collection allocations fell about 6.8%. The generated bootstrap remains
reproducible; these counts do **not** establish a wall-clock speedup. `Array[i64]`
still accounts for most cumulative requested bytes, largely due to CoW clones.

## Forward ownership snapshot reuse (2026-10-24)

The MIR ownership worklist now initializes non-entry blocks directly from
the first reachable predecessor's packed outgoing state. This avoids the CoW
clone previously incurred by intersecting the `top` or unioning the `bottom`
identity with that predecessor. Remaining predecessors use word-wise joins,
cloning the snapshot only if a bit changes. Entry-loop handling still includes
the function argument state; separate ownership states remain value-isolated.

In one instrumented self-compilation, `Array[i64]` clones declined from
1,949,722 to 966,353 (~50%). Total MIR-generated record/collection
allocations declined from 8,423,359 to 7,437,274 (~11.7%). This is a
compiler dataflow improvement rather than an ABI, storage or runtime CoW
change. Counts are workload-dependent and do not imply a measured speedup.

## Backward CFG liveness allocation reduction (2026-10-24)

The backward liveness solver reuses a jump/branch target's `live_in` array
as its `live_out` snapshot; only a distinct second branch successor requires
a union. The CoW runtime remains unchanged, so subsequent updates to the
local live mask do not modify the successor's saved state. On instrumented
modular self-compilation, `Array[i64]` clones fell from 966,353 to 870,856
and total MIR-generated record allocations from 7,437,326 to 7,343,095.
These counts do not establish a native runtime speedup; see
[MIR_PROFILE.md](MIR_PROFILE.md) for the full profile and test conditions.

## Direct ownership transfer (2026-10-24)

Forward CFG ownership propagation now executes its block instructions directly
rather than building two temporary register/value effect arrays per block.
This does not change bitset representation, CoW behavior or the C runtime ABI.
On modular self-compilation, generated MIR allocation calls fell from
7,343,095 to 7,016,214 (4.5%), with largely unchanged clone counts.
Revisiting a block repeats the instruction walk; profile on workloads with
highly iterative CFGs before assuming a speed improvement. See
[MIR_PROFILE.md](MIR_PROFILE.md) for measurement details.

## Inline scalar-record arrays (2026-10-28)

Integrated `SiriusLang-inline-record-arrays-step2.patch` against revision
`c685414`, adapting its documentation to the current reference layout.
On `tests/mir/source_inline_array_allocations.sirius` (512 two-field `Pair`
values), native `-O0` execution made 513 MIR record-allocation calls before
and 1 after. Both programs returned zero with no stdout. Counts came from an
identical temporary counter at `s_mir_record_new` in generated fixture C;
each compiler was built from its corresponding bootstrap. These counts exclude
strings, reallocations and allocations internal to dictionaries. No timing or
whole-compiler speedup is claimed.

Native/MIR inline-array tests, ARC auditing, CoW, optional indexing, structural
equality, related regressions and the bootstrap fixture suite passed. The
bootstrap reproduced exactly at generation 2. ASan/UBSan validation could not
link with the installed Windows Clang: `clang_rt.asan.lib` is missing.
