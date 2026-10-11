#!/usr/bin/env python
"""Unit tests for shared process and newline comparison utilities."""
import subprocess

from sirius_sources import normalize_newlines
from test_build import assert_native


def main() -> None:
	assert normalize_newlines(b'a\r\nb\n') == b'a\nb\n'
	assert_native(subprocess.CompletedProcess(['test'], 0, b'42\r\ntrue\r\n', b''), stdout=b'42\ntrue\n')
	assert_native(subprocess.CompletedProcess(['test'], 7, b'', b'error\r\n'), code=7, stderr_contains=b'error\n')
	assert_native(subprocess.CompletedProcess(['test'], 2, b'', b'error'), code=None, stderr_contains=b'error')
	for result, options in (
		(subprocess.CompletedProcess(['test'], 0, b'42\r\nextra\r\n', b''), {'stdout': b'42\n'}),
		(subprocess.CompletedProcess(['test'], 0, b'', b''), {'code': None}),
		(subprocess.CompletedProcess(['test'], 1, b'', b''), {'code': 0}),
		(subprocess.CompletedProcess(['test'], 1, b'', b'other'), {'code': None, 'stderr_contains': b'error'}),
	):
		try:
			assert_native(result, **options)
		except AssertionError:
			pass
		else:
			raise AssertionError(f'Expected rejection for {result!r}')
	print('PASS tool helpers: CRLF, output, exit status, diagnostics')


if __name__ == '__main__':
	main()
