# Runtime storage and host services

Paths and commands are relative to the repository root. For ownership
contracts and native register layouts, see [Ownership](OWNERSHIP.md).
Dated benchmarks and allocator experiments are in
[Runtime measurements](history/RUNTIME_MEASUREMENTS.md).

## Current storage

| Type | Representation | Retention / update | Next investigation |
| --- | --- | --- | --- |
| `S_String` | Refcounted length + inline bytes | Immutable, shared by retain | Short allocations, substring-heavy parsing |
| `S_Array` | Refcounted descriptor + homogeneous contiguous allocation; callbacks for managed elements | Clone shared values, consume owner on append | Growth/capacity, avoiding temporary buffers and unnecessary clones |
| `S_Dict` | Refcounted open-addressed entries with one aligned allocation per key/value pair | Clone all entries on shared update | Consider slot-local payloads/slabs; preserve hashing and callback semantics |
| `S_Array_i64`, `S_Dict_i64_i64` | Older specialized layouts | Separate CoW implementation | Consolidate carefully after auditing legacy/bootstrap callers |
| MIR generated records/arrays | Eligible scalar records are inline in registers and strided array slots; other record slots remain boxed | MIR ownership operations + runtime primitives | Preserve wrapper/array ABI |
| MIR generated dictionaries | `S_MirRecord` owner containing the generic `S_Dict` hash table | `S_Dict` callback-owned entries; wrapper and table have CoW | Benchmark hashing/allocations on compiler workloads |

## Compiler-visible runtime layout metadata

The canonical `src/types.sirius` table now exposes a target-neutral
`RuntimeLayout`: storage category, ownership strategy and fixed/variable
`S_MirField` slot counts. These properties drive MIR C scalar/pointer types,
field selection and retain/release dispatch, and shared interpreter ownership.
For records, field slot indices follow canonical name sorting. Arrays use a
variable number of slots, with native byte strides specialized for eligible
scalar-only record elements; optionals use zero or one; dictionary wrappers hold
one `S_Dict *`. This metadata does **not** provide portable byte offsets or
an emitted runtime registration table. The C target controls union size and
alignment, and emitted `S_MirRecord.type_id` remains executable-local.
These emitter-specific layouts do not change the standalone C runtime API or
the on-disk `.smir` format. The interpreter keeps its logical heap layout.

## C collection APIs

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

## Generated MIR storage

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
`S_Dict`, instead of alternating fields and linear key lookup. Read-only cursor helpers
enumerate occupied hash slots without copying or mutating entries; MIR uses these to
rebuild dictionaries with recursively converted values. Cursors are invalidated by
mutations/rehashing and are not source values. The outer
`S_MirRecord` remains the owned MIR register value and holds a single `S_Dict *`.
Its copy-on-write clone retains the generic dictionary; subsequent insertions
use `s_dict_insert` for independent snapshots. Typed copy/drop callbacks handle
managed keys and values. This reuses the shared C runtime without changing the
MIR instruction ABI or removing the older specialized dictionary.

Key hashing and equality use values rather than identities. Supported keys
are `i64`, `bool`, `f64`, `string`, and records, optionals and labeled unions
whose payloads recursively contain hashable values. Arrays, dictionaries and
functions are not keys. Signed floating zero hashes identically; all NaNs
belong to one dictionary-key equivalence class. Ordinary source `==` keeps
IEEE NaN semantics. The interpreter uses the same key equivalence with linear
storage. See `tools/test_structural_equality.py`.

Validation: `python tools/test_mir_compiler.py` includes shared managed-record,
array and dictionary snapshots. `python tools/test_mir_selfhost.py` verifies
native fixed-point self-hosting.

### Inline scalar-record arrays

The native emitter uses inline array slots for records with 1-4 fields of
`i64`, `f64` or `bool`, subject to the exclusions in
[Ownership](OWNERSHIP.md#current-native-representation-generated-mir_emit_csirius).
Array allocation, indexing, append, update and CoW use the descriptor's stride.
Inline slots need no per-element allocation or ARC identity; pointer-valued
slots retain their existing copy/drop behavior. Optional projection still boxes
the selected record. Nested structural equality reads each scalar field rather
than comparing padding bytes.

`python tools/test_mir_inline_arrays.py` checks widths of 8, 16, 24 and 32 bytes,
CoW snapshots, optional access, nested equality, ARC auditing and MIR parity.
`--sanitize` adds AddressSanitizer and UndefinedBehaviorSanitizer where supported.

### Hybrid tagged unions

For a payload-bearing union with any record alternative of up to three
unmanaged scalar fields (`i64`, `f64`, `bool`, or an empty record), generated
native arrays use `S_MirArrayUnion<TypeId>` elements: a declaration-order
variant tag plus a fixed payload region. Eligible alternatives copy scalar
fields inline; other alternatives use a boxed-wrapper fallback indicated by
`tag == -1`. The buffer's byte stride is fixed by the maximum eligible scalar
payload width, not by the largest managed alternative. CoW and ARC scan/drop
retain or release only the boxed fallback edges.

Outside arrays, union registers and containers keep a pointer to an owned
wrapper. The wrapper stores the tag in `fields[0]` and eligible scalar payloads
in the following slots, without a separate payload object. Large and managed
alternatives retain the record pointer in `fields[1]`. Type-directed scanning,
dropping, cloning, equality, and hashing distinguish these layouts by type and
tag. Fresh scalar constructors reserve one extra slot so consuming injection
can reuse the allocation. Borrowed injections copy the scalar fields; projection
materializes an independent owned payload record. Reading an inline array slot
now allocates just its wrapper. The MIR interpreter retains its logical boxed
representation. Scalar payload projection still allocates, and registers and
non-array containers still require a wrapper.
`python tools/test_mir_inline_unions.py` validates scalar tags, managed/large
fallback, non-array containers, CoW, optional indexing, equality, hashing, ARC,
and native/MIR parity. The allocation fixture in
`tools/test_union_hybrid_coallocation.py` drops from 1022 to 769 generated-record
allocations versus the array-only representation; scalar projection still allocates.

## Generated field helpers

The generated `S_MirField` union member is selected by one Sirius helper
(`mir_c_field_name`) for scalar, string and managed record payloads.
Array and dictionary optional lookups now share one C helper,
`s_mir_optional_from_field`, which retains managed values according to the
canonical `Optional[T]` descriptor. Missing entries pass a null pointer;
this avoids duplicate payload copying and retains while preserving CoW.
This is a source/runtime cleanup, not a change to the public C runtime ABI.

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

## Runtime failure and ARC exit policy

Recoverable failures use ordinary return values: `Optional`, status values,
and constructor-based result unions with exhaustive `match`.
Their managed payloads use the same retain/move/release obligations as successful
returns. Default floating-point arithmetic can yield IEEE-754 NaN/infinity
without trapping; integer division by zero remains fatal.

`s_panic` terminates the process without walking live caller frames. Under
`SIRIUS_ARC_AUDIT`, immediate retain/release consistency checks still apply,
but the `atexit` leak check is skipped on this fatal path. A normally returning
program must release all credits, including those held by recoverable error
values. There is no supported exception unwinder in the current runtime.

### Heap-edge ARC audit

With `SIRIUS_ARC_AUDIT`, generated MIR C registers type-directed heap-edge
scanners for records, arrays, optionals and dictionaries. Each CFG block
checks that live managed objects have at least as many reference credits as
owned incoming heap edges. A test-only `s_arc_audit_check_roots()` API checks
equality when given the complete collection of register/stack/global roots.
Regular builds compile out both checks. The audit does not scan all
standalone native/FFI layouts; generated-frame root snapshots are described below;
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
