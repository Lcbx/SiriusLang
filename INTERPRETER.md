# MIR interpreter

**Status:** First standalone executable CFG MIR subset implemented in **Sirius** (`src/mir.sirius`, `src/mir_interpreter.sirius`). A **limited typed-HIR-to-MIR scalar/string/record/array/dictionary/optional lowering and linked specialization worklist** now exist; ordinary Sirius programs now compile through MIR by default. The direct C emitter remains available through `sirius-legacy`. The interpreter is compiled once with the existing Sirius compiler and then runs hand-built MIR without generating C for that program. No compile-time evaluation is included. The proposed use of the same MIR for compile-time execution, serialized artifacts and mixed native/interpreted calls is described in [MIR_MODULES_AND_FFI.md](MIR_MODULES_AND_FFI.md); these are not current interpreter capabilities.

## Run isolated tests

```sh
python tools/test_mir.py               # Compiles the Sirius interpreter via the C bootstrap, then runs it
python tools/test_mir.py --rebuild     # Bypass build cache
python tools/test_mir_lower.py          # Lower checked scalar source to MIR and execute it
python tools/test_hir_worklist.py       # Test semantic specialization without any backend
python tools/test_mir_program.py        # Link HIR specializations into a scalar MIR program
python tools/test_mir_c.py              # Differential source execution, including optional control flow
python tools/test_mir_optional.py       # Direct optional MIR tests, interpreter and C
python tools/test_mir_lifetime.py       # CFG last-use releases, branches, loops, CoW and native equivalence
python tools/test_mir_ownership.py      # Definite initialization, moves, releases and CFG joins
python tools/test_mir_exit_cleanup.py   # CFG-edge releases, unused arguments, returns and explicit panic cleanup
python tools/test_mir_managed.py        # Shared interpreted/native managed-string ownership tests
python tools/test_mir_records.py        # Nested structural records and managed field ownership
python tools/test_mir_arrays.py         # CoW, bounds, and nested managed arrays
python tools/test_mir_dict.py           # Dictionary CoW and nested-value ownership
```

`tests/mir/driver.sirius` builds MIR blocks directly. Tests assert program results as well as instruction budgets, branch selection, function calls, verifier diagnostics, panics, invalid inputs, and uninitialized registers. `MirOutcome` returns a value, an error string, and consumed instruction/terminator steps. The separate `tests/mir/lowering_driver.sirius` and `tools/test_mir_lower.py` exercise pre-emission typed HIR, lowering, MIR verification, and execution with no C generation for the input source.

Initial coverage: `i64`, `bool`, owned `string`, canonical structural records, rank-1 arrays, dictionaries, copies, integer arithmetic/comparisons, boolean equality/negation, print, direct calls, jumps, branches, return and panic. All registers have a declared type; function parameters occupy the first registers. Calls have explicit argument register lists. Loops are CFG backedges. The verifier checks structural/type consistency before execution. Reads of potentially uninitialized registers are detected dynamically. Interpreter execution has configurable total-step and call-depth limits. Integer overflow/edge division semantics remain to be specified; avoid testing those boundaries as backend-equivalent until resolved.

**Not yet supported:** floats, arbitrary deep projection updates, record spreads/deletions, borrows, union switches, source maps, unannotated callee return types and complete HIR-to-MIR lowering. `Array.append` and direct record-field updates now have experimental support. The current lowering also supports checked string literals (with a restricted escape set), string `+`, `len(string)`, simple record/array literals, field projection, checked array access/update/length, and managed values in generic calls, assignments and returns. Unsupported syntax gives an explicit lowering error.

**Managed-value ownership:** Initialized string/record/array/dictionary/optional registers own one reference each. `MIR_COPY`/`MIR_RETAIN` preserve the source and produce another owned value; `MIR_MOVE` transfers ownership and invalidates the source; `MIR_RELEASE` invalidates its register. Function calls retain managed arguments and transfer the credits to the callee; managed returns transfer ownership to the caller. `MIR_RECORD_NEW` constructs immutable records with canonical field order; `MIR_RECORD_GET` copies a field (retaining managed fields). `MIR_ARRAY_NEW` builds rank-1 arrays, `MIR_ARRAY_GET` performs bounds-checked reads, and `MIR_ARRAY_LENGTH` returns their length. `MIR_ARRAY_SET` **consumes the source register** (which may also be the destination), cloning if shared and reusing storage if uniquely owned; the replacement element is retained before the overwritten element is dropped. Generated C reference-counts record storage and releases nested strings/records/arrays when the last owner is dropped, using canonical descriptors; `Optional[T]` uses zero/one-element owned storage and recursively releases any managed payload; all live owned registers are cleaned up on normal function return. The Sirius interpreter maintains logical record/array/dictionary reference counts in a heap with recursive drops. This is a conservative subset, not the full CFG ownership pass or proof of ARC across every exit. The Python MIR prototype from earlier exploration is not part of the maintained path.

## Concrete specialization worklist

`src/hir_specialize.sirius` owns specialization discovery and semantic body analysis independently of C or MIR. It indexes requests by declaration-body identity and canonical argument TypeIds; repeat calls and active recursive calls reuse an entry. Each worklist entry yields `HirSpecializedFunction` with typed nodes, call-to-specialization IDs and return type. `src/mir_program.sirius` lowers one `MirFunction` per completed entry, retaining **the same index** as the HIR registry; `MIR_CALL` refers to that index. The final linked program is verified before execution.

This migration slice supports direct and mutual recursion, generic parameters with scalar/string/record/array/dictionary/optional instantiations, and supported managed calls; an unannotated callee return type is explicitly rejected by the new worklist. The semantic worklist can analyze non-scalar types such as arrays, and lowers the supported checked rank-1 array subset to MIR. The separate legacy C emitter still instantiates and emits its own bodies, while the default compiler emits from linked MIR and HIR-owned specialization IDs. The two backends share the source subset used by `tools/test_mir_c.py`, and the experimental backend fails explicitly outside that subset.

## Optional values and control flow (experimental)

`MIR_OPTIONAL_NONE`, `MIR_OPTIONAL_SOME`, `MIR_OPTIONAL_HAS` and `MIR_OPTIONAL_UNWRAP` support canonical `Optional[T]`, including managed nested payloads. `MIR_ARRAY_GET_OPTIONAL` and `MIR_DICT_GET_OPTIONAL` construct optionals without panicking on a missing element/key. Both the interpreter and MIR C backend use the same owned zero/one-field representation, with recursive release.

The typed-HIR path lowers `array[?index]`, `dict.get(key)`, postfix `!` unwrap, `optional or fallback` with **lazy** fallback CFG branches, and `let value = optional else return expression`. The fallback is evaluated only when absent; unwrapping absent values panics. These are tested through both C backends and direct interpreter/native MIR tests. `MIR_DICT_GET_OR` remains an eager, internal primitive, not the implementation of source `or`.

## Experimental MIR C backend and future test policy

`./sirius input.sirius output.c` uses HIR specialization → MIR → C11. The independent `./sirius-legacy input.sirius output.c` selects the old emitter; it still accepts `--mir-c` for backend comparison. Neither command interprets MIR. Native C11 tests cover both backends. Verified MIR omits redundant initialization flags and uses explicit CFG guards for errors; overflow semantics remain unsettled.

The future unified interpreter-vs-native test CLI remains:

```sh
sirius test                   # Interpret MIR (default)
sirius test --backend=c       # Generate/compile C, then run
sirius test --backend=both    # Execute both and check expected output/exit status
```

**These `sirius test` commands are planned, not implemented.** Until the source-level interpreter CLI is wired, keep the existing C-based test runners, MIR-C differential tests and interpreter-specific runners separate. Do not silently fall back to C when an instruction is unsupported by the interpreter.

- Reuse the **same source fixtures** and expected stdout, exit status and runtime-error results for both backends. Test each backend against the independently stated expectation before comparing the two results.
- Run frontend/negative compiler cases once, MIR interpreter unit cases independently, and C emitter-specific tests only with C. CI eventually runs behavioral tests with `--backend=both`.
- Each MIR operation added to the source-level path must have interpreter and C implementations with shared behavioral tests.
- Treat the MIR verifier and language semantics, not the C emitter or interpreter alone, as the reference for valid operations. No interpreter optimization/JIT or separate language semantics.

## Next implementation steps

1. Extend the worklist to infer unannotated callee result types safely, handle constructors and full HIR operations, and retain source-span diagnostics.
2. Extend MIR lowering with `for`, record spreads/updates, array append, dictionaries and additional borrow/CoW operations; keep the implemented immutable record ownership rules.
3. Extend the default MIR C backend to remaining language features while retaining the separate legacy emitter and differential tests.
4. Extend nested-record/string/array ownership to all CFG exits and dictionaries, with static ownership data-flow checks before accepting more complex updates.
5. Implement the shared test harness and `--backend` switch after the two execution paths accept the same fixtures.

### Dictionary limitations

MIR supports `MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, and `MIR_DICT_LENGTH` for scalar, string, and recursively hashable record keys, with all currently supported MIR value types. Insertion uses owned CoW and retains nested managed values. Native MIR delegates hashing/storage to `S_Dict` (open addressing); the debug interpreter still uses linear scans with equivalent structural key equality. `MIR_DICT_GET_OR` takes a pre-evaluated fallback; source `dict.get(key) or expr` has lazy semantics and now lowers through Optional/CFG branches. Source-level `Dict[K,V]()`, `.insert`, and `len(dict)` do work through the default MIR compiler.

## CFG ownership pass (experimental)

The linked HIR specialization program runs `mir_insert_lifetimes` and then `mir_insert_exit_cleanup`. Backward liveness inserts `MIR_RELEASE` after the last register use; forward definite-initialization rejects invalid reads, moves and double release. Exit cleanup splits branch edges when needed, drops definitely owned registers that are dead on a successor, and drops remaining initialized managed values before explicit returns and panics (excluding the returned value). It recomputes ownership states after edge insertion to avoid double releases. Both rewrites are idempotent and verified, and native/interpreter tests cover them. Backend return cleanup remains enabled for defensive handling of partially initialized registers. Implicit runtime panics do not unwind references, and general ownership-credit proofs remain incomplete.

## CFG ownership balance (experimental)

`src/mir_ownership.sirius` computes forward **must-initialize** and **may-initialize** states across the CFG. `mir_check_balance` rejects any reachable `Return` or explicit `Panic` that may leave a managed register owned (except the returned register, whose ownership is transferred). `src/mir_cleanup.sirius` releases definitely owned values on edges and exits and uses `MIR_RELEASE_IF_INIT` where initialization differs between predecessor paths. The MIR interpreter and C backend implement this guarded release consistently. See `tools/test_mir_balance.py`.

This verifies explicit exits, not hidden panics from bounds checks, division, failed unwrapping, or allocation errors. Backend return cleanup is retained as defense in depth; a separate unwinding strategy is still required.
