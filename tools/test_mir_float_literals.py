#!/usr/bin/env python3
"""Regression for MIR decimal decoding, including compensated long literals."""
from pathlib import Path
import shutil
import subprocess
import tempfile

from build_support import BuildCache, compile_native, require_cc
from project_paths import ROOT, normalize_newlines


def main() -> None:
    cache = BuildCache(require_cc())
    compensated = '0.' + '0' * 320 + '1e320'
    source = f'''import Float alias F

def main() -> i64
    print(F.mir_parse_real("9.999999999999995"))
    print(9.999999999999995)
    print(F.mir_parse_real("1e-320"))
    print(1e-320)
    print(F.mir_parse_real("{compensated}"))
    print({compensated})
    return 0
'''.replace('    ', '\t')
    with tempfile.TemporaryDirectory(prefix='sirius-mir-float-') as temporary:
        work = Path(temporary)
        shutil.copyfile(ROOT / 'src/mir_float.sirius', work / 'Float.sirius')
        (work / 'App.sirius').write_text(source)
        generated = work / 'app.c'
        subprocess.run([str(cache.seed()), '--module-root', str(work), 'App', str(generated)], check=True)
        executable = compile_native(cache, generated)
        result = subprocess.run([str(executable)], capture_output=True)
        expected = b'9.99999999999999\n9.99999999999999\n9.99988867182683e-321\n9.99988867182683e-321\n0.1\n0.1\n'
        if result.returncode != 0 or normalize_newlines(result.stdout) != expected or result.stderr:
            raise AssertionError(f'float literal decoding mismatch: {result!r}')
    print('PASS MIR float literal decoding: existing precision and long compensated exponent')


if __name__ == '__main__':
    main()
