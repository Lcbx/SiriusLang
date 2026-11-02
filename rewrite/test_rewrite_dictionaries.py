#!/usr/bin/env python3
"""Native dictionary regressions: CoW, managed elements, errors and ARC."""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from build_support import BuildCache, C_FLAGS, compile_c, execute, require_cc
from project_paths import ROOT, normalize_newlines


def run(command):
    return subprocess.run([str(item) for item in command], capture_output=True, timeout=60)


def sirius(code):
    # Test snippets use differing baseline indentation; normalize each to tabs.
    lines = code.strip().replace('\n    def ', '\ndef ').replace('\n    rec ', '\nrec ').splitlines()
    spaces = [len(line) - len(line.lstrip(' ')) for line in lines if line.startswith(' ')]
    base = min(spaces) if spaces else 4
    result = []
    for line in lines:
        count = len(line) - len(line.lstrip(' '))
        level = 1 + max(0, count - base) // 4 if count else 0
        result.append('\t' * level + line.lstrip(' '))
    return '\n'.join(result) + '\n'


def main():
    cache = BuildCache(require_cc())
    modules = tuple(ROOT / 'rewrite' / name for name in (
        'lexer.sirius', 'parser.sirius', 'semantic.sirius', 'ir.sirius',
        'lower.sirius', 'emit_c.sirius'))
    compiler = cache.native_driver('rewrite-dictionaries', ROOT / 'rewrite/main.sirius', extra_modules=modules)
    valid=[
    ('string keys and values', '''def main() -> i64
        let seed = Dict[string, string]()
        let first = seed.insert("x", "old")
        let second = first.insert("x", "new")
        print(len(seed))
        print(first[!"x"])
        print(second[!"x"])
        return 0
    ''', '0\nold\nnew\n'),
    ('numeric keys and booleans', '''def main() -> i64
        var bits: Dict[i64, bool] = Dict[i64, bool]()
        bits = bits.insert(2, true)
        let old = bits
        bits = bits.insert(2, false)
        print(old[!2])
        print(bits[!2])
        return 0
    ''','true\nfalse\n'),
    ('record value', '''rec Entry(name: string, ids: Array[i64])
    def main() -> i64
        var mapping: Dict[string, Entry] = Dict[string, Entry]()
        mapping = mapping.insert("A", Entry("alpha", [10]))
        let original = mapping
        mapping = mapping.insert("A", Entry("beta", [20]))
        print(original[!"A"].name)
        print(mapping[!"A"].ids[!0])
        return 0
    ''','alpha\n20\n'),
    ('dict in record', '''rec Scope(symbols: Dict[string, i64])
    def main() -> i64
        var state = Scope(Dict[string, i64]().insert("a", 2))
        let snapshot = state
        state.symbols = state.symbols.insert("a", 7)
        print(snapshot.symbols[!"a"])
        print(state.symbols[!"a"])
        return 0
    ''','2\n7\n'),
    ('array of dictionaries', '''def main() -> i64
        let row = Dict[string, string]().insert("x", "yes")
        var rows = [row]
        let old = rows
        rows[!0] = rows[!0].insert("x", "no")
        print(old[!0][!"x"])
        print(rows[!0][!"x"])
        return 0
    ''','yes\nno\n'),
    ('nested dictionary', '''def lookup(d: Dict[string, Dict[i64, i64]]) -> i64
        return d[!"main"][!5]
    def main() -> i64
        let inner = Dict[i64, i64]().insert(5, 11)
        let outer = Dict[string, Dict[i64, i64]]().insert("main", inner)
        print(lookup(outer))
        return 0
    ''','11\n'),
    ('self-insert', '''def main() -> i64
        var d = Dict[string, string]().insert("x", "value")
        d = d.insert("x", d[!"x"])
        print(d[!"x"])
        return 0
    ''','value\n'),
    ('empty dict iteration', 'def main() -> i64\n    let names = Dict[string, i64]()\n    var seen = 0\n    for name in names\n        seen += 1\n    print(seen)\n    return 0', '0\n'),
    ('record dict keys snapshot', 'rec Binding(id: i64, name: string)\ndef main() -> i64\n    var symbols = Dict[string, Binding]().insert("x", Binding(1, "one"))\n    symbols = symbols.insert("y", Binding(2, "two"))\n    let before = symbols\n    var total = 0\n    for key in symbols\n        total += before[!key].id\n        symbols = symbols.insert("z", Binding(3, "three"))\n    print(total)\n    print(len(before))\n    print(len(symbols))\n    return 0', '3\n2\n3\n'),
    ('integer keys continue and break', 'def main() -> i64\n    var d = Dict[i64, string]()\n    for i in 0..8\n        d = d.insert(i, str(i))\n    var total = 0\n    for key in d\n        if key == 3\n            continue\n        total += key\n    print(total)\n    var count = 0\n    for key in d\n        count += 1\n        break\n    print(count)\n    return 0', '25\n1\n'),
    ('nested dict loops', 'def main() -> i64\n    let d = Dict[i64, i64]().insert(1, 4).insert(2, 5)\n    var product = 0\n    for x in d\n        for y in d\n            product += x * y\n    print(product)\n    return 0', '9\n'),
    ]
    invalid=[
    ('unhashable key', 'def main() -> i64\n    let x = Dict[Array[i64], i64]()\n    return 0\n'),
    ('wrong key', 'def main() -> i64\n    let x = Dict[string, i64]().insert(1, 2)\n    return 0\n'),
    ('wrong value', 'def main() -> i64\n    let x = Dict[string, i64]().insert("a", "wrong")\n    return 0\n'),
    ('invalid lookup', 'def main() -> i64\n    let x = Dict[string, i64]()\n    print(x[!0])\n    return 0\n'),
    ('wrong arity', 'def main() -> i64\n    let x = Dict[string, i64]().insert("x")\n    return 0\n'),
    ('no mutation through let','def main() -> i64\n    let x = Dict[string, i64]()\n    x = x.insert("a", 1)\n    return 0\n'),
    ('iterate integer', 'def main() -> i64\n    for key in 7\n        print(key)\n    return 0\n'),
    ('iterate string', 'def main() -> i64\n    for key in "abc"\n        print(key)\n    return 0\n'),
    ]
    bounds=[('missing key','def main() -> i64\n    let x = Dict[string, i64]()\n    print(x[!"absent"])\n    return 0\n')]

    with tempfile.TemporaryDirectory() as td:
        root = Path(td)
        runtime = root / 'runtime.o'
        execute([cache.cc, *C_FLAGS, '-DSIRIUS_ARC_AUDIT', '-I', str(ROOT / 'runtime'),
                 '-c', str(ROOT / 'runtime/sirius_runtime.c'), '-o', str(runtime)])
        source, generated, binary = (root / name for name in ('source.sirius', 'generated.c', 'program'))
        for name, code, expected in valid:
            source.write_text(sirius(code), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, (name, 'compile', result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode == 0 and normalize_newlines(result.stdout) == expected.encode() and not result.stderr, (name, 'run', result)
        print(f'PASS dictionaries: {len(valid)} ARC-audited native programs', flush=True)
        for name, code in invalid:
            source.write_text(sirius(code), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 1 and b'ERROR ' in result.stderr, (name, result)
        print(f'PASS dictionaries: {len(invalid)} invalid programs', flush=True)
        for name, code in bounds:
            source.write_text(sirius(code), encoding='utf-8')
            result = run([compiler, source, generated])
            assert result.returncode == 0, (name, 'compile', result)
            compile_c(cache.cc, generated, binary, runtime)
            result = run([binary])
            assert result.returncode != 0 and b'missing dictionary key' in result.stderr, (name, 'trap', result)
        print(f'PASS dictionaries: {len(bounds)} missing-key traps', flush=True)


if __name__ == '__main__':
    main()
