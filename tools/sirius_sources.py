"""Ordered Sirius sources used by the C-only development and test tools."""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOKENIZER_SOURCES = ('src/tokenizer.sirius',)
PARSER_SOURCES = TOKENIZER_SOURCES + ('src/parser.sirius',)
COMPILER_SOURCES = PARSER_SOURCES + (
	'src/types.sirius',
	'src/emit_c.sirius',
	'tests/emitter/driver.sirius',
)


def combined_sources(sources: tuple[str, ...], separator: bytes = b'\n') -> bytes:
	"""Join sources with LF bytes, regardless of the checkout's line endings.

	The self-host fixed point uses separator=b'' to preserve its historical
	concatenation exactly; other tests historically inserted extra newlines.
	"""
	return separator.join((ROOT / name).read_bytes().replace(b'\r\n', b'\n') for name in sources)
