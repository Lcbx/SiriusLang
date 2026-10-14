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
| MIR generated records/arrays/dicts | Type-directed storage emitted by `mir_emit_c.sirius` | MIR ownership operations + runtime primitives | Check overlap before unifying with generic runtime containers |

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

## Next runtime steps

1. Measure real compiler allocations and runtime container usage. Do not infer
   compiler speedups from synthetic microbenchmarks.
2. Investigate reducing dictionary entry overhead further (slot-local storage
   or slabs) only if profiles justify added complexity.
3. Audit the specialized legacy containers before consolidating or deprecating
   them; do not break the retained legacy emitter.
4. Continue testing managed/aliasing paths under ASan/UBSan on supported toolchains.

Keep the C11 API stable through MIR-default migration. Functional behavior and
snapshot semantics take priority over micro-optimizations.
