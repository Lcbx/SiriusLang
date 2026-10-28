# MIR interpreter

The native runner lowers source through the same HIR specialization and MIR
ownership/optimization pipeline as the C emitter, then executes linked MIR
without generating C for the input. Entry point: `src/run_cli.sirius`;
execution: `src/mir_interpreter.sirius`; serialization: `src/mir_wire.sirius`.
Commands and paths below are relative to the repository root.

## Run source or saved MIR

```sh
python tools/build_mir_runner.py
./sirius-run path/to/main.sirius [args...]
./sirius-run --emit-mir main.smir path/to/main.sirius
./sirius-run --load-mir main.smir [args...]
```

Source execution requires `main() -> i64`. `--emit-mir` saves optimized MIR
and exits without executing it. `--load-mir` needs no source compilation.
`arguments()[0]` is the source path or loaded artifact path; native programs
use the executable path. Other arguments pass through unchanged.

Options before the input select interpreter-only budgets:

| Option | Default | Accepted values |
| --- | --- | --- |
| `--max-steps N` | 10,000,000 instructions/terminators | Positive decimal, at most 1,000,000,000 |
| `--max-depth N` | 256 call levels | Positive decimal, at most 1,000,000,000 |

Use `--` for source paths beginning with a dash. Runner options do not appear
in `arguments()`. Invalid flags exit 2; execution failures print
`Sirius panic: ...` to stderr and exit 1. Compile-time diagnostics retain the
compiler's stdout format. `print` and `eprint` write stdout and stderr and
accept `i64`, `f64`, `bool` and `string`. Optional file reads distinguish
missing from empty files.

## Executable format (`.smir`)

`SIRIUS-MIR-EXEC/1` stores an entry index, canonical structural descriptors,
specialized functions, typed registers, CFG blocks, operands and text.
Integers use canonical decimal lines; strings are byte-length-prefixed and
may contain newlines. The loader reconstructs IDs and validates bounds,
version, canonical types, MIR structure and ownership balance. Files are
limited to 64 MiB with additional item-count bounds.

This is an experimental, compiler-version-specific format, without a stable
cross-version ABI. It contains concrete functions, not generic declarations,
export/import interfaces or native bindings. For those separate mechanisms,
see [Modules and packages](MODULES.md).

## Libraries and linking (`.smirl`)

```sh
./sirius-run --emit-lib module.smirl --export foo --export bar module.sirius
./sirius-run --list-lib module.smirl
./sirius-run --entry bar --load-lib module.smirl
./sirius-run --link-lib combined.smirl module.smirl another.smirl
```

`--entry` must precede `--load-lib`. Concrete export roots require annotated
parameter types; their combined reachable call graph is compiled once. Direct
CLI execution additionally requires a suitable zero-argument `i64` entry.
Generic roots need specialization through source modules or `.smirg` packages.

`SIRIUS-MIR-LIB/2` adds named exports/imports and canonical structural
signatures to concrete MIR. Loading checks names, indices and signatures.
The linker interns equivalent types, remaps register/return TypeIds and call
indices, and preserves exports. Duplicate export names, missing imports and
signature mismatches are errors. Only fully linked output executes. The same
ordered inputs produce byte-identical output; changing link order may change
numbering and traversal order of exports, without changing program semantics.
The former `/1` format has no compatibility reader.

A bodyless `extern def` produces a typed unresolved import slot with a panic
placeholder. Native C emission and `--emit-mir` reject unresolved imports;
produce and link libraries first. Source-module building and source-free
generic specialization are handled by the surrounding runner, not by execution
of unspecialized generic functions. Native registration and mixed
native/interpreted calls remain proposals in [Modules and FFI](MIR_MODULES_AND_FFI.md).

## Execution and ownership

MIR functions have typed registers, parameters in the first slots, explicit
call operands, CFG branches/backedges and typed return values. Verification
precedes execution; invalid register reads also have dynamic checks.
`MirOutcome` contains a result, host-visible error and consumed step count.
Specialization discovery belongs to HIR; MIR function indices preserve the
specialization registry indices. See [IR design](IR_DESIGN.md).

The interpreter handles the current executable opcode set, including scalar
arithmetic, strings, records, arrays, dictionaries, optionals, tagged unions,
function references and indirect calls. Unsupported source syntax fails
explicitly during analysis/lowering. There is no fallback emitter.

Managed registers own logical values. Copies retain; moves consume their
source; releases invalidate it. Calls retain managed arguments unless a
verified move transfers them; returns transfer ownership to the caller.
Field/index reads produce independently owned values. Consuming updates clone
shared owners and preserve CoW snapshots, retaining replacement children
before dropping old ones. Heap release follows canonical type descriptors.
For exact instruction contracts and CFG pass order, use
[Ownership](OWNERSHIP.md).

Optional lookup returns absence without panicking. Source `optional or
fallback` and `let x = optional else return value` use branches so the fallback
runs only when absent. `MIR_DICT_GET_OR` instead receives an already evaluated
fallback and is not the lowering of lazy source `or`.

`MirInterpreter.mir_audit_return_credits(types, outcome)` optionally checks a
successful execution. The returned value contributes a root; each reachable
object contributes its child edges once, including repeated child handles.
Unreachable objects must have zero references. It rejects leaked objects,
wrong-type/out-of-range handles and credit mismatches. This checks one execution,
not every path or fatal-error unwinding. See `tools/test_mir_credits.py`.

## Backend parity and limits

Both backends should match visible results, diagnostics and ownership behavior;
test expected results separately from backend agreement. The source inspection
showing a handler for every executable opcode is not evidence that all operand
combinations have been tested. `MIR_STRING_FIND` is covered by the string-op range.

Known differences and unproven boundaries:

- Native dictionaries enumerate occupied hash slots; the interpreter uses a
  linear entry list. Cursor representations and traversal order differ.
  Programs should not depend on dictionary iteration order.
- Native append can reuse unique buffers; interpreted append copies the logical
  elements into a fresh heap record. Interpreter timings do not predict native
  allocation behavior or asymptotic append costs.
- Decimal-to-`f64` parsing uses Sirius code rather than native `strtod`. Difficult
  rounding ties and extreme magnitudes do not yet have proven exact parity.
- Execution budgets apply only to the interpreter. No JIT, general compile-time
  MIR execution, effect inference or native/interpreted call bridge exists.

`i64` addition, subtraction, multiplication and negation wrap at 64 bits.
Division/remainder truncate toward zero and trap on zero or `INT64_MIN / -1`.
Floating operations may yield IEEE NaN/infinity. Explicit scalar conversions
follow [Usage](USAGE.md#syntax-and-values).

Recoverable `Optional`, status and result-union values use ordinary return
ownership; `Std.Filesystem` already offers result wrappers. `MirOutcome.error`
reports host execution failure, not a catchable Sirius value. Guard lowering
makes supported arithmetic/index/unwrap traps explicit CFG panic edges so
local cleanup can run. Fatal termination does not promise caller-frame
unwinding; allocation and other implicit aborts remain outside that guarantee.

## Validation

```sh
python tools/test_mir_run.py --all-fixtures
python tools/test_mir_wire.py --all-fixtures
python tools/test_mir_library.py
python tools/test_mir_link.py
python tools/test_mir_import.py
python tools/test_mir_credits.py
```

Direct interpreter tests build MIR with `tests/mir/driver_native.sirius`;
`tests/mir/lowering_driver_native.sirius` tests source lowering without C
emission for the input. `python tools/test_mir.py --rebuild` bypasses the build
cache. Component drivers cover HIR worklists, linked programs, direct C,
managed records/arrays/dictionaries, optionals, lifetime insertion, ownership
and exit cleanup. Use `python tools/test_all.py --group mir --list` to list the
active drivers, and [Development](DEVELOPMENT.md) for test selection.

A proposed `sirius test --backend=both` command is not implemented; use the
current runner and Python test harness.
