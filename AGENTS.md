Project overview
Sirius is a statically typed, compiled-first, functional-first language with Python-like syntax, immutable values, structural typing, and NumPy-like numerical computing. Local imperative control flow expresses sequential algorithms and local state.

Features:
  Immutable values: records and arrays have value semantics; copy-on-write makes updates efficient.
  Local imperatives: mutation is confined to local bindings and loops.
  Functional transformations: pure array operations express independent computations, eligible for   fusion, SIMD, and parallelization.
  Static duck typing: structural compatibility is resolved at compile time by specialization.
  Universal types: canonical type descriptors support memory layout, introspection, and explicit   dynamic typing.
  Acyclic memory: no mutable shared objects and no capturing closures; reference counting without a   tracing GC.

Pipeline: Source to typed HIR to MIR to C11 to native executable. Typed MIR can also be run by an interpreter.

Implemented: records, arrays, dictionaries, tagged unions, matching, generics, first-class functions, modules, ownership analysis. Capturing closures are not planned.

Source of truth: Implementation and tests over documentation for current behavior; LANGUAGE_DESIGN.md for intended semantics.

Directives:
Be concise, both when replying or in code.
Verify before acting. Inspect current code, docs, tests.
Identify a concrete problem or benefit before implementing.
Keep changes focused.
Respect language semantics.
Measure optimizations.
Revert unsuccessful experiments.
Maintain self-hosting. Implement compiler in Sirius; use Python only for tooling. Regenerate bootstrap/compiler.c when sources change.
Test proportionately. Cover valid/invalid programs, native/interpreted execution, ownership/ARC, CoW, and fixed points.
Summarize changes, validation, limitations, and regressions without aggrandizing the work done.
"Go on" from the user means : keep on current task if relevant or choose the next worthwhile task from ROADMAP.md priorities