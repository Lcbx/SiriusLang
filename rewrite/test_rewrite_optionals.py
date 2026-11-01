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
    ('explicit scalar optional construction', '''
def main() -> i64
    let a = i64?(0)
    let b = i64?()
    let present = bool?(false)
    let absent = bool?()
    print(a or 20)
    print(b or 21)
    print(present or true)
    print(absent or true)
    print(i64?(-12)!)
    return 0
''', b'0\n21\nfalse\ntrue\n-12\n'),
    ('explicit string snapshots and calls', '''
def wrap(value: string) -> Optional[string]
    return string?(value)
def missing() -> Optional[string]
    return string?()
def main() -> i64
    var value = string?("original")
    let saved = value
    let result = wrap(value!)
    value = missing()
    print(saved or "wrong")
    print(result!)
    print(value or "missing")
    return 0
''', b'original\noriginal\nmissing\n'),
    ('explicit record, array, dict constructors with ownership', '''
rec Binding(id: i64, text: string)
def choose(value: Optional[Binding]) -> string
    let binding = value or Binding(9, "absent")
    return binding.text
def main() -> i64
    let record = Binding?(Binding(3, "found"))
    let none = Binding?()
    let saved = record
    let arr = Array[string]?( ["a", "b"] )
    let noarr = Array[string]?()
    let dict = Dict[string, Binding]?(Dict[string, Binding]().insert("x", Binding(2, "item")))
    print(choose(record))
    print(choose(none))
    print(saved!.id)
    print((arr or ["wrong"])[!1])
    print(len(noarr or Array[string]()))
    print((dict or Dict[string, Binding]())[!"x"].text)
    return 0
''', b'found\nabsent\n3\nb\n0\nitem\n'),
    ('explicit nested optionals preserve some none', '''
def main() -> i64
    let empty = Optional[i64]?()
    let some_none = Optional[i64]?(i64?())
    let some_some = Optional[i64]?(i64?(5))
    print((empty or i64?(99)) or 12)
    print((some_none or i64?(99)) or 12)
    print((some_some or i64?(99)) or 12)
    let copied = some_none
    print((copied or i64?(99)) or 12)
    return 0
''', b'99\n12\n5\n12\n'),
    ('explicit optional array elements and CoW', '''
def main() -> i64
    var items = [string?("first"), string?()]
    let snapshot = items
    items[!0] = string?("changed")
    items = items.append(string?("last"))
    print(snapshot[!0] or "wrong")
    print(snapshot[!1] or "missing")
    print(items[!0] or "wrong")
    print(items[!2]!)
    print((items[?8] or string?("outer missing")) or "inner missing")
    return 0
''', b'first\nmissing\nchanged\nlast\nouter missing\n'),
    ('explicit optional record field and dictionary values', '''
rec Entry(name: string)
rec Scope(binding: Optional[Entry])
def main() -> i64
    var scopes = Dict[i64, Scope]().insert(1, Scope(Entry?(Entry("original"))))
    let copy = scopes
    scopes = scopes.insert(1, Scope(Entry?()))
    print((scopes[!1].binding or Entry("none")).name)
    print(copy[!1].binding!.name)
    return 0
''', b'none\noriginal\n'),
    ('optional constructors and indexed access do not conflict', '''
def main() -> i64
    let values = [i64?(4), i64?()]
    let source = values[?0]
    print((source or i64?()) or 6)
    print((values[?1] or i64?(7)) or 6)
    print((values[?9] or i64?(7)) or 6)
    let outer = Array[Optional[i64]]?(values)
    print(((outer or Array[Optional[i64]]())[?0] or i64?(9)) or 10)
    return 0
''', b'4\n6\n7\n4\n'),
    ('optional array indexing integers and boundaries', '''
def main() -> i64
    let a = [0, 14, -3]
    print(a[?0] or 99)
    print(a[?-1] or 99)
    print(a[?-3]!)
    print(a[?3] or 20)
    print(a[?-4] or 21)
    print(Array[i64]()[?0] or 22)
    return 0
''', b'0\n-3\n0\n20\n21\n22\n'),
    ('optional array indexing managed string and snapshots', '''
def take(values: Array[string], i: i64) -> Optional[string]
    return values[?i]
def main() -> i64
    var items = ["first", "second"]
    let saved = items
    let first = take(items, 0)
    let last = items[?-1]
    items[!0] = "changed"
    print(first or "wrong")
    print(last!)
    print(saved[?0] or "wrong")
    print(items[?0] or "wrong")
    print(items[?99] or "fallback")
    return 0
''', b'first\nsecond\nfirst\nchanged\nfallback\n'),
    ('optional array indexing records and recursive values', '''
rec Binding(node: i64, text: string)
rec Bucket(values: Array[Binding])
def lookup(b: Bucket, i: i64) -> Optional[Binding]
    return b.values[?i]
def main() -> i64
    let values = [Binding(3, "left"), Binding(4, "right")]
    let bucket = Bucket(values)
    let present = lookup(bucket, -1)
    let copy = present
    print((copy or Binding(9, "wrong")).text)
    print((lookup(bucket, 9) or Binding(-1, "missing")).node)
    print(present!.node)
    return 0
''', b'right\n-1\n4\n'),
    ('optional array indexing booleans and dictionary keys', '''
def main() -> i64
    let flags = [true, false]
    print(flags[?0] or false)
    print(flags[?1] or true)
    print(flags[?2] or true)
    let index = Dict[i64, bool]().insert(2, false)
    print(index[?2] or true)
    print(index[?-1] or true)
    return 0
''', b'true\nfalse\ntrue\nfalse\ntrue\n'),
    ('optional array indexing nested optionals and dictionaries', '''
def main() -> i64
    let names = Dict[string, string]().insert("yes", "present")
    let a = [names.get("yes"), names.get("missing")]
    let found = a[?0]
    print((found or names.get("missing")) or "wrong")
    print((a[?1] or names.get("yes")) or "wrong")
    print((a[?8] or names.get("missing")) or "absent")
    let dict = Dict[string, Array[string]]().insert("a", ["one"])
    print((dict[?"a"] or ["wrong"])[?0] or "wrong")
    print(len(dict[?"missing"] or Array[string]()))
    return 0
''', b'present\nwrong\nabsent\none\n0\n'),
    ('optional array indexing managed arrays', '''
def main() -> i64
    var values = [["a", "b"], ["c"]]
    let snapshot = values
    let first = values[?0] or ["wrong"]
    values[!0][!1] = "changed"
    print(first[?1] or "wrong")
    print((values[?0] or ["wrong"])[?1] or "wrong")
    print((snapshot[?0] or ["wrong"])[?1] or "wrong")
    print(len(values[?2] or Array[string]()))
    return 0
''', b'b\nchanged\nb\n0\n'),
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
    ('optional integer elements preserve zero and absence', '''
def main() -> i64
    var found = ["a".find("a"), "a".find("z")]
    let saved = found
    found[!1] = "b".find("b")
    print(saved[!0] or 99)
    print(saved[!1] or 42)
    print(found[!1]!)
    return 0
''', b'0\n42\n0\n'),
    ('optional array stored in record', '''
rec Config(values: Optional[Array[string]])
def main() -> i64
    let lookup = Dict[string, Array[string]]().insert("yes", ["original"])
    var config = Config(lookup.get("yes"))
    let snapshot = config
    config.values = lookup.get("no")
    print((snapshot.values or ["wrong"])[!0])
    print((config.values or ["default"])[!0])
    return 0
''', b'original\ndefault\n'),
    ('optional string record field and updates', '''
rec Box(value: Optional[string], id: i64)
def main() -> i64
    let names = Dict[string, string]().insert("present", "a\\0é")
    var box = Box(names.get("present"), 1)
    let saved = box
    box.value = names.get("missing")
    box.id += 1
    print(saved.value or "wrong")
    print(box.value or "fallback")
    print(box.id)
    box.value = names.get("present")
    print(box.value!)
    return 0
''', b'a\x00\xc3\xa9\nfallback\n2\na\x00\xc3\xa9\n'),
    ('optional record inside record and dictionary', '''
rec Binding(node: i64, label: string)
rec Scope(binding: Optional[Binding])
def choose(scope: Scope) -> string
    let entry = scope.binding else return "absent"
    return entry.label
def main() -> i64
    let names = Dict[string, Binding]().insert("a", Binding(3, "saved"))
    let a = Scope(names.get("a"))
    let b = Scope(names.get("missing"))
    let scopes = Dict[i64, Scope]().insert(1, a)
    let before = scopes
    print(choose(scopes[!1]))
    print(choose(b))
    print(before.get(1)!.binding!.node)
    return 0
''', b'saved\nabsent\n3\n'),
    ('optional array elements and CoW writes', '''
def main() -> i64
    let lookup = Dict[string, string]().insert("a", "old")
    var values = [lookup.get("a"), lookup.get("b")]
    let saved = values
    values[!0] = lookup.get("b")
    values = values.append(lookup.get("a"))
    print(saved[!0] or "wrong")
    print(saved[!1] or "empty")
    print(values[!0] or "missing")
    print(values[!-1]!)
    return 0
''', b'old\nempty\nmissing\nold\n'),
    ('nested optional dictionary values', '''
def main() -> i64
    let names = Dict[string, string]().insert("ok", "here")
    var d = Dict[i64, Optional[string]]().insert(1, names.get("ok"))
    d = d.insert(2, names.get("absent"))
    let snapshot = d
    d = d.insert(1, names.get("absent"))
    print(snapshot[!1] or "wrong")
    print(snapshot[!2] or "none")
    print((snapshot.get(1) or names.get("absent")) or "wrong")
    print((snapshot.get(3) or names.get("absent")) or "none")
    print(d[!1] or "new")
    return 0
''', b'here\nnone\nhere\nnone\nnew\n'),
    ('optional elements nested in record arrays', '''
rec Entry(text: string)
rec Bucket(items: Array[Optional[Entry]])
def main() -> i64
    let symbols = Dict[string, Entry]().insert("x", Entry("stored"))
    var buckets = [Bucket([symbols.get("x"), symbols.get("missing")])]
    let saved = buckets
    buckets[!0].items[!0] = symbols.get("missing")
    print(saved[!0].items[!0]!.text)
    print((saved[!0].items[!1] or Entry("none")).text)
    print((buckets[!0].items[!0] or Entry("changed")).text)
    return 0
''', b'stored\nnone\nchanged\n'),
]
INVALID = [
    'def main() -> i64\n    let a = i64?("wrong")\n    return 0\n',
    'def main() -> i64\n    let a = bool?(3)\n    return 0\n',
    'def main() -> i64\n    let a = i64?(1, 2)\n    return 0\n',
    'def main() -> i64\n    let a = i64?(1,)\n    return 0\n',
    'def main() -> i64\n    let a = Unknown?()\n    return 0\n',
    'def main() -> i64\n    let a = Optional[i64]?(10)\n    return 0\n',
    'def main() -> i64\n    let a = Optional[i64]()\n    return 0\n',
    'def main() -> i64\n    let a: Optional[i64] = bool?()\n    return 0\n',
    'def main() -> i64\n    let a = Array[i64]?("invalid")\n    return 0\n',
    'def main() -> i64\n    let a = Dict[string, i64]?(Array[i64]())\n    return 0\n',
    'def main() -> i64\n    let a = [1]\n    let o: Optional[string] = a[?0]\n    return 0\n',
    'def main() -> i64\n    let a = [1]\n    print(a[?"wrong"] or 0)\n    return 0\n',
    'def main() -> i64\n    let a = [1]\n    print(a[?0] + 1)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, i64]()\n    print(d[?1] or 0)\n    return 0\n',
    'def main() -> i64\n    let a = [1]\n    print(a[?0 or 0)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, i64]()\n    print(d.get(2) or 0)\n    return 0\n',
    'def main() -> i64\n    let a = [1, 2]\n    print(a.get(0) or 0)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let v: Optional[i64] = d.get("x")\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    print(d.get("x") or 1)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let result = d.get("x")!\n    return result\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    let v: Optional[string] = d.get("x")\n    print(v)\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, string]()\n    print(str(d.get("x")))\n    return 0\n',
    'def main() -> i64\n    let d = Dict[string, i64]()\n    let values: Array[Optional[bool]] = [d.get("x")]\n    return 0\n',
]
TRAPS = [
    ('explicit absent i64 unwrap', 'def main() -> i64\n    return i64?()!\n'),
    ('explicit absent nested unwrap', 'def main() -> i64\n    let x = Optional[i64]?()\n    let value = x!\n    return value or 0\n'),
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
