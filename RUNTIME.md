# Sirius runtime storage and ownership

The checked-in C11 runtime (`runtime/sirius_runtime.{c,h}`) is a bootstrap and
legacy-API boundary. The MIR C emitter additionally generates type-directed
storage for records, arrays and optionals; it reuses generic `S_Dict` for
hash-table storage. All implementations must preserve immutable-value,
reference-counted CoW semantics. See [IR_DESIGN.md](IR_DESIGN.md) for the
ownership passes, and [MIR_PROFILE.md](MIR_PROFILE.md) for performance evidence.

## Current representations

| Representation | Storage and update policy |
| --- | --- |
| `S_String` | Refcounted length and inline UTF-8 bytes; immutable shared value |
| Generic `S_Array` | Refcounted contiguous elements, typed copy/drop callbacks, consuming CoW append/set |
| Generic `S_Dict` | Refcounted open-addressed table, aligned key/value allocations, typed copy/drop callbacks |
| `S_Array_i64`, `S_Dict_i64_i64` | Retained specialized C ABI for legacy and handwritten C clients |
| MIR-generated records, arrays, optionals | `S_MirRecord` with compact type-selected `S_MirField` union; recursive retain/drop and CoW |
| MIR-generated dictionaries | Owned `S_MirRecord` wrapper around generic hashed `S_Dict`, with typed callbacks |

**Do not conflate** the specialized, generic and generated representations.
Merging them would be an ABI/ownership migration, not merely deduplication.
The interpreter uses its own simpler logical heap, including linear dictionary
storage, while implementing corresponding observable semantics.

## Implemented collection improvements

- Generic arrays use `realloc` for unique-owner capacity growth and stage
  potentially aliasing incoming elements in aligned stack storage when small
  (128 bytes), falling back to a heap temporary for larger elements.
- Generic dictionaries store an aligned key/value payload together per slot,
  preserving borrowed inputs when an insertion can clone or rehash.
- Both dictionary implementations clone a shared table at its existing
  capacity. Overwriting an existing key does not trigger unnecessary growth;
  the unique-owner generic overwrite keeps the key and only copies/replaces
  the value, including managed keys and values.
- Generated records, arrays and optionals use descriptor-directed retains,
  drops and CoW clones. Unique owners may update in place. Direct record-array
  append avoids a temporary alias and needless cloning.
- MIR dictionaries use `S_Dict` open addressing. Hashable keys currently include
  supported primitive types and records recursively composed of hashable
  fields; floating signed zero is normalized. Arrays, dictionaries and
  optionals are not supported as dictionary keys.
- Native array/optional dictionary projections share type-directed payload
  helpers; these are internal emitter/runtime cleanups, not public C ABI changes.

Both callback-managed snapshots and same-container aliasing need regression
coverage: the source element can refer to the destination collection before a
`realloc`, overwrite or CoW clone. Evaluate/retain borrowed inputs before
releasing the previous value.

## Evidence and profiling

Run the correctness suites:

```sh
python tools/test_runtime_collections.py
python tools/test_reference_c.py
python tools/test_mir_dict.py
python tools/test_mir_compiler.py
```

`test_reference_c.py` exercises independent hand-written C code using the
existing runtime API. The collection tests also cover managed CoW, replacement
and rehash behavior; sanitizer testing should be repeated on supported
platforms rather than assumed from a prior run.

For measurements:

```sh
python tools/benchmark_dictionaries.py --runs 3
python tools/profile_runtime_dict.py --runs 3
python tools/profile_ir_storage.py
```

The dictionary microbenchmark compares `S_Dict` with a **contiguous linear
search proxy**, not a historical MIR compiler build. On one prior machine,
hashed search overtook linear search for larger maps (roughly 32–128+ keys);
this supports keeping one hashed implementation for now. These nanosecond-scale
measurements are not estimates of end-to-end compile time.

Self-compilation profiling showed the old HIR body-state initialization created
unnecessary built-in `TypeTable`s. Passing the existing table reduced
instrumented dictionary constructions from **61,200 to 211** and inserts from
**390,976 to 25,034**, while sample native compiler wall times still
**overlapped**. An initial-capacity change and inline-bucket experiment were
reverted for lack of consistent speedup. Avoid new dictionary modes without
an actual workload bottleneck.

## Next steps

1. Measure real MIR-generated records/arrays/dictionaries and ownership CoW
   before altering layouts. Requested allocation bytes are **cumulative**, not
   resident memory; clone counts alone are not running times.
2. Investigate parser arena and CFG ownership state allocations, where prior
   self-compilation profiling found larger costs than generic dictionary
   lookups.
3. Preserve source-level snapshot semantics, source-alias behavior and the
   legacy C API when changing storage. Add focused native/interpreter,
   sanitizer and fixed-point coverage for every change.
4. Consider merging runtime representations only with an explicit versioned
   ABI, defined ownership operations and tested migration of consumers.
