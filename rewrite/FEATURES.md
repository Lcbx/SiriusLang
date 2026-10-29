# Features supported by the current Sirius compiler

This inventory describes the existing implementation, not the rewrite. See
the runnable examples and tests for restrictions and specific combinations.

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
