#!/usr/bin/env python3
"""End-to-end source-module compilation using independent concrete MIR artifacts."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_modules import ModuleError, build, discover, symbol
from build_support import BuildCache, require_cc
from source_manifest import MIR_RUNNER_SOURCES


def write(root: Path, module: str, source: str) -> None:
	path = root.joinpath(*module.split('.')).with_suffix('.sirius')
	path.parent.mkdir(parents=True, exist_ok=True)
	path.write_text(source, encoding='utf-8')


def run(*args: str) -> subprocess.CompletedProcess[bytes]:
	return subprocess.run(args, capture_output=True, timeout=30)


def check_failure(root: Path, entry: str, reason: str) -> None:
	try:
		discover(root, entry)
	except ModuleError as exc:
		assert reason in str(exc), (reason, exc)
	else:
		raise AssertionError(f'expected {reason!r} for module {entry}')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--runner', type=Path)
	args = parser.parse_args()
	runner = args.runner.resolve() if args.runner else BuildCache(shutil.which('clang') or require_cc()).sirius('mir-runner', MIR_RUNNER_SOURCES)
	with tempfile.TemporaryDirectory() as tmp:
		root = Path(tmp)
		write(root, 'A.B', 'export def value() -> i64\n\treturn 19\n')
		write(root, 'C.B', 'export def value() -> i64\n\treturn 23\n')
		write(root, 'Utility.Combine',
			'import A.B alias First\nimport C.B alias Second\n'
			'export def total() -> i64\n'
			'\tlet label = "First.value()"  # Second.value() is not a call\n'
			'\tif label == "First.value()"\n'
			'\t\treturn First.value() + Second.value()\n'
			'\treturn 0\n')
		write(root, 'Main',
			'import Utility.Combine alias Combine\n'
			'def main() -> i64\n\treturn Combine.total()\n')
		output = root / 'linked.smirl'
		entry = build(root, 'Main', output, runner)
		assert entry == symbol('Main', 'main')
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (42, b'', b''), result
		print('PASS source modules: transitive imports, qualified functions and isolated public names')

		first = output.read_bytes()
		build(root, 'Main', output, runner)
		assert output.read_bytes() == first, 'linked libraries must be deterministic'
		listing = run(str(runner), '--list-lib', str(output))
		assert listing.returncode == 0
		for name in ('A.B', 'C.B'):
			assert symbol(name, 'value').encode() in listing.stdout
		print('PASS source modules: reproducible output, canonical mangled exports')

		# The module builder does not rewrite bodies or string/comment contents.
		original = next(m for m in discover(root, 'Main') if m.name == 'Utility.Combine')
		assert 'First.value()' in original.path.read_text()
		assert 'Second.value() is not a call' in original.path.read_text()
		assert 'total' in original.exports
		assert symbol('A.B', 'value') != symbol('A_B', 'value')
		print('PASS source modules: untouched source, collision-free encoded paths')

		write(root, 'Private', 'def secret() -> i64\n\treturn 7\nexport def safe() -> i64\n\treturn 1\n')
		write(root, 'Bad', 'import Private alias P\ndef main() -> i64\n\treturn P.secret()\n')
		try:
			build(root, 'Bad', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'not exported' in str(exc), exc
		else:
			raise AssertionError('private function incorrectly accessible')
		write(root, 'Missing', 'import Not.Present\ndef main() -> i64\n\treturn 0\n')
		check_failure(root, 'Missing', 'not found')
		write(root, 'Cycle1', 'import Cycle2\nexport def step() -> i64\n\treturn 1\n')
		write(root, 'Cycle2', 'import Cycle1\nexport def step() -> i64\n\treturn 2\n')
		check_failure(root, 'Cycle1', 'cyclic source imports')
		write(root, 'Duplicate', 'import A.B alias Same\nimport C.B alias Same\ndef main() -> i64\n\treturn 0\n')
		check_failure(root, 'Duplicate', 'duplicate import alias')
		write(root, 'Generic', 'export def hidden(x) -> i64\n\treturn 1\n')
		assert discover(root, 'Generic')[-1].exports['hidden'].parameters == 'x'
		write(root, 'UnsupportedType', 'export def other(x: Shape) -> i64\n\treturn 1\n')
		check_failure(root, 'UnsupportedType', 'unsupported exported signature')
		print('PASS source modules: missing, cycles, private names, aliases and unsupported signatures rejected')

		write(root, 'Lists', 'export def count(xs: Array[Dict[string, i64]]) -> i64\n\treturn len(xs)\n')
		assert discover(root, 'Lists')[-1].exports['count'].parameters == 'xs: Array[Dict[string, i64]]'
		print('PASS source modules: nested generic type annotations')

		# Exercise typed HIR import signatures, not just scanner metadata.
		write(root, 'Collection.Fns',
			'export def size(values: Array[Dict[string, i64]]) -> i64\n'
			'\treturn len(values)\n')
		write(root, 'CollectionApp',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\tlet values = Dict[string, i64]().insert("answer", 42)\n'
			'\treturn F.size([values])\n')
		entry = build(root, 'CollectionApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (1, b'', b''), result
		write(root, 'WrongArgType',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\treturn F.size([4])\n')
		try:
			build(root, 'WrongArgType', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'expected' in str(exc), exc
		else:
			raise AssertionError('incompatible imported collection type accepted')
		write(root, 'WrongArgCount',
			'import Collection.Fns alias F\n'
			'def main() -> i64\n'
			'\treturn F.size()\n')
		try:
			build(root, 'WrongArgCount', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'argument count mismatch' in str(exc), exc
		else:
			raise AssertionError('incorrect imported function arity accepted')
		print('PASS source modules: typed nested collections, incompatible signatures rejected')

		# Structural record types are re-interned in the importing module. The
		# caller constructs a shape independently of the exporter's constructor.
		write(root, 'Geometry',
			'rec Point(x: i64, y: i64)\n'
			'export let ORIGIN = {x = 0, y = 0}\n'
			'export def make() -> Point\n'
			'\treturn Point(7, 5)\n'
			'export def score(value: Point) -> i64\n'
			'\treturn value.x * 10 + value.y\n'
			'export def many(points: Array[Point]) -> i64\n'
			'\treturn len(points)\n')
		write(root, 'GeometryApp',
			'import Geometry alias G\n'
			'def main() -> i64\n'
			'\tlet point = G.make()\n'
			'\treturn G.score(point) + G.many([point]) + G.score(G.ORIGIN)\n')
		entry = build(root, 'GeometryApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (76, b'', b''), result
		write(root, 'GeometryWrong',
			'import Geometry alias G\n'
			'def main() -> i64\n'
			'\treturn G.score({x=7, z=5})\n')
		try:
			build(root, 'GeometryWrong', root / 'wrong.smirl', runner)
		except ModuleError as exc:
			assert 'expected' in str(exc), exc
		else:
			raise AssertionError('incorrect imported record shape accepted')
		print('PASS source modules: imported structural records and nested collection signatures')

		# Public constructors are compiled locally from canonical typed metadata.
		# No source rewriting or runtime symbol is needed for construction.
		write(root, 'Shapes',
			'export rec Point(x: i64, y: i64)\n'
			'rec Hidden(secret: i64)\n'
			'export def score(point: Point) -> i64\n'
			'\treturn point.x * 10 + point.y\n')
		write(root, 'ShapeApp',
			'import Shapes alias S\n'
			'def main() -> i64\n'
			'\tlet point: S.Point = S.Point(3, 4)\n'
			'\tlet points: Array[S.Point] = [point]\n'
			'\treturn S.score(points[!0])\n')
		entry = build(root, 'ShapeApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (34, b'', b''), result
		listing = run(str(runner), '--list-lib', str(output))
		assert symbol('Shapes', 'Point').encode() not in listing.stdout, 'constructors must not become MIR imports'
		write(root, 'ShapeWrong',
			'import Shapes alias S\n'
			'def main() -> i64\n'
			'\treturn S.Point(1, "bad").x\n')
		try:
			build(root, 'ShapeWrong', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'expected i64, found string' in str(exc), exc
		else:
			raise AssertionError('incorrect imported constructor type accepted')
		write(root, 'ShapePrivate',
			'import Shapes alias S\n'
			'def main() -> i64\n'
			'\tlet item: S.Hidden = S.Point(1, 2)\n'
			'\treturn item.secret\n')
		try:
			build(root, 'ShapePrivate', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'not exported' in str(exc), exc
		else:
			raise AssertionError('private constructor type accepted')
		write(root, 'ShapeArity',
			'import Shapes alias S\n'
			'def main() -> i64\n'
			'\treturn S.Point(1).x\n')
		try:
			build(root, 'ShapeArity', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'argument count mismatch' in str(exc), exc
		else:
			raise AssertionError('incorrect constructor arity accepted')
		print('PASS source modules: public constructors, qualified type annotations, nested types, privacy and errors')

		# An exporting module may reference a public constructor imported from
		# another module. Its interface must carry canonicalized type shapes.
		write(root, 'Shape.Types',
			'export rec Point(x: i64, y: i64)\n')
		write(root, 'Shape.Gateway',
			'import Shape.Types alias T\n'
			'export def roundtrip(value: T.Point) -> T.Point\n'
			'\treturn value\n'
			'export def make() -> T.Point\n'
			'\treturn T.Point(5, 7)\n'
			'export rec Wrapper(value: T.Point)\n'
			'export def length(values: Array[Dict[string, T.Point]]) -> i64\n'
			'\treturn len(values)\n')
		write(root, 'ShapeApp',
			'import Shape.Gateway alias G\n'
			'rec LocalPoint(x: i64, y: i64)\n'
			'def main() -> i64\n'
			'\tlet point = G.roundtrip(G.make())\n'
			'\tlet points = Dict[string, LocalPoint]().insert("one", point)\n'
			'\tlet wrapped = G.Wrapper(point)\n'
			'\treturn point.x * 10 + point.y + G.length([points]) + wrapped.value.y\n')
		signature = discover(root, 'Shape.Gateway')[-1]
		assert signature.exports['roundtrip'].parameters == 'value: T.Point'
		assert signature.exports['make'].returns == 'T.Point'
		assert 'Wrapper' in signature.types
		entry = build(root, 'ShapeApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (65, b'', b''), result
		write(root, 'ShapeBadArg',
			'import Shape.Gateway alias G\n'
			'def main() -> i64\n'
			'\treturn G.roundtrip({x=1, z=2}).x\n')
		try:
			build(root, 'ShapeBadArg', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'expected' in str(exc), exc
		else:
			raise AssertionError('invalid transitive record shape accepted')
		write(root, 'Shape.Private', 'rec Hidden(value: i64)\n')
		write(root, 'ShapeBadExport',
			'import Shape.Private alias P\n'
			'export def reveal(value: P.Hidden) -> i64\n'
			'\treturn value.value\n')
		check_failure(root, 'ShapeBadExport', 'not exported')
		write(root, 'ShapeBadAlias',
			'export def reveal(value: Unknown.Point) -> i64\n'
			'\treturn value.x\n')
		check_failure(root, 'ShapeBadAlias', 'unknown module alias')
		print('PASS source modules: exported imported-type signatures, nested shapes, privacy and unknown aliases')

		# Constants cross module boundaries as typed HIR values; no runtime
		# constant getter or synthetic source declaration is necessary.
		write(root, 'Settings',
			'export let WIDTH: i64 = 12\n'
			'export let ENABLED = true\n'
			'export let NAME = "hi"\n'
			'export let CONFIG = {size = WIDTH, title = NAME, levels = [3, 4]}\n'
			'let PRIVATE = 90\n')
		write(root, 'SettingsApp',
			'import Settings alias S\n'
			'def main() -> i64\n'
			'\tif not S.ENABLED\n'
			'\t\treturn 2\n'
			'\tif S.NAME != "hi"\n'
			'\t\treturn 1\n'
			'\treturn S.CONFIG.size + S.CONFIG.levels[!1] + S.WIDTH\n')
		assert discover(root, 'SettingsApp')[0].constants['WIDTH'] == 'i64'
		entry = build(root, 'SettingsApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (28, b'', b''), result
		write(root, 'SettingsPrivate',
			'import Settings alias S\n'
			'def main() -> i64\n'
			'\treturn S.PRIVATE\n')
		try:
			build(root, 'SettingsPrivate', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'not exported' in str(exc), exc
		else:
			raise AssertionError('private module constant incorrectly accessible')
		print('PASS source modules: exported scalar/record/array constants, privacy')

		# Generic exports are instantiated in their defining modules on demand.
		# A transitive import can request two concrete specializations, while
		# another consumer reuses an already compiled signature.
		write(root, 'GenericOps',
			'export def size(x) -> i64\n'
			'\treturn len(x)\n'
			'export def total(point) -> i64\n'
			'\treturn point.x + point.y\n')
		write(root, 'GenericRelay',
			'import GenericOps alias O\n'
			'export def size(x) -> i64\n'
			'\treturn O.size(x)\n')
		write(root, 'GenericApp',
			'import GenericOps alias O\n'
			'import GenericRelay alias R\n'
			'def main() -> i64\n'
			'\treturn R.size("ab") + R.size([1, 2, 3]) + O.size("hi") + O.total({x=20, y=15})\n')
		entry = build(root, 'GenericApp', output, runner)
		result = run(str(runner), '--entry', entry, '--load-lib', str(output))
		assert (result.returncode, result.stdout, result.stderr) == (42, b'', b''), result
		first = output.read_bytes()
		build(root, 'GenericApp', output, runner)
		assert output.read_bytes() == first, 'generic specializations must link deterministically'
		listing = run(str(runner), '--list-lib', str(output))
		assert listing.returncode == 0
		assert symbol('GenericOps', 'size').encode() + b'_G_' in listing.stdout
		print('PASS source modules: on-demand generic specializations, transitive calls, structural arguments, deterministic linking')

		write(root, 'GenericBad',
			'import GenericOps alias O\n'
			'def main() -> i64\n'
			'\treturn O.total({wrong=1})\n')
		try:
			build(root, 'GenericBad', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'unknown record field' in str(exc), exc
		else:
			raise AssertionError('invalid generic specialization accepted')
		write(root, 'GenericArity',
			'import GenericOps alias O\n'
			'def main() -> i64\n'
			'\treturn O.size(1, 2)\n')
		try:
			build(root, 'GenericArity', root / 'bad.smirl', runner)
		except ModuleError as exc:
			assert 'argument count mismatch' in str(exc), exc
		else:
			raise AssertionError('incorrect generic argument count accepted')
		print('PASS source modules: invalid generic specializations and arity rejected')

		# The runner exposes namespaced exports without requiring source rewriting.
		unnamed = root / 'unnamed.sirius'
		unnamed.write_text('def value() -> i64\n\treturn 9\n')
		artifact = root / 'renamed.smirl'
		assert run(str(runner), '--emit-lib', str(artifact), '--export-as', 'value', 'Renamed', str(unnamed)).returncode == 0
		assert run(str(runner), '--entry', 'Renamed', '--load-lib', str(artifact)).returncode == 9
		assert run(str(runner), '--emit-lib', str(artifact), '--export-as', 'value', 'bad.name', str(unnamed)).returncode == 2
		print('PASS source modules: explicitly aliased library exports validated')


if __name__ == '__main__':
	main()
