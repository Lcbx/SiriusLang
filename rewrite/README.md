# Compiler rewrite

This directory contains an incomplete compiler implemented in Sirius. The
feature inventory in [FEATURES.md](FEATURES.md) describes the existing
compiler, not the rewrite. The existing bootstrap does not use this directory.

The first component is a lexer with compact integer kinds derived from
`TokenKind.index(label)`. These indices are internal to the rewrite, not
compatible with the existing compiler's serialized codes. Token positions are
byte offsets. Test with `python rewrite/test_rewrite_lexer.py`.
For paired native lexer timings run `python tools/benchmark_rewrite_lexer.py`;
this does not measure end-to-end compilation.

The current compiler requires one entry `def main() -> i64`. Function bodies accept integer
expressions (`+`, `-`, `*`, `/`, `%`, `&`, `|`, `<<`, `>>`, unary `-`, and parentheses), `let` and `var`
bindings, assignments (`=`, `+=`, `-=`, `*=`, `/=`), integer `print`,
`if`/`elif`/`else`, `while`, early `return`,
`break`, `continue`, `condition else return value` (also `break`/`continue`),
and `for i in start..end`. Guards evaluate the condition once and evaluate the
exit only when it is false. They reuse ordinary branch and exit nodes.

The compiler supports multiple functions with annotated parameters
and results (`i64`, `bool`, `string`, `Optional[T]` for executable payload types, record types, and
recursively composed `Array[T]` annotations over supported element types). It collects all signatures
before checking bodies, so direct calls, forward calls and mutual recursion work
natively. Parameters are immutable locals; functions have
separate scopes and typed slot storage. Checked calls retain resolved function
IDs and result types. The verifier checks call signatures, returns and definite
initialization in every function, including unused helpers. Arguments evaluate
once in source order. C declarations precede definitions; string arguments are
borrowed, parameter slots retain their own references, and returned strings own
a reference retained before local cleanup. Call results transfer that ownership
to the caller. Duplicate declarations, invalid arguments and mismatched
returns fail during checking.
`main` must remain `main() -> i64`; definitions of the builtins `len`, `str`,
`print` and `eprint` are reserved. Calls are currently expressions; standalone
call statements are not yet supported. Checked IR carries a shared `IR.TypeTable`:
type IDs index canonical descriptors, and an `Optional` descriptor contains its
payload type ID. Interning reuses equivalent types, including nested optionals;
there is no separate `OptionalI64` kind or optional bit in a type ID. Verification
checks descriptor/cache consistency and rejects invalid or cyclic payload links.
The compiler consults descriptors to select type-specific optional storage.
Executable optionals can contain scalars, strings, records, arrays, dictionaries
and nested optionals. Present optional values own their payload: copying, calls,
returns, unwrapping and defaults retain managed contents, while overwrites and
returns release them. Explicit optional constructors use `T?(value)` for a present
`Optional[T]` and `T?()` for an absent one, where `T` is a supported type
expression: `i64?(5)`, `Binding?()`, `Array[string]?(names)` or
`Optional[i64]?(i64?())` for `Some(None)`. They evaluate the payload exactly
once, require its type to match `T`, and preserve ownership across calls and
aggregate updates. `Optional[T]` remains the type annotation syntax; `T?`
without a constructor call is not yet valid in annotations. `items[?index]` and `dict[?key]`
return `Optional[T]` without trapping; `items[!index]` and `dict[!key]`
remain checked and trap when missing. Optional values can also be stored
inside records, arrays and dictionaries, including as nested optionals. Record
descriptors contain canonical fields; `Array[i64]` and `Array[string]`
have canonical element descriptors. Dictionary descriptors are canonical for `i64` or `string` keys and any supported acyclic value type.
Run `python rewrite/test_rewrite_frontend.py` and
`python rewrite/test_rewrite_functions.py` from the repository root for checking,
native execution, string snapshots and ARC coverage.
`python rewrite/test_rewrite_types.py` covers canonicalization, malformed tables,
type-table snapshots and optional IDs that differ from the default table,
including native execution, C emission and ARC auditing.

`rec` declarations support `i64`, `bool`, `string`, previously declared
record types, and recursively composed arrays of these types as fields, positional
construction, field reads, local bindings/assignment, and function parameters
and results. Declarations may appear after their uses. Constructors evaluate
arguments in declaration order; canonical layouts sort fields by name. Different
record names and declaration orders share a type when their field names and types
match. Concrete annotations require matching field sets, as in the original
compiler. A `var` record binding supports direct field assignment (`p.field = x`)
and compound field updates (`+=`, `-=`, `*=`, `/=`) for the existing field
types. The root must be a mutable local, not a function parameter or `let`.
Field/index paths such as `outer.inner.text`, `outer.inner.values[!i]` and
`records[!i].inner.text` update copied intermediate values and write them back
to the root. Index expressions are evaluated once; compound assignments read
the selected element before evaluating the right-hand side. Fields are updated in the
binding's owned record slot, with retain/release for replaced strings, arrays, and
nested record values. Other copies retain their previous values. Nested
records, strings and arrays preserve snapshots across copies, calls,
reassignment, loops and early returns. C emission uses typed structs and the
existing string/array retain/release operations. IR verification checks constructors, field indices,
copy and update types and initialization. Nested records must refer to earlier
declarations (cycles and forward references are not supported).
Recursive or forward record references, record literals, constructor bodies,
record equality and direct record formatting remain unimplemented.
`python rewrite/test_rewrite_records.py` covers native behavior,
canonical aliases, constructor order, invalid records and managed-field ARC.

`Array[T]` supports arbitrary acyclic nesting where `T` is `i64`,
`bool`, `string`, a previously declared record, or another supported array
type (for example, `Array[Array[Token]]`). Nonempty
literals (`[1, 2]`), `Array[i64]()` for empty arrays, annotations, function
parameters and results, `len(array)`, `.append(value)`, checked `[!index]`
reads, `for item in array`, and mutable local indexed updates (`a[!i] = value`,
`a[!i] += value`, etc.) work through the native backend. Negative indices count
from the end for both checked `[!index]` and optional `[?index]` reads.
Out-of-range optional reads return `None` while checked reads and writes trap. Array iteration snapshots
its receiver once and binds an immutable element on each iteration. Copies and
function parameters preserve prior values through reference counting and CoW.
`Array[string]` supports inferred nonempty literals (`["a", "b"]`),
`Array[string]()` empty construction, annotations, function calls, iteration,
checked reads and value-preserving append/set operations. Generated C uses the
existing generic `S_Array` storage with copy/drop callbacks derived from the
canonical element descriptor; `Array[i64]` keeps its specialized `S_Array_i64`
representation. C emission derives managed element copy/drop and reads
from the canonical element descriptor instead of enumerating nesting depths.
Arrays of records use `S_Array` with layout-specific copy/drop callbacks. They support
records containing `i64`, `bool`, `string`, primitive-element array fields, and
record-valued fields, positional literals, `Array[RecordName]()` construction, snapshots, append, checked indexing,
assignment of whole elements, iteration, parameters and results. Record-array
reads return owned record values; CoW updates do not mutate other copies.
Mutable arrays also support `tokens[!index].field = value` and compound
field updates on record elements. The index is evaluated once; the element
is read as a value, updated, and written back through the array CoW path.
Generated C retains and releases array values across copies,
function calls and returns.

`Dict[K,V]` supports `i64` and `string` keys, recursively composed record,
array and dictionary values, empty `Dict[K,V]()` construction, `.insert(key,
value)` returning a new dictionary, `len(dict)` and checked `dict[!key]` reads.
Checked reads trap on missing keys; `.get(key)` and `[?key]` instead return `Optional[V]`
for any supported dictionary value type, including managed records and nested
aggregates. Generic native dictionary storage retains/drops managed keys and
values and uses CoW when an earlier snapshot exists. Dictionaries can be passed
to functions and embedded in records or arrays. Dictionary iteration and
literals are not yet implemented. Run `python rewrite/test_rewrite_dictionaries.py`
and `python rewrite/test_rewrite_optionals.py` for ARC, snapshots, missing-key
behavior, invalid types and managed defaults.

The empty literal `[]` is not yet accepted; use `Array[T]()` at any
supported depth. Array-valued fields in records can be constructed, read,
replaced and recursively updated, including paths such as
`state.nodes[!i].right = target` and `forest.trees[!i][!j].text = value`.
Generated C reuses generic array copy/drop callbacks. Direct array formatting and general
array methods remain unsupported. Run `python rewrite/test_rewrite_nested_text_arrays.py` for two-level managed string array ARC and CoW coverage. Run `python rewrite/test_rewrite_arrays.py` for integer-array regressions
and direct array IR verification, and `python rewrite/test_rewrite_managed_arrays.py`
for string-element native execution, ARC auditing, invalid program
rejection and bounds traps. `python rewrite/test_rewrite_record_arrays.py`
exercises record elements, including managed fields, CoW, invalid operands,
function calls, loops and bounds traps.
`python rewrite/test_rewrite_nested_updates.py` covers indexed record-field
updates, compound operators, snapshots, invalid targets, and bounds traps.
`python rewrite/test_rewrite_recursive_updates.py` covers arbitrary paths through
supported record and array fields, including ARC, invalid types and bounds.
`python rewrite/test_rewrite_nested_arrays.py` covers two-level integer arrays, nested CoW updates, record fields, functions, invalid programs, bounds and ARC.
`python rewrite/test_rewrite_arbitrary_nesting.py` covers deeper arrays,
boolean elements, arrays of records, recursive assignment, invalid types,
checked bounds and audited native ownership.
`python rewrite/test_rewrite_record_array_fields.py` covers array-of-record
fields in nested records and arrays, CoW updates, invalid programs, and ARC.
`python rewrite/test_rewrite_composed_records.py` covers array-valued record
fields, records in arrays, calls, aliases, CoW, ARC, invalid types, and bounds traps.
`python rewrite/test_rewrite_nested_records.py` checks nested record construction,
field reads/replacement, copied snapshots, function frames and bounds traps.
The `test_rewrite_arrays.py` suite preserves integer-array regressions, while
`array_ir_test_driver.sirius` verifies integer, string, nested integer and record-array IR.

Ranges are ascending and
half-open and bind an immutable local `i`. Bound expressions are evaluated
before iteration; for compatibility with the existing compiler, a bare mutable
upper-bound name remains live (use `end + 0` to snapshot its value).
`continue` advances the range counter before retesting. Every reachable exit
path must return a value matching the function's declared result.
String literals, `string` annotations, bindings, assignment, `+`/`+=`,
comparisons, `len(string)` (UTF-8 byte length), and printing are supported.
`str` formats integers (including minimum `i64`), booleans (`true`/`false`),
strings (unchanged), and `Optional[i64]` (`Some(value)`/`None`).
`eprint` writes integers, booleans and strings to stderr
with a newline, using the same ownership rules as `print`.
Both lower to `Output(input, type, stderr)`; the C emitter chooses the specialized
runtime function.
Single/double quotes and escapes `\n`, `\t`, `\r`, `\\`, `\"`, `\'`, `\0`
work; embedded NUL bytes retain their length. Native string slots own runtime
references, release overwritten values, and are cleaned up on every return.
IR verification rejects mixing string and scalar storage. String `.slice(start,
end)`, `.starts_with(prefix)` and `.ends_with(suffix)` support chaining on any
string expression. Slice offsets are bytes and must satisfy
`0 <= start <= end <= len(text)`; empty slices and empty prefixes/suffixes work.
String `.byte_at(index)` and `.find(needle)` return `Optional[i64]`; inferred
or annotated optional bindings support copying and assignment. `optional or fallback`
evaluates its fallback only on absence, including when a present payload is zero
or false; fallback must have the optional payload's type.
Byte indices outside the string return absence; search returns the first byte
offset, with zero for an empty needle. Postfix `optional!` unwraps or traps on
absence. `let value = optional else return expression` (also `var`) unwraps
on success and evaluates the return expression only on absence. The new binding
is visible after the statement; its annotation describes the unwrapped payload type.
`f"...${expression}..."` and `f'...${expression}...'` evaluate embedded
expressions once from left to right and insert their `str` representation.
Nested interpolation, quoted strings and ordinary expressions work inside `${}`;
`\${` escapes the marker. Other braces/dollars and ordinary strings stay literal.
Expressions retain the surrounding scope and original source positions. Strings
remain single-line, without inline format specifiers. Interpolation lowers to
formatting and ordinary string concatenation, with the same ownership rules.
Optional comparison and direct optional printing remain unsupported.
`str(Optional[T])` is currently available only for `T = i64`.
Scalar-only C
programs need no string slots.
Boolean literals, bindings, `not`, printing, and scalar comparisons can
supply conditions. Nested blocks have local scopes.
Division truncates toward zero; remainder has the numerator's sign. Both trap
on a zero divisor or minimum `i64` divided by `-1`, including remainder.
Shifts require counts from 0 through 63; left shifts wrap and right shifts
zero-fill, including for negative integers. Generated C uses unsigned shifts.
Compound assignments to local names reuse variable reads, arithmetic and
ordinary assignment; mutability checks and arithmetic traps apply unchanged.
`%=` and bitwise compound assignments remain unsupported.
Field/index paths through supported types accept `=`, `+=`,
`-=`, `*=`, and `/=`.
Integer literals must fit in the positive `i64` range; in particular, a literal spelling of the
minimum `i64` is not yet supported. The compiler checks names, duplicate
bindings and mutability. It lowers the flat syntax arena to a scalar IR
used by C generation (`rewrite input.sirius output.c`).
AST nodes store `NodeKind` enum values, with exhaustive matches in semantic
analysis and lowering. IR instructions are labeled unions with exhaustive
matches in C emission.
The emitter verifies slot bounds, jump targets, initialization on every
incoming path and return placement before execution or emission. Local names
are resolved through scoped dictionaries. Straight-line IR uses a linear
verification pass; control flow uses a worklist and initialization facts at
each instruction. Facts are bitsets with one `i64` word per 64 slots; joins
intersect words. CFG storage still grows with instructions times slot words.
Timing measurements are deferred until the rewrite is complete.
C output is assembled as fragments and joined once. Lowering removes unreachable
block markers after exits and remaps jumps before verification. Typed HIR,
block-based MIR, other types and self-hosting remain to be built.
Boolean `and`/`or` short-circuit, with `and` binding more tightly than `or`.
The flat expression arena stores a logical start before the right operand and
a final copy after it; one conditional-copy IR instruction skips that operand.
Both operands are type-checked even when the left value decides the result.
`elif` chains reuse nested branch markers and are parsed iteratively.
First-class range values are not yet supported. Range
loops lower to scalar copies, comparisons, increments and existing branches;
they do not allocate an array of elements.
It does not yet implement the existing compiler's artifact formats or CLI.

`python rewrite/test_rewrite_compiler.py` builds the rewrite natively,
checks invalid inputs and generated C, and compares supported programs
with the existing native compiler.
Run the rewrite tests with `python tools/test_all.py --group rewrite`.
`python tools/benchmark_rewrite_compiler.py` measures a small supported program;
its results do not represent full compiler performance.
Use `--bindings 1000` to exercise many distinct local names. On the measured
workload, replacing linear name scans with dictionary lookup reduced median
rewrite compilation time from 0.065 s to 0.057 s (five runs each). This includes
the entire current compilation slice and process startup, not just lookup.
After adding branches and verification, this workload measured 0.063 s with
the linear verification path (0.274 s using the general CFG analysis).
