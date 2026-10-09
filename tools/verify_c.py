"""Compile/run all expected C examples as a semantic reference (not Sirius compiler tests)."""
from pathlib import Path
import subprocess
import tempfile
import shutil
import sys

ROOT = Path(__file__).resolve().parent.parent
CC = shutil.which("cc")
if not CC:
    raise SystemExit("A C11 compiler (cc) is required")

cases = sorted((ROOT / "tests" / "expected_c").glob("*.c"))
with tempfile.TemporaryDirectory() as build:
    for source in cases:
        stem = source.stem
        fixture = ROOT / "tests" / "fixtures" / f"{stem}.sirius"
        stdout = ROOT / "tests" / "expected_output" / f"{stem}.txt"
        if not fixture.is_file() or not stdout.is_file():
            raise SystemExit(f"Missing expected C/output for {stem}")
        binary = Path(build) / stem
        command = [CC, "-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic", "-I", str(ROOT / "include"), str(source), str(ROOT / "include" / "sirius_runtime.c"), "-lm", "-o", str(binary)]
        result = subprocess.run(command, text=True, capture_output=True)
        if result.returncode:
            sys.stderr.write(result.stderr)
            raise SystemExit(f"C compilation failed: {stem}")
        result = subprocess.run([str(binary)], text=True, capture_output=True, cwd=build)
        expected = stdout.read_text()
        if result.returncode or result.stdout != expected:
            raise SystemExit(f"FAILED {stem}: exit={result.returncode} stdout={result.stdout!r}, expected={expected!r}, stderr={result.stderr!r}")
        print(f"PASS {stem}")
print(f"Verified {len(cases)} C reference examples")

import json
for stem, expected in json.loads((ROOT / 'tests' / 'runtime_errors' / 'expectations.json').read_text()).items():
    with tempfile.TemporaryDirectory() as work:
        binary = Path(work) / stem
        command = [CC, '-std=c11', '-Wall', '-Wextra', '-Werror', '-pedantic', '-I', str(ROOT / 'include'),
                   str(ROOT / 'tests' / 'runtime_errors' / f'{stem}.c'), str(ROOT / 'include' / 'sirius_runtime.c'), '-lm', '-o', str(binary)]
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode:
            raise SystemExit(f'Runtime error fixture compilation failed: {result.stderr}')
        result = subprocess.run([str(binary)], capture_output=True, text=True, cwd=work)
        if result.returncode != expected['exit'] or expected['stderr_contains'] not in result.stderr:
            raise SystemExit(f'Runtime failure mismatch {stem}: {result}')
        print(f'PASS {stem} (expected runtime panic)')
