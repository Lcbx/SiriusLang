#!/usr/bin/env python3
"""Check the fixture inventory before running expensive native compiler tests."""
import json
from pathlib import Path

from project_paths import ROOT
from compiler_cases import ERRORS


TESTS = ROOT / 'tests'


def check_pairs(root: Path) -> int:
	"""Require an exact one-to-one match of .sirius sources and golden .txt files."""
	sources = {path.stem for path in (root / 'fixtures').glob('*.sirius')}
	outputs = {path.stem for path in (root / 'expected_output').glob('*.txt')}
	if not sources or sources != outputs:
		raise AssertionError(f'{root}: missing outputs={sorted(sources - outputs)}, '
			f'orphan outputs={sorted(outputs - sources)}')
	return len(sources)


def check_json_cases(root: Path, suffix: str) -> int:
	"""Check the data inventory; this does not execute design-only examples."""
	expected = json.loads((root / 'expectations.json').read_text(encoding='utf-8'))
	actual = {path.stem for path in root.glob('*' + suffix)}
	if not isinstance(expected, dict) or set(expected) != actual:
		raise AssertionError(f'{root}: cases/expectations mismatch: '
			f'without expectations={sorted(actual - set(expected))}, '
			f'without case={sorted(set(expected) - actual)}')
	return len(actual)


def main() -> None:
	counts = {name: check_pairs(TESTS / name)
		for name in ('language', 'compiler', 'parser', 'tokenizer')}
	language = TESTS / 'language'
	references = {path.stem for path in (language / 'reference_c').glob('*.c')}
	golden = {path.stem for path in (language / 'fixtures').glob('*.sirius')}
	if not references or not references <= golden:
		raise AssertionError(f'C references without Sirius fixtures: {sorted(references - golden)}')
	check_json_cases(language / 'runtime_errors', '.c')
	check_json_cases(language / 'proposed_negative', '.sirius')
	negative = {path.stem for path in (TESTS / 'compiler/negative').glob('*.sirius')}
	if negative != set(ERRORS):
		raise AssertionError(f'Compiler diagnostics inventory mismatch: '
			f'unknown fixtures={sorted(negative - set(ERRORS))}, '
			f'missing fixtures={sorted(set(ERRORS) - negative)}')
	# MIR and package tests have their own input/output conventions.
	print('PASS fixture pairs: ' + ', '.join(f'{name}={count}' for name, count in counts.items()))
	print(f'PASS C references={len(references)}; compiler negatives={len(negative)}; proposed and runtime inventories')


if __name__ == '__main__':
	main()
