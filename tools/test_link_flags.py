#!/usr/bin/env python3
"""Check C linker argument selection for native Windows and POSIX targets."""
import subprocess
from types import SimpleNamespace
from pathlib import Path
from unittest.mock import patch

import build_support


def main() -> None:
	for platform, target, expected in (
		('linux', '', ('-lm',)),
		('win32', 'x86_64-pc-windows-msvc', ('-Xlinker', '/STACK:16777216')),
		('win32', 'x86_64-w64-mingw32', ('-Wl,--stack,16777216', '-lm')),
		('win32', 'x86_64-w64-windows-gnu', ('-Wl,--stack,16777216', '-lm')),
	):
		commands = []
		def fake_execute(command):
			commands.append(command)
			return subprocess.CompletedProcess(command, 0, target + '\n', '')
		with patch.object(build_support, 'sys', SimpleNamespace(platform=platform)), \
			patch.object(build_support, 'execute', fake_execute):
			assert build_support.link_flags('clang') == expected
			build_support.compile_c('clang', Path('input.c'), Path('result.exe'), Path('runtime.o'))
			command = commands[-1]
			assert command[-len(expected)-2:-2] == list(expected), command
			assert ('-lm' in command) == ('-lm' in expected), command
			if 'msvc' in target:
				assert '-Wl,--stack,16777216' not in command
			assert ('-dumpmachine' in str(commands[0])) == (platform == 'win32')

	with patch.object(build_support, 'sys', SimpleNamespace(platform='win32')), \
		patch.object(build_support, 'execute', return_value=subprocess.CompletedProcess([], 0, 'unknown\n', '')):
		try:
			build_support.link_flags('clang')
		except RuntimeError as error:
			assert 'Unsupported Windows C compiler target' in str(error)
		else:
			raise AssertionError('Unexpectedly accepted an unknown Windows target')
	print('PASS linker flags: MSVC, MinGW, POSIX and unknown targets')


if __name__ == '__main__':
	main()
