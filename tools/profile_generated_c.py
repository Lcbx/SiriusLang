#!/usr/bin/env python3
"""Report the largest generated MIR-C functions in a compiler bootstrap."""
import argparse
from pathlib import Path
import re

from project_paths import ROOT


DEFINITION = re.compile(r'(?m)^static [^\n]*\bs_mir_f([0-9]+)\([^\n]*\)\n\{\n')


def main() -> None:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('source', nargs='?', type=Path, default=ROOT / 'bootstrap/compiler.c')
	parser.add_argument('--top', type=int, default=20)
	args = parser.parse_args()
	if args.top < 1:
		parser.error('--top must be positive')
	content = args.source.read_text(encoding='utf-8')
	matches = list(DEFINITION.finditer(content))
	if not matches:
		parser.error('no generated MIR-C definitions found')
	end = content.find('\nint main(', matches[-1].end())
	if end < 0:
		end = len(content)
	# Per-function numbers characterize emitted C, not MIR liveness or peak
	# simultaneous register pressure. A separate interference analysis is
	# needed before reusing slots safely across CFG joins.
	functions = []
	for i, match in enumerate(matches):
		body = content[match.start():(matches[i + 1].start() if i + 1 < len(matches) else end)]
		registers = len(re.findall(r'(?m)^\t(?:S_String \*|S_MirRecord \*|int64_t |bool |double )r[0-9]+ = ', body))
		blocks = len(re.findall(r'(?m)^block[0-9]+: ;$', body))
		releases = len(re.findall(r'(?m)^\tif \(r[0-9]+\) \{ s_(?:mir_record|string)_release\(', body))
		functions.append((int(match.group(1)), len(body), registers, blocks, releases))
	sizes = sorted(functions, key=lambda entry: entry[1], reverse=True)
	print(f'Generated C: {len(content):,} bytes; {len(sizes)} definitions')
	print(f'Top {min(args.top, len(sizes))} functions: {sum(row[1] for row in sizes[:args.top]):,} bytes '
		f'({sum(row[1] for row in sizes[:args.top]) / len(content):.1%} of file)')
	print('Function ID   Bytes  Registers  CFG blocks  Guarded releases')
	for index, size, registers, blocks, releases in sizes[:args.top]:
		print(f'{index:11} {size:>9,} {registers:>10,} {blocks:>11,} {releases:>17,}')


if __name__ == '__main__':
	main()
