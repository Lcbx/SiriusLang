#!/usr/bin/env python
"""Ordered Sirius sources for the self-hosted MIR compiler and interpreter."""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOKENIZER_SOURCES = ('src/tokenizer.sirius',)
PARSER_SOURCES = TOKENIZER_SOURCES + ('src/parser.sirius',)
FRONTEND_SOURCES = PARSER_SOURCES + (
	'src/types.sirius',
	'src/compile_common.sirius',
	'src/hir.sirius', 'src/hir_resolve.sirius', 'src/hir_constants.sirius', 'src/hir_declarations.sirius', 'src/hir_analyze.sirius',
	'src/module.sirius',
	'src/module_resolve.sirius',
)
MODULE_FRONTEND_SOURCES = FRONTEND_SOURCES + ('src/module_interface.sirius',)
MODULE_SCANNER_SOURCES = MODULE_FRONTEND_SOURCES + ('src/module_scan.sirius', 'src/module_scan_cli.sirius')
MIR_SOURCES = (
	'src/hir_specialize.sirius',
	'src/mir.sirius',
	'src/mir_cfg.sirius',
	'src/mir_verify.sirius',
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
MIR_DRIVER_SOURCES = COMPILER_SOURCES[:-1] + ('src/mir_interpreter.sirius',)
MIR_RUNNER_SOURCES = MODULE_FRONTEND_SOURCES + MIR_SOURCES[:-1] + (
	'src/mir_interpreter.sirius', 'src/mir_wire.sirius', 'src/mir_library.sirius', 'src/mir_link.sirius',
	'src/module_scan.sirius', 'src/module_build.sirius', 'src/run_cli.sirius',)


def normalize_newlines(data: bytes) -> bytes:
	"""Normalize CRLF output and source bytes for portable comparisons."""
	return data.replace(b'\r\n', b'\n')


def combined_sources(sources: tuple[str, ...], separator: bytes = b'\n') -> bytes:
	"""Join sources with LF bytes; bootstrap concatenation uses no separator."""
	return separator.join(normalize_newlines((ROOT / name).read_bytes()) for name in sources)
