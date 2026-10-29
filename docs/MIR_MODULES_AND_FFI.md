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
specialization registry and C FFI support. The C interop syntax below is an
**agreed design**, not implemented behavior. Other illustrative APIs remain
design sketches.

## Compile-time execution

The proposed model uses the general MIR interpreter for requested computations
and module constants. It does not require a separate restricted interpreter,
mandatory purity, sandbox or execution budget. Host-provided I/O can have side
effects; builds need not be reproducible. The proposed C FFI described below
is **native-only**: the MIR runner rejects programs containing C FFI constructs.

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

Native and interpreted **Sirius** values targeting the same ABI should agree
on canonical record order, primitive/array representations, ARC and CoW
operations. Matching layouts alone do not define a calling convention:
interpreted-to-native calls need adapters; reverse callbacks need dispatch or
trampolines. Type-descriptor identity must use semantic compatibility, not
pointer equality. None of this implies compatibility with arbitrary C types.

The following specifies the **chosen initial C FFI design**, not yet implemented.
It is separate from Sirius-to-Sirius MIR library linking and from the future
native-library registration ABI. The native compiler emits C11; it does not
parse C declarations or inspect the layout of foreign structs.

### C source blocks

`inline C """..."""` is a **module-level** declaration containing verbatim C
translation-unit source. It may contain `#include` directives, types, helper
functions, and definitions. Multiple blocks are permitted and retain source
order. The compiler emits each block once for its originating module, before
emitted function bodies, without parsing or transforming its contents.

```sirius
inline C """
    #include <stdint.h>

    struct color {
        char r;
        char g;
        char b;
    };

    int64_t fast_square(int64_t x) {
        return x * x;
    }

    struct color make_color(void) {
        return (struct color){12, 34, 56};
    }

    int64_t red_component(struct color c) {
        return (unsigned char)c.r;
    }
"""

extern C rec color
extern C def fast_square(x: i64) -> i64
extern C def make_color() -> color
extern C def red_component(c: color) -> i64

def main() -> i64
    let c = make_color()
    return fast_square(red_component(c))
```

The embedded C is arbitrary native code, not a Sirius expression and not a
mechanism for accessing Sirius local variables. The host C compiler diagnoses
invalid C and symbol conflicts. There are no automatic safety or purity
guarantees for its side effects. Normal C linkage rules apply; repeated global
symbols across modules must be avoided by the author.

### Foreign declarations and symbol names

- `extern C def name(args...) -> T` declares a foreign function with the C ABI
  and the **unmangled** C symbol `name`. It has no Sirius body. Argument and
  result types must be supported FFI types. Sirius checks the declaration's
  own types, but does not verify the definition's signature in C; the C
  compiler can check it when a compatible C header/prototype is visible.
- `extern C rec name` declares an **opaque, nominal foreign value type** mapped
  to C `struct name`, passed and returned **by value**, not implicitly as a
  pointer. Its complete C definition must be available when generated C needs
  its size or passes it by value. The compiler delegates size, alignment, and
  calling convention to the target C compiler.
- A foreign record has no Sirius-visible fields, constructor, structural
  equivalence, destructuring, or record update. Reading a member requires an
  explicitly declared C function, such as `red_component` above. It is not
  interchangeable with an ordinary Sirius `rec`, even if the fields happen to
  agree. Imported references retain the original foreign type identity;
  independently declared types are not equated merely by spelling.
- Ordinary `extern def` keeps its existing meaning: an unresolved, typed
  **Sirius MIR-library** import. It is **not** an alias for `extern C def`.
- Foreign declarations can be exported and consumed through ordinary Sirius
  modules. An `import` of a Sirius FFI wrapper does not itself import or parse
  C headers; the wrapper supplies its `inline C` source/declarations.

Initial opaque records are C **values**, not a general facility for `T *`,
`void *`, incomplete pointer handles, arbitrary C unions, arrays, function
pointers, or callbacks. These require later explicit FFI types and rules.

### Supported boundary types

Only four Sirius types receive automatic conversions. Opaque foreign records
are passed through as C values without inspecting or converting their fields:

| Sirius declaration type | C signature representation | Conversion |
| --- | --- | --- |
| `i64` | `int64_t` | Signed 64-bit integer |
| `f64` | `double` | IEEE double value |
| `bool` | C11 `_Bool` / `bool` | Boolean value |
| `string` | `const char *` | Temporary NUL-terminated input / copied result |
| `extern C rec R` | `struct R` | Opaque C aggregate passed/returned by value; no field conversion |

`stdint.h` and `stdbool.h` are made available by the native emitter where
needed; authors may also include their C headers normally. Existing C
functions must match the declared ABI; other C numeric widths and C pointer
types do **not** silently map to Sirius types.

For `string` arguments, the bridge creates a temporary NUL-terminated UTF-8
buffer whose lifetime extends through the C call. The C function must not
retain the pointer. Embedded NUL bytes are rejected rather than silently
truncated. For `string` results, the bridge checks for a non-null C pointer,
then copies bytes through the first NUL into a Sirius-owned string before the
pointer may become invalid. A null result is an error, not an empty string.
No automatic `free` is performed: if the C function allocates a returned
buffer, a C wrapper must arrange its release after conversion, or use a
separate ownership protocol. This initial interface cannot represent binary
buffers or nullable C strings directly.

Opaque C records may be bound, passed, returned, and copied using the C
aggregate's ordinary value-copy behavior. Sirius does not inspect their
contents or manage any resources referenced *inside* them. In particular,
copying an opaque record does not duplicate pointed-to resources, and dropping
it does not call a C destructor. Initial use is therefore restricted to
C-value-safe aggregates; externally owned resources require explicit C
functions and a later handle/ownership design. Equality, hashing, matching
on fields, and automatic conversion to Sirius records are unavailable.
Support for placing foreign records into Sirius containers is not part of the
initial FFI scope.

### Calling an existing C library

A C header can be included in `inline C`; foreign functions are declared
explicitly in Sirius. No C header parser is implied:

```sirius
inline C """
    #include <math.h>
"""

extern C def sqrt(x: f64) -> f64
```

The build must also link the required native library, e.g. `libm` (`-lm` on
platforms that need it). Linker arguments or build configuration supply this;
**no `import C`, `link`, or `source` declaration syntax is adopted yet**.
Functions taking unsupported C types need an adapter written in a C source
block or a separately compiled C translation unit. The latter's build-system
integration remains separate from the language syntax.

### MIR and execution modes

C source and foreign declarations must survive source/HIR/MIR lowering as
explicit foreign metadata for native emission, rather than being mistaken for
ordinary Sirius calls or structural record types. The existing MIR operations
need not imply that opaque C values have a known Sirius memory layout.

**The MIR runner rejects a program containing any `inline C` block,
`extern C def`, or `extern C rec`, even if unused or unreachable.** It reports an
unsupported-C-FFI error instead of attempting interpretation, providing dummy
values, or calling native code. This also applies to compile-time MIR
execution of such a program. Native compilation is the only supported
execution path for the initial feature; no interpreter FFI adapter or native
library registration hook is required.

This restriction is specific to the **C** FFI. It does not change interpretation
of ordinary Sirius functions or the existing `.smirl` linking process.

### Boundaries and future extensions

The initial FFI deliberately omits automatic header translation, arbitrary C
pointers, field access, foreign-layout Sirius records, foreign callbacks,
implicit conversions for C integer widths, owned C handles/destructors, and
Sirius-managed containers crossing the boundary. In particular, matching
Sirius and C record fields is never sufficient for ABI compatibility: C can
depend on declaration order, alignment, padding, bitfields, unions, borrowed
pointers, and allocator-specific state.

Future extensions may add explicit pointer/handle types, nullable results,
foreign struct field declarations, library/link configuration, or native-call
adapters for MIR. Those are separate design decisions, not promises of
behavior in the initial FFI.

## Remaining design decisions

- Compile-time syntax, staging and type-level operations, apart from the
  explicit prohibition on interpreting C FFI programs.
- Type/symbol identity and native ABI compatibility across compiler versions.
- Generic overload visibility, cyclic specialization and interface evolution.
- Long-term MIR compatibility and dependency-bundle versioning.
- Registry ownership, hook ABI and cross-boundary error/value exchange.
- Foreign pointers, callbacks, external resource lifetime, build/link options,
  and whether/how to support opaque foreign values in arrays and dictionaries.
- Namespace-wide re-exports, multiple files per namespace and imported-operator
  overload policy. Current named-function overloads do not settle operator scope.

A possible longer-term implementation order is the native-only C FFI above,
general compile-time MIR evaluation for ordinary Sirius code, versioned native
registration, interpreted-to-native calls, then reverse callbacks and richer
C wrappers. This is an architectural suggestion, not a roadmap commitment or
evidence that prerequisites are complete.
