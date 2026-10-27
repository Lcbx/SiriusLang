Project overview
Sirius is a statically typed, compiled-first, functional-first language with Python-like syntax, immutable values, structural typing, and NumPy-like numerical computing. Local imperative control flow expresses sequential algorithms and local state.

Design principles:
Immutable values: records and arrays have value semantics; copy-on-write makes updates efficient.
Local imperatives: mutation is confined to local bindings and loops.
Functional transformations: pure array operations express independent computations, eligible for fusion, SIMD, and parallelization.
Static duck typing: structural compatibility is resolved at compile time by specialization.
Universal types: canonical type descriptors support memory layout, introspection, and explicit dynamic typing.
Acyclic memory: no mutable shared objects and no capturing closures; reference counting without a tracing GC.

Pipeline: Source to typed HIR to MIR to C11 to native executable. Typed MIR can also be run by an interpreter.

Status:
Implemented: records, arrays, dictionaries, tagged unions, matching, generics, first-class functions, modules, ownership analysis. Capturing closures are not planned.

Project references: README.md, LANGUAGE_DESIGN.md, ROADMAP.md, docs/, src/, runtime/, stdlib/, tests/, examples/, tools/.
Source of truth: Implementation and tests over documentation for current behavior; LANGUAGE_DESIGN.md for intended semantics.

Directives:
1. Verify before acting. Inspect current code, docs, tests, and Git history. Never assume.
2. Justify every change. Identify a concrete problem or benefit before implementing.
3. Challenge the approach. Address root causes, consider simpler alternatives and consequences.
4. Keep changes focused. Follow roadmap, favor composition, avoid speculative abstractions.
5. Respect language semantics. Consult LANGUAGE_DESIGN.md, preserve value semantics and acyclic memory.
6. Measure optimizations. Establish baselines, compare on representative workloads, account for all costs.
7. Revert unsuccessful experiments. Revert changes with no benefit and report negative results.
8. Maintain self-hosting. Implement compiler in Sirius; use Python only for tooling. Regenerate bootstrap/compiler.c when sources change.
9. Test proportionately. Cover valid/invalid programs, native/interpreted execution, ownership/ARC, CoW, and fixed points.
10. Deliver reviewable changes. Provide incremental Git diffs, keep generated files separate, preserve style.
11. Report accurately. Summarize changes, validation, limitations, and regressions without fabrication.
12. Go on. Choose the next worthwhile task from ROADMAP.md priorities, verify, and implement or stop.