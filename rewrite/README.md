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

The current compiler accepts one `def main() -> i64` containing integer
expressions (`+`, `-`, `*`, `/`, `%`, `&`, `|`, `<<`, `>>`, unary `-`, and parentheses), `let` and `var`
bindings, assignments (`=`, `+=`, `-=`, `*=`, `/=`), integer `print`,
`if`/`elif`/`else`, `while`, early `return`,
`break`, `continue`, `condition else return value` (also `break`/`continue`),
and `for i in start..end`. Guards evaluate the condition once and evaluate the
exit only when it is false. They reuse ordinary branch and exit nodes.
Ranges are ascending and
half-open and bind an immutable local `i`. Bound expressions are evaluated
before iteration; for compatibility with the existing compiler, a bare mutable
upper-bound name remains live (use `end + 0` to snapshot its value).
`continue` advances the range counter before retesting. Every reachable exit
path must return an integer.
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
`%=`, bitwise compound assignments and updates to fields/indices remain unsupported.
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
Array iteration and first-class range values are not yet supported. Range
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
