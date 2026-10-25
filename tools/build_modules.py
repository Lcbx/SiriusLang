#!/usr/bin/env python3
"""Optional launcher and standalone scanner client for Sirius source modules.

The Sirius runner owns discovery, typed imports, compilation and linking.
"""
import argparse
import os
from dataclasses import dataclass
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from build_support import BuildCache, require_cc


class ModuleError(Exception):
	pass


@dataclass(frozen=True)
class Export:
	name: str
	parameters: str
	returns: str


@dataclass
class Module:
	name: str
	path: Path
	imports: dict[str, str]
	exports: dict[str, Export]
	constants: dict[str, str]
	types: dict[str, str]
	has_main: bool


def symbol(module: str, name: str) -> str:
	"""Unambiguous, identifier-safe encoding (including underscores)."""
	return 'M' + ''.join(f'_{len(part)}_{part}' for part in (*module.split('.'), name))


def execute(*argv: str) -> subprocess.CompletedProcess[str]:
	result = subprocess.run(argv, text=True, capture_output=True, encoding='utf-8')
	if result.returncode:
		raise ModuleError(f'command failed ({result.returncode}): {" ".join(argv)}\n{result.stdout}{result.stderr}')
	return result


def default_scanner() -> Path:
	return BuildCache(shutil.which('clang') or require_cc()).native_tool('module_scan_cli')


def discover(root: Path, entry: str, scanner: Path | None = None) -> list[Module]:
	root = root.resolve()
	scanner = scanner or default_scanner()
	text = execute(str(scanner), str(root), entry).stdout
	lines = text.splitlines()
	if not lines or lines[0] != 'SIRIUS-MODULES/1':
		raise ModuleError('unrecognized module manifest')
	ordered: list[Module] = []
	current: Module | None = None
	seen: set[str] = set()
	for line in lines[1:]:
		if not line:
			continue
		parts = line.split('\t')
		if len(parts) == 2 and parts[0] == 'M':
			name = parts[1]
			if not name or name in seen or any(not section.isidentifier() for section in name.split('.')):
				raise ModuleError(f'invalid/duplicate module in manifest: {name}')
			path = root.joinpath(*name.split('.')).with_suffix('.sirius')
			if not path.is_file() or not path.resolve().is_relative_to(root):
				raise ModuleError(f'module {name} not found under source root: {path}')
			current = Module(name, path, {}, {}, {}, {}, False)
			ordered.append(current)
			seen.add(name)
		elif current is not None and len(parts) == 3 and parts[0] == 'I':
			_, alias, dependency = parts
			if not alias or alias in current.imports:
				raise ModuleError(f'duplicate/invalid import alias: {alias}')
			current.imports[alias] = dependency
		elif current is not None and len(parts) == 4 and parts[0] == 'E':
			_, name, parameters, returns = parts
			if not name or name in current.exports or not returns:
				raise ModuleError(f'duplicate/invalid exported function: {name}')
			current.exports[name] = Export(name, parameters, returns)
		elif current is not None and len(parts) == 3 and parts[0] == 'C':
			_, name, type_name = parts
			if not name or name in current.exports or name in current.constants or not type_name:
				raise ModuleError(f'duplicate/invalid exported constant: {name}')
			current.constants[name] = type_name
		elif current is not None and len(parts) == 3 and parts[0] == 'U':
			_, name, type_name = parts
			if not name or name in current.types or name in current.exports or name in current.constants or not type_name:
				raise ModuleError(f'duplicate/invalid exported union: {name}')
			current.types[name] = type_name
		elif current is not None and len(parts) == 3 and parts[0] == 'T':
			_, name, type_name = parts
			if not name or name in current.types or name in current.exports or name in current.constants or not type_name:
				raise ModuleError(f'duplicate/invalid exported constructor: {name}')
			current.types[name] = type_name
		elif current is not None and parts == ['F', 'main']:
			if current.has_main:
				raise ModuleError('duplicate main marker in module manifest')
			current.has_main = True
		else:
			raise ModuleError(f'invalid module manifest record: {line}')
	if not ordered or ordered[-1].name != entry:
		raise ModuleError('module manifest does not end with requested entry module')
	for module in ordered:
		if any(dependency not in seen for dependency in module.imports.values()):
			raise ModuleError(f'missing dependency in manifest for {module.name}')
	return ordered


def build(root: Path, entry: str, output: Path, runner: Path, scanner: Path | None = None) -> str:
	"""Delegate module discovery, compilation, and in-memory linking to Sirius."""
	if scanner is not None:
		raise ModuleError('--scanner is only supported by the standalone discovery command')
	output = output.resolve()
	output.parent.mkdir(parents=True, exist_ok=True)
	execute(str(runner), '--build-modules', str(root.resolve()), entry, str(output))
	return symbol(entry, 'main')


def main() -> int:
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--root', type=Path, required=True, help='source root containing dotted-path modules')
	parser.add_argument('--entry', required=True, help='dotted entry module path')
	parser.add_argument('--output', type=Path, required=True, help='linked concrete MIR library')
	parser.add_argument('--runner', type=Path, default=Path('./sirius-run'), help='built Sirius MIR runner')
	parser.add_argument('--scanner', type=Path, help='native Sirius scanner (built automatically if omitted)')
	parser.add_argument('--run', action='store_true', help='run main() from linked artifact')
	args, rest = parser.parse_known_args()
	try:
		runner = args.runner.resolve()
		if not runner.is_file():
			raise ModuleError(f'MIR runner not found: {runner} (build with tools/build_mir_runner.py)')
		name = build(args.root, args.entry, args.output, runner, args.scanner.resolve() if args.scanner else None)
		if args.run:
			if rest and rest[0] == '--':
				rest = rest[1:]
			result = subprocess.run([str(runner), '--entry', name, '--load-lib', str(args.output.resolve()), *rest])
			return result.returncode
		if rest:
			raise ModuleError(f'unexpected arguments: {rest}')
		print(f'Linked {args.output} (entry: {name})')
		return 0
	except (ModuleError, OSError) as exc:
		print(f'module error: {exc}', file=sys.stderr)
		return 2


if __name__ == '__main__':
	sys.exit(main())
