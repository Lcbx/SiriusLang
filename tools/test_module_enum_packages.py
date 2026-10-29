#!/usr/bin/env python3
"""Payload-free unions survive source-free generic packages and linked MIR."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, C_FLAGS, check_native, link_flags, require_cc


PROVIDER = '''export enum Signal
\tIdle
\tRunning
\tStopped

export rec Raw()

export def choose(value) -> Signal
\tif value > 0
\t\treturn Running()
\treturn Idle()

export def score(value)
\tmatch value
\t\tIdle()
\t\t\treturn 1
\t\tRunning()
\t\t\treturn 2
\t\tStopped()
\t\t\treturn 3

export def append_idle(values)
\tlet raw = Raw()
\tlet item: Signal = Signal.Idle(raw)
\treturn values.append(item)
'''

CONSUMER = '''import Ops alias O

# Importer names must not capture the package's constructors or type alias.
enum Signal
	Running
	Failure
rec Raw()

def main() -> i64
\tlet first = O.choose(1)
\tlet second = O.choose(0)
\tlet original = [first, second]
\tlet extended = O.append_idle(original)
\tif len(original) != 2 or len(extended) != 3
\t\treturn 91
\treturn O.score(extended[!0]) + O.score(extended[!1]) + O.score(extended[!2])
'''


def run(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
    return subprocess.run([str(runner), *(str(arg) for arg in args)],
                          capture_output=True, timeout=45)


def checked(runner: Path, *args: object) -> subprocess.CompletedProcess[bytes]:
    result = run(runner, *args)
    assert result.returncode == 0, (args, result.stdout, result.stderr)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runner', type=Path)
    args = parser.parse_args()
    cache = BuildCache(shutil.which('clang') or require_cc())
    runner = args.runner.resolve() if args.runner else cache.mir_runner()
    with tempfile.TemporaryDirectory(prefix='sirius-enum-package-') as directory:
        root = Path(directory)
        provider = root / 'Ops.sirius'
        provider.write_text(PROVIDER, encoding='utf-8')
        package = root / 'Ops.smirg'
        checked(runner, '--emit-generic', 'Ops', package, provider)
        serialized = package.read_bytes()
        checked(runner, '--emit-generic', 'Ops', package, provider)
        assert package.read_bytes() == serialized
        provider.unlink()  # No provider source during specialization or linking.
        (root / 'App.sirius').write_text(CONSUMER, encoding='utf-8')
        linked = root / 'app.smirl'
        checked(runner, '--build-modules', root, 'App', linked)
        first = linked.read_bytes()
        checked(runner, '--build-modules', root, 'App', linked)
        assert linked.read_bytes() == first, 'source-free build must be deterministic'
        symbol = 'M_3_App_4_main'
        interpreted = run(runner, '--entry', symbol, '--load-lib', linked)
        assert (interpreted.returncode, interpreted.stdout, interpreted.stderr) == (4, b'', b''), interpreted
        # Linked, serialized MIR must also generate correct native code and
        # preserve balanced ownership for arrays of scalar enum values.
        generated = root / 'app.c'
        checked(runner, '--emit-linked-c', linked, symbol, generated)
        check_native(cache, generated, code=4)
        audited = root / 'app-arc'
        compiled = subprocess.run([cache.cc, *C_FLAGS, '-Wno-unused-function', '-DSIRIUS_ARC_AUDIT',
                                   '-I', str(Path(__file__).resolve().parent.parent / 'runtime'),
                                   str(generated), str(Path(__file__).resolve().parent.parent / 'runtime/sirius_runtime.c'),
                                   *link_flags(cache.cc), '-o', str(audited)], capture_output=True, text=True)
        assert compiled.returncode == 0, compiled.stderr
        observed = subprocess.run([str(audited)], capture_output=True)
        assert (observed.returncode, observed.stdout, observed.stderr) == (4, b'', b''), observed
        package.write_bytes(serialized[:-1])
        bad = run(runner, '--build-modules', root, 'App', linked)
        assert bad.returncode != 0 and b'invalid generic package' in bad.stderr, bad
        print('PASS enum packages: source-free specialization, linked MIR, native/MIR, ARC, deterministic output, corrupt package rejection')


if __name__ == '__main__':
    main()
