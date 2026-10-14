# Runtime implementation review

The C runtime is the stable bootstrap boundary while the MIR backend becomes
standard. This review records the existing structures, current work and design
constraints; it does not propose language-level semantic changes.

## Current storage

| Type | Representation | Retention / update | Next investigation |
| --- | --- | --- | --- |
| `S_String` | Refcounted length + inline bytes | Immutable, shared by retain | Short allocations, substring-heavy parsing |
| `S_Array` | Refcounted descriptor + homogeneous contiguous allocation; callbacks for managed elements | Clone shared values, consume owner on append | Growth/capacity, avoiding temporary buffers and unnecessary clones |
| `S_Dict` | Refcounted open-addressed entries with one aligned allocation per key/value pair | Clone all entries on shared update | Consider slot-local payloads/slabs; preserve hashing and callback semantics |
| `S_Array_i64`, `S_Dict_i64_i64` | Older specialized layouts | Separate CoW implementation | Consolidate carefully after auditing legacy/bootstrap callers |
| MIR generated records/arrays | Type-directed `S_MirRecord` fields emitted by `mir_emit_c.sirius` | MIR ownership operations + runtime primitives | Preserve wrapper/array ABI |
| MIR generated dictionaries | `S_MirRecord` owner containing the generic `S_Dict` hash table | `S_Dict` callback-owned entries; wrapper and table have CoW | Benchmark hashing/allocations on compiler workloads |

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
  handwritten C references and the retained legacy emitter.
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

1. Measure allocations in **MIR-generated** collection and record operations;
   these are the relevant paths for compiler self-hosting.
2. Consider unifying generic and emitted runtime storage only after a concrete
   stable ABI and differential tests exist. Avoid merging specialized legacy
   structures merely because they have similar operations.
3. Further dictionary layout changes (inline entries, slabs) require evidence
   of a real workload bottleneck.
4. Continue testing managed values, aliasing and CoW with sanitizers.

Keep the C11 API stable through MIR-default migration. Functional behavior and
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
`S_Dict`, instead of alternating fields and linear key lookup. The outer
`S_MirRecord` remains the owned MIR register value and holds a single `S_Dict *`.
Its copy-on-write clone retains the generic dictionary; subsequent insertions
use `s_dict_insert` for independent snapshots. Typed copy/drop callbacks handle
managed keys and values. This reuses the shared C runtime without changing the
MIR instruction ABI or removing the retained legacy dictionary.

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
