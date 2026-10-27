# Sirius documentation

Commands and source paths are relative to the repository root unless stated
otherwise. For a first build, start with the [top-level README](../README.md).

## Guides

| Document | Scope |
| --- | --- |
| [Usage](USAGE.md) | Compile, run, interpret, and use the callback and numerical libraries |
| [Modules and packages](MODULES.md) | Source modules, re-exports, MIR libraries, generic packages |
| [Development](DEVELOPMENT.md) | Bootstrap, builds, test runners, diagnostics |
| [Language design](../LANGUAGE_DESIGN.md) | Intended syntax and semantics (including unimplemented features) |
| [Roadmap](../ROADMAP.md) | Remaining work and development principles |
| [Examples](../examples/README.md) | Runnable programs and supported feature inventory |

## Compiler and runtime reference

| Document | Scope |
| --- | --- |
| [Compiler structure](COMPILER_STRUCTURE.md) | Active source modules and refactoring boundaries |
| [IR design](IR_DESIGN.md) | Typed HIR, CFG MIR, lowering and specialization |
| [Ownership](OWNERSHIP.md) | ARC/CoW contracts, liveness and static proofs |
| [Interpreter](INTERPRETER.md) | Dynamic execution, MIR wire formats, runtime limits |
| [Runtime](RUNTIME.md) | C storage, collection behavior and profiling |
| [Modules, MIR and FFI design](MIR_MODULES_AND_FFI.md) | Current module model and proposed ABI/registration architecture |

## Historical records

These capture past results, not necessarily current performance or priorities:

- [MIR performance measurements](history/MIR_PROFILE.md)
- [Roadmap and milestone log (2026-10-25)](history/ROADMAP_20261025.md)
