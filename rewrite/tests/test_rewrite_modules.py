#!/usr/bin/env python3
"""Focused rewrite source-graph loading, namespace and visibility checks."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))
from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc
from rewrite_paths import SOURCE, DRIVERS


def main() -> None:
    cache = BuildCache(require_cc())
    driver = cache.native_driver('rewrite-source-graph', DRIVERS / 'modules_test_driver.sirius',
        extra_modules=(SOURCE / 'lexer.sirius', SOURCE / 'parser.sirius', SOURCE / 'modules.sirius'))
    with tempfile.TemporaryDirectory(prefix='rewrite-modules-') as tmp:
        root = Path(tmp)
        (root / 'Sub').mkdir()
        (root / 'Utils.sirius').write_text('''export def public_fn() -> i64
\treturn 1
def private_fn() -> i64
\treturn 2
export rec Box(value: i64)
rec Box2(value: string)
export let Choice = Box | Box2
export enum Color
\tRed
\tBlue
''')
        (root / 'Sub' / 'Mid.sirius').write_text('''import Utils alias U
export def mid() -> i64
\treturn 4
''')
        (root / 'Main.sirius').write_text('''import Utils alias U
import Sub.Mid alias M
export def main() -> i64
\treturn 0
''')
        def run(entry: str = 'Main') -> subprocess.CompletedProcess[str]:
            return subprocess.run([str(driver), str(root), entry], capture_output=True,
                text=True, timeout=40)
        ok = run()
        assert (ok.returncode, ok.stdout, ok.stderr) == (0, 'Utils\nSub.Mid\nMain\n', ''), ok
        assert run().stdout == ok.stdout, 'module order is nondeterministic'
        # Exercise ownership of parser state, transitive graphs and alias maps.
        audited = root / 'modules-audited'
        execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
            '-I', str(SOURCE.parents[1] / 'runtime'), str(driver.parent / 'generated.c'),
            str(SOURCE.parents[1] / 'runtime' / 'sirius_runtime.c'),
            *link_flags(cache.cc), '-o', str(audited)])
        audit = subprocess.run([str(audited), str(root), 'Main'], capture_output=True,
            text=True, timeout=40)
        assert (audit.returncode, audit.stdout, audit.stderr) == (0, ok.stdout, ''), audit
        # A duplicate dependency is loaded once; private names are not exported.
        (root / 'Main.sirius').write_text('import Utils alias U\nimport Utils alias X\ndef main() -> i64\n\treturn 0\n')
        # Driver expects U; this also confirms two aliases can reference one unit.
        assert (r := run()).returncode == 0 and r.stdout == 'Utils\nMain\n', r
        invalid = [
            ('duplicate alias', 'import Utils alias U\nimport Sub.Mid alias U\ndef main() -> i64\n\treturn 0\n', 'duplicate import alias'),
            ('declaration collision', 'import Utils alias U\nrec U(value: i64)\ndef main() -> i64\n\treturn 0\n', 'import alias conflicts with declaration'),
            ('missing import', 'import NoSuch alias U\ndef main() -> i64\n\treturn 0\n', 'cannot read module: NoSuch'),
            ('duplicate declaration', 'import Utils alias U\nrec R(x: i64)\nrec R(y: i64)\ndef main() -> i64\n\treturn 0\n', 'duplicate declaration'),
            ('malformed dotted path', 'import Utils..Bad alias U\ndef main() -> i64\n\treturn 0\n', "expected 'alias'"),
            ('missing alias', 'import Utils\ndef main() -> i64\n\treturn 0\n', "expected 'alias'"),
            ('invalid export', 'export import Utils alias U\ndef main() -> i64\n\treturn 0\n', 'export requires a declaration'),
            ('cycle', 'import Loop alias U\ndef main() -> i64\n\treturn 0\n', 'cyclic source import'),
        ]
        (root / 'Loop.sirius').write_text('import Main alias Main\n')
        for label, source, expected in invalid:
            (root / 'Main.sirius').write_text(source)
            result = run()
            assert result.returncode != 0 and expected in result.stderr, (label, result)
            if label == 'cycle':
                audit_error = subprocess.run([str(audited), str(root), 'Main'],
                    capture_output=True, text=True, timeout=40)
                assert audit_error.returncode == 1 and 'cyclic source import' in audit_error.stderr \
                    and 'ARC audit' not in audit_error.stderr, audit_error
        print('PASS rewrite source-module graph: 3 valid DAG checks; 8 invalid cases; ARC audited; visibility/alias queries')


if __name__ == '__main__':
    main()
