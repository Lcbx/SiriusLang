"""Build stage-1 with Python, then check the Sirius stage-2/3 fixed point."""
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
SOURCES = (
	'src/tokenizer.sirius',
	'src/parser.sirius',
	'src/emit_c.sirius',
	'tests/emitter/driver.sirius',
)
REFERENCES = (
	'01_scalars', '02_branches', '03_loops', '04_recursion',
	'06_constructor', '07_record_spread', '10_generic', '17_dict',
	'20_structural_layout', '29_strings_and_arguments', '36_string_byte_at',
)
EMITTER_CASES = (
	'50_ordered_calls', '51_managed_constructor', '52_adjacent_if',
	'53_field_index_update', '54_shadowing',
)


def execute(args: list[str]) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(args, capture_output=True, text=True, encoding='utf-8')
	if result.returncode:
		raise RuntimeError(f'{" ".join(args)}\n{result.stdout}{result.stderr}')
	return result


def compile_c(cc: str, source: Path, output: Path) -> None:
	execute([cc, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic',
		'-I', str(ROOT / 'include'), str(source),
		str(ROOT / 'include/sirius_runtime.c'), '-lm', '-o', str(output)])


def main() -> None:
	cc = shutil.which('cc') or shutil.which('gcc')
	if cc is None:
		raise SystemExit('A C11 compiler (cc or gcc) is required')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		source = work / 'compiler.sirius'
		# Stage source is byte-oriented: normalize Windows checkout line endings.
		source.write_bytes(b''.join((ROOT / part).read_bytes().replace(b'\r\n', b'\n') for part in SOURCES))
		stage0_c = work / 'stage0_output.c'
		execute([sys.executable, str(ROOT / 'bootstrap_python/sirius.py'),
			'compile', str(source), '-o', str(stage0_c)])
		stage1 = work / ('stage1.exe' if sys.platform == 'win32' else 'stage1')
		compile_c(cc, stage0_c, stage1)

		stage2_c = work / 'stage2.c'
		execute([str(stage1), str(source), str(stage2_c)])
		stage2 = work / ('stage2.exe' if sys.platform == 'win32' else 'stage2')
		compile_c(cc, stage2_c, stage2)

		stage3_c = work / 'stage3.c'
		execute([str(stage2), str(source), str(stage3_c)])
		if stage2_c.read_bytes() != stage3_c.read_bytes():
			raise AssertionError('Stage-2 and stage-3 generated C differ')
		print('PASS stage-2/stage-3 byte-identical C')

		committed = ROOT / 'bootstrap_c/compiler.c'
		# Git's Windows checkout may convert the committed C to CRLF.
		# Compare canonical LF content, but keep stage-2/stage-3 byte comparison strict.
		generated = stage3_c.read_bytes()
		checked_in = committed.read_bytes().replace(b'\r\n', b'\n')
		if generated != checked_in:
			from itertools import zip_longest
			for line, (expected, actual) in enumerate(zip_longest(
				checked_in.split(b'\n'), generated.split(b'\n')), 1):
				if expected != actual:
					raise AssertionError(
						f'Committed bootstrap C differs at line {line} (not just CRLF):\n'
						f'committed: {str(expected[:160])}\n'
						f'generated: {str(actual[:160])}')

		print('PASS committed bootstrap C matches')
		stage3 = work / ('stage3.exe' if sys.platform == 'win32' else 'stage3')
		compile_c(cc, stage3_c, stage3)

		for stem in REFERENCES + EMITTER_CASES:
			if stem in REFERENCES:
				folder = ROOT / 'tests'
			else:
				folder = ROOT / 'tests/emitter'
			input_file = folder / 'fixtures' / f'{stem}.sirius'
			normalized = work / f'{stem}.sirius'
			normalized.write_bytes(input_file.read_bytes().replace(b'\r\n', b'\n'))
			generated = work / f'{stem}.c'
			execute([str(stage3), str(normalized), str(generated)])
			binary = work / (f'{stem}.exe' if sys.platform == 'win32' else stem)
			compile_c(cc, generated, binary)
			output = execute([str(binary)]).stdout
			expected = (folder / 'expected_output' / f'{stem}.txt').read_text(encoding='utf-8')
			if output != expected:
				raise AssertionError(f'{stem}: expected {expected!r}, got {output!r}')
			print(f'PASS stage-3 {stem}')
		print(f'Self-hosting verified; {len(REFERENCES) + len(EMITTER_CASES)} executable checks passed')


if __name__ == '__main__':
	main()
