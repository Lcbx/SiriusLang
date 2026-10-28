# Function values and callback inference

Sirius function values use `Fn[parameters..., return]`. Named generic functions
specialize from an expected signature. Expression-bodied lambdas are
noncapturing; capturing closures are not planned. See [Usage](USAGE.md) for a
typed function example and [Standard library](STDLIB.md) for callback helpers.

## Inference and explicit context

Overload resolution uses parameter types, including the callback's full `Fn`
signature. Generic named callbacks and noncapturing lambdas receive an
expected `Fn` type when one is available. Generic helpers can also infer
callback **parameter** types from their own concrete inputs and uses of the
callback (for example, an array element in `map` or an index in `generate`).
Independently typable arguments may appear before or after a callback, including
nested calls such as `map(values, fn(x) x + 1)`. A lambda cannot capture
outer locals: `map(values, fn(x) box.run(x))` is unsupported even with `x`
annotated; pass `box.run` directly when no other state is needed, or use
`map_with_context` to pass a runtime context explicitly:

```sirius
import Std.Functional alias F

def apply_box(box, values)
	return F.map_with_context(values, box, fn(x, ctx) ctx.run(x))
```

This passes `box` as an ordinary argument on each
callback invocation. It works with managed record fields and source-free generic
packages, and returns correctly typed empty arrays. Context is immutable from
the callback's perspective; existing CoW semantics still apply. A callback
parameter whose type cannot be inferred requires an annotation.

## Forwarding and aliases

The compiler inspects callback argument types ahead of time without changing
runtime evaluation order. The callback's
return type is then inferred from its body. Multi-stage callback-result chains
are also supported when an independently typed argument seeds the chain,
for example `pipeline(fn(s) len(s), fn(x) str(x), [7, 12])` when `pipeline`
calls the first callback on the second callback's result. A callback passed
through another generic function can also infer its inputs from later arguments
(e.g. `relay(fn(x) x * 2, [7, 14])` where `relay` forwards to a generic
`consume(callback, values)`). This works across multiple forwarding functions
and branch bodies. Local callback aliases, record fields holding callbacks, and
mutable reassignments also carry constraints. Nested paths such as
`box.inner.run` are followed through record copies; overwriting `box.inner`
invalidates aliases beneath that field. After conditional assignments,
an alias contributes constraints only when preserved on every continuing path;
exhaustive `match` arms are joined similarly. Branches ending with `return`
do not weaken the aliases of paths that continue, and unreachable statements
do not add constraints. Loops do not assume a body executes. For example,
`let box = {run = callback}` can forward `box.run` to `consume`, even when
`callback` is still generic. Conversely, a **record literal argument** can infer
an unannotated callback field from a generic callee that invokes that field:

```sirius
def sum_box(box, values) -> i64
	var total = 0
	for value in values
		total += box.run(value)
	return total

let answer = sum_box({run = fn(x) x * 2}, [7, 14])
```

Nested literal fields (`box.inner.run`) and independently constrained fields
work as well, including with `.smirg` generic declarations. The inference is
limited to record literals with identifiable callback fields; it does not
infer arbitrary record transformations or types without a concrete seed.
## Function fields and diagnostics

Record function fields support indirect calls (`box.run(value)`),
including nested projections; extracting a field first (`let f = box.run;
f(value)`) also works. Dependency cycles without a concrete type seed
still require explicit `Fn` annotations. Compilation diagnostics distinguish
**conflicting** inferred signatures, **cyclic** forwarding dependencies without
a type seed, and **underdetermined** callback parameter types. Conflicts include
the first mismatching type or arity and callback-call lines in the defining
body; cycles include the forwarding chain. For unresolved or cyclic cases,
annotate callback parameters explicitly.
Source-free `.smirg` packages preserve generic inferred-result definitions.
See [Standard library](STDLIB.md#functional-array-helpers) for result types
and empty-array behavior.
