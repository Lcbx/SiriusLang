"""Locations and ordered source list for the native-only rewrite compiler.

BuildCache.native_driver stages these modules by basename in its temporary
module root, so preserve their source identities while reorganizing folders.
"""
from project_paths import ROOT

REWRITE = ROOT / 'rewrite'
SOURCE = REWRITE / 'src'
DRIVERS = REWRITE / 'tests' / 'drivers'
BENCHMARKS = REWRITE / 'benchmarks'
ENTRY = SOURCE / 'main.sirius'
MODULES = tuple(SOURCE / name for name in (
    'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
    'lower.sirius', 'emit_c.sirius', 'modules.sirius'))
