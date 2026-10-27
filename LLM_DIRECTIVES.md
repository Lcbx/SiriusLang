# SiriusLang — Project Directives

## Project overview

Sirius is a statically typed, compiled-first, functional-first language combining Python-like syntax, local imperative control flow, immutable values, structural typing, and NumPy-like numerical computing. Typed MIR also supports interpretation.

* **Immutable values:** Records and arrays have value semantics, with copy-on-write (CoW) for efficient updates.
* **Local imperatives:** Mutable bindings and conventional loops express sequential algorithms and local state.
* **Functional transformations:** Pure array operations express independent computations, eligible for fusion, SIMD, and parallelization.
* **Static duck-typing:** Structural compatibility through compile-time specialization (monomorphization).
* **Universal types:** Canonical descriptors support memory layout, introspection, and explicit dynamic typing.
* **Acyclic memory:** No mutable shared objects or capturing closures; reference counting without tracing GC.


Compiler pipeline: **Source → typed HIR → MIR → C11 → native executable**, with an alternative MIR interpreter.

Implemented features include records, arrays, dictionaries, tagged unions, matching, generics, first-class functions, modules, and ownership analysis. Compile-time execution, multidimensional arrays, capturing closures, purity analysis, and concurrency remain incomplete or planned.

## Project references

- `README.md` — overview and usage.
- `LANGUAGE_DESIGN.md` — intended semantics.
- `ROADMAP.md` — priorities.
- `docs/` — architecture and technical details.
- `src/`, `runtime/`, `stdlib/` — implementation.
- `tests/`, `examples/` — expected behavior.
- `tools/` — build, bootstrap, testing, profiling.

**Implementation and tests take precedence over outdated documentation.**

## Working directives

1. **Verify before acting.** Inspect the current code, documentation, and tests. Never assume a feature is missing, a bug exists, or a previous patch is absent.

2. **Evidence over activity.** Don't make changes merely to demonstrate progress. Identify a concrete problem or benefit before implementing anything. Prefer no change over an unjustified change.

3. **Challenge assumptions.** Assess whether the proposed approach actually solves the problem. Consider simpler alternatives, displaced costs, unintended consequences, and existing mechanisms.

4. **Keep changes focused.** Follow the roadmap, favor composition and existing infrastructure, avoid speculative abstractions and unrelated refactoring.

5. **Respect language semantics.** Preserve compatibility unless a breaking change is justified. Consult the language design before changing semantics.

6. **Measure optimizations.** Establish a baseline, account for all relevant costs (including CoW, ARC, memory, and compilation overhead), and measure representative end-to-end workloads. Don't confuse synthetic benchmarks, allocation counts, or theoretical improvements with actual performance gains.

7. **Revert unsuccessful experiments.** If a change provides no demonstrated benefit or clear structural justification, recommend reverting it. Negative results are useful results.

8. **Maintain self-hosting.** Implement compiler logic in Sirius; use Python primarily for tooling. Regenerate `bootstrap/compiler.c`, never edit it manually.

9. **Test proportionately.** Cover valid and invalid programs, native and interpreted execution, ownership/ARC, modules, and bootstrap fixed points as relevant. Distinguish tests actually run from those assumed to pass.

10. **Deliver reviewable changes.** Provide incremental Git diffs against an identified baseline, preserve existing style and comments where practical, and update affected documentation without duplication.

11. **Report accurately.** Summarize what changed, why, validation results, limitations, regressions, and next steps. State uncertainty and failures plainly. Never overstate progress or fabricate evidence.

For open-ended requests such as **"go on"**, choose the next coherent, worthwhile, testable roadmap task. Verify that it is needed before implementing it. Don't manufacture work to maintain momentum.