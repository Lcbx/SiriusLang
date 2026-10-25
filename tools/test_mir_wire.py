#!/usr/bin/env python
"""Round-trip and malformed-input tests for versioned MIR executables."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, require_cc, options
from compiler_cases import REFERENCE
from project_paths import ROOT, normalize_newlines
from test_mir_run import CASES, FAILURE_CASES


def check_result(actual: subprocess.CompletedProcess[bytes], expected: subprocess.CompletedProcess[bytes], name: str) -> None:
	if (actual.returncode, normalize_newlines(actual.stdout), normalize_newlines(actual.stderr)) != (
		expected.returncode, normalize_newlines(expected.stdout), normalize_newlines(expected.stderr)):
		raise AssertionError(f'{name}: direct={expected!r}, loaded={actual!r}')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	parser.add_argument('--runner', type=Path, help='prebuilt sirius-run binary')
	parser.add_argument('--all-fixtures', action='store_true', help='check all 91 shared executable fixtures')
	args = parser.parse_args()
	cache = BuildCache(shutil.which('clang') or require_cc(), rebuild=args.rebuild)
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as temp:
		work = Path(temp)
		(work / 'file.txt').write_text('hello from a file\n')
		(work / 'empty.txt').write_text('')
		all_cases = {**{name: source for name, (source, _, _) in CASES.items()},
			**{name: source for name, (source, _) in FAILURE_CASES.items()},
			'embedded_newlines': 'def main() -> i64\n\tlet r = {text = "first\\nsecond\\tend"}\n\tprint(r.text)\n\treturn len(r.text)\n',
			'path_kind': 'def main() -> i64\n\tlet path = arguments()[!1]\n\tprint(path_kind(path))\n\treturn 0\n',
			'big_int': 'def main() -> i64\n\tlet x = 9223372036854775807\n\tprint(x)\n\treturn 0\n',
			'min_int': 'def main() -> i64\n\tlet x = -9223372036854775807 - 1\n\tprint(x)\n\treturn 0\n',
		}
		for name in ('success', 'failure', 'direct', 'positional', 'fallthrough', 'managed'):
			all_cases['union_' + name] = (ROOT / 'tests/mir' / f'source_union_{name}.sirius').read_text()
		for name in ('optional_dict', 'dict_conversion', 'dict_managed', 'dict_growth'):
			all_cases['union_' + name] = (ROOT / 'tests/mir' / f'source_union_{name}.sirius').read_text()
		def roundtrip(source: Path, name: str, argv: list[str] | None = None) -> None:
			argv = argv or []
			artifact = work / (name + '.smir')
			created = subprocess.run([str(runner), '--emit-mir', str(artifact), str(source)], capture_output=True, timeout=25)
			if created.returncode or created.stdout or created.stderr:
				raise AssertionError(f'{name} emit: {created!r}')
			data = artifact.read_bytes()
			if not data.startswith(b'SIRIUS-MIR-EXEC/1\n'):
				raise AssertionError(f'{name} artifact header')
			direct = subprocess.run([str(runner), str(source), *argv], capture_output=True, timeout=25)
			loaded = subprocess.run([str(runner), '--load-mir', str(artifact), *argv], capture_output=True, timeout=25)
			check_result(loaded, direct, name)
			# Serialization is deterministic across separately compiled runs.
			second = work / (name + '-again.smir')
			result = subprocess.run([str(runner), '--emit-mir', str(second), str(source)], capture_output=True, timeout=25)
			if result.returncode or data != second.read_bytes():
				raise AssertionError(f'{name} non-deterministic serialization')
			return data

		for name, source in all_cases.items():
			path = work / (name + '.sirius')
			path.write_text(source)
			argv = []
			if name == 'arguments':
				argv = ['provided']
			elif name == 'read_file':
				argv = [str(work / 'file.txt')]
			elif name == 'missing_file':
				argv = [str(work / 'absent.txt')]
			elif name == 'empty_file':
				argv = [str(work / 'empty.txt')]
			elif name == 'path_kind':
				argv = [str(work / 'file.txt')]
			roundtrip(path, name, argv)
		print(f'PASS MIR wire: {len(all_cases)} round-trip/determinism cases')

		# A loaded artifact must not depend on the original source file.
		(work / 'embedded_newlines.sirius').unlink()
		sourceless = subprocess.run([str(runner), '--load-mir', str(work / 'embedded_newlines.smir')], capture_output=True, timeout=25)
		if sourceless.returncode != 16 or normalize_newlines(sourceless.stdout) != b'first\nsecond\tend\n' or sourceless.stderr:
			raise AssertionError(f'load without source: {sourceless!r}')
		print('PASS MIR wire: loading without source')

		path = work / 'embedded_newlines.smir'
		valid = path.read_bytes()
		invalid = (
			b'', b'SIRIUS-MIR-EXEC/2\n' + valid.split(b'\n', 1)[1],
			valid + b'extra', valid[:-1], valid[:len(valid) // 2],
			valid.replace(b'SIRIUS-MIR-EXEC/1\n', b'SIRIUS-MIR-EXEC/1\n9223372036854775808\n', 1),
			valid.replace(b'SIRIUS-MIR-EXEC/1\n', b'SIRIUS-MIR-EXEC/1\n-1\n', 1),
			valid.replace(b'SIRIUS-MIR-EXEC/1\n', b'SIRIUS-MIR-EXEC/1\n0\n1000001\n', 1),
		)
		for i, data in enumerate(invalid):
			bad = work / 'corrupt.smir'
			bad.write_bytes(data)
			result = subprocess.run([str(runner), '--load-mir', str(bad)], capture_output=True, timeout=25)
			if result.returncode != 2 or result.stdout or (b'invalid MIR artifact:' not in result.stderr and b'cannot read MIR artifact' not in result.stderr):
				raise AssertionError(f'corrupt case {i}: {result!r}')
		print(f'PASS MIR wire: {len(invalid)} corrupted and incompatible artifacts rejected')

		if args.all_fixtures:
			fixtures = [ROOT / 'tests/fixtures' / (name + '.sirius') for name in REFERENCE]
			fixtures += sorted((ROOT / 'tests/compiler/fixtures').glob('*.sirius'))
			passed = 0
			for source in fixtures:
				argv = [str(work / 'file.txt')] if source.stem == '48_file_io' else []
				roundtrip(source, 'shared_' + source.stem, argv)
				passed += 1
			print(f'PASS MIR wire: {passed} shared compiler fixtures')


if __name__ == '__main__':
	main()
