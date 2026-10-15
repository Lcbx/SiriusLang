# Deferred final-syntax module tests

These are **design-target** source tests, not part of the passing compiler suite.
They specify the intended file-path module model (`Math/Generic.sirius` →
`Math.Generic`), qualified imports, exported generics, structural compatibility,
and copy-on-write when the same generic body is specialized more than once.

The active compiler does not yet implement `import Math.Generic alias Generic`.
Until then, `tools/test_mir_generic_semantics.py` tests the same value/type
semantics using independently compiled libraries and the temporary typed
`extern def` bridge. Do not treat these deferred files as passing regressions.
