# Self-compilation plan

The goal is for the rewrite to compile its own Sirius sources to C11 and rebuild
itself reproducibly. A system C compiler and the existing C runtime remain build
dependencies. Python handles test/build orchestration only.

## Baseline and completion criteria

Today the original compiler builds the rewrite. The executable subset accepts
typed functions with one `main() -> i64`, scalar/string expressions,
`Optional[T]` for executable payloads, interpolation and local control flow. Forward and mutually
recursive calls execute natively.
Per-function flat IR carries checked types and call targets; the verifier checks
all bodies, including unused functions. Records with scalar/string fields
now support constructors, field reads, direct updates on mutable bindings,
copies and calls in native C. `Array[i64]` and `Array[string]` now support literals, iteration,
checked reads and CoW updates, including arrays of records with managed
fields; recursive field/index updates now work for supported aggregate types,
including recursive `Array[T]` layouts for supported scalars and records.
A module loader remains unimplemented.
Previously declared record types are accepted as record fields. Array-valued
record fields (`Array[i64]`, `Array[string]`, `Array[Record]`) are supported,
including records stored in arrays.
See [README.md](README.md) for the subset;
[FEATURES.md](FEATURES.md) inventories the original compiler, not prerequisites
that must all be implemented before self-compilation.

Self-compilation is complete when:

- The rewrite compiles the complete source graph rooted at [main.sirius](main.sirius),
  without using the original compiler for any module.
- Successive rewrite-built compilers emit identical C for that graph under the
  same options. Generated C compiles with the project's strict C11 checks.
- Rewrite-built compilers pass the rewrite tests, including native execution, invalid programs, ARC auditing and CoW snapshots.
- A separate rewrite C seed can reproduce that fixed point without the original
  compiler or its MIR runner.

## Ordered milestones

### 1. Functions and a shared type model

- [x] Parse multiple declarations and function bodies; collect signatures before
  checking bodies so forward calls and mutual recursion type-check. Checked calls
  carry resolved function IDs, and signatures carry the existing scalar type IDs.
  Covered natively by `test_rewrite_frontend.py`.
- [x] Introduce canonical type IDs for scalars and composed types. Carry checked
  types, resolved names and call targets into per-function IR; do not recover
  types from syntax during C emission. `IR.TypeTable` interns `Optional` descriptors
  by payload type ID, including nested types; three scalar IDs are reserved.
  The checker and both IR consumers share the table. Noncanonical caches,
  duplicate descriptors and invalid/cyclic payload links fail verification.
  `test_rewrite_types.py` covers interning, table snapshots and different optional
  IDs across native execution and C emission, with ARC checks. Managed optional
  payloads were subsequently implemented in milestone 3.
- [x] Add typed parameters, results, direct calls and per-call local frames.
  Preserve source-order argument evaluation and verify each function's inputs,
  returns, control flow and slot types.
- [x] Define argument borrowing, owned results and cleanup on every return,
  initially for strings. Generate C declarations before function definitions.

`test_rewrite_functions.py` covers recursive and mutually recursive scalar/string
calls, mixed argument types, source order, lazy defaults, early exits, string
snapshots, invalid calls, missing returns and callee traps. Generated programs
run with ARC auditing; direct IR tests verify function ownership contracts.
The pre-call compiler baseline already fails ARC credit checks when linked to an
audited runtime: C emission fails even for `return 0`. Full compiler-process auditing remains a blocker for the stage 7
audited bootstrap checks; these failures are separate from generated-program
ownership.

Exit check: recursive and mutually recursive scalar/string functions run natively
with audited generated C; wrong arity/types and missing returns fail.
Keep the existing flat control-flow representation until a concrete requirement
justifies replacing it.

### 2. Records, arrays and value-preserving updates

- [x] Add flat annotated records, positional constructors, field reads and
  structural identity, including managed string fields and record parameters/
  results. Canonical fields are sorted by name; constructor arguments retain
  declaration order. Concrete annotations require matching field names/types.
  Native C uses typed value structs and existing string ARC; typed value
  copies retain snapshots.
- [x] Support `Array[i64]` and `Array[string]` record fields in native C,
  with owned snapshots and recursive field-level array retain/release in native
  C, including arrays of records containing arrays. Covered by
  `test_rewrite_composed_records.py` and direct record IR tests.
- [x] Allow fields containing previously declared record types, retaining/releasing
  their contents recursively. Tests cover multi-level reads, replacement,
  calls and snapshots in
  `test_rewrite_nested_records.py`. Forward/cyclic record definitions remain unsupported.
- [x] Support record fields of type `Array[Array[i64]]`, including recursive
  updates, owned inner-element snapshots, and audited native C checks.
- [x] Support `Array[Array[string]]` record fields through managed element callbacks.
- [x] Extend record fields to nested arrays of records and deeper array layouts.
  Resolve `Array[T]` recursively from canonical type descriptors, including
  arrays of booleans. Optional payloads now support managed values in bindings,
  signatures and aggregate fields/elements, including nested optionals.
- [x] Add the `Array[i64]` subset: nonempty literals, `Array[i64]()`, `len`,
  `.append`, `for item in array`, checked `[!index]` reads (including Python-style
  negative indices), bounds traps and indexed mutable updates. Use the existing
  reference-counted `S_Array_i64` runtime with CoW; native results,
  the existing `test_rewrite_arrays.py` covers integer-array regressions,
  and `test_rewrite_managed_arrays.py` adds managed string-array ARC/parity checks.
  `array_ir_test_driver.sirius` verifies both element types.
- [x] Extend arrays to `string` elements using generic `S_Array` storage and
  string copy/drop callbacks; test CoW overwrite, append, function calls,
  iteration, UTF-8, embedded NUL, invalid types and bounds in native C.
- [x] Extend array elements to flat records with `i64`, `bool` and `string`
  fields. Reuse generic `S_Array` with per-record retain/release callbacks;
  cover construction, calls, iteration, overwrite, self-assignment, snapshots,
  invalid programs and bounds traps in `test_rewrite_record_arrays.py`.
- [x] Support `Array[Array[i64]]`: nested literals and empty construction,
  annotations, calls, iteration, append/set, negative checked indexing and
  recursive CoW updates. Generic `S_Array` owns `S_Array_i64 *` elements via
  retain/release callbacks; see `test_rewrite_nested_arrays.py`.
- [x] Support `Array[Array[string]]` with generic outer `S_Array` storage and retain/release callbacks for inner managed arrays; `test_rewrite_nested_text_arrays.py` covers native ARC, CoW, calls, record fields and errors.
- [x] Derive generic native array element copy/drop/get operations from type
  descriptors instead of separate nested-string, nested-integer and record
  array emitters. Keep `Array[i64]` specialized.
- [x] Extend array elements to nested record arrays and arbitrary acyclic
  array depth. Descriptor-driven element copy/drop callbacks recursively own
  managed record and array elements. Tests cover snapshots and update paths
  through multiple arrays, record fields and function boundaries;
  see `test_rewrite_arbitrary_nesting.py`.
- [x] Support direct field updates on mutable record bindings (`p.index += 1`,
  `p.text = new_text`) for flat scalar/string fields. Typed `SetField` updates
  the owned slot, retaining new strings and releasing overwritten ones. Source
  `FieldTarget` references the actual binding, avoiding a redundant full-record
  copy. Tested with ARC-audited generated C.
- [x] Support one-level indexed record-field updates (`tokens[!i].text = value`,
  `tokens[!i].kind += 1`). Read the record into a value slot, update the field,
  then write the record back via `ArraySet`. Evaluate the index once, enforce
  mutable array bindings, and preserve snapshots in audited C.
- [x] Extend field/index updates to arbitrary paths through supported
  record and array types (`outer.inner.name`, `outer.inner.data[!i]`,
  `rows[!i].inner.name`). Intermediate reads own values and are written back in
  reverse order through `SetField`/`ArraySet`; CoW preserves snapshots and
  indices are evaluated once. Covered by `test_rewrite_recursive_updates.py`.
  `p.nodes[!id].right` is now covered with record-array-valued fields.
  See `test_rewrite_record_array_fields.py` for owned record elements,
  function boundaries and audited CoW updates.
- [x] Extend native IR verification and C ownership through supported nested
  array/record layouts, reusing `S_Array`, typed callbacks and the specialized
  `S_Array_i64` storage. No second runtime is needed.

Exit check: arrays of token-like records, nested arrays of initialization facts,
and parser-state-style updates preserve aliases and pass ARC auditing, including
overwrites, loop exits and early returns.

### 3. Optionals, dictionaries and string assembly

- [x] Generalize `Optional[T]`, unwrapping, lazy defaults and `else return` to
  records, strings, arrays, dictionaries and other executable payloads, with
  type-specific ARC ownership (`test_rewrite_optionals.py`).
- [x] Support `Optional[T]` in record fields, array elements and dictionary
  values, including nested optionals and CoW/ARC snapshots.
- [x] Add optional `[?index]` reads for arrays and dictionaries, including
  negative array indices and ARC-owned aggregate payloads.
- [x] Add explicit `T?(value)` and `T?()` optional construction, including nested
  optionals, managed payload ownership, type verification and native ARC tests.
  Retain `Optional[T]` for annotations; `T?` annotation sugar is not in scope.
- [x] Add canonical `Dict[K,V]` descriptors, `Dict[K,V]()`, `.insert`, `len`,
  and checked `[!key]` reads for `i64` and `string` keys with supported
  acyclic values. Native C uses generic dictionary copy/drop callbacks; CoW
  and ARC snapshots are covered by `test_rewrite_dictionaries.py`.
- [x] Add `.get(key) -> Optional[V]` to dictionaries, including managed record,
  array and dictionary payloads; checked reads continue to trap on absence.
- [x] Support `for key in dict` with a read-only snapshot and no mandated
  key order; native lowering uses the existing dictionary cursor runtime.
  Cover `i64` and `string` keys, nested loops, CoW and managed symbol-table
  values with ARC-audited native tests.
- [ ] Migrate the rewrite's symbol-table operations to rewrite-built native
  code as later source features become available.
- [x] Add separator `.join(Array[string])`, reusing the existing runtime
  implementation with UTF-8 byte and embedded-NUL behavior. Cover empty and
  singleton arrays, nested calls, ARC and invalid operand types.

Exit check: the scoped binding dictionary patterns in [semantic.sirius](semantic.sirius)
and chunk assembly in [lower.sirius](lower.sirius) run in small standalone programs;
missing lookups, managed defaults and shared updates are covered.

### 4. Enums, labeled unions and matching

- [ ] Add enum declarations, record-based union aliases, contextual constructor
  selection and explicit `Union.Label(payload)` construction. Collect dependent
  declarations before resolving their types, including enum fields in unions.
- [ ] Implement `Union.index(label_or_value)` in declaration order.
- [ ] Add exhaustive `match`: payload binding, bare tags, grouped alternatives
  and shared compatible payload binders, as used by [ir.sirius](ir.sirius).
- [ ] Implement structural equality for the enum/record/union values used by the
  compiler, without replacing structural types with nominal record identities.

Exit check: the token/node enums and instruction union shapes can be declared,
constructed and matched. Same-shaped labels stay distinct; nonexhaustive matches,
invalid payloads and incompatible grouped binders fail.

### 5. Specialization required by the compiler

- [ ] Specialize functions with omitted parameter types from concrete call sites;
  infer results and cache specializations by function identity and argument types.
  Require annotations where recursive result inference cannot terminate.
- [ ] Implement the generic comparison and output operations required by
  compiler sources, specializing native C operations by checked type.
  Reject unsupported operations during checking.

Exit check: repeated calls reuse one specialization; integer, Boolean and string
specializations execute correctly in audited native C. First-class
functions, lambdas and general callback inference are not needed by today's graph.

### 6. Source modules and compiler I/O

- [ ] Load acyclic source imports with aliases and exports; preserve private
  names, qualified type/constructor references and cross-module specialization.
  Assign stable symbols and emission order independent of dictionary iteration.
- [ ] Support `arguments()`, `read_text` and `write_text`, including failure
  results, argument arrays, and `Optional[string]` early exits used by the CLI.
- [ ] Add a source-module compilation entry point rooted at `main`, while keeping
  single-file compilation. Define the actual CLI before documenting
  a bootstrap command; no existing artifact formats are required for this stage.

Exit checks: compile [lexer.sirius](lexer.sirius) with its test driver first, then
the parser, checker, IR verifier and full compiler graph. Test missing/private
imports, cycles, colliding names and I/O errors. Track each remaining source
construct as a concrete blocker rather than flattening modules or stripping types
to get a one-off build.

### 7. Self-compilation and a separate seed

- [ ] Add a rewrite-specific build/fixed-point test under `rewrite/`. Pin the
  source graph, entry module, runtime, compiler options and deterministic output;
  bypass stale caches when checking generations.
- [ ] Use the original compiler once to build rewrite compiler R1. R1 compiles
  the same rewrite sources to R2 C; build R2. R2 emits R3 C; build R3. R3 emits
  R4 C. Require R3 C and R4 C to match byte-for-byte, allowing only a documented
  line-ending normalization, and investigate rather than hide differences.
- [ ] Run rewrite tests with R2/R3 as the compilers under test. Add a standalone
  path that needs only the rewrite seed, runtime and system C compiler; include
  ARC-audited compiler executions as well as generated programs.
- [ ] After those checks pass, commit a separate rewrite C seed and document its
  regeneration/check procedure. Leave the original `bootstrap/compiler.c` and
  its tooling unchanged until a separately reviewed compiler cutover.

## Working rules and deferred work

For each milestone, keep focused valid/invalid fixtures and native C
checks. Shared features can use the original compiler as a reference; interpolation
and other rewrite-only behavior need independent expectations from the design.
Add ARC/CoW tests with each managed type, rather than deferring ownership until
the end. Keep the rewrite buildable by the original compiler during the transition.

Use the current arena IDs, dictionary lookup and bitset facts where suitable.
Separate semantic checking from lowering as types grow, but avoid speculative
frameworks or storage rewrites. Defer profiling until the rewrite is complete,
as requested; measure later optimizations and revert unsuccessful experiments.

Full compatibility with the original compiler is a later target: floating-point
computing, broader collection APIs, first-class functions, package serialization,
source-free generics, optimization passes and parallel execution do not block
self-compilation unless the rewrite sources acquire a concrete dependency.

Current regression command: `python tools/test_all.py --group rewrite`.
