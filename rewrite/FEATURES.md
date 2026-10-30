# Features supported by the current Sirius compiler

This inventory describes the existing self-hosted compiler. The incomplete
rewrite has a separate [supported subset](README.md). Implementation and tests
are the source of truth; the language design also describes future features.

- Values and types: `i64`, `f64`, `bool`, `string`, structural records,
  arrays, dictionaries, optionals, enums and labeled unions. Union indices
  are available through `Union.index(value_or_label)` in declaration order.
- Bindings and control flow: `let`, local `var` and updates, conditionals,
  loops, `break`, `continue`, `return`, guards and exhaustive `match`.
- Functions: annotations and inference, generic specialization, recursion and
  `recur`, noncapturing function values and lambdas, and typed overloads.
- Collections: value semantics and copy-on-write updates, checked and optional
  indexing, negative indices, and statically proven plain indexing.
- Expressions: integer and floating-point arithmetic, comparisons, logical and
  bitwise operators, ranges, explicit numeric conversions, and lazy optional
  defaults. Integer arithmetic wraps; numeric promotion is not implicit.
- Records and unions: structural record construction and destructuring,
  labeled alternatives, explicit union construction, and grouped match arms.
- Strings and I/O: UTF-8 byte access, slicing, searching and joining, console
  output, command-line arguments, and filesystem operations.
- Modules: source imports, exported functions, records and constants, named
  re-exports, concrete MIR libraries and source-free generic packages.
- Execution: typed HIR to MIR, native C11 generation, MIR interpretation,
  executable MIR serialization, and a self-hosting C bootstrap.
- Standard library: sequential functional array operations, filesystem and
  `Result` helpers, and checked two-dimensional `f64` matrices.

## Details and boundaries

| Area | Supported behavior and example syntax |
| --- | --- |
| Syntax and scope | Indented blocks, `#` comments, multiline parenthesized expressions, inferred or annotated `let`/`var`, top-level constants, nested local scopes and shadowing. Only mutable local bindings can be updated. |
| Control flow | `if`/`elif`/`else`, `while`, `for x in array`, half-open `0..n` ranges, `break`, `continue`, early `return`, and `condition else return value` guards. |
| Operators | `+ - * / %`, comparisons, short-circuit Boolean `and`/`or`, `not`, integer `& | << >>`, and compound updates such as `+=`. Integer addition, subtraction, multiplication and negation wrap at 64 bits. Division truncates toward zero; division/remainder trap on zero divisors and minimum `i64` with divisor `-1`. |
| Numbers | No implicit promotion; `f64(3)` converts an integer (nearest, ties to even), `i64(3.9)` truncates toward zero and traps outside the finite `i64` range. `sqrt` accepts `f64`. |
| Records | `rec Point(x: i64, y: i64)`, positional construction, structural literals `{x = 1, y = 2}`, field access, local destructuring and nested updates. Record literals support `record.*` spread, field overrides and `del field`. Generic constructors support computed `pub` fields, `pub value.field` shorthand and fixed-type conditional overrides; see [constructor tests](../tools/test_constructor_publications.py). |
| Functions | Inferred parameters/results specialize at concrete call sites; annotated overloads resolve by parameter types. Ordinary recursion and tail `recur` work. `Fn[i64,i64]`, `fn(x) x + 1`, and calls through function-valued record fields are supported. Callbacks cannot capture locals; inference sometimes needs an explicit `Fn` annotation. |
| Arrays | Homogeneous literals, `Array[T]()`, rank-one alias `Array[T,1]`, `len`, iteration and `.append(value)`. Nested arrays and managed/union elements work. `[!i]` checks bounds, `[?i]` returns `Optional[T]`, and `[i]` requires a supported static bounds proof. Negative indices count from the end. |
| Value semantics | Records, arrays and dictionaries preserve snapshots across updates; nested record/array updates use copy-on-write. Reference counting manages acyclic storage. |
| Dictionaries | `Dict[K,V]()`, `.insert`, `.get`, and `len`. Keys include scalars and supported structural records, optionals and unions; array-containing keys are rejected. |
| Equality | Scalars, records, optionals and tagged unions support `==`/`!=`; record equality recursively compares contained collections. Direct array `==` is rejected (reserved for elementwise comparison). |
| Optionals | Optional indexing, dictionary lookups and filesystem reads produce optionals. `value or fallback` evaluates the fallback only on absence, even when the payload is zero/false. `let x = optional else return value` unwraps or exits; `optional!` unwraps or traps. |
| Unions | `let Result = Ok | Error`, `enum`, payload destructuring, bare-tag matches, grouped tags `A | B`, and shared compatible bindings `(A | B)(x)`. Matches must be exhaustive. `Result.index(value_or_label)` follows declaration order. |
| Union coercions | Expected union types guide constructor injection, including `Array[Result]` literals and typed collection updates. Stored structural records lose constructor labels; use `Result.Ok(record)` to disambiguate identical payload shapes. |
| Strings and I/O | Concatenation, comparison, byte length `len`, `str(i64)`, `.slice`, `.byte_at`, `.find`, `.starts_with`, `.ends_with`, separator `.join`, scalar `print`, and `arguments()`. String offsets are UTF-8 byte offsets. |
| Filesystem | Built-in `read_text`, `write_text`, `list_dir`, `path_kind`; [filesystem examples](../examples/filesystem_walk.sirius) show optionals and failure handling. `Std.Filesystem` adds structured errors and directory walks. |
| Modules and artifacts | Imports with aliases, exported functions/records/constants/unions, named re-exports, concrete `.smirl` libraries, source-free generic `.smirg` packages and executable `.smir`. See runnable [module examples](../docs/MODULES.md); formats are experimental. |
| Standard library | `Std.Functional`: map/filter/generate, indexed and explicit-context maps, partition, fold/scan, zip/zip_with, enumerate, count_if/any/all/find_index. `empty_array_like` preserves element types. `Std.Numeric`: checked matrix construction, reshape, transpose, multiplication and singleton-axis broadcast addition. See [usage and imports](../docs/STDLIB.md). |

Not supported generally: capturing closures, dynamic `Any` and reflection,
general compile-time MIR execution, N-dimensional arrays, automatic numerical
fusion or SIMD, effect inference, parallel mapping, and native/interpreted FFI
registration. See [README](../README.md), [examples](../examples/README.md),
and [roadmap](../ROADMAP.md) for scope and remaining restrictions.

## Syntax examples

Each block is a complete program for the existing compiler, not a claim that
all these features already work in the rewrite. Indentation is tabs.

Local mutation, ranges, conditionals and integer arithmetic:

```sirius
def main() -> i64
	var total = 0
	for i in 0..5
		if i % 2 == 0
			total += i
	print(total) # 6
	return 0
```

Structural records, nested arrays, checked negative indexing and CoW snapshots:

```sirius
def main() -> i64
	var state = {scores = [10, 20]}
	let snapshot = state
	state.scores[!-1] = 25
	print(snapshot.scores[!1]) # 20
	print(state.scores[!1]) # 25
	return 0
```

Generic specialization and a noncapturing function value:

```sirius
def twice(x)
	return x + x

def apply(f: Fn[i64,i64], x: i64) -> i64
	return f(x)

def main() -> i64
	print(apply(twice, 21)) # 42
	return 0
```

Enums are labeled unions. `index` accepts either a value or a label; matching
is exhaustive and can group alternatives:

```sirius
enum Color
	Red
	Green
	Blue

def warm(color: Color) -> bool
	match color
		Red
			return true
		Green | Blue
			return false

def main() -> i64
	let color: Color = Green()
	print(Color.index(color)) # 1
	print(Color.index(Blue)) # 2
	if warm(color)
		return 1
	return 0
```

Boolean bindings, negation and comparisons (use parentheses to negate a
comparison as a whole):

```sirius
def main() -> i64
	let enabled: bool = true
	print(not enabled) # false
	print(not (3 >= 2)) # false
	return 0
```

Dictionaries and lazy optional defaults:

```sirius
def main() -> i64
	var counts = Dict[string, i64]()
	for word in ["red", "blue", "red"]
		counts = counts.insert(word, (counts.get(word) or 0) + 1)
	print(counts.get("red") or 0) # 2
	print(counts.get("missing") or 9) # 9
	return 0
```

Further complete examples cover [payload unions and grouped matching](../examples/event_log.sirius),
[bitwise operations](../examples/bitwise_rgb.sirius),
[floating-point arithmetic and union construction](../examples/geometry.sirius),
[recursion and loops](../examples/fibonacci.sirius), and
[filesystem operations, arguments and guards](../examples/filesystem_walk.sirius).
For multi-file imports and exports see [module examples](../docs/MODULES.md);
for array transformations and matrices see [standard library examples](../docs/STDLIB.md).

Optional indexing, early exit, guards and absence-based defaults:

```sirius
def read(values: Array[i64], index: i64) -> i64
	index >= 0 else return -1
	let value = values[?index] else return -1
	return value

def main() -> i64
	print(read([0, 7], 1)) # 7
	print(read([0, 7], 9)) # -1
	print([0][?0] or 99) # 0
	return 0
```

Payload unions, typed heterogeneous collections and exhaustive matching:

```sirius
rec Ok(value: i64)
rec Error(message: string)
let Result = Ok | Error

def value_or_zero(result: Result) -> i64
	match result
		Ok(value)
			return value
		Error
			return 0

def main() -> i64
	let results: Array[Result] = [Ok(7), Error("missing")]
	print(value_or_zero(results[!0])) # 7
	print(value_or_zero(results[!1])) # 0
	return 0
```

Structural compatibility, destructuring, equality and noncapturing lambdas:

```sirius
rec Point(x: i64, y: i64)

def apply(f: Fn[i64,i64], x: i64) -> i64
	return f(x)

def main() -> i64
	let point: Point = {x = 3, y = 4}
	let {x = .x, y = .y} = point
	print(x + y) # 7
	print(point == Point(3, 4)) # true
	print(apply(fn(n) n * n, 5)) # 25
	print(i64(sqrt(f64(81)))) # 9
	return 0
```

String byte operations, searching, joining and integer formatting:

```sirius
def main() -> i64
	let text = "hello world"
	print(text.slice(0, 5)) # hello
	print(text.byte_at(1) or -1) # 101
	print(text.byte_at(0)!) # 104
	print(text.find("world") or -1) # 6
	print(text.starts_with("hello") and text.ends_with("world")) # true
	print(", ".join(["one", str(2)])) # one, 2
	return 0
```

Recursion, `recur`, typed overloads and additional string/collection cases have
executable examples in the [language fixtures](../tests/language/fixtures).
`python tools/test_rewrite_compiler.py` checks every complete block above
with the existing native compiler and MIR interpreter (including printed output).
