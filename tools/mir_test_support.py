"""Shared native drivers for MIR tests."""
from pathlib import Path

from build_support import BuildCache
from project_paths import ROOT


def ownership_transfers_driver(cache: BuildCache) -> Path:
	fixtures = ROOT / 'tests/mir'
	modules = tuple(fixtures / name for name in (
		'call_move_driver_native.sirius', 'constructor_move_driver_native.sirius',
		'credit_driver_native.sirius', 'opt_driver_native.sirius'))
	return cache.native_driver('mir-ownership-transfers',
		fixtures / 'ownership_transfers_driver_native.sirius', extra_modules=modules)
