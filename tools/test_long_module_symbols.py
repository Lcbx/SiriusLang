#!/usr/bin/env python3
"""Check long structural symbol names in source and packaged imports."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT


FIELDS = (
	'first_long_field', 'second_long_field', 'third_long_field',
	'fourth_long_field', 'fifth_long_field', 'sixth_long_field',
)


def run(*args: object) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=90)


def checked(*args: object) -> subprocess.CompletedProcess[bytes]:
	result = run(*args)
	assert result.returncode == 0, (args, result.stdout, result.stderr)
	return result


def record(fields: tuple[str, ...], left: int, right: int) -> str:
	return '{' + ', '.join(f'{name} = {left if i == 0 else right if i == 1 else 0}'
		for i, name in enumerate(fields)) + '}'


def verify(runner: Path, cc: str, root: Path, expected: int) -> tuple[bytes, bytes]:
	library = root / 'linked.smirl'
	checked(runner, '--build-modules', root, 'App', library)
	original = library.read_bytes()
	checked(runner, '--build-modules', root, 'App', library)
	assert library.read_bytes() == original, 'module builds must be deterministic'
	listing = checked(runner, '--list-lib', library).stdout
	assert b'import ' not in listing, listing
	for line in listing.splitlines():
		name = line.split(b' : ', 1)[0]
		assert len(name) <= 4096, name
	assert run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library).returncode == expected
	generated = root / 'linked.c'
	checked(runner, '--emit-linked-c', library, 'M_3_App_4_main', generated)
	binary = root / 'linked-native'
	checked(cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
		'-I', ROOT / 'runtime', generated, ROOT / 'runtime/sirius_runtime.c',
		*link_flags(cc), '-o', binary)
	assert run(binary).returncode == expected
	return listing, original


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	cc = require_cc()
	runner = args.runner.resolve() if args.runner else BuildCache(cc).native_tool('run_cli')
	with tempfile.TemporaryDirectory(prefix='sirius-long-symbols-') as folder:
		root = Path(folder)
		changed = (*FIELDS[:-1], 'sixth_long_fielD')
		(root / 'Ops.sirius').write_text(
			'export def take(value) -> i64\n'
			'\treturn value.first_long_field + value.second_long_field\n', encoding='utf-8')
		(root / 'App.sirius').write_text(
			'import Ops alias O\n'
			'def main() -> i64\n'
			f'\tlet first = O.take({record(FIELDS, 20, 22)})\n'
			f'\tlet second = O.take({record(changed, 18, 24)})\n'
			'\tlet short = O.take({first_long_field = 19, second_long_field = 23})\n'
			'\treturn first + second + short\n', encoding='utf-8')
		listing, _ = verify(runner, cc, root, 126)
		long_names = [line.split(b' : ', 1)[0] for line in listing.splitlines()
			if line.startswith(b'M_3_Ops_4_take_G_') and len(line.split(b' : ', 1)[0]) > 256]
		assert len(long_names) == 2 and long_names[0] != long_names[1], listing
		assert b'M_3_Ops_4_take_G_' in listing, 'short names must preserve their encoding'

		# Transport exactly the same generic bodies through a source-free package.
		package = root / 'Ops.smirg'
		checked(runner, '--emit-generic', 'Ops', package, root / 'Ops.sirius')
		(root / 'Ops.sirius').unlink()
		packaged_listing, _ = verify(runner, cc, root, 126)
		assert sorted(long_names) == sorted(line.split(b' : ', 1)[0]
			for line in packaged_listing.splitlines()
			if line.startswith(b'M_3_Ops_4_take_G_') and len(line.split(b' : ', 1)[0]) > 256)

		# Concrete overloads use the same long-name fallback, but a different marker.
		(root / 'Ops.smirg').unlink()
		(root / 'Ops.sirius').write_text(
			'export rec Wide(' + ', '.join(f'{name}: i64' for name in FIELDS) + ')\n'
			+ ''.join(f'\tpub {name}\n' for name in FIELDS)
			+ 'export def calc(x: Wide) -> i64\n'
			'\treturn x.first_long_field + x.second_long_field\n'
			'export def calc(x: i64) -> i64\n\treturn x + 1\n', encoding='utf-8')
		(root / 'App.sirius').write_text(
			'import Ops alias O\ndef main() -> i64\n'
			'\treturn O.calc(O.Wide(20, 22, 0, 0, 0, 0)) + O.calc(0)\n', encoding='utf-8')
		overloads, _ = verify(runner, cc, root, 43)
		assert b'M_3_Ops_4_calc_O_' in overloads and b'M_3_Ops_4_calc_O_3_i64' in overloads, overloads
		assert any(len(line.split(b' : ', 1)[0]) > 256 for line in overloads.splitlines()), overloads

		# The relaxed name bound remains finite and is enforced on serialization.
		for length, accepted in ((4096, True), (4097, False)):
			name = 'f' + 'a' * (length - 1)
			long_source = root / 'long_export.sirius'
			long_source.write_text(f'def {name}() -> i64\n\treturn 1\n', encoding='utf-8')
			result = run(runner, '--emit-lib', root / 'long_export.smirl', '--export', name, long_source)
			if accepted:
				assert result.returncode == 0, result
			else:
				assert result.returncode != 0 and b'invalid MIR export name' in result.stderr, result
	print('PASS long structural symbols: generic/overload, source/package, length bounds, MIR/native ARC')


if __name__ == '__main__':
	main()
