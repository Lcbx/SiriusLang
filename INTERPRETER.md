# MIR interpreter

**Status:** First standalone executable CFG MIR subset implemented in **Sirius** (`src/mir.sirius`, `src/mir_interpreter.sirius`). A **limited typed-HIR-to-MIR scalar/string/record/array/dictionary lowering and linked specialization worklist** now exist; ordinary Sirius programs compile through the existing direct C emitter by default, while supported scalar/string/record/array/dictionary programs may opt in to `--mir-c`. The interpreter is compiled once with the existing Sirius compiler and then runs hand-built MIR without generating C for that program. No compile-time evaluation is included.

## Run isolated tests

```sh
python tools/test_mir.py               # Compiles the Sirius interpreter via the C bootstrap, then runs it
python tools/test_mir.py --rebuild     # Bypass build cache
python tools/test_mir_lower.py          # Lower checked scalar source to MIR and execute it
python tools/test_hir_worklist.py       # Test semantic specialization without any backend
python tools/test_mir_program.py        # Link HIR specializations into a scalar MIR program
python tools/test_mir_c.py              # Compare native MIR-C and legacy C on shared scalar/string/record cases
python tools/test_mir_managed.py        # Shared interpreted/native managed-string ownership tests
python tools/test_mir_records.py        # Nested structural records and managed field ownership
python tools/test_mir_arrays.py         # CoW, bounds, and nested managed arrays
python tools/test_mir_dict.py           # Dictionary CoW and nested-value ownership
```

`tests/mir/driver.sirius` builds MIR blocks directly. Tests assert program results as well as instruction budgets, branch selection, function calls, verifier diagnostics, panics, invalid inputs, and uninitialized registers. `MirOutcome` returns a value, an error string, and consumed instruction/terminator steps. The separate `tests/mir/lowering_driver.sirius` and `tools/test_mir_lower.py` exercise pre-emission typed HIR, lowering, MIR verification, and execution with no C generation for the input source.

Initial coverage: `i64`, `bool`, owned `string`, canonical structural records, rank-1 arrays, dictionaries, copies, integer arithmetic/comparisons, boolean equality/negation, print, direct calls, jumps, branches, return and panic. All registers have a declared type; function parameters occupy the first registers. Calls have explicit argument register lists. Loops are CFG backedges. The verifier checks structural/type consistency before execution. Reads of potentially uninitialized registers are detected dynamically. Interpreter execution has configurable total-step and call-depth limits. Integer overflow/edge division semantics remain to be specified; avoid testing those boundaries as backend-equivalent until resolved.

**Not yet supported:** floats, optional dictionary lookups, array append/optional indexing, record spreads/deletions/updates, borrows, union switches, source maps, unannotated callee return types and full HIR-to-MIR lowering. The current lowering also supports checked string literals (with a restricted escape set), string `+`, `len(string)`, simple record/array literals, field projection, checked array access/update/length, and managed values in generic calls, assignments and returns. Unsupported syntax gives an explicit lowering error.

**Managed-value ownership:** Initialized string/record/array/dictionary registers own one reference each. `MIR_COPY`/`MIR_RETAIN` preserve the source and produce another owned value; `MIR_MOVE` transfers ownership and invalidates the source; `MIR_RELEASE` invalidates its register. Function calls retain managed arguments and transfer the credits to the callee; managed returns transfer ownership to the caller. `MIR_RECORD_NEW` constructs immutable records with canonical field order; `MIR_RECORD_GET` copies a field (retaining managed fields). `MIR_ARRAY_NEW` builds rank-1 arrays, `MIR_ARRAY_GET` performs bounds-checked reads, and `MIR_ARRAY_LENGTH` returns their length. `MIR_ARRAY_SET` **consumes the source register** (which may also be the destination), cloning if shared and reusing storage if uniquely owned; the replacement element is retained before the overwritten element is dropped. Generated C reference-counts record storage and releases nested strings/records/arrays when the last owner is dropped, using canonical descriptors; all live owned registers are cleaned up on normal function return. The Sirius interpreter maintains logical record/array/dictionary reference counts in a heap with recursive drops. This is a conservative subset, not the full CFG ownership pass or proof of ARC across every exit. The Python MIR prototype from earlier exploration is not part of the maintained path.

## Concrete specialization worklist

`src/hir_specialize.sirius` owns specialization discovery and semantic body analysis independently of C or MIR. It indexes requests by declaration-body identity and canonical argument TypeIds; repeat calls and active recursive calls reuse an entry. Each worklist entry yields `HirSpecializedFunction` with typed nodes, call-to-specialization IDs and return type. `src/mir_program.sirius` lowers one `MirFunction` per completed entry, retaining **the same index** as the HIR registry; `MIR_CALL` refers to that index. The final linked program is verified before execution.

This migration slice supports direct and mutual recursion, generic parameters with scalar/string/record/array/dictionary instantiations, and scalar/string/record/array/dictionary calls; an unannotated callee return type is explicitly rejected by the new worklist. The semantic worklist can analyze non-scalar types such as arrays, and lowers the supported checked rank-1 array subset to MIR. The default C emitter still instantiates and emits its own bodies, but the opt-in `--mir-c` backend now emits from the linked MIR and HIR-owned specialization IDs. The two backends share the source subset used by `tools/test_mir_c.py`, and the experimental backend fails explicitly outside that subset.

## Experimental MIR C backend and future test policy

`./sirius --mir-c input.sirius output.c` currently selects supported scalar/string/record/array/dictionary HIR specialization → MIR → C11, bypassing the legacy specialization/emission path. It is **not** the interpreter path. The default `./sirius input.sirius output.c` still supports the wider bootstrap language. Strict C11 tests exercise native execution of both for a common scalar/string/record/array/dictionary subset. MIR currently tracks initialized registers dynamically in generated C, and handles division by zero and panic; overflow semantics remain unsettled.

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
3. Expand the opt-in MIR C backend to the full supported language, then switch default production emission to the **same** specialized MIR while retaining differential tests.
4. Extend nested-record/string/array ownership to all CFG exits and dictionaries, with static ownership data-flow checks before accepting more complex updates.
5. Implement the shared test harness and `--backend` switch after the two execution paths accept the same fixtures.

### Dictionary limitations

MIR supports `MIR_DICT_NEW`, `MIR_DICT_INSERT`, `MIR_DICT_GET_OR`, and `MIR_DICT_LENGTH` for string and integer keys, with all currently supported MIR value types. Insertion uses owned CoW and retains nested managed values. The baseline uses a linear key scan instead of hashing. `MIR_DICT_GET_OR` takes a pre-evaluated fallback; source `dict.get(key) or expr` has lazy semantics and therefore still needs Optional/CFG lowering. Source-level `Dict[K,V]()`, `.insert`, and `len(dict)` do work through `--mir-c`.
