#!/usr/bin/env python3
"""Exercise exhaustive match-result unions and source diagnostics."""
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, execute, require_cc, compile_native
from project_paths import ROOT


HEADER = (
    'rec Success(value: i64)\n'
    'rec Failure(code: i64)\n'
    'let Result = Success | Failure\n'
)


def source(arms: str) -> str:
    return (HEADER + 'def unwrap(value: Result) -> i64\n\tmatch value\n' + arms
        + 'def main() -> i64\n\tlet value: Result = Success(42)\n\treturn unwrap(value)\n')


def main() -> None:
    cache = BuildCache(require_cc())
    # Tests both the record-TypeId and label-index checkers and their payloads.
    binary = cache.native_driver('hir-match-outcomes', ROOT / 'tests/types/union_driver_native.sirius')
    assert execute([str(binary)]).stdout == 'PASS structural union types and exhaustive match checking\n'
    compiler = cache.seed()
    cases = (
        ('valid', '\t\tSuccess(value)\n\t\t\treturn value\n\t\tFailure(code)\n\t\t\treturn -code\n', ''),
        ('missing', '\t\tSuccess(value)\n\t\t\treturn value\n', 'non-exhaustive match'),
        ('repeated', '\t\tSuccess(value)\n\t\t\treturn value\n\t\tSuccess(other)\n\t\t\treturn other\n\t\tFailure(code)\n\t\t\treturn code\n', 'duplicate match alternative'),
    )
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        for name, arms, diagnostic in cases:
            program = root / (name + '.sirius')
            generated = root / (name + '.c')
            program.write_text(source(arms), encoding='utf-8')
            result = subprocess.run((str(compiler), str(program), str(generated)),
                capture_output=True, text=True)
            if diagnostic:
                assert result.returncode != 0 and diagnostic in (result.stdout + result.stderr), (name, result)
            else:
                assert result.returncode == 0, result
                native = compile_native(cache, generated)
                executed = subprocess.run((str(native),), capture_output=True)
                assert executed.returncode == 42, executed
    print('PASS typed match outcomes and source diagnostics')


if __name__ == '__main__':
    main()
