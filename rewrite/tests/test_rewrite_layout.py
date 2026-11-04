#!/usr/bin/env python3
"""Guard the rewrite source inventory, driver paths and test discovery."""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools'))

from project_paths import ROOT
from rewrite_paths import BENCHMARKS, DRIVERS, ENTRY, MODULES, REWRITE, SOURCE
from test_all import select_scripts


def main() -> None:
    assert ENTRY == SOURCE / 'main.sirius' and ENTRY.is_file()
    assert set(SOURCE.glob('*.sirius')) == set(MODULES) | {ENTRY}
    assert len(MODULES) == len(set(MODULES))
    suites = set((REWRITE / 'tests').glob('test_rewrite_*.py'))
    for script in suites:
        for name in re.findall(r"DRIVERS / '([^']+\.sirius)'", script.read_text(encoding='utf-8')):
            assert (DRIVERS / name).is_file(), (script, name)
    assert {p.name for p in BENCHMARKS.glob('*.sirius')} == {
        'lexer_bench.sirius', 'lexer_reference_bench.sirius'}
    assert not list(REWRITE.glob('*.sirius')) and not list(REWRITE.glob('test_*.py'))
    discovered = set(select_scripts(ROOT / 'tools', groups=('rewrite',)))
    assert discovered == suites, (suites - discovered, discovered - suites)
    for document in (REWRITE / 'README.md', REWRITE / 'TODO.md', REWRITE / 'FEATURES.md'):
        text = document.read_text(encoding='utf-8')
        for target in re.findall(r'python (rewrite/[^`\s]+\.py)', text):
            assert (ROOT / target).is_file(), (document, target)
        for target in re.findall(r'`(tests/(?:drivers/)?[\w.]+\.(?:py|sirius))`', text):
            assert (REWRITE / target).is_file(), (document, target)
        for target in re.findall(r'\[[^]]+\]\(([^)]+)\)', text):
            if not target.startswith(('https:', 'http:', '#')):
                assert (document.parent / target.split('#', 1)[0]).exists(), (document, target)
    print(f'PASS rewrite layout: {len(MODULES)} modules, {len(suites)} suites, drivers, benchmarks, documentation links')


if __name__ == '__main__':
    main()
