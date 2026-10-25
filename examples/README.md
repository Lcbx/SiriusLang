# Sirius examples

These are **working programs for the current self-hosted MIR compiler**, not
proposals for the future language. `filesystem_walk.sirius` was already present;
the other examples have checked-in expected output and are exercised by
`python tools/test_examples.py` (also selected by `tools/test_all.py`).

## Programs

| Program | Demonstrates |
| --- | --- |
| [`fizzbuzz.sirius`](fizzbuzz.sirius) | `for` ranges, `if`/`elif`, remainder and printing |
| [`fibonacci.sirius`](fibonacci.sirius) | Functions, sequential loops and mutable locals |
| [`prime_sieve.sirius`](prime_sieve.sirius) | Sieve of Eratosthenes, arrays, `while`, checked indexing |
| [`game_of_life.sirius`](game_of_life.sirius) | Conway's Game of Life, nested arrays, value-based generations |
| [`insertion_sort.sirius`](insertion_sort.sirius) | Sorting, indexed updates, copy-on-write snapshots |
| [`word_frequencies.sirius`](word_frequencies.sirius) | Dictionary insertion and optional lookup/fallback |
| [`cow_nested.sirius`](cow_nested.sirius) | Record constructors, nested CoW updates, destructuring |
| [`generic_choose.sirius`](generic_choose.sirius) | Compile-time specialization of one function for three types |
| [`geometry.sirius`](geometry.sirius) | `f64`, record constructors and `sqrt` |
| [`bitwise_rgb.sirius`](bitwise_rgb.sirius) | `i64` bitwise operations and shifts |
| [`filesystem_walk.sirius`](filesystem_walk.sirius) | Recursive directory traversal, command-line args and filesystem optionals |

All standalone examples have `main() -> i64` and return zero on success.
`game_of_life.sirius` shows a 5×5 blinker through two generations; it uses
only **new value construction**, not shared mutable state.

## Running

From the project root, build the compiler and compile any example:

```sh
python tools/build_compiler.py
./sirius examples/game_of_life.sirius game_of_life.c
cc -std=c11 -I runtime game_of_life.c runtime/sirius_runtime.c -lm -o game_of_life
./game_of_life
python tools/test_examples.py
```

On Windows, use `sirius.exe` / `game_of_life.exe`, and use the appropriate C
linker flags for your toolchain. The test script selects the native C compiler
and linker flags using `tools/build_support.py`.

If you built `sirius-run` with `python tools/build_mir_runner.py`, you can also
run a source directly through the **MIR interpreter**:

```sh
./sirius-run examples/game_of_life.sirius
```

`filesystem_walk.sirius` takes a directory argument and is tested separately
by `python tools/test_filesystem.py`.

## Implemented language features (checked against source tests)

- **Core:** `let`/`var`, annotated and inferred functions, `return`, generic
  specialization, ordinary recursion and `recur`; `if`/`elif`/`else`, `while`,
  `for`, `break`/`continue`, guards and short-circuit Boolean expressions.
- **Values:** `i64`, `f64`, `bool`, UTF-8 strings; `rec` constructors and
  structural records with spreads/deletion; local `let` record destructuring.
- **Collections:** homogeneous arrays (including nested arrays), typed
  dictionaries, immutable snapshots with CoW, record/array nested updates,
  iteration, optional lookups, unwrap/fallback and checked indexing.
- **Runtime:** printing, string concatenation and basic string methods,
  file I/O and directory listing, argument access, bitwise integer operations.
- **Toolchain:** source modules/imports, specialized generic packages,
  C11 emission, MIR interpretation and MIR serialization are exercised by
  dedicated tests under `tools/`.

### Not yet implemented / relevant restrictions

- **Tagged unions and exhaustive `match`** are available in source modules,
  including `Std.Filesystem` `Result` wrappers. The standalone examples retain
  `Optional` where it keeps the baseline self-contained; constructor overrides
  and nested implicit union conversions remain incomplete.
- General `comptime`, `Any` with runtime descriptor access, vectorized
  NumPy-like computation, parallel `map`, and capturing closures are future
  features, not suitable for current executable examples.
- Use `a[!index]` for checked array access and `a[?index]` for optional
  lookup. Plain `a[index]` is supported for immutable `let` arrays with proven lengths, including array literals, aliases, empty `Array[T]()` constructors and fixed `.append(...)` chains. Elements may be computed at runtime; rectangular row shapes survive immutable aliases and shape-compatible appends, so `len(a[i])` can be proven inside a loop. Mutable arrays, unknown-length function results, unproven ragged dimensions and negative indices require checked/optional access.
- Calls to ordinary named functions work. User-defined `value.method(...)`
  extension-call syntax is not yet supported consistently by the MIR emitter;
  `geometry.sirius` deliberately calls `distance(origin, point)` instead.
- Program entry points are zero-argument functions returning `i64`; use
  `arguments()` for command-line parameters. Floating-point-to-integer
  conversion is not currently provided as a general intrinsic.

## Potential improvements (not yet implemented)

The examples deliberately use the **supported language subset**. These are
opportunities to revisit them as new features become available, not syntax that
can be used today. Changes should preserve the current examples as reference
implementations for comparison.

| Example | Potential feature or library improvement | Expected benefit |
| --- | --- | --- |
| `game_of_life.sirius` | Native 2D arrays, slicing/neighbor shifts, elementwise operations and broadcasting | Express neighbor counts and generation updates as array operations instead of nested loops; benchmark allocation costs and performance |
| `prime_sieve.sirius` | Array generation, slices and bulk Boolean-mask updates | Replace repeated element updates with concise bulk operations; check CoW behavior |
| `word_frequencies.sirius` | Standard-library string splitting, normalization and tokenization | Process raw text or file contents instead of an already-tokenized word array |
| `filesystem_walk.sirius` | Use `Std.Filesystem.walk_files_result` with exhaustive matching | Report directory-listing and metadata failures instead of silently ignoring them; the best-effort example remains a baseline |
| `geometry.sirius` | Consistent user-defined extension-method calls | Allow `point.distance(other)` as an ergonomic alternative to a named function |
| `insertion_sort.sirius` | Negative indexing and improved array slicing | Simplify indexing and enable comparison with alternative sorting approaches |
| `cow_nested.sirius` | More expressive nested-update APIs | Make immutable updates easier to write while testing structural sharing and ARC |
| `generic_choose.sirius` | Broader structural polymorphism and generic diagnostics | Exercise specialization with richer record shapes and clearer errors |
| `fizzbuzz.sirius`, `fibonacci.sirius`, `bitwise_rgb.sirius` | No essential missing feature | Keep as simple baseline examples for control flow, arithmetic and bitwise operations |

### Suggested implementation and example order

1. **Checked filesystem example.** Adapt the standalone walker to use
   `Std.Filesystem.walk_files_result` in a module build; retain the original
   implementation as a baseline for comparison.
2. **Multidimensional arrays and vector operations.** Revisit Game of Life and
   the prime sieve. Compare readability, allocation counts, native performance
   and MIR-interpreter parity against their imperative versions.
3. **String-processing standard library.** Extend word frequencies to read and
   tokenize real text. This should not require new core language semantics.
4. **Pure collection transformations.** Explore `map`, `filter`, `reduce` and
   array generation after their effect/purity rules are defined. `map` should
   not be used for ordered side effects such as FizzBuzz printing; `for`
   remains appropriate for sequential effects.
5. **Extension methods and indexing ergonomics.** Update geometry and sorting
   once the MIR backend supports them consistently.

Other planned capabilities—general compile-time execution, dynamic `Any`,
reflection and parallel transformations—have little immediate benefit for these
small examples and need not block their evolution.

When revising an example, keep its expected output and add native/interpreter
parity tests. For optimization-oriented rewrites, retain the original or a
baseline variant so behavior and resource use can be compared meaningfully.

For the **target** semantics (which may be broader), see
[`LANGUAGE_DESIGN.md`](../LANGUAGE_DESIGN.md). For the current compiler and
its roadmap, see [`README.md`](../README.md) and [`ROADMAP.md`](../ROADMAP.md).
