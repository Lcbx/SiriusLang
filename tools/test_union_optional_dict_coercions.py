#!/usr/bin/env python3
"""Check recursive union conversion through Optional and dictionaries."""
import argparse
import subprocess
import tempfile
from pathlib import Path

from build_support import BuildCache, C_FLAGS, execute, link_flags, require_cc, check_native
from project_paths import ROOT


CASES = (
	('source_union_optional_dict', 15),
	('source_union_dict_conversion', 14),
	('source_union_dict_managed', 4),
	('source_union_dict_growth', 31),
	('source_union_optional_fallback', 0),
)

INVALID = (
	('optional primitive', 'let result: Optional[Result] = Dict[i64,i64]().get(1)'),
	('dictionary key mismatch', 'let result: Dict[i64,Result] = Dict[string,Success]()'),
	('dictionary value mismatch', 'let result = Dict[i64,Result]().insert(1, 42)'),
	('dictionary wrong variant', 'let result = Dict[i64,Result]().insert(1, Other(true))'),
	('nested dictionary mismatch', 'let result: Dict[i64,Array[Result]] = Dict[i64,Array[i64]]()'),
	('fallback primitive', 'let result = Dict[i64,Result]().get(0) or 42'),
	('fallback wrong variant', 'let result = Dict[i64,Result]().get(0) or Other(true)'),
	('fallback ambiguous stored record', 'let raw = Success(1)\n\tlet result = Dict[i64,Twins]().get(0) or raw'),
	('fallback unrelated label', 'let result = Dict[i64,Twins]().get(0) or Unrelated(1)'),
	('fallback wrong nested element', 'let result = Dict[i64,Array[Result]]().get(0) or [Other(true)]'),
	('fallback incompatible callback', 'let result = Dict[i64,Fn[i64,i64]]().get(0) or fn(x: string) len(x)'),
)

PREFIX = '''rec Success(value: i64)
rec Failure(code: i64)
rec Other(flag: bool)
rec Twin(value: i64)
rec Unrelated(value: i64)
let Result = Success | Failure
let Twins = Success | Twin
def main() -> i64
'''

PROVIDER = '''export rec Good(value: i64)
export rec Bad(value: i64)
export let Result = Good | Bad

export def choose(mapping, key: i64) -> Result
    return mapping.get(key) or Bad(9)
'''

APP = '''import Provider alias P

# A copied generic fallback must use the defining module's constructor.
rec Bad(value: bool)

def score(result: P.Result) -> i64
    match result
        P.Good(value)
            return value
        P.Bad(value)
            return -value

def main() -> i64
    let mapping = Dict[i64,P.Result]().insert(1, P.Good(7))
    if score(P.choose(mapping, 1)) != 7 or score(P.choose(mapping, 99)) != -9
        return 1
    let fallback = mapping.get(99) or P.Bad(4)
    if score(fallback) != -4
        return 2
    return 0
'''


def audit(cache, generated, binary, code):
	execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cache.cc), '-o', str(binary)])
	result = subprocess.run([str(binary)], capture_output=True)
	assert (result.returncode, result.stdout, result.stderr) == (code, b'', b''), result


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--compiler', type=Path, help='Use an existing native compiler')
	parser.add_argument('--runner', type=Path, help='Use an existing MIR runner')
	args = parser.parse_args()
	cache = BuildCache(require_cc())
	compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
	runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
	with tempfile.TemporaryDirectory() as directory:
		folder = Path(directory)
		for name, code in CASES:
			source = ROOT / 'tests/mir' / f'{name}.sirius'
			generated = folder / f'{name}.c'
			execute([str(compiler), str(source), str(generated)])
			check_native(cache, generated, code=code)
			interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
			assert interpreted.returncode == code and not interpreted.stderr, (name, interpreted)
			if name in ('source_union_dict_managed', 'source_union_optional_fallback'):
				audit(cache, generated, folder / 'managed_audit', code)
		for name, line in INVALID:
			source = folder / 'invalid.sirius'
			generated = folder / 'invalid.c'
			generated.unlink(missing_ok=True)
			source.write_text(PREFIX + '\t' + line + '\n\treturn 0\n', encoding='utf-8')
			compiled = subprocess.run([str(compiler), str(source), str(generated)], capture_output=True)
			interpreted = subprocess.run([str(runner), str(source)], capture_output=True)
			assert compiled.returncode != 0 and interpreted.returncode != 0 and not generated.exists(), (name, compiled, interpreted)
		for name, text in (('Provider', PROVIDER), ('App', APP)):
			(folder / f'{name}.sirius').write_text(text.replace('    ', '\t'), encoding='utf-8')
		for packaged in (False, True):
			if packaged:
				execute([str(runner), '--emit-generic', 'Provider', str(folder / 'Provider.smirg'), str(folder / 'Provider.sirius'), str(folder)])
				(folder / 'Provider.sirius').unlink()
			generated = folder / 'fallback_app.c'
			execute([str(compiler), '--module-root', str(folder), 'App', str(generated)])
			audit(cache, generated, folder / 'module_audit', 0)
			library = folder / 'App.smirl'
			execute([str(runner), '--build-modules', str(folder), 'App', str(library)])
			interpreted = subprocess.run([str(runner), '--entry', 'M_3_App_4_main', '--load-lib', str(library)], capture_output=True)
			assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (0, b'', b''), interpreted
	print('PASS Optional/Dict union coercions: contextual lazy fallbacks, ARC/CoW, source-free imports, native/MIR, negatives')


if __name__ == '__main__':
	main()
