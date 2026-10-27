# Test data and execution

**Run tests from the repository root using `python tools/test_all.py`.**
The Python test drivers stay in `tools/` so they can import the same build
helpers as standalone development commands. Their inputs live here; this
separation avoids copying compiler setup code into each test directory.

## Layout

| Directory | Contents | Execution |
| --- | --- | --- |
| `language/fixtures/`, `language/expected_output/` | Shared source/output golden cases | `test_compiler.py`, `test_mir_run.py` (selected cases), `test_reference_c.py` |
| `language/reference_c/` | Independent C implementations for a subset of language cases | `test_reference_c.py`; **not** generated compiler output |
| `language/runtime_errors/` | C runtime panic fixtures and expected exit/stderr | `test_reference_c.py` |
| `language/proposed_negative/` | Design-oriented invalid programs and aspirational error codes | **Not** run as compiler diagnostic tests |
| `compiler/fixtures/`, `compiler/expected_output/` | Compiler-native positive source/output cases | `test_compiler.py`, `test_bootstrap.py`, optional MIR parity |
| `compiler/negative/` | Compiler diagnostics expected to reject source | `test_compiler.py`, `test_bootstrap.py` |
| `compiler/runtime_errors/` | Runtime failures used by targeted compiler regression drivers | Relevant `test_*.py` driver |
| `parser/`, `tokenizer/` | Golden fixtures, expected output and Sirius test drivers | `test_parser.py`, `test_tokenizer.py` |
| `hir/`, `mir/`, `types/` | Component-level Sirius drivers, MIR artifacts and input programs | Corresponding focused drivers |
| `deferred/` | Explicitly deferred module experiments | Not included in the default test suite |

`tools/test_stdlib_numeric.py` exercises the experimental `Std.Numeric` rank-2
wrapper (including broadcast addition and degenerate axes) through native, MIR, linked-C, and ARC-audited execution.

Use `python tools/test_all.py --group smoke` for inexpensive structural checks,
including `test_fixture_layout.py`. Test drivers run sequentially, but some
run their internal cases concurrently using the shared build cache. The
full suite can take considerably longer than the smoke checks.

## Conventions

- Put runnable source cases in `fixtures/<stem>.sirius`, golden output in
  `expected_output/<stem>.txt`; both filenames must match exactly.
- Put expected-to-fail compiler programs in `compiler/negative/` and update
  `tools/compiler_cases.py`'s diagnostic expectations. These are distinct
  from the *proposed* error-code examples in `language/proposed_negative/`.
- Use isolated temporary directories for generated C, executables and
  process outputs; use `BuildCache` for shared native tools. Do not write
  generated files into checked-in fixture directories.
- Add new independent Python drivers as `tools/test_<feature>.py` with a
  `main()` and `if __name__ == '__main__'` guard. Reuse
  `tools/build_support.py` and `tools/project_paths.py`. Register any new
  filename pattern in the groups defined by `tools/test_all.py`.
- For compiled/interpreted equivalence, assert expected output/exit status
  separately as well as backend parity; test failure paths and managed
  ownership where relevant.
- Keep manual fixture generators `tools/generate_core_fixtures.py` and
  `tools/generate_extended_fixtures.py` out of automatic test discovery.
  They **overwrite tracked fixture files** and are not test runners.

See [Development](../docs/DEVELOPMENT.md) for the runner and bootstrap commands.
