# Deferred final-syntax module tests

These are **design-target** source tests, not part of the passing compiler suite.
They specify the intended file-path module model (`Math/Generic.sirius` →
`Math.Generic`), qualified imports, exported generics, structural compatibility,
and copy-on-write when the same generic body is specialized more than once.

The positive fixture also specifies record literals/field selection, generic
array literals, value-returning builtin calls, nested generic calls, and early-return branches. The negative fixtures specify
errors that appear only after instantiating a generic body with concrete types,
including calls to other generics and conditional type checking.

The active compiler does not yet implement `import Math.Generic alias Generic`.
Until then, `tools/test_mir_generic_semantics.py` tests the same value/type
semantics using independently compiled libraries and the temporary typed
`extern def` bridge. Do not treat these deferred files as passing regressions.
