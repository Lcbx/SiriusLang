# Sirius examples

These are working programs for the current self-hosted compiler. The twelve
argument-free programs have checked-in golden outputs, validated by
`python tools/test_examples.py`. The directory walker is exercised separately
by `python tools/test_filesystem.py`. No standard-library imports are needed.

## Programs

| Program | Demonstrates |
| --- | --- |
| [`fizzbuzz.sirius`](fizzbuzz.sirius) | Ranges, conditionals, remainder and printing |
| [`fibonacci.sirius`](fibonacci.sirius) | Functions, sequential loops and mutable locals |
| [`prime_sieve.sirius`](prime_sieve.sirius) | Checked indexing, guards, recoverable errors and exhaustive matching |
| [`game_of_life.sirius`](game_of_life.sirius) | Nested arrays and value-based generations of a 5x5 blinker |
| [`insertion_sort.sirius`](insertion_sort.sirius) | Sorting, CoW snapshots, string conversion and joining |
| [`word_frequencies.sirius`](word_frequencies.sirius) | Dictionaries, lazy Optional defaults and `Found`/`Missing` results |
| [`cow_nested.sirius`](cow_nested.sirius) | Nested CoW updates, negative indexing and record destructuring |
| [`generic_choose.sirius`](generic_choose.sirius) | One generic function specialized for numbers, strings, and structural records |
| [`geometry.sirius`](geometry.sirius) | Same-shaped union alternatives, shared bindings, tag inspection and `f64` arithmetic |
| [`matrix_multiplication.sirius`](matrix_multiplication.sirius) | Flat row-major matrices, matrix/vector products and checked shape errors |
| [`event_log.sirius`](event_log.sirius) | Heterogeneous `Array[Event]`, payload matching and grouped tag-only classification |
| [`bitwise_rgb.sirius`](bitwise_rgb.sirius) | Bitwise packing and exhaustive dispatch over an enum of RGB channels |
| [`filesystem_walk.sirius`](filesystem_walk.sirius) | Breadth-first traversal, arguments, guards and filesystem optionals |

All programs use `main() -> i64` and return zero on success. The walker requires
one directory argument and returns 2 for invalid arguments or a failed listing.

`geometry.sirius` gives `Rectangle` and `Triangle` the same `width`/`height`
payload shape. Their union labels remain distinct. One shared binding list
reads the dimensions, and `Shape.index` distinguishes their area rules.
`event_log.sirius` uses `Joined | Left` to count membership changes without
extracting either payload.

The matrix example keeps its representation local: flat `Array[f64]` storage,
shape checks that guard multiplication against overflow, ordinary loops, and
fresh result arrays. It prints each floating-point result separately because
`str()` currently accepts only `i64`. For reusable module-based numerical
operations, `Std.Numeric` provides a rank-2 matrix wrapper with `matmul`,
transpose, reshape, and singleton-axis broadcast addition. Native ranked arrays
are not a prerequisite. See [Standard library](../docs/STDLIB.md).

## Running

From the project root:

```sh
python tools/build_compiler.py
./sirius examples/game_of_life.sirius game_of_life.c
cc -std=c11 -I runtime game_of_life.c runtime/sirius_runtime.c -lm -o game_of_life
./game_of_life
python tools/test_examples.py
```

To check every golden output in both native and interpreted execution:

```sh
python tools/build_mir_runner.py
python tools/test_examples.py --runner ./sirius-run
```

The runner can also execute a source directly:

```sh
./sirius-run examples/game_of_life.sirius
./sirius-run examples/filesystem_walk.sirius .
python tools/test_filesystem.py --runner ./sirius-run
```

On Windows, use `.exe` executable names and the linker flags appropriate to
your C toolchain. The test scripts select the C compiler and linker flags using
`tools/build_support.py`.

## Supported features and restrictions

- **Values and control flow:** `let`/`var`, structural records, local record
  destructuring, arrays, dictionaries, loops, guards, recursion and `recur`.
  Updates preserve immutable snapshots through CoW.
- **Functions:** inferred and annotated functions, concrete generic
  specialization, first-class `Fn` values and noncapturing lambdas. Capturing
  closures are excluded by design; `Std.Functional.map_with_context` provides
  explicit context passing. Generic callback inference works where a concrete
  signature can be established; explicit `Fn` annotations handle other cases.
- **Unions:** alternatives are distinguished by constructor label, including
  identical payload shapes. Matching is exhaustive. Bare tags discard payloads;
  `A | B` shares a tag-only branch and `(A | B)(value)` shares compatible
  bindings. Stored structural records lose their constructor label, so ambiguous
  injection requires explicit `Union.Label(record)` construction.
- **Collections:** array elements have one static type, which can itself be a
  union. Mixed-variant literals require an expected type such as
  `Array[Event]`; unannotated literals remain homogeneous. Typed appends and
  dictionary inserts preserve direct constructor labels.
- **Optionals:** `or` defaults run only on absence and receive the optional's
  payload type as context. `else return` can exit early instead. Absence is
  independent of the payload's truthiness.
- **Indexing:** `[!i]` is checked, `[?i]` returns an Optional, and negative
  indices count from the end. Plain `[i]` requires a supported compile-time
  bounds proof; use checked or optional access when bounds are unknown.
- **Numerics:** explicit `f64(i64)` conversion rounds to nearest, ties to even;
  `i64(f64)` truncates toward zero and traps for nonfinite or out-of-range
  values. There is no implicit numeric promotion. Integer addition,
  subtraction, multiplication and negation wrap at 64 bits.
- **Calls and strings:** ordinary functions and calls through `Fn`-valued
  record fields work; general user-defined extension-method syntax is not
  supported. String helpers include slicing, searching, UTF-8 byte access and
  joining. General splitting, normalization and formatting APIs remain limited.
- **Execution:** functional array helpers are sequential and purity is not
  enforced. General compile-time MIR execution, runtime `Any`/reflection,
  general N-dimensional arrays, numerical fusion and parallel mapping remain
  future work. Artifact formats and native/interpreted FFI are not stable ABIs.

Source modules, named re-exports, MIR libraries, and source-free generic
packages are covered by dedicated tests. See [Usage](../docs/USAGE.md) and
[Modules and packages](../docs/MODULES.md) for module-based programs.

For intended semantics, see [Language design](../LANGUAGE_DESIGN.md). For the
current implementation and open priorities, see [README](../README.md) and
[Roadmap](../ROADMAP.md).
