#!/usr/bin/env python3
"""Project paths and portable newline handling shared by build and test tools."""
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent.parent


def normalize_newlines(data: bytes) -> bytes:
	return data.replace(b'\r\n', b'\n')


def executable_path(path: Path, platform: str | None = None) -> Path:
	"""Return a native executable path with the platform-required suffix."""
	if platform is None:
		platform = sys.platform
	if platform == 'win32' and path.suffix.lower() != '.exe':
		return path.with_name(path.name + '.exe')
	return path
