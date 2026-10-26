# Sirius — Language Design

> **Status:** Language target, not an inventory of implemented compiler features. The current self-hosted Sirius compiler implements a subset; see [README.md](README.md) for implementation status.
>
> **Implementation note:** `i64` bitwise `&`, `|`, `<<`, and `>>` are now
supported. This is distinct from `|` in type-union expressions.

> **Clarifications:** The initial bootstrap was written in Python; dictionaries are `Dict` (not `Map`); structural records have lexicographically ordered fields. Negative array indexing is deferred. For proposed modules, shared MIR execution, libraries, and FFI, see [MIR_MODULES_AND_FFI.md](docs/MIR_MODULES_AND_FFI.md).

## 1. Core Philosophy

A statically typed, compiled-first, functional-first language combining Python-like syntax, local imperative control flow, immutable values, structural typing, and NumPy-like numerical computing. Typed MIR also supports interpretation.

* **Immutable values:** Records and arrays have value semantics, with copy-on-write (CoW) for efficient updates.
* **Local imperatives:** Mutable bindings and conventional loops express sequential algorithms and local state.
* **Functional transformations:** Pure array operations express independent computations, eligible for fusion, SIMD, and parallelization.
* **Static duck-typing:** Structural compatibility through compile-time specialization (monomorphization).
* **Universal types:** Canonical descriptors support memory layout, introspection, and explicit dynamic typing.
* **Acyclic memory:** No mutable shared objects or capturing closures; reference counting without tracing GC.

**Guiding principle:** Minimize overlapping constructs. Functional operations transform collections; imperative control flow handles sequencing and state. Higher-level utilities should be expressible through simpler primitives.

## 2. Syntax and Bindings

Tab-based indentation, no colons or block delimiters.

| Keyword | Meaning |
| --- | --- |
| `let` | Immutable binding |
| `var` | Mutable binding |
| `pub` | Publish a constructor field |
| `export` | Expose a module definition |
| `recur` | Guaranteed tail recursion |

```sirius
let x = 10
var y = 20

if x > 5
	y += x

for i in 0..10
	y += i

while y > 20
	y -= 1
```

* Bindings have fixed static types.
* Function parameters and loop variables are immutable.
* `for` and `while` execute sequentially, supporting `break`, `continue`, and side effects.
* `0..n` is half-open.
* `and`, `or`, `not` short-circuit on scalar booleans.
* Strict numeric typing: no implicit promotion; literals may adopt contextual types. Without context, integer literals default to `i64`, floating literals to `f64`.
* Explicit conversions (`f32(x)`, `i64(x)`, etc.), with defined conversion and overflow semantics (details still to be specified).

## 3. Immutable Values and Local Mutation

Records and arrays are immutable values. Assignment through `var` replaces the value, potentially reusing uniquely owned storage.

```sirius
var point = {x = 10, y = 20}
let original = point

point.x += 5

# point.x == 15
# original.x == 10
```

**CoW:** Storage may be modified in place when exclusively owned and safe to overwrite; otherwise it is copied. Nested values support structural sharing.

### Record destructuring

Destructuring names every source field explicitly; binding names need not match:

```sirius
let {first = .a, second = .b} = value
```

Equivalent to `let first = value.a` and `let second = value.b`. The source
expression is evaluated once. `.a` and `.b` are field selectors relative to
that source, not positional patterns or standalone expressions. Fields may be
listed in any order, but implicit/positional destructuring
(`let {first, second} = value`) is not supported. Missing fields or repeated binding names are
compile-time errors. Structural typing determines field compatibility; source
record fields not listed are ignored. Initially restricted to `let` bindings;
recursive patterns and `var` destructuring are deferred.

Record updates use spreading:

```sirius
let updated = {
	point.*
	del y
	x = 30
	z = 40
}
```

Operations execute top-to-bottom; later fields override earlier ones. Record schemas are statically resolved. Canonical record layout orders fields lexicographically by name, independently of source order.

Indexed assignment follows the same value semantics:

```sirius
var a = [1, 2, 3]
a[!1] = 10
```

Field and checked indexed assignments require a mutable root binding. Nested paths and compound assignments evaluate indices once and reconstruct modified values from the leaf to the root using CoW.

## 4. Functions, Constructors, and Methods

Functions have two mutually exclusive modes.

**Evaluator:** Explicitly returns a value.

```sirius
def add(a, b)
	return a + b
```

**Constructor:** `rec` automatically publishes its parameters into an immutable structural record. An optional body adds computed fields or overrides parameters.

```sirius
rec Point(x, y)
	pub length = sqrt(x*x + y*y)
```

A parameter-only constructor needs no body: `rec Pair(x: i64, y: i64)`.

`def` is reserved for evaluators; constructors must use `rec`.

Constructor rules:

* Fields must be initialized unconditionally.
* Branches may override existing fields, not introduce new ones.
* Constructors cannot return early or mix `pub` with `return expr`.
* `pub x` publishes a binding and freezes ordinary assignment.
* `pub x = expr` creates or overrides a published field (including an implicit `rec` field).
* `pub value.x` abbreviates `pub x = value.x`.

The implemented MIR subset evaluates unconditional `pub` statements in source
order, allowing later publications to refer to earlier fields and to override
them. A final `if`/`elif`/`else` may override those existing fields (with the
same type); branches cannot introduce fields or contain other statements.
Exported constructors with annotated parameters may also publish computed
fields and conditionally override existing fields. Their final structural
record shape is part of the module interface. Source imports and source-free
`.smirg` packages transport these constructor bodies for HIR-checked inline
construction, including re-exports. The portable subset also resolves defining-module constants and direct or
transitive local helper calls hygienically: referenced values and helper bodies
are copied into private, owner-qualified bindings before HIR analysis. Source-free
packages retain the same dependencies and include them in interface fingerprints.
Unknown free names are rejected; general closure capture remains unsupported.

**Methods:** Ordinary functions whose first parameter is named `this`, invoked with dot syntax.

```sirius
def move(this, dx, dy)
	return {
		this.*
		x = this.x + dx
		y = this.y + dy
	}

var p = Point(3.0, 4.0)
p = p.move(1.0, 2.0)
```

`p.move(dx, dy)` statically resolves to `move(p, dx, dy)`, without virtual dispatch or receiver capture. Function-valued fields take precedence over extension methods.

## 5. Static Typing and Descriptors

Types are inferred; generic functions specialize for concrete argument types.

```sirius
def norm(p)
	return sqrt(p.x*p.x + p.y*p.y)
```

Works with any compatible record containing `x` and `y`.

* Structural compatibility is checked at compile time.
* Structurally identical records share a canonical type and deterministic layout, with fields sorted by name.
* Different record layouts receive separate specializations.
* Generic specializations are cached by concrete type signature.
* Optional annotations support recursion, interfaces, and numeric precision.
* No general row-polymorphic solver or runtime structural lookup for static values.
* Concrete function types depend on parameter types, return type, and effect guarantees, not implementation identity.

### Universal descriptors

Every type has a canonical descriptor containing:

* Kind, size, alignment, and identity.
* Record field names, types, and offsets.
* Array element type and rank.
* Type-specific memory-management and generic operations.

Across modules and execution modes, structural type identity is semantic, not based on process-local descriptor addresses. Portable library interfaces preserve exported type information (see [MIR_MODULES_AND_FFI.md](docs/MIR_MODULES_AND_FFI.md)).

```sirius
let t = dtype(p)

for field in t.fields
	print(field.name, field.type)
```

Ordinary values carry no runtime type ID.

### Dynamic typing

Explicit `Any` values carry a descriptor and inline or boxed storage.

```sirius
let value: Any = p
let x = value.get("x")  # Optional[Any]
```

Dynamic lookup and checked conversion are explicit. No runtime specialization is required.

### Constructor-based tagged unions

`let Result = Success | Failure` is a compile-time union type alias formed from constructor result types; `match` tests a runtime union tag and binds the published fields:

```sirius
rec Success(value: i64)
rec Failure(code: i64)

let Result = Success | Failure

def unwrap(result: Result) -> i64
	match result
		Success(value)
			return value
		Failure(code)
			return -code
```

Patterns bind constructor parameters first in declaration order; optional
additional binders then select the remaining published fields in canonical
lexicographic name order. Patterns may omit these additional binders but must
include every constructor parameter.

**Current design choice:** Constructors produce structurally typed records. Union alternatives must therefore have distinct canonical record types; constructors with identical result shapes are not distinguishable alternatives. Nominal variant identity is not currently specified.

### Errors and fatal failures

* Recoverable errors are ordinary returned values, normally constructor-based
  `Result` unions. `match` on a union must cover every alternative; missing
  cases are a compile-time error. There is no exception-unwinding mechanism for
  recoverable errors.
* `Optional` remains suitable for simple absence, while status values or
  documented numeric sentinels avoid mandatory wrapping of routine operations.
  Default IEEE-754 arithmetic may produce `NaN` (`0.0 / 0.0`, negative `sqrt`)
  or infinity (nonzero finite value divided by zero), without a runtime error.
  Integer division has no NaN representation; zero divisors and signed
  overflow currently cause an explicit fatal trap.
* A fatal trap terminates the process rather than returning an error union.
  Process exit may discard live ARC frames; no language-level cleanup or
  destructors are promised after a fatal trap. Invalid MIR, out-of-memory
  and fatal programmer assertions follow this policy unless reclassified.
* Source-level union aliases, constructor-to-union injection in annotated
  bindings, call arguments and returns, and exhaustive `match` are executable
  through both C and MIR interpretation. A managed wrapper stores the stable
  alternative tag and an owned record payload; copies/returns follow ordinary
  ARC rules. Direct variant extraction checks the runtime tag before borrowing.
  `export let Result = Success | Failure` publishes an alias in the module's
  typed interface. Importing modules can use `Types.Result` in annotations and
  `Types.Success(value)` / `Types.Failure(code)` as qualified `match` arms.
  Named re-exports preserve the union's structural identity. Source-free
  `.smirg` packages can expose public constructors, aliases and constants
  alongside generic functions; concrete non-generic functions still require
  compiled libraries or source. Implicit union coercions traverse structural
  records, homogeneous arrays, `Optional` payloads and dictionary values
  (with identical key types), rebuilding containers without mutating sources.
  Contextually typed array literals accept mixed union variants; otherwise
  literals remain homogeneous. Typed `.append` and `.insert` inject compatible
  union values. Dictionary key conversion and unrelated record layouts are
  unsupported.
  `Std.Filesystem` provides optional `ReadResult`, `WriteResult`, `ListResult`,
  `WalkResult`, and `PathKindResult` wrappers using
  `FilesystemFailure(operation, path, kind, code)`. Filesystem
  builtins preserve a thread-local error snapshot: portable `kind` values
  distinguish not found, permission denied, not a directory, is a directory,
  invalid path, and other I/O failures; `code` retains the native errno/Win32
  value. Success clears both fields. `filesystem_error_kind()` and
  `filesystem_error_code()` must be read before the next filesystem operation.
  `path_kind(path)` returns `1=regular`, `2=directory`, `3=other` (including
  symbolic links), or `0=metadata error`; the failure snapshot is available
  immediately. `path_kind_result` wraps this in `PathKindSuccess` or
  `FilesystemFailure`. `walk_files_result` reports both directory-enumeration
  and per-entry metadata failures, keeping the existing non-following symlink
  policy. This is not an atomic snapshot of a changing filesystem.
  The underlying `Optional`/`bool` results remain supported; these and
  IEEE-754 in-band results remain valid.

## 6. Operators

Operators resolve statically to functions, including generic implementations.

```sirius
def __add__(a: {x: f32, y: f32}, b: {x: f32, y: f32})
	return {x = a.x + b.x, y = a.y + b.y}
```

* Arithmetic, comparison, indexing, and iteration support overloading.
* Ambiguous matches are compile-time errors.
* **Implemented named-function subset:** same-name `def` overloads with fully
  annotated, distinct parameter signatures. Direct calls resolve exact matches;
  first-class overloads require a contextual `Fn` type. Concrete module exports
  use stable `_O` type-signature mangling, keeping non-overloaded symbols intact.
  Generic, `extern`, and constructor overloads remain unsupported.
* Imported overloads must be explicitly visible.
* Compiler-recognized primitives support optimized numerical lowering.

## 7. Arrays and Vectorization

`Array[T, N]` is a homogeneous multidimensional standard-library type, inspired by Codon-NumPy. Element type `T` and rank `N` are static; shapes are dynamic. `Array[T]` is shorthand for `Array[T, 1]`.

```sirius
let x = [1.0, 2.0, 3.0]
let y = x * 2.0 + 1.0
let mask = y > 3.0
let result = where(mask, y, 0.0)
```

### Storage

* Dense contiguous row-major buffers with reference-counted CoW.
* Rectangular multidimensional literals.
* Arbitrary statically representable element types, including records.
* `reshape` may share storage; transpose initially copies.
* No arbitrary strided views or advanced indexing initially.

### Broadcasting

* Dimensions align from the right; dimensions must match or have size 1.
* Scalars broadcast to compatible arrays.
* Invalid shapes trigger runtime panics.
* Matrix multiplication uses a separate operation.
* `&`, `|`, `~` operate elementwise on boolean arrays.
* Conditions require scalar booleans.

### Numerical operations

| Category | Functions |
| --- | --- |
| Elementwise | `abs`, `sqrt`, `exp`, `log`, `sin`, `cos` |
| Selection | `where`, `minimum`, `maximum` |
| Reductions | `sum`, `mean`, `min`, `max`, `any`, `all` |
| Shape | `shape`, `reshape`, `transpose` |
| Construction | `zeros`, `ones`, `full`, `arange`, `generate` |

Functions accept compatible scalars or arrays. Reductions initially operate over all elements. Reduction order is preserved initially; parallel reductions require explicitly defined numerical semantics. `where` evaluates arguments eagerly.

### Functional transformations

The minimal collection API consists of:

* `map`: Pure elementwise transformation, preserving array shape.
* `filter`: Pure selection, initially 1D, preserving relative element order.
* `generate`: Pure construction from indices and a statically ranked shape.

```sirius
let doubled = values.map(fn(x) x * 2)
let positive = values.filter(fn(x) x > 0)
let grid = generate((100, 100), fn(i, j) i*i + j*j)
```

Callbacks may execute in any order, sequentially, vectorized, fused, or in parallel. Results must preserve specified shape and ordering semantics.

Callbacks must be pure:

* Local variables and local mutation are permitted.
* Inputs and explicit context are immutable.
* I/O and externally observable mutation are forbidden.
* Purity is checked transitively, including calls through function values.
* Panics are permitted; when multiple elements fail, which failure is reported is unspecified.

### Lambdas and context

Functions and non-capturing lambdas are first-class values with concrete signatures. Compatible non-capturing functions can be stored as typed function pointers. Qualified references to exported functions and their named re-exports use the same function types as local functions.

**Current implementation:** `Fn[parameters...,return]` values support indirect calls, named functions and expression-bodied noncapturing lambdas (`fn(x: i64) x + 1`). Lambda parameters and unannotated generic named functions can be specialized from a contextual `Fn` type in bindings, arguments, returns, assignments and typed arrays; otherwise annotate the parameters explicitly. Qualified imports, named re-exports and source-free `.smirg` generic declarations support contextual specialization; some cross-module lambda arguments may still require explicit annotations. No captures of enclosing local variables or transitive purity checking are implemented. `Std.Functional` has one generic `fold(values, initial, callback)` which infers the accumulator/result type per specialization, including cross-module and source-free `.smirg` cases. `count_if`, `any`, `all`, and `find_index` also specialize on input types. `map`, `filter` and `generate` now return generically typed arrays, including empty arrays, using `empty_array_like(array_or_callback)` to infer an element type from an array or a concrete callback result. `enumerate` now returns generic structural indexed records for any array element type, including managed, nested and empty arrays; legacy named record types remain available for compatibility. Generic helper calls can infer the input types of unannotated lambdas and named generic callbacks from concrete arguments and how their callee uses them (such as array elements, indices or accumulators). Independently typed arguments may precede or follow the callback, including nested generic array results; type-only lookahead does not change runtime argument evaluation order. A bounded multi-stage pass also infers callback signatures that depend on another callback's result, provided an independently typed argument seeds the chain. Callback forwarding through local generic functions can infer parameter types from independently typed arguments preceding or following the forwarded callback, including through multiple forwarding layers and branch bodies. The probe also follows callback aliases in local bindings, nested record-field paths and straight-line mutable assignments; overwritten aliases are invalidated. Conditional assignments retain callback aliases only if every branch preserves them; an `if` without `else` includes the unchanged path, and loop joins account for zero iterations. Callback requirements found in separate branches must agree. Exhaustive `match` arms merge only aliases from continuing paths, with pattern-bound fields available to local callback constraints; `return`, `recur`, `break`, `continue` and guard exits stop or bypass alias propagation as appropriate. Nested record copies, projections and assignments preserve tracked callback paths; assigning to an enclosing field invalidates its descendants. Inference remains conservative around underdetermined cross-module pattern-bound callback types. Direct calls on record function fields are not yet lowered to MIR (extract the field to a local first). This is conservative, not full higher-order constraint solving: an explicit `Fn` binding is still required when no types can seed the dependency chain or a unique signature cannot be determined. The compiler distinguishes conflicting callback signatures, recursive forwarding dependencies without a usable type seed, and underdetermined callbacks; source-free generic imports use the same diagnostics.

**Capturing closures are not supported.** Higher-order functions accept optional explicit context arguments.

```sirius
let ctx = {scale = 3, offset = 2}
let result = values.map(fn(x, ctx) x * ctx.scale + ctx.offset, ctx)
```

Module constants may be referenced without captures. Bound receiver objects are not implicit.

### Indexing

```sirius
a[i]       # Statically proven in bounds
a[?i]      # Optional[T]; returns None if out of bounds
a[!i]      # Runtime-checked; panics if out of bounds
optional!   # Unbox Optional[T]; panics if absent
```

Only straightforward static bounds checks are required. Multidimensional indexing uses `a[i, j]`; partial indexing produces lower-rank arrays.

```sirius
let value = a[?i] or 0
let value = a[?i] else return false
```

`or` on `Optional` selects a fallback on absence, independent of truthiness. Prefix `?` and `!` only modify indexing; postfix `!` unboxes any `Optional`. Indexed assignment uses `a[!i] = value`.

### Guards

An expression guard requires a scalar `bool`. Execution continues when it is `true`; otherwise the `else` branch terminates the current path with `return`, `break`, or `continue`:

```sirius
value >= 0 else return -1
found else continue
ready else break
```

The condition is evaluated once, and the exit runs only when it is false. This is a statement, not a boolean-valued expression. Optional extraction with `let value = optional else return fallback` remains a separate, value-binding form.

## 8. Sequential Iteration and Recursion

Conventional loops are the primary mechanism for sequential algorithms, aggregation, and effects.

```sirius
var total = 0
for x in values
	total += x
```

More elaborate functional utilities (`fold`, `reduce`, `scan`, `iterate`, `each`, etc.) can be implemented as ordinary library functions using loops.

Ordinary recursion is supported. Direct self-tail recursion uses `recur`, guaranteed to lower to a loop.

```sirius
def factorial(n, acc = 1)
	if n <= 1
		return acc
	recur factorial(n - 1, acc * n)
```

## 9. Strings

Strings are immutable UTF-8 values with CoW storage.

The standard library provides concatenation, comparison, searching, slicing, splitting, joining, replacement, and explicit formatting.

`byte_at(i)` returns `Optional[i64]` containing the unsigned UTF-8 byte at zero-based index `i` (`None` when out of bounds). Byte-oriented operations are distinct from Unicode-aware operations. Efficient string construction may use a standard-library builder backed by private mutable storage.

## 10. Memory and Runtime

* Acyclic managed values with non-atomic reference counting, initially single-threaded.
* CoW, uniqueness checks, structural sharing, and compiler-managed buffer reuse.
* Private mutable buffers for immutable-value construction.
* Canonical type descriptors and optional boxing for `Any`.
* Opaque handles for external resources.
* `Optional[T]` for absence; panics for violated assertions.
* Sequential execution of effectful operations.
* Purity/effect analysis for functions and functional callbacks.
* Explicit FFI access to native pointers, layouts, and ownership.
* A shared Sirius value representation and runtime ABI for native and interpreted execution; foreign C ABIs are separate.

### Array execution

Numerical operations use compiler/runtime primitives rather than dynamic NumPy-like dispatch.

* Specialized kernels by element type and rank.
* Contiguous loops and broadcasting through zero strides.
* Runtime shape validation.
* Safe CoW reuse, including overlapping inputs.
* Pure elementwise expressions eligible for fusion and parallel execution.
* Initial SIMD through C compiler autovectorization.

Fusion, scheduling, and reuse are optimizations, not observable semantics.

## 11. Modules

```sirius
import Math.Stats alias Stats

export let DEFAULT_LIMIT = 100

export def process(data)
	return Stats.mean(data)
```

**Target module semantics (not yet fully implemented):**

* One module per source file, named from its path relative to a source root (`Math/Stats.sirius` → `Math.Stats`). Case-sensitive names must resolve unambiguously; optional `namespace` declarations are deferred.
* `import Math.Stats alias Stats` provides qualified access. Declarations are private by default; `export` makes them public, and `export Alias.member alias Name` re-exports a public member under a chosen name (omitting `alias Name` retains the original name). Namespace-wide re-exports are deferred. No wildcard imports or runtime module objects.
* Imports are initially acyclic, resolved independently of statement order. Structural records remain compatible across modules; generic specialization uses the defining declaration and canonical concrete types.
* Module-level `let` bindings are evaluated at **compile time**, potentially with side effects, but do not cause runtime module initialization.

See [MIR_MODULES_AND_FFI.md](docs/MIR_MODULES_AND_FFI.md) for the complete module, library, specialization, and interoperability design. The current compiler supports typed concrete function imports, structural record signatures, exported constructor bodies and compile-time constants, and source-available cross-module generic specialization with explicit concrete return annotations, and source-independent generic specialization from portable `.smirg` HIR
packages (not generic executable MIR libraries); see [README.md](README.md) for status.

## 12. Compiler

Self-hosted compiler targeting C11, initially bootstrapped using **Python** and now maintained from committed generated C. Zig's C toolchain is optional.

### Bootstrap strategy

1. Define a minimal language subset sufficient to implement a compiler.
2. Implement a small Python compiler for this subset.
3. Implement a Sirius compiler in that subset; compile it using Python and establish self-hosting.
4. Commit generated C bootstrap sources and the minimal native runtime.
5. Extend the self-hosted compiler iteratively, regenerating bootstrap C as needed.

The Python bootstrap is then frozen; subsequent development uses Sirius.

### Initial language subset

* Types: Scalars, strings, records, tagged unions, `Optional`, basic generics.
* Control flow: `let`, `var`, conditionals, loops, recursion, returns, basic pattern matching.
* Functions: Named functions, explicit signatures, non-capturing function values.
* Collections: Dynamic 1D arrays, indexed access, hash maps (`Dict`).
* Memory: Reference counting, immutable values, basic CoW.
* Runtime: File I/O, diagnostics, minimal standard library.
* Typing: Explicit annotations where needed; basic structural compatibility.

**Implementation note:** The compiler-authoring subset needs generic 1D arrays and dictionaries, practical string/byte APIs, and command-line/file I/O before Sirius can conveniently host its own lexer, parser, and symbol tables. These are bootstrap subset improvements, not requirements to implement the full language first.

### MIR execution modes (design target)

Typed MIR is intended to share Sirius value semantics across native C11 output, unrestricted compile-time interpretation, saved-MIR runtime interpretation, and eventual mixed native/interpreted calls. Compile-time effects (including I/O and FFI when available on the host) are permitted; no mandatory purity sandbox or execution budget is planned. The syntax and staging of explicit compile-time execution remain unresolved.

Portable library metadata must expose exported type/signature identities and generic MIR for downstream specialization; compiled specializations may be registered through a native hook. Sirius native/interpreter ABI compatibility does **not** imply C FFI layout compatibility. See [MIR_MODULES_AND_FFI.md](docs/MIR_MODULES_AND_FFI.md) for artifacts, call adapters, host/target distinctions, and proposed implementation stages.

## 13. Open Semantic Decisions

These remain **unresolved**, not implied guarantees:

* Numeric overflow, division/remainder, conversion, and floating-point corner cases.
* Structural equality and hashing for all types (especially floats and arrays with elementwise `==`).
* Whether future nominal variant identity should distinguish union alternatives with identical structural record shapes (currently indistinguishable).
* Imported-operator overload visibility and explicit compile-time evaluation syntax/staging.

Portable MIR formats, registration/dispatch ABIs, and C interoperability are separate open architectural questions in [MIR_MODULES_AND_FFI.md](docs/MIR_MODULES_AND_FFI.md).

## 14. Future Improvements

### High priority

* Array fusion, SIMD lowering, and parallel transformations.
* Advanced functional-but-in-place (FBIP) reuse.
* ARC and ownership optimization.
* More precise purity/effect analysis.
* Structured concurrency: `spawn`, `sync`, channels, cancellation.
* Atomic RC for shared allocations and an M:N scheduler.
* Advanced pattern matching and richer algebraic data types.

### Medium priority

* Strided views, slicing, and advanced indexing.
* **Python-style negative array indexing** (`a[?-1]` accesses the final element), including checked reads and CoW writes.
* Axis-specific reductions and richer linear algebra.
* Richer reflection and serialization.
* String builders and advanced Unicode operations.
* Advanced static bounds analysis.
* Mutual tail recursion and code deduplication.
* Direct LLVM backend if beneficial.

### Low priority / potentially unnecessary

* Capturing closures.
* Full row-polymorphic constraint solver.
* Conditional constructor fields.
* General automatic tail-call optimization.
* Runtime native code generation / JIT (runtime MIR specialization and interpretation are part of the execution design).
* Mutable shared objects.
* General exception unwinding.
* Generalized NumPy ufunc dispatch.
* User-level SIMD intrinsics.
* GPU kernel compilation.
* Arbitrary union types.

**Language influences:** Python (syntax), NumPy (arrays), Codon (compiled numerics), Roc (immutable values and CoW), Koka (FBIP and ARC), Futhark (functional arrays), Zig (compiler implementation).

**Guiding principle:** One immutable value model, complementary functional and imperative constructs, and compiler optimizations that preserve observable semantics.
