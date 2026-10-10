> **Implementation status (2026-10-09):** `bootstrap_python/syntax.py` implements a syntax AST and `bootstrap_python/codegen.py` currently performs type resolution, specialization, managed lifetime tracking, and direct C emission together. The typed HIR/MIR dataclasses in `target_ir.py` are a **target architecture**, not the representation currently emitted or consumed by the running compiler. The updated roadmap prioritizes making the Python subset sufficient for compiler authoring, then writing a small self-hosting compiler in Sirius. Typed HIR and CFG MIR should be introduced in that Sirius compiler, instead of implementing the full pipeline twice; retain the 24 independent C reference cases and the additional compiler-authoring tests throughout. As of this revision generic typed arrays/dictionaries, string inspection, CLI arguments and ownership callbacks are implemented in the direct Python-to-C backend; a typed HIR/CFG MIR is still deferred until Sirius self-hosting.

# Inferred IR for the Sirius bootstrap compiler

Use **tokens -> AST -> typed/specialized HIR -> explicit-control-flow MIR -> C**.
`target_ir.py` defines proposed Python `dataclass` representations for HIR and MIR. It is a structural design, not a working compiler.

## Syntax AST (implemented in Python; to reimplement and extend in Sirius)

Keep syntax faithfully: indentation-sensitive blocks, declarations (`let`, `var`), constructors (`pub`), record spreads/deletions, loops, `match`, indexing modes, and source spans. AST expressions do **not** generate C or infer their own types. Maintain a `SourceSpan` for every parsed declaration, expression, and statement.

Implement a real expression precedence hierarchy; `and`/`or` must short-circuit, and binary operators must respect precedence and associativity.

## Typed HIR

* Each expression has an interned `TypeId`, source `Span`, and resolved symbol/function identity.
* Canonical record types intern by lexicographically sorted field names/types, not constructor names. Different shapes use distinct `TypeId`s. Their layout is computed deterministically (alignment/offsets).
* Generic calls are specialized by `(definition SymbolId, concrete argument TypeIds)`; resolved function calls refer to the specialization. The return type and effect signature are attached to the specialization. Avoid recursively instantiating the same in-progress specialization.
* A constructor becomes an ordinary function producing an immutable record. Validate unconditional field publication, freeze published bindings, and statically resolve field layouts.
* Extension method calls become ordinary calls with the receiver inserted as the first argument.
* Operator overloads become statically resolved function calls (or known primitive operations).
* Indexing is explicitly `PROVEN`, `OPTIONAL`, or `PANIC` at the HIR level. The type of an `OPTIONAL` index is `Optional[element_type]`.
* Structural-field lookups are replaced with integer field indices. Dynamic `Any` is not part of the first bootstrap.
* Function values have a typed signature and effect guarantees. Noncapturing lambdas become named, generated functions.
* Purity checking uses a transitive call graph; panics are permitted in otherwise pure functions, whereas I/O and impure FFI are effects.

## Lowered MIR (CFG)

A function is `FunctionMIR(locals, arguments, blocks, entry)`.
Each basic block holds ordered instructions and one terminator (`Jump`, `Branch`, `Switch`, `ReturnTerm`, or `PanicTerm`).

* Expressions become typed temporaries, making evaluation order explicit before the C backend sees them.
* `if`, `elif`, `and`, `or`, `while`, `for`, and `Optional` fallback become CFG branches. In particular, **`and`/`or` are not eager arithmetic instructions**.
* `for i in a..b` lowers to integer bounds and a loop (confirmed half-open).
* `recur f(a, b)` evaluates **all** new arguments first and then jumps to the function's entry block. Direct self-tail recurrence never creates another C call frame.
* `let Result = Success | Failure` defines a compile-time descriptor alias for a union of constructor return types. Resolve `|` over constructor descriptors in a compile-time type expression, not as the ordinary bitwise operator over function values. `Success` and `Failure` are ordinary record constructors. In a union-expected context, an ordinary record result is implicitly injected into its union alternative based on its canonical structural type.
* Constructor patterns such as `Success(value)` resolve statically to their record payload shape, bind fields, and become `Switch` over a union discriminant. Check exhaustiveness. The current subset requires structurally distinct alternative record types; identical payload shapes would have identical type IDs and cannot be distinguished by origin alone. Nominal constructor tags remain a possible later extension.
* Union type descriptors intern by an order-independent canonical set of alternative type IDs, with deterministic tag numbering; constructor names are used for source-level pattern matching rather than as record type identities.
* Array access becomes a bounds proof/optional result/checked panic path, depending on syntax.
* **Deferred (medium priority):** signed negative array indices normalize relative to array length before the bounds check (`-1` is last element). This applies to reads and CoW writes; optional/checked/proven access retain their current semantics. See `tests/deferred/25_negative_indexing.sirius`.
* Record spread, deletions and overrides are resolved to a new record layout; preserve left-to-right **expression evaluation** even if a field is later overwritten.

## Memory management in MIR

Value semantics have to be visible in the compiler, not reconstructed from textual C:

* `Retain` and `Release` track managed storage; values embedded in records/variants require recursively generated retain/drop operations.
* A plain copy of an inline record copies scalars and retains managed fields. Records may therefore share nested arrays safely.
* `CoWSet(root, path, value)` resolves nested field/index updates and assures unique ownership of any modified buffer. Reuse uniquely owned allocation; otherwise copy the path (with reference management).
* Function inputs are *logically immutable*. For a nonescaping read-only parameter C may safely borrow, but retained/returned/stored values must have well-defined ownership credits.
* Function returns transfer ownership to the caller. All control-flow exits must release live owned temporaries; the ARC pass can insert necessary releases after CFG creation.
* In the first version, insert conservative retains and releases; ownership/escape/FBIP optimization can come later.

## Worked lowering examples

### `08_array_cow`

Sirius:

```sirius
var a = [10, 20, 30]
let original = a
a[1?]! = 99
```

Illustrative MIR (notation rather than exact Python serialization):

```text
a        = array_make_i64(10, 20, 30)
original = alias(a)
retain(a)                    ; one ownership credit for original
index    = const_i64(1)
value    = const_i64(99)
cow_set(a, index[PANIC], value)
...                          ; read a and original separately
release(original)
release(a)
```

### `04_recursion`

```sirius
recur factorial(n - 1, acc * n)
```

```text
next_n   = sub_i64(n, 1)
next_acc = mul_i64(acc, n)
n        = next_n
acc      = next_acc
jump factorial_entry
```

### `16_tagged_union`

```sirius
def Success(value: i64)
    pub value

def Failure(code: i64)
    pub code

let Result = Success | Failure
```

`Success(7)` first produces a record `{value: i64}`. When passed to a parameter of type `Result`, HIR inserts `UnionInject(Result, SuccessTag, record)`; in C this is a tagged union with a record payload. A match checks the tag and extracts the corresponding record before binding its fields.

### `13_optional_early_exit`

```sirius
let x = values[index?] else return false
```

```text
optional = array_get_optional(values, index)
branch(optional.present, has_value, missing)
missing: return false
has_value: x = optional.value
```

## Why avoid emitting C while parsing?

The uploaded GDScript `Parser.py` uses generator-based expression type inference and writes via `self.out` during parsing. That was workable for a transpiler, but Sirius requires global decisions about specialization, purity, shared ownership, field layout, and CFG exits. Mixing these with token consumption would make correct CoW and lifetime insertion unnecessarily hard.

The existing SLY-based `Tokenizer.py` is a useful reference, especially newline/tab handling. It should not be reused unchanged: Sirius needs `..`, `?`, `!`, `pub`, `recur`, explicit operator precedence, and properly distinguished INDENT/DEDENT tokens. This also provides clean, localized syntax diagnostics.
