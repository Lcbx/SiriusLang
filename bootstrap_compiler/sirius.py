"""Sirius bootstrap compiler CLI: python bootstrap_compiler/sirius.py compile input.sirius -o out.c"""
from __future__ import annotations
import argparse
from pathlib import Path
import sys
from syntax import Parser, CompileError
from codegen import Compiler


def main() -> int:
    parser = argparse.ArgumentParser(prog='sirius')
    sub = parser.add_subparsers(dest='action', required=True)
    compile_cmd = sub.add_parser('compile')
    compile_cmd.add_argument('source', type=Path)
    compile_cmd.add_argument('-o', '--output', type=Path, required=True)
    tokens_cmd = sub.add_parser('tokens')
    tokens_cmd.add_argument('source', type=Path)
    ns = parser.parse_args()
    try:
        source = ns.source.read_text(encoding='utf8')
        if ns.action == 'tokens':
            from syntax import tokenize
            for token in tokenize(source):
                print(f'{token.line}:{token.col} {token.value}')
            return 0
        ast = Parser(source).parse()
        c = Compiler(ast).compile()
        ns.output.parent.mkdir(parents=True, exist_ok=True)
        ns.output.write_text(c, encoding='utf8')
    except (CompileError, UnicodeError, OSError) as exc:
        print(f'{ns.source}: {exc}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
