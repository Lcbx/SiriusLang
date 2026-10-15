#!/usr/bin/env python
"""Ordered Sirius sources for the MIR default and retained legacy compilers."""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOKENIZER_SOURCES = ('src/tokenizer.sirius',)
PARSER_SOURCES = TOKENIZER_SOURCES + ('src/parser.sirius',)
FRONTEND_SOURCES = PARSER_SOURCES + (
	'src/types.sirius',
	'src/compile_common.sirius',
	'src/hir.sirius',
)
MIR_SOURCES = (
	'src/hir_specialize.sirius',
	'src/mir.sirius',
	'src/mir_common.sirius',
	'src/mir_lower.sirius',
	'src/mir_ownership.sirius',
	'src/mir_guard.sirius',
	'src/mir_lifetime.sirius',
	'src/mir_cleanup.sirius',
	'src/mir_opt.sirius',
	'src/mir_program.sirius',
	'src/mir_emit_c.sirius',
)
# The normal compiler and committed bootstrap include only the MIR emitter.
COMPILER_SOURCES = FRONTEND_SOURCES + MIR_SOURCES + ('src/cli.sirius',)
MIR_ONLY_COMPILER_SOURCES = COMPILER_SOURCES  # compatibility with MIR test tools
# Preserve legacy emission behind a separately compiled executable.
LEGACY_COMPILER_SOURCES = FRONTEND_SOURCES + ('src/emit_c.sirius',) + MIR_SOURCES + (
	'src/legacy_cli.sirius',)
MIR_DRIVER_SOURCES = COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius',)
MIR_RUNNER_SOURCES = FRONTEND_SOURCES + MIR_SOURCES[:-1] + (
	'src/mir_interpreter.sirius', 'src/mir_wire.sirius', 'src/mir_library.sirius', 'src/run_cli.sirius',)


def normalize_newlines(data: bytes) -> bytes:
	"""Normalize CRLF output and source bytes for portable comparisons."""
	return data.replace(b'\r\n', b'\n')


def combined_sources(sources: tuple[str, ...], separator: bytes = b'\n') -> bytes:
	"""Join sources with LF bytes; bootstrap concatenation uses no separator."""
	return separator.join(normalize_newlines((ROOT / name).read_bytes()) for name in sources)
