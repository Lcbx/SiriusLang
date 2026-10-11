# MIR interpreter

**Status:** First standalone executable CFG MIR subset implemented in **Sirius** (`src/mir.sirius`, `src/mir_interpreter.sirius`). A **limited typed-HIR-to-MIR scalar lowering** now exists; ordinary Sirius programs still compile through the existing direct C emitter. The interpreter is compiled once with the existing Sirius compiler and then runs hand-built MIR without generating C for that program. No compile-time evaluation is included.

## Run isolated tests

```sh
python tools/test_mir.py               # Compiles the Sirius interpreter via the C bootstrap, then runs it
python tools/test_mir.py --rebuild     # Bypass build cache
python tools/test_mir_lower.py          # Lower checked scalar source to MIR and execute it
```

`tests/mir/driver.sirius` builds MIR blocks directly. Tests assert program results as well as instruction budgets, branch selection, function calls, verifier diagnostics, panics, invalid inputs, and uninitialized registers. `MirOutcome` returns a value, an error string, and consumed instruction/terminator steps. The separate `tests/mir/lowering_driver.sirius` and `tools/test_mir_lower.py` exercise pre-emission typed HIR, lowering, MIR verification, and execution with no C generation for the input source.

Initial scalar coverage: `i64`, `bool`, copies, integer arithmetic/comparisons, boolean equality/negation, print, direct calls, jumps, branches, return and panic. All registers have a declared type; function parameters occupy the first registers. Calls have explicit argument register lists. Loops are CFG backedges. The verifier checks structural/type consistency before execution. Reads of potentially uninitialized registers are detected dynamically. Interpreter execution has configurable total-step and call-depth limits. Integer overflow/edge division semantics remain to be specified; avoid testing those boundaries as backend-equivalent until resolved.

**Not yet supported:** float/string/collection values in MIR, moves/borrows/ARC/CoW, union switches, source maps, specialized calls and full HIR-to-MIR lowering. The current lowering covers checked `i64`/`bool` literals and locals, scalar operations, return, `if`/`elif`/`else`, `while`, and short-circuit `and`/`or`; other syntax gives an explicit lowering error. The Python MIR prototype from earlier exploration is not part of the maintained path.

## Backend policy (after lowering is in place)

```sh
sirius test                   # Interpret MIR (default)
sirius test --backend=c       # Generate/compile C, then run
sirius test --backend=both    # Execute both and check expected output/exit status
```

**These commands are planned, not implemented.** Until both backends consume a shared MIR, keep the existing C-based test runners and `tools/test_mir.py` separate. Do not silently fall back to C when an instruction is unsupported by the interpreter.

- Reuse the **same source fixtures** and expected stdout, exit status and runtime-error results for both backends. Test each backend against the independently stated expectation before comparing the two results.
- Run frontend/negative compiler cases once, MIR interpreter unit cases independently, and C emitter-specific tests only with C. CI eventually runs behavioral tests with `--backend=both`.
- Each MIR operation added to the source-level path must have interpreter and C implementations with shared behavioral tests.
- Treat the MIR verifier and language semantics, not the C emitter or interpreter alone, as the reference for valid operations. No interpreter optimization/JIT or separate language semantics.

## Next implementation steps

1. Extend the new HIR-owned specialization registry into a complete pre-emission typed-function worklist, linking each specialization to its eventual `MirFunction`.
2. Extend the checked scalar lowering subset with direct specialized calls, `for`, and remaining scalar control flow before migrating any C emitter paths.
3. Adapt C emission to the **same** MIR in small increments, testing existing fixtures and both results.
4. Add managed values and explicit ownership operations in both backends before running record/array/dictionary fixtures through interpretation.
5. Implement the shared test harness and `--backend` switch when both execution paths actually work on the same source fixtures.
