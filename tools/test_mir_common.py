#!/usr/bin/env python
"""Verify shared MIR semantics independently of the two execution backends."""
import argparse

from build_support import BuildCache, check_sirius_driver, options, require_cc


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	options(parser)
	args = parser.parse_args()
	cache = BuildCache(require_cc(), rebuild=args.rebuild)
	output = check_sirius_driver(cache, 'mir-common', (
		'src/types.sirius', 'src/compile_common.sirius', 'src/mir.sirius', 'src/mir_cfg.sirius', 'src/mir_verify.sirius', 'src/mir_common.sirius',
		'tests/mir/common_driver.sirius',
	), 'PASS MIR backend common semantics\n')
	print(output, end='')


if __name__ == '__main__':
	main()
