#!/usr/bin/env python3
"""Heterogeneous labeled union payloads survive source-free generic packages."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc
from project_paths import ROOT


PROVIDER = '''export rec Meters(value: i64)
    pub value
export rec Feet(value: i64)
    pub value
export rec Tiny(ok: bool)
    pub ok
export rec Large(code: i64, text: string, items: Array[i64])
    pub code
    pub text
    pub items
export rec Empty()
export rec Raw(value: i64)
    pub value
export rec Wide(code: i64, text: string, items: Array[i64])
    pub code
    pub text
    pub items
export let Reading = Meters | Feet | Tiny | Large | Empty

export def make_meters(n) -> Reading
    return Meters(n)

export def make_large(n) -> Reading
    return Large(n, "hey", [1, 2])

export def explicit_feet(data) -> Reading
    return Reading.Feet(data)

export def explicit_large(data) -> Reading
    return Reading.Large(data)

export def append_meters(values, data)
    return values.append(Reading.Meters(data))

export def score(value)
    match value
        Meters(n)
            return n
        Feet(n)
            return 2 * n
        Tiny(ok)
            if ok
                return 5
            return 0
        Large(n, text, items)
            return n + len(text) + len(items)
        Empty()
            return 1
'''.replace('    ', '\t')

CONSUMER = '''import Ops alias O
# Names in the importer must not capture generic definitions from Ops.
rec Meters(value: i64)
rec Feet(value: i64)
rec Large(n: i64)
let Reading = Meters | Feet

def main() -> i64
    let raw = O.Raw(7)
    let wide = O.Wide(6, "ok", [1, 2, 3])
    let values: Array[O.Reading] = [O.make_meters(3), O.explicit_feet(raw), O.make_large(4)]
    let snapshot = values
    let grown = O.append_meters(values, O.Raw(5))
    let table = Dict[i64,O.Reading]().insert(2, O.explicit_large(wide))
    let found = table.get(2) else return 99
    if len(snapshot) != 3 or len(grown) != 4
        return 98
    return O.score(snapshot[!0]) + O.score(snapshot[!1]) + O.score(snapshot[!2]) + O.score(grown[!3]) + O.score(found)
'''.replace('    ', '\t')


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
    return subprocess.run([str(runner), *(str(arg) for arg in args)], capture_output=True, timeout=90)


def checked(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
    result = run(runner, *args)
    assert result.returncode == 0, (args, result.returncode, result.stdout, result.stderr)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runner', type=Path)
    parser.add_argument('--cc', default=shutil.which('clang') or require_cc())
    args = parser.parse_args()
    cache = BuildCache(shutil.which(args.cc) or args.cc)
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    with tempfile.TemporaryDirectory(prefix='sirius-hetero-package-') as directory:
        root = Path(directory)
        provider = root / 'Ops.sirius'
        provider.write_text(PROVIDER, encoding='utf-8')
        package = root / 'Ops.smirg'
        checked(runner, '--emit-generic', 'Ops', package, provider)
        serialized = package.read_bytes()
        checked(runner, '--emit-generic', 'Ops', package, provider)
        assert package.read_bytes() == serialized
        provider.unlink()
        (root / 'App.sirius').write_text(CONSUMER, encoding='utf-8')
        linked = root / 'app.smirl'
        checked(runner, '--build-modules', root, 'App', linked)
        data = linked.read_bytes()
        checked(runner, '--build-modules', root, 'App', linked)
        assert linked.read_bytes() == data
        symbol = 'M_3_App_4_main'
        interpreted = run(runner, '--entry', symbol, '--load-lib', linked)
        assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (42, b'', b''), interpreted
        generated = root / 'app.c'
        checked(runner, '--emit-linked-c', linked, symbol, generated)
        check_native(cache, generated, code=42)
        audited = root / 'app-arc'
        compiled = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                                   '-I', str(ROOT / 'runtime'), str(generated), str(ROOT / 'runtime/sirius_runtime.c'),
                                   *link_flags(cache.cc), '-o', str(audited)], capture_output=True, text=True)
        assert compiled.returncode == 0, compiled.stderr
        observed = subprocess.run([str(audited)], capture_output=True)
        assert (observed.returncode, observed.stdout, observed.stderr) == (42, b'', b''), observed
        invalid = (
            ('relabel', 'let existing: O.Reading = O.make_meters(3)\n\treturn O.score(O.explicit_feet(existing))', b'cannot relabel'),
            ('wrong shape', 'return O.score(O.explicit_large(O.Raw(5)))', b'record is incompatible'),
        )
        for name, body, diagnostic in invalid:
            (root / 'App.sirius').write_text('import Ops alias O\n'
                                             + 'def main() -> i64\n\t' + body + '\n', encoding='utf-8')
            rejected = run(runner, '--build-modules', root, 'App', linked)
            assert rejected.returncode != 0 and diagnostic in rejected.stderr, (name, rejected)
    print('PASS heterogeneous union source-free package: imported aliases, managed payloads, arrays, Dict, CoW, native/MIR, ARC, invalid conversions')


if __name__ == '__main__':
    main()
