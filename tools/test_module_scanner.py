#!/usr/bin/env python3
"""Native Sirius scanner regressions independent of the full MIR runner build."""
from pathlib import Path
import tempfile

from build_modules import ModuleError, default_scanner, discover, execute, symbol


def write(root: Path, name: str, source: str) -> None:
	path = root.joinpath(*name.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(source, encoding='utf-8')


def reject(root: Path, name: str, message: str) -> None:
	try:
		discover(root, name, SCANNER)
	except ModuleError as exc:
		assert message in str(exc), (message, str(exc))
	else:
		raise AssertionError(f'{name}: expected {message!r}')


SCANNER = default_scanner()


def main() -> None:
	with tempfile.TemporaryDirectory() as temporary:
		root = Path(temporary)
		write(root, 'A.B', 'export def value() -> i64\n\treturn 19\n')
		write(root, 'C.B', 'export def value() -> i64\n\treturn 23\n')
		write(root, 'Utility.Combine',
			'import A.B alias First\nimport C.B alias Second\n'
			'export def add(a: Array[Dict[string, Optional[i64]]]) -> i64\n'
			'\tlet label = "import A.B export def value()"\n'
			'\treturn First.value() + Second.value() + len(a)\n')
		write(root, 'App', 'import Utility.Combine alias Combine\ndef main() -> i64\n\treturn 42\n')
		modules = discover(root, 'App', SCANNER)
		assert [module.name for module in modules] == ['A.B', 'C.B', 'Utility.Combine', 'App']
		assert list(modules[2].imports.items()) == [('First', 'A.B'), ('Second', 'C.B')]
		assert modules[2].exports['add'].parameters == 'a: Array[Dict[string, Optional[i64]]]'
		assert modules[2].exports['add'].returns == 'i64'
		assert modules[-1].has_main and not modules[0].has_main
		assert list(modules[2].exports) == ['add']
		assert symbol('A.B', 'value') != symbol('A_B', 'value')
		first = execute(str(SCANNER), str(root), 'App').stdout
		assert execute(str(SCANNER), str(root), 'App').stdout == first
		assert 'let label' not in first
		print('PASS scanner: transitive dependencies, type signatures, deterministic metadata')

		write(root, 'BadMissing', 'import Missing.Sub\ndef main() -> i64\n\treturn 0\n')
		reject(root, 'BadMissing', 'not found under source root')
		write(root, 'CycleA', 'import CycleB\nexport def one() -> i64\n\treturn 1\n')
		write(root, 'CycleB', 'import CycleA\nexport def two() -> i64\n\treturn 2\n')
		reject(root, 'CycleA', 'cyclic source imports')
		write(root, 'Duplicate', 'import A.B alias Same\nimport C.B alias Same\ndef main() -> i64\n\treturn 0\n')
		reject(root, 'Duplicate', 'duplicate import alias')
		write(root, 'Reserved', 'import A.B alias while\ndef main() -> i64\n\treturn 0\n')
		reject(root, 'Reserved', 'invalid import alias')
		write(root, 'NoAnnotation', 'export def bad(x) -> i64\n\treturn 0\n')
		reject(root, 'NoAnnotation', 'exported function parameters require annotations')
		write(root, 'NoReturn', 'export def bad(x: i64)\n\treturn x\n')
		reject(root, 'NoReturn', 'exported function requires a return type')
		write(root, 'UnknownType', 'export def bad(x: Something) -> i64\n\treturn 0\n')
		reject(root, 'UnknownType', 'unsupported exported signature')
		write(root, 'Malformed', 'export def nope(\n')
		reject(root, 'Malformed', ':')
		reject(root, '../A.B', 'invalid module path')
		print('PASS scanner: cycles, invalid declarations, invalid paths and types rejected')


if __name__ == '__main__':
	main()
