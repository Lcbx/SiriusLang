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

export def conflicting_arity(callback) -> i64
	let first = callback(7)
	let second = callback(7, 14)
	return 0

export def relay_conflict(callback, enabled: bool) -> i64
	return conflict(callback, enabled)
'''

GOOD = '''import Provider alias P

def main() -> i64
	let ignored = P.unused(fn(x: i64) x * 2, 42)
	let applied = P.consume(fn(x) x * 2, [7, 14])
	if ignored != 42 or applied != 42
		return 1
	return 42
'''

# The leading fragments remain stable; the additional fragments ensure errors
# carry actionable provenance through source modules and .smirg packages.
CASES = {
	'Underdetermined': ('P.unused(fn(x) x * 2, 42)',
		(b"underdetermined callback parameters in 'unused'", b'annotate the callback parameters')),
	'Conflicting': ('P.conflict(fn(x) x * 2, true)',
		(b"conflicting generic callback parameter types in 'conflict'", b'parameter 1: i64 versus string', b'callback calls at lines 6 and 8')),
	'ConflictingArity': ('P.conflicting_arity(fn(x) x * 2)',
		(b"conflicting generic callback parameter types in 'conflicting_arity'", b'callback arity 1 versus 2')),
	'ConflictingForwarded': ('P.relay_conflict(fn(x) x * 2, true)',
		(b"conflicting generic callback parameter types in 'relay_conflict'", b'parameter 1: i64 versus string')),
	'Cyclic': ('P.left(fn(x) x * 2, [7, 14])',
		(b"cyclic callback dependency in 'left'", b'left -> right -> left', b'annotate the callback parameters')),
}


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=120)


def verify_compiler(compiler: Path, folder: Path, cc: BuildCache) -> None:
	for name, (expression, fragments) in CASES.items():
		output = folder / f'{name}.c'
		result = run(compiler, '--module-root', folder, name, output)
		assert result.returncode != 0 and all(part in result.stderr for part in (*fragments, b"defined in module 'Provider'")), (name, result)
	generated = folder / 'Good.c'
	result = run(compiler, '--module-root', folder, 'Good', generated)
	assert result.returncode == 0, result
	# Qualify nested module declarations, but not local callback errors.
	result = run(compiler, '--module-root', folder, 'NestedCase', folder / 'NestedCase.c')
	assert result.returncode != 0 and b"defined in module 'Nested.Provider'" in result.stderr, result
	result = run(compiler, '--module-root', folder, 'Local', folder / 'Local.c')
	assert result.returncode != 0 and b"in 'conflict'" in result.stderr and b'defined in module' not in result.stderr, result
	run_result = run(compile_native(cc, generated))
	assert run_result.returncode == 42 and run_result.stderr == b'', run_result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--native-only', action='store_true')
	parser.add_argument('--compiler', type=Path, help='Use a rebuilt compiler before bootstrap regeneration')
	parser.add_argument('--package-runner', type=Path)
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.seed()
	with tempfile.TemporaryDirectory(prefix='sirius-callback-diagnostics-') as tmp:
		root = Path(tmp)
		(root / 'Provider.sirius').write_text(PROVIDER)
		(root / 'Good.sirius').write_text(GOOD)
		(root / 'Nested').mkdir()
		(root / 'Nested' / 'Provider.sirius').write_text(PROVIDER)
		(root / 'NestedCase.sirius').write_text('import Nested.Provider alias P\n\ndef main() -> i64\n\treturn P.conflict(fn(x) x * 2, true)\n')
		(root / 'Local.sirius').write_text(PROVIDER + '\ndef main() -> i64\n\treturn conflict(fn(x) x * 2, true)\n')
		for name, (expression, _) in CASES.items():
			(root / f'{name}.sirius').write_text(f'import Provider alias P\n\ndef main() -> i64\n\treturn {expression}\n')
		verify_compiler(compiler, root, cache)
		if not args.native_only:
			runner = args.package_runner or cache.mir_runner()
			package = root / 'Provider.smirg'
			result = run(runner, '--emit-generic', 'Provider', package, root / 'Provider.sirius', root)
			assert result.returncode == 0, result
			result = run(runner, '--emit-generic', 'Nested.Provider', root / 'Nested' / 'Provider.smirg', root / 'Nested' / 'Provider.sirius', root)
			assert result.returncode == 0, result
			(root / 'Provider.sirius').unlink()
			(root / 'Nested' / 'Provider.sirius').unlink()
			verify_compiler(compiler, root, cache)
	print('PASS callback diagnostics: conflict types/arity/locations, forwarding, cycles and underdetermined')


if __name__ == '__main__':
	main()
