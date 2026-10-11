#!/usr/bin/env python
"""Verify bootstrap update/check dispatch without compiling the compiler."""
from contextlib import ExitStack
from pathlib import Path
import sys
import tempfile
from unittest.mock import patch

import test_selfhost


class FakeCache:
	def __init__(self, cc: str, rebuild: bool = False):
		self.cc = cc

	def seed(self) -> Path:
		return Path('seed')

	def runtime(self) -> Path:
		return Path('runtime.o')


def run_mode(arguments: list[str], *, committed_source: bytes,
	converges: bool = True) -> tuple[bytes, int, int]:
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		(root / 'bootstrap_c').mkdir()
		committed = root / 'bootstrap_c/compiler.c'
		committed.write_bytes(committed_source)
		generated = b'/* generated bootstrap */\n'
		generations = []
		checks = []

		def fake_execute(command: list[str]) -> None:
			output = Path(command[-1])
			if not output.name.startswith('generation_'):
				raise AssertionError(f'Unexpected compiler invocation: {command!r}')
			generations.append(output)
			output.write_bytes(generated if converges else generated + bytes(str(len(generations)), 'ascii'))

		def fake_compile(cc: str, source: Path, output: Path, runtime: Path) -> None:
			output.write_bytes(b'executable')

		def fake_check_cases(*args: object, **kwargs: object) -> None:
			checks.append(1)

		with ExitStack() as stack:
			stack.enter_context(patch.object(test_selfhost, 'ROOT', root))
			stack.enter_context(patch.object(test_selfhost, 'require_cc', return_value='cc'))
			stack.enter_context(patch.object(test_selfhost, 'BuildCache', FakeCache))
			stack.enter_context(patch.object(test_selfhost, 'combined_sources', return_value=b'compiler source'))
			stack.enter_context(patch.object(test_selfhost, 'execute', fake_execute))
			stack.enter_context(patch.object(test_selfhost, 'compile_c', fake_compile))
			stack.enter_context(patch.object(test_selfhost, 'check_cases', fake_check_cases))
			stack.enter_context(patch.object(sys, 'argv', ['test_selfhost.py', *arguments]))
			if converges:
				test_selfhost.main()
			else:
				try:
					test_selfhost.main()
				except AssertionError as error:
					assert 'fixed point' in str(error)
				else:
					raise AssertionError('Expected nonconvergent bootstrap to fail')
		return committed.read_bytes(), len(checks), len(generations)


def main() -> None:
	generated = b'/* generated bootstrap */\n'
	for mode in ([], ['--check']):
		committed, checks, generations = run_mode(mode, committed_source=generated)
		assert committed == generated and checks == 1 and generations == 2

	committed, checks, generations = run_mode(['--update-bootstrap'], committed_source=b'old\n')
	assert committed == generated and checks == 0 and generations == 2

	try:
		run_mode(['--check'], committed_source=b'old\n')
	except AssertionError as error:
		assert 'differs' in str(error)
	else:
		raise AssertionError('Stale bootstrap was accepted by --check')

	committed, checks, generations = run_mode(['--update-bootstrap'], committed_source=b'old\n', converges=False)
	assert committed == b'old\n' and checks == 0 and generations == 8

	print('PASS bootstrap modes: update, check, default, stale and nonconvergent')


if __name__ == '__main__':
	main()
