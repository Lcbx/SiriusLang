#!/usr/bin/env python3
"""Declaration graphs and export-aware elimination through the native compiler."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, execute, link_flags, require_cc
from project_paths import ROOT, normalize_newlines


SOURCE = '''def ctor_helper(value: i64) -> i64
	print("KEEP_CTOR_HELPER")
	return value
enum Status
	Idle
	Busy
def index(value: i64) -> i64
	print("DROP_INDEX")
	return value
rec Live(value: i64)
	pub derived = ctor_helper(value)
rec Dead(value: i64)
	pub message = "DROP_CONSTRUCTOR"
	pub derived = dead_ctor_helper(value)
def dead_ctor_helper(value: i64) -> i64
	print("DROP_CTOR_HELPER")
	return value
export rec Public(value: i64)
export def kept(value: i64) -> i64
	print("KEEP_EXPORT")
	return export_helper(value)
def export_helper(value: i64) -> i64
	print("KEEP_EXPORT_HELPER")
	return value
def twice(value: i64) -> i64
	print("KEEP_REFERENCE")
	return value * 2
def apply(callback: Fn[i64,i64], value: i64) -> i64
	return callback(value)
def identity(value)
	return value
def shadow(value: i64) -> i64
	print("DROP_SHADOW")
	return value
def dead_a() -> i64
	print("DROP_CYCLE")
	return dead_b()
def dead_b() -> i64
	return dead_a()
def unchecked_unused() -> i64
	return missing_function()
def countdown(value: i64) -> i64
	if value == 0
		return 0
	return countdown(value - 1)
def main() -> i64
	let status: Status = Idle()
	if Status.index(status) != 0
		return 2
	let box = Live(2)
	let callback: Fn[i64,i64] = twice
	let shadow = fn(value: i64) value + 1
	if shadow(identity(apply(callback, box.derived))) != 5 or countdown(2) != 0
		return 1
	return 0
'''

HELPERS = '''export let CONSTANT = 42
export def callback(value: i64) -> i64
	return value + 1
export def kept() -> i64
	print("KEEP_MODULE_EXPORT")
	return private_helper()
def private_helper() -> i64
	print("KEEP_MODULE_HELPER")
	return 0
def dead() -> i64
	print("DROP_MODULE_FUNCTION")
	return 0
export rec Live(value: i64)
	pub value
rec Dead(value: i64)
	pub text = "DROP_MODULE_CONSTRUCTOR"
'''

APP = '''import Helpers alias H
import Facade alias F
def unused() -> i64
	print("DROP_APP_FUNCTION")
	return 0
def main() -> i64
	let callback: Fn[i64,i64] = F.again
	let record = H.Live(3)
	if callback(record.value) != 4 or F.NUMBER != 42
		return 1
	return 0
'''


def run(*command, code=0):
    result = subprocess.run([str(part) for part in command], capture_output=True, timeout=180)
    assert result.returncode == code, (command, result.returncode, result.stdout, result.stderr)
    return result


def literal(text):
    return '"' + ''.join(f'\\{byte:03o}' for byte in text.encode('utf-8')) + '"'


def code_markers(path, kept, removed):
    generated = path.read_text(encoding='utf-8')
    for marker in kept:
        assert literal(marker) in generated, ('missing public/reachable code', marker)
    for marker in removed:
        assert literal(marker) not in generated, ('emitted unused private code', marker)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = cache.native_tool('run_cli')
    with tempfile.TemporaryDirectory(prefix='sirius-call-graph-') as temporary:
        folder = Path(temporary)
        source = folder / 'single.sirius'
        source.write_text(SOURCE, encoding='utf-8')
        plain = folder / 'plain.c'
        generated = folder / 'graph.c'
        assert run(compiler, source, plain).stdout == b''
        first = run(compiler, '--call-graph', source, generated)
        graph = normalize_newlines(first.stdout).decode('utf-8')
        assert plain.read_bytes() == generated.read_bytes(), 'report changed code generation'
        assert first.stdout == run(compiler, source, generated, '--call-graph').stdout
        assert 'function kept [used] [exported]' in graph
        assert 'constructor Public [used] [exported]' in graph
        assert 'function main [used] [entry]' in graph
        assert '  calls Live\n' in graph and '  calls ctor_helper\n' in graph
        assert '  calls export_helper\n' in graph and '  references twice\n' in graph
        assert '  calls countdown\n' in graph and 'dynamic/member calls' in graph
        unused = graph.split('Unused functions and constructors', 1)[1]
        for name in ('constructor Dead', 'function dead_a', 'function dead_b',
                     'function dead_ctor_helper', 'function shadow', 'function unchecked_unused',
                     'constructor Busy', 'function index'):
            assert name + '\n' in unused, (name, graph)
        code_markers(generated,
                     ('KEEP_EXPORT', 'KEEP_EXPORT_HELPER', 'KEEP_REFERENCE', 'KEEP_CTOR_HELPER'),
                     ('DROP_CONSTRUCTOR', 'DROP_CTOR_HELPER', 'DROP_SHADOW', 'DROP_CYCLE', 'DROP_INDEX'))
        expected = b'KEEP_CTOR_HELPER\nKEEP_REFERENCE\n'
        check_native(cache, generated, stdout=expected)
        assert normalize_newlines(run(runner, source).stdout) == expected
        audited = folder / 'audited.exe'
        execute([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                 '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
                 *link_flags(cache.cc), '-o', str(audited)])
        assert normalize_newlines(run(audited).stdout) == expected
        # Public functions without local callers must also compile under GCC's
        # strict unused-function diagnostics, without compiler-specific attributes.
        gcc = shutil.which('gcc')
        if gcc:
            execute([gcc, *C_FLAGS, '-I', str(ROOT / 'runtime'), str(generated),
                     str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(gcc),
                     '-o', str(folder / 'gcc.exe')])

        (folder / 'Helpers.sirius').write_text(HELPERS, encoding='utf-8')
        (folder / 'Facade.sirius').write_text(
            'import Helpers alias H\nexport H.callback alias again\n'
            'export H.CONSTANT alias NUMBER\n', encoding='utf-8')
        (folder / 'App.sirius').write_text(APP, encoding='utf-8')
        module_c = folder / 'module.c'
        module_graph = normalize_newlines(run(compiler, '--call-graph', '--module-root',
                                             folder, 'App', module_c).stdout).decode('utf-8')
        assert '  references Helpers.callback\n' in module_graph, module_graph
        assert '  calls Helpers.Live\n' in module_graph, module_graph
        assert 'references Helpers.CONSTANT' not in module_graph
        assert 'function Helpers.kept [used] [exported]' in module_graph
        assert 'function Helpers.private_helper [used]' in module_graph
        unused = module_graph.split('Unused functions and constructors', 1)[1]
        assert 'function Helpers.dead\n' in unused and 'constructor Helpers.Dead\n' in unused
        assert 'function App.unused\n' in unused
        code_markers(module_c, ('KEEP_MODULE_EXPORT', 'KEEP_MODULE_HELPER'),
                     ('DROP_MODULE_FUNCTION', 'DROP_MODULE_CONSTRUCTOR', 'DROP_APP_FUNCTION'))
        check_native(cache, module_c)
        assert run(compiler, '--module-root', folder, 'App', plain).stdout == b''
        assert plain.read_bytes() == module_c.read_bytes()

        bad = folder / 'invalid.sirius'
        bad.write_text('def main() -> i64\n\treturn absent(1)\n', encoding='utf-8')
        failed = run(compiler, '--call-graph', bad, folder / 'invalid.c', code=1)
        assert b'Call graph' not in failed.stdout and not (folder / 'invalid.c').exists()
        assert run(compiler, '--call-graph', code=2).stdout == b''
        bad.write_text('export def kept() -> i64\n\treturn "wrong"\n'
                       'def main() -> i64\n\treturn 0\n', encoding='utf-8')
        run(compiler, bad, folder / 'bad_export.c', code=1)
        run(runner, bad, code=1)
    print('PASS call graph: constructors, references, recursion, shadowing, exports, '
          'unused cycles, pruning, modules/re-exports, deterministic output, ARC and GCC')


if __name__ == '__main__':
    main()
