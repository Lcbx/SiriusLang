#!/usr/bin/env python
"""Run the shared compiler fixtures through the standalone MIR-only backend."""
import argparse
from pathlib import Path
import tempfile

from build_support import BuildCache, assert_native, execute, options, require_cc, run_native, validate_jobs
from compiler_cases import check_cases
from source_manifest import MIR_ONLY_COMPILER_SOURCES


# Two legacy negative fixtures require only backend limitations, not
# language errors: MIR supports f64 arrays and Optional fallbacks in elif.
NEGATIVE_EXCLUDE = ('04_unsupported_array', '27_optional_elif')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	compiler = cache.sirius('mir-only-regression', MIR_ONLY_COMPILER_SOURCES, separator=b'')
	with tempfile.TemporaryDirectory() as temporary:
		work = Path(temporary)
		check_cases(cache.cc, compiler, work, jobs=validate_jobs(args.jobs),
			runtime=cache.runtime(), negative_exclude=NEGATIVE_EXCLUDE)
		elif_case = work / 'optional_elif.sirius'
		elif_case.write_text('def main() -> i64\n\tlet table = Dict[i64, i64]()\n\tif false\n\t\tprint(1)\n\telif (table.get(1) or 0) == 0\n\t\tprint(2)\n\treturn 0\n')
		output = work / 'optional_elif.c'
		execute([str(compiler), str(elif_case), str(output)])
		assert_native(run_native(cache, output), stdout=b'2\n')
		print('PASS MIR-only Optional fallback in elif')
		f64_case = work / 'f64_array.sirius'
		f64_case.write_text('def main() -> i64\n\tlet xs = [1.0, 2.0]\n\tif xs[!0] == 1.0 and xs[!1] == 2.0\n\t\treturn 0\n\treturn 1\n')
		output = work / 'f64_array.c'
		execute([str(compiler), str(f64_case), str(output)])
		generated = output.read_text(encoding='utf-8')
		if 'bool init' in generated or 'uninitialized register' in generated:
			raise AssertionError('Verified MIR C must not emit redundant runtime register checks')
		assert_native(run_native(cache, output))
		print('PASS MIR-only f64 arrays and comparisons, no runtime register checks')
		f64_dict = work / 'f64_dict.sirius'
		f64_dict.write_text(
			'def main() -> i64\n'
			'\tvar d = Dict[i64, f64]()\n'
			'\td = d.insert(1, 2.5)\n'
			'\tlet snapshot = d\n'
			'\td = d.insert(1, 3.75)\n'
			'\tif (snapshot.get(1) or 0.0) == 2.5 and (d.get(1) or 0.0) == 3.75\n'
			'\t\treturn 0\n'
			'\treturn 1\n')
		output = work / 'f64_dict.c'
		execute([str(compiler), str(f64_dict), str(output)])
		assert_native(run_native(cache, output))
		print('PASS MIR-only f64 dictionary values and CoW')


if __name__ == '__main__':
	main()
