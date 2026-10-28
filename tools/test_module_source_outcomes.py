#!/usr/bin/env python3
"""Source lookup's found/missing/invalid union must preserve module fallback rules."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, require_cc


def compile_module(compiler: Path, root: Path, module: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run([str(compiler), '--module-root', str(root), module,
        str(root / 'output.c')], capture_output=True, text=True, timeout=120)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='prebuilt modular compiler')
    args = parser.parse_args()
    compiler = args.compiler.resolve() if args.compiler else BuildCache(require_cc()).native_tool('modular_cli')
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        (root / 'Lib.sirius').write_text('export def number() -> i64\n\treturn 42\n', encoding='utf-8')
        (root / 'Main.sirius').write_text('import Lib alias L\ndef main() -> i64\n\treturn L.number()\n', encoding='utf-8')
        result = compile_module(compiler, root, 'Main')
        assert result.returncode == 0 and (root / 'output.c').is_file(), result.stderr
        (root / 'output.c').unlink()

        # A missing source may be resolved through a package instead of
        # producing the ordinary source-not-found diagnostic.
        (root / 'Lib.sirius').unlink()
        result = compile_module(compiler, root, 'Main')
        assert result.returncode != 0 and 'not found under source root' in result.stderr, result.stderr
        assert not (root / 'output.c').exists()
        (root / 'Lib.smirg').write_text('invalid package bytes', encoding='utf-8')
        result = compile_module(compiler, root, 'Main')
        assert result.returncode != 0 and 'invalid generic package Lib' in result.stderr, result.stderr
        assert not (root / 'output.c').exists()

        # If source exists but is malformed, it must *not* fall through to the
        # package of the same name, even though a package file exists.
        (root / 'Lib.sirius').write_text('export def broken(\n', encoding='utf-8')
        result = compile_module(compiler, root, 'Main')
        assert result.returncode != 0 and 'Lib:' in result.stderr, result.stderr
        assert 'invalid generic package' not in result.stderr, result.stderr
        assert not (root / 'output.c').exists()
        print('PASS source outcome union: found, missing, package fallback, invalid source')


if __name__ == '__main__':
    main()
