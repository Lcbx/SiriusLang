#!/usr/bin/env python3
"""Check distinct diagnostics for callback conflicts, cycles and missing constraints."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, compile_native, require_cc


PROVIDER = '''export def unused(callback, value: i64) -> i64
	return value

export def conflict(callback, enabled: bool) -> i64
	if enabled
		let left = callback(7)
	else
		let right = callback("seven")
	return 0

export def left(callback, values) -> i64
	return right(callback, values)

export def right(callback, values) -> i64
	return left(callback, values)

export def consume(callback, values) -> i64
	var total = 0
	for value in values
		total += callback(value)
	return total
'''

GOOD = '''import Provider alias P

def main() -> i64
	let ignored = P.unused(fn(x: i64) x * 2, 42)
	let applied = P.consume(fn(x) x * 2, [7, 14])
	if ignored != 42 or applied != 42
		return 1
	return 42
'''

CASES = {
	'Underdetermined': ('P.unused(fn(x) x * 2, 42)', b"underdetermined callback parameters in 'unused'"),
	'Conflicting': ('P.conflict(fn(x) x * 2, true)', b"conflicting generic callback parameter types in 'conflict'"),
	'Cyclic': ('P.left(fn(x) x * 2, [7, 14])', b"cyclic callback dependency in 'left'"),
}


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=120)


def verify_compiler(compiler: Path, folder: Path, cc: BuildCache) -> None:
	for name, (expression, diagnostic) in CASES.items():
		output = folder / f'{name}.c'
		result = run(compiler, '--module-root', folder, name, output)
		assert result.returncode != 0 and diagnostic in result.stderr, (name, result)
	generated = folder / 'Good.c'
	result = run(compiler, '--module-root', folder, 'Good', generated)
	assert result.returncode == 0, result
	run_result = run(compile_native(cc, generated))
	assert run_result.returncode == 42 and run_result.stderr == b'', run_result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--package-runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	with tempfile.TemporaryDirectory(prefix='sirius-callback-diagnostics-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'Good.sirius').write_text(GOOD)
		for name, (expression, _) in CASES.items():
			(root / f'{name}.sirius').write_text(f'import Provider alias P\n\ndef main() -> i64\n\treturn {expression}\n')
		verify_compiler(cache.seed(), root, cache)
		if not args.native_only:
			runner = args.package_runner or cache.native_tool('run_cli')
			package = root / 'Provider.smirg'
			result = run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius', root)
			assert result.returncode == 0, result
			(root / 'Provider.sirius').unlink()
			verify_compiler(cache.seed(), root, cache)
	print('PASS callback diagnostics: conflicting, underdetermined and cyclic constraints')


if __name__ == '__main__':
	main()
