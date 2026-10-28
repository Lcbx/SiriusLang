#!/usr/bin/env python3
"""Grouped tag-only match branches: scopes, exhaustiveness, modules and ARC."""
import argparse
from pathlib import Path
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, link_flags, require_cc
from project_paths import ROOT, executable_path


HEADER = '''rec Success(value: f64)
rec Error1(message: string)
rec Error2(message: string)
rec Error3(code: i64)
rec Unrelated(message: string)
let Result = Success | Error1 | Error2 | Error3
'''
INVALID = (
    ('duplicate within group', 'Error1 | Error1 | Error2 | Error3', 'duplicate match alternative'),
    ('duplicate across groups', 'Error1 | Error2\n\t\t\treturn false\n\t\tError2 | Error3', 'duplicate match alternative'),
    ('missing tag', 'Error1 | Error2', 'non-exhaustive match'),
    ('unknown tag', 'Error1 | Missing | Error2 | Error3', 'unknown match constructor'),
    ('nonmember tag', 'Error1 | Unrelated | Error2 | Error3', 'match arm is not a union alternative'),
    ('bound first pattern', 'Error1(message) | Error2 | Error3', 'grouped match tags cannot bind payloads'),
    ('bound later pattern', 'Error1 | Error2(message) | Error3', 'grouped match tags cannot bind payloads'),
    ('binding scope', 'Error1 | Error2 | Error3\n\t\t\tlet ignored = value', "unknown name 'value'"),
    ('trailing separator', 'Error1 |', 'variant name'),
)
PROVIDER = '''export rec Success(value: f64)
export rec Error1(message: string)
export rec Error2(message: string)
export let Result = Success | Error1 | Error2

export def apply(result: Result, callback) -> bool
    match result
        Success(value)
            return callback(value)
        Error1 | Error2
            return callback(0.0)
'''
APP = '''import Provider alias P

# The provider's generic patterns must retain their defining-module labels.
rec Error1(value: bool)

def positive(result: P.Result) -> bool
    match result
        P.Success(value)
            return value > 0.5
        P.Error1 | P.Error2
            return false

def main() -> i64
    let good: P.Result = P.Success(0.75)
    let first: P.Result = P.Error1("one")
    let second: P.Result = P.Error2("two")
    if not positive(good) or positive(first) or positive(second)
        return 1
    if not P.apply(good, fn(x) x > 0.5) or P.apply(first, fn(x) x > 0.5) or P.apply(second, fn(x) x > 0.5)
        return 2
    return 0
'''


def run(*command, code=0, quiet=True):
    result = subprocess.run([str(arg) for arg in command], capture_output=True, timeout=180)
    assert result.returncode == code and result.stderr == b'', (command, result)
    if quiet:
        assert result.stdout == b'', (command, result)


def native(cache, source, binary):
    run(cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
        '-I', ROOT / 'runtime', source, ROOT / 'runtime/sirius_runtime.c',
        *link_flags(cache.cc), '-o', binary)
    run(binary)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(require_cc())
    compiler = args.compiler.resolve() if args.compiler else cache.native_tool('modular_cli')
    runner = args.runner.resolve() if args.runner else cache.native_tool('run_cli')
    with tempfile.TemporaryDirectory() as temp:
        root = Path(temp)
        output = root / 'match.c'
        binary = executable_path(root / 'match')
        fixture = ROOT / 'tests/mir/source_match_tags.sirius'
        run(compiler, fixture, output)
        native(cache, output, binary)
        run(runner, fixture)
        for name, tags, diagnostic in INVALID:
            source = root / 'invalid.sirius'
            source.write_text(HEADER + 'def test(result: Result) -> bool\n\tmatch result\n'
                              '\t\tSuccess(value)\n\t\t\treturn value > 0.5\n'
                              '\t\t' + tags + '\n\t\t\treturn false\n'
                              'def main() -> i64\n\tlet x = test(Success(0.75))\n\treturn 0\n')
            for tool in (compiler, runner):
                command = [str(tool), str(source)]
                if tool == compiler:
                    output.unlink(missing_ok=True)
                    command.append(str(output))
                result = subprocess.run(command, capture_output=True, timeout=180)
                assert result.returncode != 0 and diagnostic.encode() in result.stdout + result.stderr, (name, result)
            assert not output.exists(), name
        for name, text in (('Provider', PROVIDER), ('App', APP)):
            (root / f'{name}.sirius').write_text(text.replace('    ', '\t'))
        for packaged in (False, True):
            if packaged:
                run(runner, '--emit-generic', 'Provider', root / 'Provider.smirg', root / 'Provider.sirius', root, quiet=False)
                (root / 'Provider.sirius').unlink()
            run(compiler, '--module-root', root, 'App', output)
            native(cache, output, binary)
            library = root / 'App.smirl'
            run(runner, '--build-modules', root, 'App', library, quiet=False)
            run(runner, '--entry', 'M_3_App_4_main', '--load-lib', library)
    print('PASS grouped match tags: payload discard, enums, scopes, callbacks, qualified/source-free imports, ARC, native/MIR, negatives')


if __name__ == '__main__':
    main()
