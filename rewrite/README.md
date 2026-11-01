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
and results (`i64`, `bool`, `string`, `Optional[i64]`, `Array[i64]`, `Array[string]`,
and flat record types, including `Array[RecordName]`). It collects all signatures
before checking bodies, so direct calls, forward calls and mutual recursion work
natively and through `--run`. Parameters are immutable locals; functions have
separate scopes and typed slot storage. Checked calls retain resolved function
IDs and result types. The verifier checks call signatures, returns and definite
initialization in every function, including unused helpers. Arguments evaluate
once in source order. C declarations precede definitions; string arguments are
borrowed, parameter slots retain their own references, and returned strings own
a reference retained before local cleanup. Call results transfer that ownership
to the caller. The interpreter saves explicit frames, rather than recursing on
its host call stack. Duplicate declarations, invalid arguments and mismatched
returns fail during checking.
`main` must remain `main() -> i64`; definitions of the builtins `len`, `str`,
`print` and `eprint` are reserved. Calls are currently expressions; standalone
call statements are not yet supported. Checked IR carries a shared `IR.TypeTable`:
type IDs index canonical descriptors, and an `Optional` descriptor contains its
payload type ID. Interning reuses equivalent types, including nested optionals;
there is no separate `OptionalI64` kind or optional bit in a type ID. Verification
checks descriptor/cache consistency and rejects invalid or cyclic payload links.
Both backends consult descriptors to select optional storage. The table can
represent optional booleans, strings and nested optionals, but their source
syntax, operations and runtime layouts remain unimplemented; executable optional
values still require `i64` payloads. Record descriptors contain canonical fields; `Array[i64]` and `Array[string]`
have canonical element descriptors. Dictionary descriptors remain future work.
Run `python rewrite/test_rewrite_frontend.py` and
`python rewrite/test_rewrite_functions.py` from the repository root for checking,
native/interpreted execution, string snapshots and ARC coverage.
`python rewrite/test_rewrite_types.py` covers canonicalization, malformed tables,
type-table snapshots and optional IDs that differ from the default table,
including native/interpreted execution, C emission and ARC auditing.

`rec` declarations support `i64`, `bool`, `string`, `Array[i64]`,
`Array[string]` and previously declared record types as fields, positional
construction, field reads, local bindings/assignment, and function parameters
and results. Declarations may appear after their uses. Constructors evaluate
arguments in declaration order; canonical layouts sort fields by name. Different
record names and declaration orders share a type when their field names and types
match. Concrete annotations require matching field sets, as in the original
compiler. A `var` record binding supports direct field assignment (`p.field = x`)
and compound field updates (`+=`, `-=`, `*=`, `/=`) for the existing field
types. The base must be a mutable local, not a function parameter or `let`;
deeper field/index paths are not yet supported. Fields are updated in the
binding's owned record slot, with retain/release for replaced strings, arrays, and
nested record values. Other copies retain their previous values. Nested
records, strings and arrays preserve snapshots across copies, calls,
reassignment, loops and early returns. C emission uses typed structs and the
existing string/array retain/release operations; the interpreter stores scalar,
string and array fields separately. IR verification checks constructors, field indices,
copy and update types and initialization. Nested records must refer to earlier
declarations (cycles and forward references are not supported); the interpreter
uses an append-only pool of immutable nested values shared across call frames.
Nested array fields, record literals, constructor bodies, record equality
and direct record formatting remain unimplemented.
`python rewrite/test_rewrite_records.py` covers native/MIR parity,
canonical aliases, constructor order, invalid records and managed-field ARC.

`Array[i64]`, `Array[string]`, and arrays of records (`Array[Token]`)
are supported by the rewrite. Nonempty
literals (`[1, 2]`), `Array[i64]()` for empty arrays, annotations, function
parameters and results, `len(array)`, `.append(value)`, checked `[!index]`
reads, `for item in array`, and mutable local indexed updates (`a[!i] = value`,
`a[!i] += value`, etc.) work through both backends. Negative indices count
from the end, and out-of-range reads or writes trap. Array iteration snapshots
its receiver once and binds an immutable element on each iteration. Copies and
function parameters preserve prior values through reference counting and CoW.
`Array[string]` supports inferred nonempty literals (`["a", "b"]`),
`Array[string]()` empty construction, annotations, function calls, iteration,
checked reads and value-preserving append/set operations. Generated C uses the
existing generic `S_Array` storage with string retain/release element callbacks;
`Array[i64]` keeps its specialized `S_Array_i64` representation. Arrays of
records use `S_Array` with layout-specific copy/drop callbacks. They support
records containing `i64`, `bool`, `string`, primitive-element array fields, and
record-valued fields, positional literals, `Array[RecordName]()` construction, snapshots, append, checked indexing,
assignment of whole elements, iteration, parameters and results. Record-array
reads return owned record values; CoW updates do not mutate other copies.
Mutable arrays also support `tokens[!index].field = value` and compound
field updates on record elements. The index is evaluated once; the element
is read as a value, updated, and written back through the array CoW path.
The interpreter stores record arrays in a separate typed pool. Both backends retain
and release array values across copies, function calls and returns.

The empty literal `[]` is not yet accepted; use `Array[i64]()` or
`Array[string]()` or `Array[RecordName]()`. Arrays in records now support
`Array[i64]` and `Array[string]` fields, including records stored in arrays,
with value-preserving field replacement. Nested arrays, record-array-valued fields,
deeper nested update paths (such as `p.nodes[!id].right = x`), optional index reads,
direct array formatting and general
array methods remain unsupported. Run `python rewrite/test_rewrite_arrays.py` for integer-array regressions
and direct array IR verification, and `python rewrite/test_rewrite_managed_arrays.py`
for string-element interpreter/native parity, ARC auditing, invalid program
rejection and bounds traps. `python rewrite/test_rewrite_record_arrays.py`
exercises record elements, including managed fields, CoW, invalid operands,
function calls, loops and bounds traps.
`python rewrite/test_rewrite_nested_updates.py` covers indexed record-field
updates, compound operators, snapshots, invalid targets, and bounds traps.
`python rewrite/test_rewrite_composed_records.py` covers array-valued record
fields, records in arrays, calls, aliases, CoW, ARC, invalid types, and bounds traps.
`python rewrite/test_rewrite_nested_records.py` checks nested record construction,
field reads/replacement, copied snapshots, function frames and bounds traps.
The `test_rewrite_arrays.py` suite preserves integer-array regressions, while
`array_ir_test_driver.sirius` verifies both integer and managed string-array IR.

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
runtime function, and the interpreter dispatches on the same explicit type.
Single/double quotes and escapes `\n`, `\t`, `\r`, `\\`, `\"`, `\'`, `\0`
work; embedded NUL bytes retain their length. Native string slots own runtime
references, release overwritten values, and are cleaned up on every return.
IR verification rejects mixing string and scalar storage. String `.slice(start,
end)`, `.starts_with(prefix)` and `.ends_with(suffix)` support chaining on any
string expression. Slice offsets are bytes and must satisfy
`0 <= start <= end <= len(text)`; empty slices and empty prefixes/suffixes work.
String `.byte_at(index)` and `.find(needle)` return `Optional[i64]`; inferred
or annotated optional bindings support copying and assignment. `optional or i64`
evaluates its fallback only on absence, including when the present payload is zero.
Byte indices outside the string return absence; search returns the first byte
offset, with zero for an empty needle. Postfix `optional!` unwraps or traps on
absence. `let value = optional else return expression` (also `var`) unwraps
on success and evaluates the return expression only on absence. The new binding
is visible after the statement; its annotation describes the unwrapped `i64`.
`f"...${expression}..."` and `f'...${expression}...'` evaluate embedded
expressions once from left to right and insert their `str` representation.
Nested interpolation, quoted strings and ordinary expressions work inside `${}`;
`\${` escapes the marker. Other braces/dollars and ordinary strings stay literal.
Expressions retain the surrounding scope and original source positions. Strings
remain single-line, without inline format specifiers. Interpolation lowers to
formatting and ordinary string concatenation, with the same ownership rules.
Optional comparison, direct optional printing and other optional payload types
remain unsupported. Scalar-only C
programs need no string slots.
Boolean literals, bindings, `not`, printing, and scalar comparisons can
supply conditions. Nested blocks have local scopes.
Division truncates toward zero; remainder has the numerator's sign. Both trap
on a zero divisor or minimum `i64` divided by `-1`, including remainder.
Shifts require counts from 0 through 63; left shifts wrap and right shifts
zero-fill, including for negative integers. Generated C uses unsigned shifts.
Compound assignments to local names reuse variable reads, arithmetic and
ordinary assignment; mutability checks and arithmetic traps apply unchanged.
`%=`, bitwise compound assignments and deeper nested update paths remain unsupported.
Flat record fields, indexed assignments and record fields inside arrays support `=`, `+=`,
`-=`, `*=`, and `/=`.
Integer literals must fit in the positive `i64` range; in particular, a literal spelling of the
minimum `i64` is not yet supported. The compiler checks names, duplicate
bindings and mutability. It lowers the flat syntax arena to a scalar IR
shared by C generation and an interpreter (`rewrite --run input.sirius`).
AST nodes store `NodeKind` enum values, with exhaustive matches in semantic
analysis and lowering. IR instructions are labeled unions with exhaustive
matches in both consumers.
Both consumers verify slot bounds, jump targets, initialization on every
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

`python rewrite/test_rewrite_compiler.py` builds the compiler both natively and
as an interpreted MIR module, checks invalid inputs, and compares supported
programs with the existing native and interpreted implementations.
Run both rewrite test scripts with `python tools/test_all.py --group rewrite`.
`python tools/benchmark_rewrite_compiler.py` measures a small supported program;
its results do not represent full compiler performance.
Use `--bindings 1000` to exercise many distinct local names. On the measured
workload, replacing linear name scans with dictionary lookup reduced median
rewrite compilation time from 0.065 s to 0.057 s (five runs each). This includes
the entire current compilation slice and process startup, not just lookup.
After adding branches and verification, this workload measured 0.063 s with
the linear verification path (0.274 s using the general CFG analysis).
