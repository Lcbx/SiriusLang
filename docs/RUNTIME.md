# Runtime implementation review

> Paths and commands in this document are relative to the repository root.

The C runtime remains the stable bootstrap boundary; the committed C compiler
now also supports the Sirius `i64` bitwise operators used for packed MIR ownership.
This is a compiler-side representation change, not a replacement of runtime
collection layouts.

## Current storage

| Type | Representation | Retention / update | Next investigation |
| --- | --- | --- | --- |
| `S_String` | Refcounted length + inline bytes | Immutable, shared by retain | Short allocations, substring-heavy parsing |
| `S_Array` | Refcounted descriptor + homogeneous contiguous allocation; callbacks for managed elements | Clone shared values, consume owner on append | Growth/capacity, avoiding temporary buffers and unnecessary clones |
| `S_Dict` | Refcounted open-addressed entries with one aligned allocation per key/value pair | Clone all entries on shared update | Consider slot-local payloads/slabs; preserve hashing and callback semantics |
| `S_Array_i64`, `S_Dict_i64_i64` | Older specialized layouts | Separate CoW implementation | Consolidate carefully after auditing legacy/bootstrap callers |
| MIR generated records/arrays | Type-directed `S_MirRecord` fields emitted by `mir_emit_c.sirius` | MIR ownership operations + runtime primitives | Preserve wrapper/array ABI |
| MIR generated dictionaries | `S_MirRecord` owner containing the generic `S_Dict` hash table | `S_Dict` callback-owned entries; wrapper and table have CoW | Benchmark hashing/allocations on compiler workloads |

## Compiler-visible runtime layout metadata

The canonical `src/types.sirius` table now exposes a target-neutral
`RuntimeLayout`: storage category, ownership strategy and fixed/variable
`S_MirField` slot counts. These properties drive MIR C scalar/pointer types,
field selection and retain/release dispatch, and shared interpreter ownership.
For records, field slot indices follow canonical name sorting. Arrays use a
variable number of slots; optionals use zero or one; dictionary wrappers hold
one `S_Dict *`. This metadata does **not** provide portable byte offsets or
an emitted runtime registration table. The C target controls union size and
alignment, and emitted `S_MirRecord.type_id` remains executable-local.
Neither the C runtime ABI nor the on-disk `.smir` format changed.

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

## Collection cleanup after MIR-default migration

Two collection implementations remain intentionally separate:

- `S_Array_i64` / `S_Dict_i64_i64` are publicly exposed scalar layouts used by
  handwritten C references and existing runtime clients.
- `S_Array` / `S_Dict` support callbacks for managed values and are used by
  generic runtime operations. The MIR C emitter uses `S_Dict` for dictionaries
  through an owned `S_MirRecord` wrapper. Records and arrays still use
  separate type-directed storage.

Merging these structures now would change their ABI without eliminating the
MIR-generated representation, so keep both until there is a tested consumer
migration plan. In particular, do not claim generic dictionary microbenchmark
improvements as self-compilation improvements.

Both dictionary implementations now clone a shared table at its existing
capacity instead of repeatedly reinserting/rehashing entries. The generic
clone also preserves occupied probe positions while individually retaining
managed key/value payloads. Insertions probe before growing: replacing an
existing key no longer causes growth when the table reaches its load threshold.
The scalar-only map now shares the same probe helper across get and insert.
`tests/runtime/collections.c` covers repeated overwrites near the threshold,
shared snapshots, borrowed-value keys, and the retained scalar API. Run
`python tools/test_runtime_collections.py` and the reference-C suite; the
collection test was also checked with Clang ASan/UBSan.

## Next runtime steps

1. **Measured:** MIR-generated record/collection allocation counts in self-hosting
   (see the October 2026 profile below). Continue with type-directed profiling
   before attempting further allocation or layout changes.
2. Consider unifying generic and emitted runtime storage only after a concrete
   stable ABI and differential tests exist. Avoid merging older specialized
   structures merely because they have similar operations.
3. Further dictionary layout changes (inline entries, slabs) require evidence
   of a real workload bottleneck.
4. Continue testing managed values, aliasing and CoW with sanitizers.

Keep the C11 API stable during runtime cleanup. Functional behavior and
snapshot semantics take priority over micro-optimizations.

## MIR-generated storage cleanup

The default compiler uses the type-directed `S_MirRecord` representation emitted by
`src/mir_emit_c.sirius`. Its records, optional values, arrays and dictionaries share
a compact tagged-by-type field union, but retain and release nested values according
to their *static* type descriptors.

Shared CoW cloning now uses **one** `s_mir_record_unique_owned` helper across
record-field updates, array set/append and the outer MIR dictionary wrapper. The helper copies
the field storage, retains managed fields by their type descriptor, then consumes
the original owner's reference. Collection operations still replace/release their
previous values normally, and unique owners continue to update in place. The
allocation constructor also guards against field-count arithmetic overflow.

The MIR dictionary payload now uses the existing generic open-addressed
`S_Dict`, instead of alternating fields and linear key lookup. Read-only cursor helpers enumerate occupied hash slots without copying or mutating entries; MIR uses these to rebuild dictionaries with recursively converted values. Cursors are invalidated by mutations/rehashing and are not source values. The outer
`S_MirRecord` remains the owned MIR register value and holds a single `S_Dict *`.
Its copy-on-write clone retains the generic dictionary; subsequent insertions
use `s_dict_insert` for independent snapshots. Typed copy/drop callbacks handle
managed keys and values. This reuses the shared C runtime without changing the
MIR instruction ABI or removing the older specialized dictionary.

Key hashing/equality is value-based: `i64`, `bool`, `f64` (including signed-zero
normalization), `string`, and canonical records recursively composed of these
hashable fields. Structural record hashing combines fields in canonical order,
similar in spirit to Python tuple hashing. Arrays, dictionaries, and optionals
remain unhashable keys until explicit equality semantics are defined. The
interpreter uses corresponding recursive equality but keeps its simple list
storage for debugging.

Validation: `python tools/test_mir_compiler.py` includes shared managed-record,
array and dictionary snapshots. `python tools/test_mir_selfhost.py` verifies
native fixed-point self-hosting.

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

## MIR emitter representation cleanup

The generated `S_MirField` union member is selected by one Sirius helper
(`mir_c_field_name`) for scalar, string and managed record payloads.
Array and dictionary optional lookups now share one C helper,
`s_mir_optional_from_field`, which retains managed values according to the
canonical `Optional[T]` descriptor. Missing entries pass a null pointer;
this avoids duplicate payload copying and retains while preserving CoW.
This is a source/runtime cleanup, not a change to the public C runtime ABI.

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

## Filesystem primitives

The generic filesystem surface starts with `list_dir(string) -> Optional[Array[string]]`
and `is_dir(string) -> bool`. C enumerates immediate entries with sorted UTF-8
names and explicit failure (`none`), while the MIR interpreter calls the same
Sirius builtins. Directory symlinks are not followed by `is_dir`. The
Sirius-side `walk_files` helper lives in `stdlib/Std/Filesystem.sirius`;
recursion policy is not hardwired in the C runtime. File I/O and directory
enumeration remain effectful and host-dependent. `read_text` and `write_text`
share a file-opening helper: MSVC uses `_wfopen_s` with UTF-8-to-UTF-16
path conversion, other Windows toolchains use `_wfopen`, and POSIX uses the
standard C `fopen`. The runtime does not suppress MSVC CRT warnings globally.
Embedded NUL bytes in paths are rejected. `tools/test_filesystem.py` checks
Unicode filenames, binary round-trips, and invalid paths.

`Std.Filesystem` also exports `read_text_result`, `write_text_result` and
`list_dir_result`. These return `ReadResult`, `WriteResult` and `ListResult`
constructor-based unions with exhaustive `match`. Their success variants are
`ReadSuccess(content: string)`, `WriteSuccess(bytes: i64)`, and
`ListSuccess(entries: Array[string])`; all share a `FilesystemFailure(operation:
string, path: string, kind: i64, code: i64)` error variant. The byte count is
reported only after a successful write. The wrappers preserve the existing
`Optional`/`bool` semantics and capture the OS error *immediately* after failure.

The C runtime keeps a thread-local snapshot for each `read_text`, `write_text`,
`list_dir` or `path_kind` attempt. On success, both getters return zero. On failure,
`filesystem_error_kind()` returns a platform-independent category:
`1=not_found`, `2=permission`, `3=not_directory`, `4=is_directory`,
`5=invalid_path`, `6=other`; `filesystem_error_code()` returns the nonzero
native numeric code (`errno` from POSIX/CRT file I/O, Win32 error from
Windows directory enumeration). The numeric code is platform-specific; do not
serialize it as a portable error taxonomy. Query the snapshot before another
filesystem operation overwrites it. General code should use the `Result`
wrappers instead of the ambient getters. This is not an atomic return of
value+error for arbitrary asynchronous interleaving, although the snapshot
itself is per-thread. `walk_files` keeps its best-effort behavior.
`path_kind(path) -> i64` performs an lstat-style metadata query: `0=error`,
`1=regular file`, `2=directory`, `3=other` (including symlinks/reparse points).
It sets the same error snapshot on failure and clears it on success.
`Std.Filesystem.path_kind_result(path) -> PathKindResult` wraps it in
`PathKindSuccess(kind)` or `FilesystemFailure("path_kind", path, kind, code)`.
`walk_files_result(root) -> WalkResult` checks both directory listing and
per-entry metadata, reporting the first error as `FilesystemFailure` with the
operation and affected path, otherwise returning `WalkSuccess(files)` (including
for an empty root). It preserves breadth-first ordering and does not follow
symlinks when deciding which children to traverse; a symlink supplied as the
root can still be enumerated. Filesystem changes between
listing and metadata lookup may still cause a reported error (TOCTOU); this
is not an atomic snapshot and already-collected files are not returned on error.
`tools/test_filesystem_result.py` and `tools/test_filesystem_walk_result.py`
check native/interpreted parity.

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
[MIR_PROFILE.md](history/MIR_PROFILE.md) for the full profile and test conditions.

## Direct ownership transfer (2026-10-24)

Forward CFG ownership propagation now executes its block instructions directly
rather than building two temporary register/value effect arrays per block.
This does not change bitset representation, CoW behavior or the C runtime ABI.
On modular self-compilation, generated MIR allocation calls fell from
7,343,095 to 7,016,214 (4.5%), with largely unchanged clone counts.
Revisiting a block repeats the instruction walk; profile on workloads with
highly iterative CFGs before assuming a speed improvement. See
[MIR_PROFILE.md](history/MIR_PROFILE.md) for measurement details.

## Runtime failure and ARC exit policy

Recoverable failures use ordinary return values (`Optional` and status values
now; constructor-based `Result` unions with exhaustive `match` are planned).
Their managed payloads use the same retain/move/release obligations as successful
returns. Default floating-point arithmetic can yield IEEE-754 NaN/infinity
without trapping; integer division by zero remains fatal.

`s_panic` terminates the process without walking live caller frames. Under
`SIRIUS_ARC_AUDIT`, immediate retain/release consistency checks still apply,
but the `atexit` leak check is skipped on this fatal path. A normally returning
program must release all credits, including those held by recoverable error
values. There is no supported exception unwinder in the current runtime.

### Opt-in heap-edge ARC audit (2026-10-24)

With `SIRIUS_ARC_AUDIT`, generated MIR C registers type-directed heap-edge
scanners for records, arrays, optionals and dictionaries. Each CFG block
checks that live managed objects have at least as many reference credits as
owned incoming heap edges. A test-only `s_arc_audit_check_roots()` API checks
equality when given the complete collection of register/stack/global roots.
Regular builds compile out both checks. The audit does not yet scan all
standalone native/FFI layouts or synthesize roots across every calling frame;
see [OWNERSHIP.md](OWNERSHIP.md).

### Native ARC root snapshots (audit builds)

With `SIRIUS_ARC_AUDIT`, generated MIR functions register their managed
register-slot addresses in a stack of `S_ArcAuditFrame` entries. Each basic-block
boundary calls `s_arc_audit_check_frames()` to count live roots from every
active caller/callee frame, known native globals (including CLI arguments),
and type-directed heap-child scanners. Shadow ARC credits must equal these
incoming credits. Transferred call arguments leave the caller slots before
callee execution; borrowed arguments retain their own credits.

This is a **dynamic** assertion at stable CFG boundaries, not a static proof or
an instruction-by-instruction invariant. Additional native/FFI objects need
explicit scanner and root registration to participate in exact snapshots.
Fatal `s_panic` still terminates without unwinding managed frames. Audit frame
registration and scanning incur no normal-runtime audit cost; debug-only
metadata is removed without the audit define. `MIR_CALL_MOVE` now materializes
call arguments before clearing moved caller slots in both build modes.
