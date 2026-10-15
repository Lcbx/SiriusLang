# Sirius modules, MIR libraries and FFI

**Status:** Source modules, persistent typed generic artifacts and native/FFI registration
are proposed. Versioned concrete MIR executables, named multi-entry libraries,
cross-artifact merging, source `extern def` declarations and signature-checked
concrete imports are implemented. Resolved libraries can also emit native C.
Current commands are in [INTERPRETER.md](INTERPRETER.md); core semantics in
[LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md). Names and ABIs below are provisional.

## Modules and visibility (planned)

A file `Math/Stats.sirius`, relative to a source root, defines `Math.Stats`.
Imports are compile-time namespaces, not runtime-initialized objects:

```sirius
import Math.Stats alias Stats
export def describe(xs)
	return Stats.mean(xs)
```

Private declarations are the default. `export` exposes a definition;
`export import Math.Stats alias Stats` explicitly re-exports a namespace.
Imports should be statically resolved, case-sensitive and initially acyclic.
Optional `namespace` declarations and multiple files per namespace are
**deferred**, rather than required for module resolution. Imported overload
visibility needs a more precise rule.

Module `let` values are evaluated at **compile time** and do not cause runtime
module initialization. A restricted compiler-side constant evaluator already
supports scalar, string, array and record expressions; general evaluation of
user functions through MIR remains future work. Compile-time effects may be
allowed, including I/O when available on the host.

## Shared execution model

```text
source -> syntax / types / concrete HIR specialization -> optimized CFG MIR
                                                         |-> C11 native code
                                                         |-> runtime interpreter
                                                         |-> versioned saved MIR
                                                         `-> comptime interpreter (planned)
```

The interpreter runs concrete MIR from source or a saved artifact.
`SIRIUS-MIR-EXEC/1` contains a concrete executable entry and its closed call
graph; `SIRIUS-MIR-LIB/1` contains named concrete roots and reachable functions.
`SIRIUS-MIR-LIB/2` adds unresolved concrete imports with canonical structural
signatures. The linker remaps types/functions, resolves imports against named
exports and verifies the result, emitting a resolved version-1 library.
Source consumers can use `extern def` (fully typed, no body) to declare imports
automatically; `--import` retains compatibility with legacy placeholder sources.
Provider specializations remain exposed through `--expose`. These are **internal
versioned formats**, not stable ABIs. `SIRIUS-MIR-LIB/3` retains provider
source plus named templates. Version 4 instead stores a syntax arena (tokens,
AST nodes/edges and source locations), omitting the original text and
reparsing during specialization; version-3 artifacts remain readable. At link time a missing concrete import can
specialize a template against the importer's canonical structural types,
reusing the HIR/MIR pipeline; equal requests share a specialization. This is a
syntax-backed intermediate step: generated specializations may have concrete external
imports, including imports fulfilled by other templates, which are expanded
transitively. Linked results contain only concrete specializations. `--emit-c-lib` emits
standalone C from a resolved library,
but does not implement cross-backend function-call dispatch.

### Compile-time MIR execution (planned)

Reuse the same MIR interpreter rather than maintaining a separate restricted
language evaluator. Initially do **not** require inferred purity, a sandbox,
or a mandatory execution budget. Such guardrails may be added later without
altering the basic interpreter. Compiler-host effects can make builds
non-reproducible; this is an accepted initial tradeoff.

The unsolved part is staging: how to type-check, specialize and execute MIR
when the computation generates types or definitions needed by later analysis.
Define `comptime` syntax, order of declaration evaluation, error propagation
and dependency handling before implementing it. This is distinct from the
longer-term transitive purity checking needed by parallel `map`.

## Generic specialization across libraries (partial)

A signature alone cannot instantiate a generic body. Version-6 artifacts persist syntax, token spellings, verified module declarations (function headers, module constant values and canonical types), and simple symbolic-type HIR bodies instead of provider text. This avoids reparsing and repeated module declaration analysis per specialization. Versions 3–5 remain readable. Version 6 validates the saved simple HIR plans against syntax and substitutes canonical concrete argument types when linking. Operators are checked again after substitution; complex bodies still use ordinary HIR analysis.
The linker remaps imported structural types into the provider's type table,
builds concrete MIR, recursively expands imports from other libraries, and
checks each concrete signature. External function declarations in template
sources still need concrete types. The remaining target is general parameterized HIR for control flow, calls, record/array operations and importable constructors.

At a call boundary:

1. Resolve the declaration's persistent identity and visible overloads.
2. Construct a specialization key from that identity and canonical argument
   types (plus any other specialization-relevant parameters).
3. Reuse a compatible compiled specialization, or specialize the generic
   body and compile or interpret the concrete MIR.
4. Ensure cache invalidation when the implementation or ABI changes.

A fully native program may specialize at build/link time; a mixed program may
interpret an absent specialization at runtime without a JIT. The registry,
artifact packaging and recursive/cyclic specialization policy are not yet
settled. Structural `TypeId` indices allocated within one compiler process
must never serve as persistent cross-library identities.

## Native artifact registration (planned)

A library interface needs exported names, canonical types/signatures, layout
and ownership metadata, dependencies and compatibility information. Generic
MIR is necessary for new specializations; an optional native binary provides
already-compiled ones. A versioned C-callable registration hook could expose
symbols to the interpreter/compiler, for example:

```c
/* Illustration only; no such API is implemented. */
void sirius_register_v1(SiriusRegistry *registry);
```

The host must be able to inspect interface metadata **without executing**
foreign target code. Native/interpreted dispatch still requires a calling
convention, ownership transfers and error adapters; matching record or array
layouts do not establish a function-call ABI or valid function pointers across
loaded binaries.

## External C interoperability (planned)

Sirius records have canonical **name-sorted** layouts. C structs may differ in
field order, alignment, padding, bitfields, unions, pointers, allocator and
lifetime rules. Structurally equal Sirius records are **not automatically C
ABI compatible**.

Begin with explicit wrappers around C scalar values and opaque handles, with
conversion and documented ownership at the boundary. Foreign-layout records,
callbacks, buffer sharing, imported headers and richer C pointer APIs can be
added deliberately; do not let the internal Sirius layout silently change to
fit an unrelated C ABI.

## Proposed sequence / unresolved contracts

1. Implement separate-source module resolution and explicit exported metadata.
2. Preserve generic bodies and persistent semantic type/declaration IDs across
   modules; implement cross-artifact call resolution and specialization.
3. Integrate MIR execution into compile-time evaluation once staging is clear.
4. Specify stable registry/call/ownership/error ABIs; register native symbols,
   then support interpreted→native and native→interpreted dispatch.
5. Add explicit foreign C wrappers before exposing richer foreign layouts.

Version negotiation, target compatibility, dependency cycles, generic overload
visibility, compile-time staging and native resource ownership remain open
architectural decisions. Existing concrete serialization is a useful base,
not evidence that these contracts are already solved.
