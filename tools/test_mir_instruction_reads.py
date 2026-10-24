#!/usr/bin/env python3
"""Check allocation-free MIR operand-read fast paths and their semantics."""
from build_support import BuildCache, execute, require_cc
from project_paths import ROOT


def main() -> None:
	cache = BuildCache(require_cc())
	output = execute([str(cache.native_driver('mir-instruction-reads', ROOT / 'tests/mir/instruction_reads_driver_native.sirius'))]).stdout
	assert output == 'PASS MIR instruction reads\n', output
	print(output, end='')


if __name__ == '__main__':
	main()
