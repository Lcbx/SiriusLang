# Module and FFI architecture

This document describes **proposed extensions** to the implemented module
system. For current syntax, commands and package restrictions, use
[Modules and packages](MODULES.md); for concrete MIR loading and execution,
use [Interpreter](INTERPRETER.md). Intended language semantics are in
[Language design](../LANGUAGE_DESIGN.md).

## Current boundary

Source modules, concrete `.smirl` linking and source-free `.smirg`
specialization work at build time. `.smirg` carries syntax/typed-HIR snapshots,
not executable generic MIR. Native-module imports build the compiler itself;
`bootstrap/compiler.c` is generated from that module graph.

The following remain unimplemented: general compile-time MIR execution,
native-library registration, mixed native/interpreted calls, a runtime generic
specialization registry and a stable foreign ABI. Illustrative APIs below
are design sketches.

## Compile-time execution

The proposed model uses the general MIR interpreter for requested computations
and module constants. It does not require a separate restricted interpreter,
mandatory purity, sandbox or execution budget. Host-provided I/O and native
calls could have side effects; builds need not be reproducible.

Staging and explicit compile-time syntax remain undecided, including operations
that produce types or declarations. MIR evaluation during type-checking must
first compile its own dependencies. Current module constants use supported
compile-time expressions; importing a module does not execute arbitrary
runtime initialization.

## Specialization across library boundaries

For a concrete call, the target process is:

1. Resolve the originating declaration and applicable overload.
2. Form a key from declaration identity, canonical argument types and any
   other specialization-relevant parameters.
3. Reuse a compatible native specialization if available.
4. Otherwise specialize the generic representation and either compile it
   ahead of time or execute concrete MIR.

Native applications perform required monomorphization at build/link time.
A future mixed application could specialize on first use and interpret a
missing specialization, without a native JIT. Cached implementations must be
invalidated when implementation or ABI compatibility changes. Ownership and
persistence of a shared specialization registry remain open.

Current source and `.smirg` builds already perform build-time specialization;
this does not establish a runtime registry or native binary lookup mechanism.
Ordinary concrete function bodies are linked from source or `.smirl`, not
transported as concrete exports in `.smirg`.

## Native library registration

A future library would provide:

| Component | Purpose |
| --- | --- |
| Portable interface | Export names/signatures, semantic type identities, layout compatibility, dependencies and versions |
| Generic representation | Definitions needed for new concrete specializations |
| Optional native binary | Already compiled specializations and runtime support |
| C-callable registration hook | Publish addresses, descriptors and implementations to a running host |

An illustrative hook, with no finalized ABI:

```c
void sirius_register_v1(SiriusRegistry *registry);
```

The compiler/interpreter may execute that hook only for a compatible host
binary. Interface metadata must remain inspectable without running target
code, including during cross-compilation. The hook does not replace generic
bodies needed for new specializations. Independent binaries cannot assume
matching descriptor addresses or function pointers.

Still to specify: packaging and discovery, loading, registry lifetime, symbol
and version identity, compatible target triples, hook ABI and native/interpreted
call adapters.

## Calling conventions and C interoperability

Native and interpreted values targeting the same ABI should agree on canonical
record order, primitive/array representations, ARC and CoW operations. Matching
layouts alone do not define a calling convention: interpreted-to-native calls
need adapters; native-to-interpreted callbacks need dispatch or trampolines.
Descriptor identity must use semantic compatibility, not pointer equality.

Third-party C structures can depend on declaration order, padding, alignment,
unions, bitfields, flexible arrays, borrowed pointers, allocator-specific buffers
and custom ownership. Structural compatibility with a Sirius record does not
establish C ABI compatibility.

Start with explicit wrappers using C primitives and opaque handles, converting
to ordinary Sirius values at the boundary. Foreign-layout records, imported C
declarations, callbacks and shared-buffer conversions can follow once their
syntax, ownership and lifetime rules are defined.

## Remaining design decisions

- Compile-time syntax, staging and type-level operations.
- Type/symbol identity and native ABI compatibility across compiler versions.
- Generic overload visibility, cyclic specialization and interface evolution.
- Long-term MIR compatibility and dependency-bundle versioning.
- Registry ownership, hook ABI and cross-boundary error/value exchange.
- Foreign record layout, callbacks and pointer lifetimes.
- Namespace-wide re-exports, multiple files per namespace and imported-operator
  overload policy. Current named-function overloads do not settle operator scope.

A possible implementation order is general compile-time MIR evaluation,
versioned native registration, interpreted-to-native calls, then reverse
callbacks and richer C wrappers. This is an architectural suggestion, not a
roadmap commitment or evidence that prerequisites are complete.
