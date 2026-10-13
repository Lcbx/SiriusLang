#!/usr/bin/env python
"""Audit explicit CFG trap guards and report non-unwinding operations."""
from build_support import BuildCache, execute, require_cc
from source_manifest import MIR_DRIVER_SOURCES


def main() -> None:
	cache = BuildCache(require_cc())
	driver = cache.sirius('mir-failure-contract', MIR_DRIVER_SOURCES + (
		'tests/mir/failure_contract_driver.sirius',
	))
	result = execute([str(driver)])
	if result.stdout.replace('\r\n', '\n') != 'PASS MIR failure contract\n':
		raise AssertionError(result)
	print('PASS MIR failure contract audit')


if __name__ == '__main__':
	main()
