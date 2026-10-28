# Source modules, libraries, and generic packages

Sirius has three separate mechanisms:

- **Source modules:** `import` plus exported declarations, discovered and
  compiled from a source root.
- **Concrete MIR libraries (`.smirl`):** optimized, specialized MIR with named
  exports/imports, linkable by structural signatures.
- **Generic declaration packages (`.smirg`):** validated, versioned typed-HIR
  snapshots used to specialize exported generic definitions **without source**.

Their formats are experimental, compiler-version-dependent, and **not** a
stable native plugin ABI. See [Interpreter](INTERPRETER.md) for executable
`.smir` (a different format), and [Modules/FFI architecture](MIR_MODULES_AND_FFI.md)
for design proposals.

## Import and export source modules

Given `Math/Stats.sirius` under a source root:

```sirius
export def increment(x: i64) -> i64
	return x + 1
```

And `App.sirius`:

```sirius
import Math.Stats alias Stats

def main() -> i64
	print(Stats.increment(41))
	return 0
```

Build and run via the native MIR runner:

```sh
python tools/build_mir_runner.py
./sirius-run --build-modules . App app.smirl
./sirius-run --entry M_3_App_4_main --load-lib app.smirl
```

The `.` in the example is the source root containing both files. In a
repository fixture, choose its own directory as the root. Python's
`tools/build_modules.py` provides an optional launcher; Sirius itself resolves
and links the source graph. Native C modules can be compiled with
`./sirius --module-root . App app.c`.

Supported public declarations include `export def`, `export rec`, `export let`,
union aliases, and named member re-exports (`export Alias.member alias Name`).
Qualified imports resolve functions, constants, constructors, and public type
annotations; named re-exports can be transitive. Public constructors can
publish computed fields and fixed-type conditional overrides. Their imported
bodies can use defining-module constants and private helper functions (also
through `.smirg` packages), without capturing names in the consumer.

Concrete exported overloads use deterministic, length-prefixed type-signature
mangling (the `_O` symbol suffix), while non-overloaded public names keep
their existing ABI names. Both qualified imports and named re-exports resolve
by parameter types. Existing on-demand generic specializations retain the
separate `_G` symbol suffix. Full type encodings remain unambiguous and
TypeId-independent. MIR import/export names support up to 4096 characters, accommodating larger
structural signatures within the `SIRIUS-MIR-LIB/2` format. Libraries using longer
symbols require a compiler with the increased acceptance bound. Concrete
overload bodies belong in linked `.smirl` libraries; source-free `.smirg`
packages do not carry them.

Concrete exported functions require explicit parameter and return types.
Generic exports with at least one unannotated parameter may omit the return
annotation: each concrete specialization infers its result from the body.
Imports, named re-exports and source-free `.smirg` definitions preserve this
inference; recursive inferred returns still require an explicit return type.
Generic imports also retain enough defining-module syntax to derive callback
parameter types from concrete caller arguments, even when the exported result
has an explicit return annotation. This works with source-free `.smirg` packages.
References in transported generic bodies—including qualified calls, function
values, constructor calls, type annotations and match alternatives—bind to
the defining module's imports, not similarly named aliases in the consumer.
A missing defining-module import is rejected instead of falling back to the
consumer namespace.
Exported `let` values are compile-time immutable scalars, strings, homogeneous
arrays or structural records (including nested combinations). Namespace-wide
re-exports, standalone type aliases, and runtime module objects are not
available.

## Concrete MIR libraries (`.smirl`)

Emit, inspect and run selected concrete exports:

```sh
./sirius-run --emit-lib utils.smirl --export first --export second utils.sirius
./sirius-run --list-lib utils.smirl
./sirius-run --entry first --load-lib utils.smirl
./sirius-run --link-lib combined.smirl first.smirl second.smirl
```

Cross-library calls use a typed `extern def` without a body in the consumer;
the linker matches import and export **names plus canonical structural
signatures**. Missing or incompatible imports are errors. A library with
unresolved imports cannot execute directly. The current library format is
`SIRIUS-MIR-LIB/2`.

## Source-free generic packages (`.smirg`)

Package generic exports and explicitly specialize one entry:

```sh
./sirius-run --emit-generic Ops ops.smirg Ops.sirius
./sirius-run --specialize-generic ops.smirg twice twice.smirl i64
./sirius-run --list-lib twice.smirl
```

The module builder can also find an `Ops.smirg` at the same source-root path as
`Ops.sirius` when source is absent, specialize requested functions, and link
them into the importing program. Source wins when both exist.

Packages importing other modules can pin their typed interfaces:

```sh
./sirius-run --emit-generic Adapter Adapter.smirg Adapter.sirius .
```

To distribute the transitive package closure when *every dependency already*
has its own `.smirg` artifact:

```sh
./sirius-run --bundle-generic Adapter.smirg Adapter-bundle.smirg .
```

Install that bundle under the module's expected `.smirg` filename in the
target root. The bundle embeds unchanged packages; it does not compile source
or become executable bytecode. The builder checks package identity and pinned
interfaces recursively, including named re-export targets and exported
constant values. External `.sirius` and `.smirg` files take precedence over
embedded copies. Nested bundles and unresolved dependencies are rejected.

The only supported package format is `SIRIUS-MIR-GENERIC/3`, including
dependency-free packages (with a zero dependency count). Older `/1` and `/2`
artifacts must be regenerated. Bundles use `SIRIUS-MIR-BUNDLE/1`. The bundle has a
64 MiB size limit and a 256-package limit. Standalone
`--specialize-generic` is restricted to dependency-free packages; for
package imports use the module builder. Generic function bodies may use payload-free `enum` alternatives and
explicit `Union.Label(record)` construction even after the defining source is
removed. Defining-module type aliases and constructors retain their original
identity across imports. `tools/test_module_enum_packages.py` exercises this
through linked MIR, the interpreter, native C and ARC auditing. This also
works for heterogeneous union alternatives with different payload sizes and
managed fields (strings and arrays), including same-shaped labels, explicit
construction from compatible records, arrays, dictionaries and CoW. The
source-free regression `tools/test_module_heterogeneous_union_packages.py`
checks native/MIR parity, ARC auditing, deterministic artifacts, and rejection
of attempts to relabel tagged values or construct from incompatible records.
Concrete non-generic function bodies are **not** transported by `.smirg`
packages: use sources or compiled `.smirl` libraries for them.

## Builder internals

`src/module_scan.sirius` traverses dotted imports and retains parsed ASTs,
HIR declarations and canonical typed exports. Only the standalone scanner
formats `SIRIUS-MODULES/1`; that scanner is source-only.
`src/module_interface.sirius` re-interns shared types and constants.
`src/module_build.sirius` imports typed declarations without reparsing and
orchestrates concrete MIR compilation and in-memory linking.

Within a build, a depth-first traversal visits each dependency once, caches
parsed sources and validates pinned interfaces after dependencies. Imports
resolve to structured external declarations with length-prefixed module names.
Concrete export signatures can reference imported public record types, including
nested collection types. Constructors lower inline without a runtime import.

Computed constructor fields and fixed-type conditional overrides travel as AST
subtrees. Canonical result types and normalized bodies are pinned in public
interfaces. Defining-module constants and private helpers use owner-qualified
bindings to prevent importer capture; referenced constants/helper bodies
contribute to dependency fingerprints. General lexical capture and externally
defined constructor helpers are unsupported.

## Validation

```sh
python tools/test_source_modules.py
python tools/test_mir_library.py
python tools/test_mir_link.py
python tools/test_module_packages.py
python tools/test_module_heterogeneous_union_packages.py
python tools/test_module_package_dependencies.py
python tools/test_module_capture_imports.py
python tools/test_module_bundles.py
```

For HIR/module internals see [Compiler structure](COMPILER_STRUCTURE.md) and
[IR design](IR_DESIGN.md).
