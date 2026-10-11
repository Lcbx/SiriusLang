#!/usr/bin/env python
"""Ordered Sirius sources used by the C-only development and test tools."""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOKENIZER_SOURCES = ('src/tokenizer.sirius',)
PARSER_SOURCES = TOKENIZER_SOURCES + ('src/parser.sirius',)
COMPILER_SOURCES = PARSER_SOURCES + (
	'src/types.sirius',
	'src/compile_common.sirius',
	'src/hir.sirius',
	'src/emit_c.sirius',
	'src/hir_specialize.sirius',
	'src/mir.sirius',
	'src/mir_common.sirius',
	'src/mir_lower.sirius',
	'src/mir_ownership.sirius',
	'src/mir_guard.sirius',
	'src/mir_lifetime.sirius',
	'src/mir_cleanup.sirius',
	'src/mir_program.sirius',
	'src/mir_emit_c.sirius',
	'tests/emitter/driver.sirius',
)
MIR_DRIVER_SOURCES = COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius',)
# Standalone compiler sources without the legacy C emitter. Do not use as the
# default until this compiler can compile itself and pass the regression suite.
MIR_ONLY_COMPILER_SOURCES = tuple(source for source in COMPILER_SOURCES
	if source not in ('src/emit_c.sirius', 'tests/emitter/driver.sirius')) + (
	'tests/emitter/mir_driver.sirius',)



def normalize_newlines(data: bytes) -> bytes:
	"""Normalize CRLF output and source bytes for portable comparisons."""
	return data.replace(b'\r\n', b'\n')


def combined_sources(sources: tuple[str, ...], separator: bytes = b'\n') -> bytes:
	"""Join sources with LF bytes, regardless of the checkout's line endings.

	The self-host fixed point uses separator=b'' to preserve its historical
	concatenation exactly; other tests historically inserted extra newlines.
	"""
	return separator.join(normalize_newlines((ROOT / name).read_bytes()) for name in sources)
