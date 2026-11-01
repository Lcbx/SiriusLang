#!/usr/bin/env python3
"""Managed Optional[T] lookup, defaults, unwinding and ARC regressions."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines

MODULES = tuple(ROOT / 'rewrite' / name for name in (
    'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
    'lower.sirius', 'emit_c.sirius'))

def run(*args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, timeout=60)

VALID = [
    ('scalar and lazy missing default', '''
def main() -> i64
    let d = Dict[string, i64]().insert("zero", 0)
    print(d.get("zero") or (1 / 0))
    print(d.get("absent") or 19)
    print(d.get("zero")!)
    return 0
''', b'0\n19\n0\n'),
    ('boolean and alias', '''
def main() -> i64
    let d = Dict[i64, bool]().insert(1, false)
    let a: Optional[bool] = d.get(1)
    print(a or true)
    print(d.get(2) or true)
    print(a!)
    return 0
''', b'false\ntrue\nfalse\n'),
    ('string snapshots and assignment', '''
def main() -> i64
    let d = Dict[string, string]().insert("a", "hello")
    var x: Optional[string] = d.get("a")
    let saved = x
    x = d.get("missing")
    print(saved or "wrong")
    print(x or "fallback")
    print(saved!)
    return 0
''', b'hello\nfallback\nhello\n'),
    ('record symbol table', '''
rec Binding(node: i64, label: string)
def resolve(bindings: Dict[string, Binding], name: string) -> Optional[Binding]
    return bindings.get(name)
def main() -> i64
    let names = Dict[string, Binding]().insert("x", Binding(7, "original"))
    let found: Optional[Binding] = resolve(names, "x")
    let shadow = found
    let missing = resolve(names, "y")
    let value = shadow or Binding(99, "default")
    let other = missing or Binding(8, "absent")
    print(value.node)
    print(value.label)
    print(other.label)
    print(found!.node)
    return 0
''', b'7\noriginal\nabsent\n7\n'),
    ('arrays optional payload', '''
def main() -> i64
    let source = ["first", "second"]
    let d = Dict[string, Array[string]]().insert("x", source)
    let found: Optional[Array[string]] = d.get("x")
    let copy = found
    let values = copy or ["wrong"]
    let missing = d.get("y") or ["fallback"]
    print(values[!1])
    print(missing[!0])
    return 0
''', b'second\nfallback\n'),
    ('nested dictionary payload', '''
def main() -> i64
    let inner = Dict[string, string]().insert("key", "value")
    let d = Dict[i64, Dict[string, string]]().insert(5, inner)
    let opt = d.get(5)
    let copy = opt
    let chosen = copy or Dict[string, string]()
    print(chosen[!"key"])
    print(len(d.get(0) or Dict[string, string]()))
    return 0
''', b'value\n0\n'),
    ('optional early exit and calls', '''
def lookup(d: Dict[string, string], key: string) -> Optional[string]
    return d.get(key)
def must(d: Dict[string, string], key: string) -> string
    let found = lookup(d, key) else return "no"
    return found
def main() -> i64
    let d = Dict[string, string]().insert("a", "yes")
    print(must(d, "a"))
    print(must(d, "b"))
    return 0
''', b'yes\nno\n'),
    ('record early exit', '''
rec Entry(text: string, count: i64)
def check(d: Dict[string, Entry], key: string) -> i64
    let found = d.get(key) else return -1
    return found.count
def main() -> i64
    let d = Dict[string, Entry]().insert("x", Entry("abc", 15))
    print(check(d, "x"))
    print(check(d, "missing"))
    return 0
''', b'15\n-1\n'),
]
INVALID = [
    'def main() -> i64\n    let d = Dict[string, i64]()\n    print(d.get(2) or 0)\n    return 0\n',
    'def main() -> i64\n    let a = [1, 2]\n    print(a.get(0) or 0)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let v: Optional[i64] = d.get("x")\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    print(d.get("x") or 1)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let result = d.get("x")!\n    return result\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let v: Optional[string] = d.get("x")\n    print(v)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    print(str(d.get("x")))\n    return 0\n',
]
TRAPS = [
    ('missing string', 'def main() -> i64\n    let d = Dict[string, string]()\n    print(d.get("none")!)\n    return 0\n'),
    ('missing record', 'rec Item(name: string)\ndef main() -> i64\n    let d = Dict[string, Item]()\n    let v = d.get("none")!\n    print(v.name)\n    return 0\n'),
]

def main():
    cache = BuildCache(require_cc())
    compiler = cache.native_driver('rewrite-optionals', ROOT / 'rewrite/main.sirius', extra_modules=MODULES)
    with tempfile.TemporaryDirectory() as td:
        root = Path(td)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, generated, binary = (root / name for name in ('source.sirius', 'generated.c', 'program'))
        for name, program, expected in VALID:
            source.write_text(program.strip().replace('    ', '\t') + '\n')
            result = run(compiler, source, generated)
            assert result.returncode == 0, (name, 'compile', result.stderr)
            compile_c(cache.cc, generated, binary, runtime)
            result = run(binary)
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected and not result.stderr, (name, 'run', result)
        print(f'PASS Optionals: {len(VALID)} ARC-audited native programs', flush=True)
        for program in INVALID:
            source.write_text(program.replace('    ', '\t'))
            result = run(compiler, source, generated)
            assert result.returncode == 1 and b'ERROR' in result.stderr, (program, result)
        print(f'PASS Optionals: {len(INVALID)} invalid programs', flush=True)
        for name, program in TRAPS:
            source.write_text(program.replace('    ', '\t'))
            result = run(compiler, source, generated)
            assert result.returncode == 0, (name, result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run(binary)
            assert result.returncode != 0 and b'missing optional value' in result.stderr, (name, result)
        print(f'PASS Optionals: {len(TRAPS)} missing-value traps', flush=True)

if __name__ == '__main__':
    main()
