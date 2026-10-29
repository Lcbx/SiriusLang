# Compiler rewrite

This directory contains an incomplete compiler implemented in Sirius. The
feature inventory in [FEATURES.md](FEATURES.md) describes the existing
compiler, not the rewrite. The existing bootstrap does not use this directory.

The first component is a lexer with compact integer kinds derived from
`TokenKind.index(label)`. These indices are internal to the rewrite, not
compatible with the existing compiler's serialized codes. Token positions are
byte offsets. Test with `python tools/test_rewrite_lexer.py`.
For paired native lexer timings run `python tools/benchmark_rewrite_lexer.py`;
this does not measure end-to-end compilation.

The current compiler accepts one `def main() -> i64` containing integer
expressions (`+`, `-`, `*`, `&`, `|`, unary `-`, and parentheses), `let` and `var`
bindings, assignments, integer `print`, `if`/`else`, `while`, early `return`,
`break`, and `continue`. Every reachable exit path must return an integer.
Boolean literals, bindings, `not`, printing, and scalar comparisons can
supply conditions. Nested blocks have local scopes. Integer literals
must fit in the positive `i64` range; in particular, a literal spelling of the
minimum `i64` is not yet supported. The compiler checks names, duplicate
bindings and mutability. It lowers the postorder syntax arena to a scalar IR
shared by C generation and an interpreter (`rewrite --run input.sirius`).
AST nodes store `NodeKind` enum values, with exhaustive matches in semantic
analysis and lowering. IR instructions are labeled unions with exhaustive
matches in both consumers.
Both consumers verify slot bounds, jump targets, initialization on every
incoming path and return placement before execution or emission. Local names
are resolved through scoped dictionaries. Straight-line IR uses a linear
verification pass; control flow uses a worklist and initialization facts at
each instruction. The latter still has quadratic worst-case storage.
C output is assembled as fragments and joined once. Lowering removes unreachable
block markers after exits and remaps jumps before verification. Typed HIR,
block-based MIR, other types and self-hosting remain to be built.
`elif`, `for`, and short-circuit `and`/`or`
are not yet supported.
It does not yet implement the existing compiler's artifact formats or CLI.

`python tools/test_rewrite_compiler.py` builds the compiler both natively and
as an interpreted MIR module, checks invalid inputs, and compares supported
programs with the existing native and interpreted implementations.
`python tools/benchmark_rewrite_compiler.py` measures a small supported program;
its results do not represent full compiler performance.
Use `--bindings 1000` to exercise many distinct local names. On the measured
workload, replacing linear name scans with dictionary lookup reduced median
rewrite compilation time from 0.065 s to 0.057 s (five runs each). This includes
the entire current compilation slice and process startup, not just lookup.
After adding branches and verification, this workload measured 0.063 s with
the linear verification path (0.274 s using the general CFG analysis).
