# MIR interpreter

**Status:** Sirius now has two execution paths for the *same* typed CFG MIR:
`src/mir_emit_c.sirius` emits native C11, while `src/mir_interpreter.sirius`
executes it dynamically. `src/run_cli.sirius` lowers a source file through the
normal HIR specialization, MIR ownership/optimization pipeline and interprets
the linked program. The runner itself is built once as a native executable;
the input source does not generate C. `src/mir_wire.sirius` also persists and
reloads concrete executable MIR. The interpreter itself does not perform compile-time execution or effect
inference. The surrounding runner supports source-free generic specialization
through `.smirg` packages and concrete MIR library linking.

## Dynamic source execution

```sh
python tools/build_mir_runner.py
./sirius-run path/to/main.sirius [args...]
python tools/test_mir_run.py --all-fixtures
```

`main() -> i64` is required, as for the native emitter. Command-line arguments
are supplied by the host. Optional file reads distinguish missing files from
empty files. `print` and `eprint` use stdout and stderr respectively; explicit
runtime panics report to stderr. `--max-steps N` and `--max-depth N` set
interpreter-only limits (defaults: 10 million instructions and 256 call levels).
All 91 shared compiler fixtures run successfully through the interpreter.

## Portable executable MIR

```sh
./sirius-run --emit-mir main.smir path/to/main.sirius
./sirius-run --load-mir main.smir [args...]
python tools/test_mir_wire.py --all-fixtures
```

`--emit-mir` saves the *post-lowering and ownership-optimized* MIR and exits
without executing it. `--load-mir` interprets that artifact, without the source
compiler. The file includes a versioned header (`SIRIUS-MIR-EXEC/1`), entry
index, canonical structural type descriptors, specialized functions, typed
registers, CFG blocks, instruction operands and text payloads. Integers use
canonical decimal lines; strings are byte-length-prefixed so embedded newlines
remain unambiguous. IDs are reconstructed, not process-local pointers. The
loader validates bounds, version, type identities, MIR structure and ownership
balance before executing. The format has a 64 MiB file limit and item-count
limits; its version is tied to the current implementation, not a stable public
cross-version ABI.

With source execution, `arguments()[0]` is the source path; with loaded MIR it
is the artifact path. Remaining positional arguments are passed unchanged. The
native C emitter uses the native executable path for `arguments()[0]`.

Artifacts contain only already specialized, executable functions. They do
**not** preserve generic source/HIR needed to instantiate new specializations,
exported symbols, imports, native bindings or a persistent library interface.
Named exports in the separate concrete-library format are described below;
concrete imports are supported in the separate library format; generic
interfaces are stored separately in `.smirg` packages. This feature does not imply compile-time execution
or purity inference.

## Named concrete MIR libraries

The concrete library format (`SIRIUS-MIR-LIB/2`) packages the same optimized executable
MIR with a table of named entry functions and canonical **structural** function
signatures. It is independent of the process-local type descriptor indices.

```sh
./sirius-run --emit-lib module.smirl --export foo --export bar module.sirius
./sirius-run --list-lib module.smirl
./sirius-run --entry bar --load-lib module.smirl
./sirius-run --link-lib combined.smirl module.smirl another.smirl
python tools/test_mir_library.py
python tools/test_mir_link.py
python tools/test_mir_import.py
```

Export roots can have parameters when each parameter has an explicit
concrete type annotation. Unannotated generic roots are not exportable yet.
The worklist compiles their combined reachable call graph, including functions
called by multiple roots. Selection for direct CLI execution additionally
requires return type `i64`. The loader verifies unique export names, function
indices and the stored signatures against reconstructed MIR types before any
execution. `SIRIUS-MIR-EXEC/1` remains readable by the current runner. Neither
artifact format is a public compatibility commitment; breaking changes are
allowed while Sirius is experimental.

The concrete linker combines one or more validated libraries into one
program. It interns structurally equivalent types into a shared type table,
remaps all function register/return type IDs, relocates internal MIR calls,
and preserves named exports. Duplicate exported names are errors. Link order
changes numbering and export order, not execution semantics; repeating the
same ordered inputs yields byte-identical output.

A consuming source can declare `extern def f(x: i64) -> i64` without a
body. Its concrete call site creates an unresolved import slot. The library
stores import names and canonical signatures; the linker matches those against
other libraries' named exports, validates structural type equivalence, then
relocates `MIR_CALL`/`MIR_CALL_MOVE` instructions. Missing or mismatched
symbols are rejected, regardless of input order. Import slots have checked
panic placeholders, and the runner refuses to execute an unresolved library.
Only fully linked output is currently emitted (no partial linking).

The native C compiler and `--emit-mir` reject source programs containing
unresolved imports; build a MIR library and link it first. Exported concrete
functions, not generic definitions, are the unit of linkage. The `/2` library
format replaces `/1`, without a compatibility reader.

**Now available separately:** a Sirius-native source-module scanner and
`--build-modules` pipeline compile concrete modules and link them into a
loadable MIR library. **Also implemented:** exported generic specialization through portable
`.smirg` packages. Native C registration and native/interpreter interop remain
future work.
`--entry` must precede `--load-lib` in the CLI invocation.

## Run isolated tests

```sh
python tools/test_mir.py               # Compiles the Sirius interpreter via the C bootstrap, then runs it
python tools/test_mir.py --rebuild     # Bypass build cache
python tools/test_mir_lower.py          # Lower checked scalar source to MIR and execute it
python tools/test_hir_worklist.py       # Test semantic specialization without any backend
python tools/test_mir_program.py        # Link HIR specializations into a scalar MIR program
python tools/test_mir_direct_c.py       # Direct MIR-to-C runtime checks
python tools/test_mir_optional.py       # Direct optional MIR tests, interpreter and C
python tools/test_mir_lifetime.py       # CFG last-use releases, branches, loops, CoW and native equivalence
python tools/test_mir_ownership.py      # Definite initialization, moves, releases and CFG joins
python tools/test_mir_exit_cleanup.py   # CFG-edge releases, unused arguments, returns and explicit panic cleanup
python tools/test_mir_managed.py        # Shared interpreted/native managed-string ownership tests
python tools/test_mir_records.py        # Nested structural records and managed field ownership
python tools/test_mir_arrays.py         # CoW, bounds, and nested managed arrays
python tools/test_mir_dict.py           # Dictionary CoW and nested-value ownership
```

`tests/mir/driver_native.sirius` builds MIR blocks directly. Tests assert program results as well as instruction budgets, branch selection, function calls, verifier diagnostics, panics, invalid inputs, and uninitialized registers. `MirOutcome` returns a value, an error string, and consumed instruction/terminator steps. The separate `tests/mir/lowering_driver_native.sirius` and `tools/test_mir_lower.py` exercise pre-emission typed HIR, lowering, MIR verification, and execution with no C generation for the input source.

Initial coverage: `i64`, `f64`, `bool`, owned `string`, canonical structural records, rank-1 arrays, dictionaries, copies, integer arithmetic/comparisons, boolean equality/negation, print, direct calls, jumps, branches, return and panic. All registers have a declared type; function parameters occupy the first registers. Calls have explicit argument register lists. Loops are CFG backedges. The verifier checks structural/type consistency before execution. Reads of potentially uninitialized registers are detected dynamically. Interpreter execution has configurable total-step and call-depth limits. Integer overflow/edge division semantics remain to be specified; avoid testing those boundaries as backend-equivalent until resolved.

**Dynamic interpreter limitations:** fatal implicit runtime failures deliberately do not unwind caller frames; exact decimal literal rounding at all IEEE-754 boundaries, execution of generic declarations without specialization, and cross-boundary native calls. `f64` now uses a separate real-valued field in `MirValue`, including for nested collection elements, and implements every current MIR floating opcode (`sqrt`, comparisons, arithmetic and print). Decimal spelling is parsed by a Sirius implementation rather than native `strtod`; parity for difficult rounding ties and extreme magnitudes remains to be proven. The source frontend already supports several deeper updates, record spreads and type inference; source programs use the same MIR lowerer for either backend. `Array.append` and direct record-field updates now have experimental support. The current lowering also supports checked string literals (with a restricted escape set), string `+`, `len(string)`, simple record/array literals, field projection, checked array access/update/length, and managed values in generic calls, assignments and returns. Unsupported syntax gives an explicit lowering error.

**Managed-value ownership:** Initialized string/record/array/dictionary/optional registers own one reference each. `MIR_COPY`/`MIR_RETAIN` preserve the source and produce another owned value; `MIR_MOVE` transfers ownership and invalidates the source; `MIR_RELEASE` invalidates its register. Function calls retain managed arguments and transfer the credits to the callee; managed returns transfer ownership to the caller. `MIR_RECORD_NEW` constructs immutable records with canonical field order; `MIR_RECORD_GET` copies a field (retaining managed fields). `MIR_ARRAY_NEW` builds rank-1 arrays, `MIR_ARRAY_GET` performs bounds-checked reads, and `MIR_ARRAY_LENGTH` returns their length. `MIR_ARRAY_SET` **consumes the source register** (which may also be the destination), cloning if shared and reusing storage if uniquely owned; the replacement element is retained before the overwritten element is dropped. Generated C reference-counts record storage and releases nested strings/records/arrays when the last owner is dropped, using canonical descriptors; `Optional[T]` uses zero/one-element owned storage and recursively releases any managed payload; all live owned registers are cleaned up on normal function return. The Sirius interpreter maintains logical record/array/dictionary reference counts in a heap with recursive drops. This is a conservative subset, not the full CFG ownership pass or proof of ARC across every exit. The Python MIR prototype from earlier exploration is not part of the maintained path.

## Concrete specialization worklist

`src/hir_specialize.sirius` owns specialization discovery and semantic body analysis independently of C or MIR. It indexes requests by declaration-body identity and canonical argument TypeIds; repeat calls and active recursive calls reuse an entry. Each worklist entry yields `HirSpecializedFunction` with typed nodes, call-to-specialization IDs and return type. `src/mir_program.sirius` lowers one `MirFunction` per completed entry, retaining **the same index** as the HIR registry; `MIR_CALL` refers to that index. The final linked program is verified before execution.

This migration slice supports direct and mutual recursion, generic parameters with scalar/string/record/array/dictionary/optional instantiations, and supported managed calls; an unannotated callee return type is explicitly rejected by the new worklist. The semantic worklist can analyze non-scalar types such as arrays, and lowers the supported checked rank-1 array subset to MIR. The compiler emits from linked MIR and HIR-owned specialization IDs; unsupported features fail explicitly rather than falling back to another emitter.

## Optional values and control flow (experimental)

`MIR_OPTIONAL_NONE`, `MIR_OPTIONAL_SOME`, `MIR_OPTIONAL_HAS` and `MIR_OPTIONAL_UNWRAP` support canonical `Optional[T]`, including managed nested payloads. `MIR_ARRAY_GET_OPTIONAL` and `MIR_DICT_GET_OPTIONAL` construct optionals without panicking on a missing element/key. Both the interpreter and MIR C backend use the same owned zero/one-field representation, with recursive release.

The typed-HIR path lowers `array[?index]`, `dict.get(key)`, postfix `!` unwrap, `optional or fallback` with **lazy** fallback CFG branches, and `let value = optional else return expression`. The fallback is evaluated only when absent; unwrapping absent values panics. These are tested through the C backend and direct interpreter/native MIR tests. `MIR_DICT_GET_OR` remains an eager, internal primitive, not the implementation of source `or`.

## Native/dynamic equivalence policy

`./sirius input.sirius output.c` emits C from MIR. `./sirius-run input.sirius`
interprets that same MIR after lowering; unsupported operations are reported,
never silently compiled as a fallback.

`tools/test_mir_run.py` checks explicitly expected results for both execution
modes, then optionally checks the common source fixtures. Future `sirius test
--backend=both` remains only a proposed convenience interface: the current
runner and test harness are separate commands. Both execution paths should be
tested from the same fixtures before extending opcode coverage, with the MIR
verifier and language semantics as the authority. No JIT or purity inference is
required by this dynamic-execution work.

The standalone runner accepts `sirius-run [--max-steps N] [--max-depth N]
input.sirius [args...]` (defaults: ten million steps and 256 nested calls).
Limits must be positive decimal integers up to one billion. The `--` separator
allows source filenames beginning with a dash. Execution-limit failures report
`Sirius panic: ...` to stderr and exit with status 1; invalid CLI flags exit 2.
Interpreter options are not included in the program's `arguments()` array.
Compile-time diagnostics still use the compiler's existing stdout format.
`print` and `eprint` accept the same four printable scalar types.

## Next implementation steps

1. **Done:** `f64` payloads, operations and representative collections,
   `sqrt`, scientific notation and printed-value parity tests.
2. Runtime-failure messages, stdout, stderr and exit status now match for
   four representative cases, including a nested managed-array failure.
   `eprint` emits stderr in both backends. Fatal implicit aborts deliberately do not unwind caller frames; verify ordinary return/in-band result ownership instead.
3. **Done in part:** 91 positive fixtures, 18 differential examples and three
   runtime-failure cases; add more corner cases and CLI execution budgets.
4. **Done:** versioned serialization and loading of concrete MIR executables,
   concrete libraries and separate portable generic packages. Next: interpreted/compiled
   native-library boundaries. General comptime remains deferred.

### Dictionary limitations

MIR supports `MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, and `MIR_DICT_LENGTH` for scalar, string, and recursively hashable record keys, with all currently supported MIR value types. Insertion uses owned CoW and retains nested managed values. Native MIR delegates hashing/storage to `S_Dict` (open addressing); the debug interpreter still uses linear scans with equivalent structural key equality. `MIR_DICT_GET_OR` takes a pre-evaluated fallback; source `dict.get(key) or expr` has lazy semantics and now lowers through Optional/CFG branches. Source-level `Dict[K,V]()`, `.insert`, and `len(dict)` do work through the default MIR compiler.

## CFG ownership pass (experimental)

The linked HIR specialization program runs `mir_insert_lifetimes` and then `mir_insert_exit_cleanup`. Backward liveness inserts `MIR_RELEASE` after the last register use; forward definite-initialization rejects invalid reads, moves and double release. Exit cleanup splits branch edges when needed, drops definitely owned registers that are dead on a successor, and drops remaining initialized managed values before explicit returns and panics (excluding the returned value). It recomputes ownership states after edge insertion to avoid double releases. Both rewrites are idempotent and verified, and native/interpreter tests cover them. Backend return cleanup remains enabled for defensive handling of partially initialized registers. Implicit runtime panics do not unwind references, and general ownership-credit proofs remain incomplete.

## CFG ownership balance (experimental)

`src/mir_ownership.sirius` computes forward **must-initialize** and **may-initialize** states across the CFG. `mir_check_balance` checks managed borrow/transfer/release/overwrite credit transitions under both states, and rejects any reachable `Return` or explicit `Panic` that may leave a managed register owned (except the returned register, whose ownership is transferred). This is a register-credit audit, not a heap-reference or exceptional-unwinding proof. `src/mir_cleanup.sirius` releases definitely owned values on edges and exits and uses `MIR_RELEASE_IF_INIT` where initialization differs between predecessor paths. The MIR interpreter and C backend implement this guarded release consistently. See `tools/test_mir_balance.py`.

This verifies normal returns and explicit panic exits. Guard insertion now makes division, shifts, checked indexing and unwrap failures explicit CFG paths; allocation failure and other process-fatal aborts may still bypass caller-frame cleanup by design. Backend return cleanup is retained as defense in depth.

## Optional return-credit audit

`MirInterpreter.mir_audit_return_credits(types, outcome)` validates a successful run: the return value contributes one root credit, each reachable heap object contributes one credit for each of its owned child fields (not per reference to the parent), and all unreachable heap objects must have zero references. It reports mismatched reference counts, forged type/handle pairs and leaked objects. The audit is opt-in and does not unwind failures or replace the static MIR ownership/balance proof. Exercise it with `python tools/test_mir_credits.py`.

## Failure results and host execution errors

Routine recoverable failures currently return `Optional`/status values;
constructor-based `Result` unions can now use ordinary MIR return and branch
ownership rules, with tagged record payloads and HIR-exhaustive matching.
Standard-library APIs have not yet adopted `Result`. IEEE-754 floating operations may return NaN/infinity
rather than trapping. Invalid integer division, checked indexing and forced
unwrap still take explicit fatal `MIR_PANIC` paths.

`MirOutcome.error` reports a **host-visible execution failure**, such as a
step/depth budget abort or fatal MIR panic. It is not a catchable Sirius value.
Neither dynamic execution nor native `s_panic` promises to unwind all live
calling frames on fatal termination. Ownership-credit audits apply to
successful normal returns and future union-valued recoverable errors.
