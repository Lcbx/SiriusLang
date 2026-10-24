#!/usr/bin/env python3
"""Validate compiler module imports and explicitly documented entry points."""
import re
from pathlib import Path

from project_paths import ROOT


ENTRY_POINTS = ('modular_cli', 'run_cli', 'module_scan_cli')
# This analysis experiment is exercised by the native HIR test driver, but is
# not yet part of the production compiler's module graph.
TEST_ONLY = ('hir_parametric',)


def module_graph(root: Path) -> dict[str, set[str]]:
	modules = {path.stem: path for path in (root / 'src').glob('*.sirius')}
	graph = {}
	for name, path in modules.items():
		imports = set(re.findall(r'^import ([a-z][a-z0-9_]*)\b', path.read_text(encoding='utf-8'), re.MULTILINE))
		missing = imports - modules.keys()
		assert not missing, f'{path}: missing imports {sorted(missing)}'
		graph[name] = imports
	return graph


def reachable(graph: dict[str, set[str]], roots: tuple[str, ...]) -> set[str]:
	assert set(roots) <= graph.keys()
	seen = set()
	pending = list(roots)
	while pending:
		name = pending.pop()
		if name not in seen:
			seen.add(name)
			pending.extend(graph[name] - seen)
	return seen


def main() -> None:
	graph = module_graph(ROOT)
	active = reachable(graph, ENTRY_POINTS)
	orphans = graph.keys() - active
	assert orphans == set(TEST_ONLY), f'Unreachable Sirius modules: {sorted(orphans)}'
	assert not (ROOT / 'src/mir_parametric.sirius').exists(), 'obsolete flat-snapshot module restored'
	print(f'PASS {len(graph)} Sirius modules: {len(active)} active, {len(orphans)} test-only')


if __name__ == '__main__':
	main()
