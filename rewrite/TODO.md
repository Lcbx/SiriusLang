# Self-compilation plan

The goal is for the rewrite to compile its own Sirius sources to C11 and rebuild
itself reproducibly. A system C compiler and the existing C runtime remain build
dependencies. Python handles test/build orchestration only.

## Baseline and completion criteria

Today the original compiler builds the rewrite. The rewrite accepts one
`main() -> i64`, scalar/string expressions, `Optional[i64]`, interpolation and
local control flow. Its flat verified IR has no user-defined functions,
aggregate storage or module loader. See [README.md](README.md) for the subset;
[FEATURES.md](FEATURES.md) inventories the original compiler, not prerequisites
that must all be implemented before self-compilation.

Self-compilation is complete when:

- The rewrite compiles the complete source graph rooted at [main.sirius](main.sirius),
  including the interpreter, without using the original compiler for any module.
- Successive rewrite-built compilers emit identical C for that graph under the
  same options. Generated C compiles with the project's strict C11 checks.
- Rewrite-built compilers pass the rewrite tests, including interpreted/native
  behavior, invalid programs, ARC auditing and CoW snapshots.
- A separate rewrite C seed can reproduce that fixed point without the original
  compiler or its MIR runner. Existing MIR-based differential tests may still
  use the original runner as a reference.

## Ordered milestones

### 1. Functions and a shared type model

- [ ] Parse multiple declarations and function bodies; collect signatures before
  checking bodies so forward calls and mutual recursion work.
- [ ] Introduce canonical type IDs for scalars and composed types. Carry checked
  types, resolved names and call targets into per-function IR; do not recover
  types from syntax during C emission.
- [ ] Add typed parameters, results, direct calls and per-call interpreter frames.
  Preserve source-order argument evaluation and verify each function's inputs,
  returns, control flow and slot types.
- [ ] Define argument borrowing, owned results and cleanup on every return,
  initially for strings. Generate C declarations before function definitions.

Exit check: recursive and mutually recursive scalar/string functions run natively
and through the rewrite interpreter; wrong arity/types and missing returns fail.
Keep the existing flat control-flow representation until a concrete requirement
justifies replacing it.

### 2. Records, arrays and value-preserving updates

- [ ] Add annotated records, positional constructors, field reads and structural
  compatibility, including managed fields and composed annotations.
- [ ] Add array literals, `Array[T]()`, `len`, `.append`, array iteration and
  checked `[!index]` reads, including negative indices and bounds traps.
- [ ] Support local field/index updates such as `p.index += 1` and
  `p.nodes[!id].right = target`. Evaluate update paths once and apply CoW through
  the containing records/arrays, preserving snapshots and immutable bindings.
- [ ] Extend IR, verification, interpretation and C ownership together. Reuse
  suitable existing runtime operations/layout descriptors instead of introducing
  a second runtime or packing managed values into untyped integer slots.

Exit check: arrays of token-like records, nested arrays of initialization facts,
and parser-state-style updates preserve aliases and pass ARC auditing, including
overwrites, loop exits and early returns.

### 3. Optionals, dictionaries and string assembly

- [ ] Generalize `Optional[T]`, unwrapping, lazy defaults and `else return` to
  records, strings and other managed payloads. Add optional `[?index]` access.
- [ ] Add `Dict[K,V]()`, `.get`, `.insert` and `len`, starting with the actual
  `Dict[string, Binding]` symbol tables. Preserve CoW scope snapshots.
- [ ] Add separator `.join(Array[string])`, used by literal decoding and C
  emission, with UTF-8 byte and embedded-NUL behavior matching current strings.

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
- [ ] Compile the generic `compare` and `output` helpers in
  [interpreter.sirius](interpreter.sirius), preserving static comparisons and the
  specialized output calls. Reject unsupported operations during checking.

Exit check: repeated calls reuse one specialization; integer, Boolean and string
specializations agree between native and interpreted execution. First-class
functions, lambdas and general callback inference are not needed by today's graph.

### 6. Source modules and compiler I/O

- [ ] Load acyclic source imports with aliases and exports; preserve private
  names, qualified type/constructor references and cross-module specialization.
  Assign stable symbols and emission order independent of dictionary iteration.
- [ ] Support `arguments()`, `read_text` and `write_text`, including failure
  results, argument arrays, and `Optional[string]` early exits used by the CLI.
- [ ] Add a source-module compilation entry point rooted at `main`, while keeping
  single-file compilation and `--run`. Define the actual CLI before documenting
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
  `--run` parity and ARC-audited compiler executions as well as generated programs.
- [ ] After those checks pass, commit a separate rewrite C seed and document its
  regeneration/check procedure. Leave the original `bootstrap/compiler.c` and
  its tooling unchanged until a separately reviewed compiler cutover.

## Working rules and deferred work

For each milestone, keep focused valid/invalid fixtures and native/interpreted
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
