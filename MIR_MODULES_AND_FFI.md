# Sirius — MIR, Modules, Libraries, and FFI

> **Status:** Library/module/FFI design target. Concrete executable MIR serialization, named exports, cross-artifact merging and signature-checked concrete imports are implemented. Source-level module imports and generic library linking are not. Core semantics are defined in [LANGUAGE_DESIGN.md](LANGUAGE_DESIGN.md); current capabilities are in [README.md](README.md). API names and artifacts below are provisional.

## 1. Module resolution and visibility

Given a source root, `Math/Stats.sirius` defines module `Math.Stats`:

```python
import Math.Stats alias Stats

export def describe(xs)
	return Stats.mean(xs)
```

Imports name compile-time namespaces; they do not instantiate runtime module objects. An ordinary import does not make the imported namespace public. An explicit re-export does:

```python
export import Math.Stats alias Stats
```

Overload resolution for imported operators and any future support for multiple files per namespace remain undecided. Module-level `let` bindings evaluate during compilation and may have effects; importing a module does not execute initialization code in the final program. The current compiler only supports limited `i64` module constants.

## 2. Shared MIR execution

```text
Sirius source
    -> parse / analyze / specialize / lower
    -> typed MIR
         |-> C emitter -> native code
         |-> interpreter at compile time (deferred)
         |-> serialized executable MIR -> interpreter at runtime (implemented)
         `-> combinations of native and interpreted functions
```

### Compile-time evaluation

The compiler would use the general MIR interpreter to evaluate requested computations and module constants. No separate restricted interpreter, mandatory purity requirement, sandbox or execution budget is planned. I/O, native calls and other side effects would be allowed when the host provides them; builds need not be reproducible.

Explicit compile-time syntax, the ordering of compilation and evaluation, and compile-time operations that generate types or declarations remain to be designed. In particular, evaluating MIR while type-checking its own dependencies requires staged compilation.

### Serialized MIR

An **executable** artifact contains concrete callable functions and their transitive dependencies.
This is implemented as `SIRIUS-MIR-EXEC/1` in `src/mir_wire.sirius` with
`--emit-mir` and `--load-mir` runner options. It stores canonical type descriptors,
concrete MIR functions, entry index and CFG instructions. The loader verifies
the format, types and ownership before execution. Version compatibility is
currently exact. The separate `SIRIUS-MIR-LIB/1` format supports multiple
named concrete zero-argument roots and canonical structural signatures. It
validates names, indices and signatures on loading, and carries the entire
concrete transitive call graph, and supports dependency calls through `SIRIUS-MIR-LIB/2` import slots,
resolved by name and structural concrete signature at link time. The current
source-level bridge uses placeholder function bodies; no external declarations
or partial generic linking exist yet. A **library** artifact may additionally preserve generic function bodies for future specializations. Serialization must include persistent type identities, symbol references, and format/ABI version information, rather than process-local pointers.

The format is initially versioned for compiler compatibility, not promised as a permanently stable cross-version interchange standard.

## 3. Specializing across library boundaries

For an exported generic function:

```python
export def double(x)
	return x + x
```

The exported signature and record descriptors are sufficient to *type-check* a prospective call, but **not** to produce a specialization: the generic implementation and the overloads it depends on must also remain available.

For a call with concrete argument types:

1. Resolve the originating module/declaration and overloads.
2. Construct a specialization key from that identity and canonical concrete types (and any other specialization-relevant parameters).
3. Reuse an existing compatible native specialization when available.
4. Otherwise specialize the generic MIR, then either compile it ahead of time or execute its specialized MIR in the interpreter.

A purely native application performs required monomorphization at build/link time. A mixed application may specialize on first use and interpret a missing specialization; no native JIT compiler is required. Cached implementations must be invalidated when their defining implementation or ABI changes.

A single common specialization registry is a useful implementation goal, but its exact ownership and persistence are not settled.

## 4. Library artifacts and registration

Conceptually, a Sirius library provides:

| Part | Purpose |
| --- | --- |
| Portable interface metadata | Exported names, signatures, type descriptors/layouts, identity, dependencies, compatibility information |
| Generic MIR | Bodies and supporting definitions needed to create new concrete specializations |
| Native binary (optional) | Already compiled specializations and runtime support |
| C-callable registration hook (native binary) | Register native symbol addresses, descriptors, and implementations in a running host |

A possible **illustrative**, not finalized, entry point is:

```c
void sirius_register_v1(SiriusRegistry *registry);
```

A compiler/interpreter can call the hook when it can load and execute the library for its **host** platform. The interface metadata must also be readable without executing target code, allowing cross-compilation or inspection of an incompatible native binary. The native hook does not replace the generic MIR required for new specializations.

Details still to specify: artifact packaging, discovery/loading, registry lifetime, hook ABI, symbol/version identity, compatible target triples, and native/interpreted call adapters. Function pointers and type descriptors cannot be assumed to have the same process-local addresses in independently loaded libraries.

## 5. ABI and C interoperability

### Sirius native/interpreter boundary

Sirius values should use matching layouts and runtime operations when a native library and the interpreter target the same ABI. This includes canonical record ordering, primitive and array representations, reference counts, and CoW ownership. Matching data layout **does not by itself define** a function-call ABI; interpreted calls still require adapters, and native-to-interpreted calls eventually require dispatch/trampolines.

Canonical descriptor identity must be based on semantic type information and compatibility rules, not merely pointer equality.

### Third-party C boundary

C APIs may use declaration-order structures, padding, platform-dependent alignment, unions, bitfields, flexible arrays, borrowed pointers, allocator-specific buffers, and custom ownership conventions. A structurally equivalent Sirius record is **not automatically C ABI-compatible**.

Initial interop should use explicit wrappers with C primitives and opaque handles, converting to ordinary Sirius values at the boundary. Explicit C-layout records, imported C declarations, callbacks, and owned/buffer-sharing conversions can be added later. Their syntax and safety rules are undecided.

## 6. Suggested implementation order (proposal)

1. Define portable exported-symbol and type-descriptor metadata in a module interface.
2. Make generic MIR available to cross-module specialization and add canonical specialization keys.
3. **Done for concrete executables and named concrete libraries:** serialize/load specialized MIR, validate exported symbols and signatures, and compare dynamic execution. Extend to imported modules and generic interfaces later.
4. Evaluate module constants through the general MIR interpreter, then add explicit compile-time evaluation syntax.
5. Register already compiled native specializations through a versioned C hook.
6. Implement interpreted-to-native dispatch; later native-to-interpreted callbacks/trampolines.
7. Add explicit C wrappers and then richer foreign-layout facilities if justified.

These steps are architectural suggestions, not commitments or claims that all prerequisites are already satisfied.

## 7. Open questions

* Exact staging and syntax for `comptime` and compile-time type-level operations.
* Canonical type-identity encoding and native ABI compatibility across compiler versions.
* Generic overload visibility, recursive/cyclic specialization, and exported interface evolution.
* MIR versioning, dependency closure, and whether packages contain one artifact or several.
* The registration hook ABI, ownership of the registry, and how native/interpreted calls exchange errors and values.
* C foreign record declarations, callbacks, ownership, and pointer-lifetime rules.
