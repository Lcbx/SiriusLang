#!/usr/bin/env python3
"""Check local Markdown document links against checked-in files."""
import re
from tempfile import TemporaryDirectory
from pathlib import Path
from urllib.parse import unquote

from project_paths import ROOT


LINK = re.compile(r'\[[^\]]+\]\(([^)]+)\)')
ROOTED_PATH = re.compile(r'(?<![\w/])(?:src|tools|tests|runtime|bootstrap|examples)/[\w./-]+\.(?:sirius|py|c|h|txt|md)\b')


def broken_links(document: Path) -> list[str]:
	bad = []
	in_fence = False
	for line_number, line in enumerate(document.read_text(encoding='utf-8').splitlines(), 1):
		if line.lstrip().startswith('```'):
			in_fence = not in_fence
			continue
		if in_fence:
			continue
		for raw in LINK.findall(line):
			path = unquote(raw.split('#', 1)[0])
			if path and not re.match(r'^[a-zA-Z][a-zA-Z0-9+.-]*:', path) and not (document.parent / path).exists():
				bad.append(f'{document.name}:{line_number}: {raw}')
	return bad


def main() -> None:
	with TemporaryDirectory() as temporary:
		root = Path(temporary)
		(root / 'existing.md').write_text('# Existing\n', encoding='utf-8')
		fixture = root / 'links.md'
		fixture.write_text('[found](existing.md#title)\n[missing](lost.md)\n```\n[ignored](also_lost.md)\n```\n', encoding='utf-8')
		assert broken_links(fixture) == ['links.md:2: lost.md']
	with_links = sorted(path for path in ROOT.rglob('*.md') if '.build' not in path.parts)
	errors = [error for doc in with_links for error in broken_links(doc)]
	# Paths written as inline code or plain text do not use Markdown links,
	# but should still refer to files in the current source tree.
	for doc in with_links:
		for number, line in enumerate(doc.read_text(encoding='utf-8').splitlines(), 1):
			for reference in ROOTED_PATH.findall(line):
				if not (ROOT / reference).exists():
					errors.append(f'{doc.relative_to(ROOT)}:{number}: {reference}')
	if errors:
		raise AssertionError('Broken documentation references:\n' + '\n'.join(errors))
	print(f'PASS links and source paths in {len(with_links)} Markdown documents')


if __name__ == '__main__':
	main()
