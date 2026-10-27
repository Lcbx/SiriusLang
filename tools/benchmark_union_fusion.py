#!/usr/bin/env python3
"""Experimental coallocated record/union pair in *generated Sirius C*.

This is an exact-pattern C rewrite, NOT a language implementation. It only
fuses immediately adjacent record construction, union wrapping and release.
The production compiler, MIR and runtime are unmodified. The experiment
preserves independent ARC credits for wrappers and extracted records.
"""
import argparse
import re
import shutil
import statistics
import subprocess
import tempfile
import time
from pathlib import Path

from build_support import BuildCache, link_flags
from project_paths import ROOT, executable_path

# Require complete adjacent record-new, union-new, release sequence. Do not
# rewrite reused, escaped, or non-adjacent records. Leave retain/release in
# place to preserve existing ARC accounting and interpret the fair layout cost.
PAIR = re.compile(
    r'(?P<start>\t\{ S_MirRecord \*record = )s_mir_record_new\((?P<ptype>\d+), (?P<fields>\d+)\);\n'
    r'(?P<body>(?:\t[^\n]*\n)*?)'
    r'\ts_mir_record_store\(&(?P<record>r\d+), record\);\n\t\}\n'
    r'\t\{ S_MirRecord \*variant = s_mir_record_new\((?P<utype>\d+), 2\);\n'
    r'(?P<tag>\t\tvariant->fields\[0\]\.bits = \d+;\n)'
    r'\t\tvariant->fields\[1\]\.record = s_mir_record_retain\((?P=record)\);\n'
    r'(?P<store>\ts_mir_record_store\(&r\d+, variant\);\n\t\}\n)'
    r'\ts_mir_record_release\((?P=record)\); (?P=record) = NULL;'
)

# Placed after generated s_mir_record_new so audit scan/new dependencies exist.
HELPERS = '''/* EXPERIMENT: coallocation, not part of Sirius production runtime. */
#define S_MIR_FUSED_WRAPPER SIZE_MAX
#define S_MIR_FUSED_PAYLOAD (SIZE_MAX - 1)
#define S_MIR_FUSED_OFFSET (sizeof(S_MirRecord) + 2 * sizeof(S_MirField))
static inline S_MirRecord *s_mir_new_coallocated(int64_t ptype, size_t fields, int64_t utype) {
    if (fields > (SIZE_MAX - S_MIR_FUSED_OFFSET - sizeof(S_MirRecord)) / sizeof(S_MirField)) s_panic("collection too large");
    S_MirRecord *w = calloc(1, S_MIR_FUSED_OFFSET + sizeof(S_MirRecord) + fields * sizeof(S_MirField));
    if (!w) s_panic("out of memory");
    S_MirRecord *p = (S_MirRecord *)((unsigned char *)w + S_MIR_FUSED_OFFSET);
    w->type_id = utype; w->refs = 1; w->length = 2; w->capacity = S_MIR_FUSED_WRAPPER;
    p->type_id = ptype; p->refs = 1; p->length = fields; p->capacity = S_MIR_FUSED_PAYLOAD;
    w->fields[1].record = p;
    s_arc_audit_new(w, w->refs); s_arc_audit_watch(w, s_mir_record_arc_scan);
    s_arc_audit_new(p, p->refs); s_arc_audit_watch(p, s_mir_record_arc_scan);
    return p;
}
static inline S_MirRecord *s_mir_coallocated_wrapper(S_MirRecord *p) {
    if (!p || p->capacity != S_MIR_FUSED_PAYLOAD) s_panic("not a fused record");
    return (S_MirRecord *)((unsigned char *)p - S_MIR_FUSED_OFFSET);
}
'''


def transform(text: str) -> tuple[str, int]:
    # Runtime invariants for this particular revision of the code generator.
    new_end = 'static inline S_MirRecord *s_mir_record_retain(S_MirRecord *r) {'
    if text.count(new_end) != 1:
        raise AssertionError('generated C record-new/retain boundary changed')
    def replace(m: re.Match[str]) -> str:
        return (
            m['start'] + f's_mir_new_coallocated({m["ptype"]}, {m["fields"]}, {m["utype"]});\n'
            + m['body'] + f'\ts_mir_record_store(&{m["record"]}, record);\n\t}}\n'
            + '\t{ S_MirRecord *variant = s_mir_coallocated_wrapper(' + m['record'] + ');\n'
            + m['tag'] + '\t\tvariant->fields[1].record = s_mir_record_retain(' + m['record'] + ');\n'
            + m['store'] + '\ts_mir_record_release(' + m['record'] + '); ' + m['record'] + ' = NULL;'
        )
    new, count = PAIR.subn(replace, text)
    if count == 0:
        raise AssertionError('no complete immediate union constructor sequences matched')
    new = new.replace(new_end, HELPERS + new_end, 1)
    # Once the wrapper reaches zero refs, dropping the payload may free the
    # allocation. Returning avoids touching the now-dangling wrapper pointer.
    union_arm = 's_mir_record_release(r->fields[1].record); break;'
    if union_arm not in new:
        raise AssertionError('generated union release branch changed')
    new = new.replace(union_arm,
        'if (r->capacity == S_MIR_FUSED_WRAPPER) { s_mir_record_release(r->fields[1].record); return; } '
        + union_arm)
    tail = '\tfree(r);\n}\n\nstatic inline void s_mir_string_store'
    if new.count(tail) != 1:
        raise AssertionError('generated record-release tail changed')
    new = new.replace(tail,
        '\tif (r->capacity == S_MIR_FUSED_PAYLOAD) free((unsigned char *)r - S_MIR_FUSED_OFFSET);\n'
        '\telse free(r);\n}\n\nstatic inline void s_mir_string_store', 1)
    return new, count


def source(count: int, managed: bool) -> str:
    second = 'name: string, samples: Array[i64]' if managed else 'more: i64, extra: i64, last: i64'
    args = '"abc", [i, i + 1]' if managed else 'i + 1, i + 2, i + 3'
    return f'''rec Small(value: i64)
rec Large(value: i64, {second})
let Variant = Small | Large

def main() -> i64
\tvar data = Array[Variant]()
\tfor i in 0..{count}
\t\tif i % 2 == 0
\t\t\tdata = data.append(Small(i))
\t\telse
\t\t\tdata = data.append(Large(i, {args}))
\tvar total = 0
\tfor item in data
\t\tmatch item
\t\t\tSmall(value)
\t\t\t\ttotal += value
\t\t\tLarge(value, a, b{', c' if not managed else ''})
\t\t\t\ttotal += value
\treturn total % 251
'''


def cow_source() -> str:
    return """rec Small(value: i64)
rec Large(value: i64, label: string, samples: Array[i64])
let Variant = Small | Large

def main() -> i64
\tvar values = Array[Variant]().append(Large(10, "x", [2, 3])).append(Small(4))
\tlet snapshot = values
\tvalues = values.append(Small(99))
\tvar total = 0
\tfor item in snapshot
\t\tmatch item
\t\t\tSmall(value)
\t\t\t\ttotal += value
\t\t\tLarge(value, name, samples)
\t\t\t\ttotal += value + len(name) + samples[!1]
\treturn total
"""


def cow_correctness(compiler: Path, cc: str, sanitize: bool) -> None:
    with tempfile.TemporaryDirectory(prefix='sirius-union-fusion-cow-') as tmp:
        d = Path(tmp)
        sr, orig, transformed = d / 'cow.sirius', d / 'cow.c', d / 'fused.c'
        sr.write_text(cow_source(), encoding='utf-8')
        check([str(compiler), str(sr), str(orig)])
        rewritten, sites = transform(orig.read_text(encoding='utf-8'))
        if sites < 2:
            raise AssertionError('CoW regression did not cover direct union construction')
        transformed.write_text(rewritten, encoding='utf-8')
        for name, path in (('current', orig), ('fused', transformed)):
            exe = executable_path(d / name)
            build(cc, path, exe, audit=True, sanitizer=sanitize)
            check([str(exe)], expected=18)
    print(f'CoW + escaped match managed fields: PASS (sites={sites}, ARC audited)')


def check(cmd: list[str], expected: int | None = None) -> None:
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if expected is None:
        ok = proc.returncode == 0
    else:
        ok = proc.returncode == expected and not proc.stdout and not proc.stderr
    if not ok:
        raise RuntimeError(f'{cmd!r}: expected {expected}, code {proc.returncode}, stdout={proc.stdout!r}, stderr={proc.stderr!r}')


def build(cc: str, source_c: Path, binary: Path, audit: bool, sanitizer: bool) -> None:
    flags = ['-std=c11', '-O1' if sanitizer else '-O2', '-w']
    if audit:
        flags += ['-DSIRIUS_ARC_AUDIT']
    if sanitizer:
        flags += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-g']
    check([cc, *flags, '-I', str(ROOT / 'runtime'), str(source_c),
           str(ROOT / 'runtime/sirius_runtime.c'), *link_flags(cc), '-o', str(binary)])


def evaluate(compiler: Path, cc: str, count: int, runs: int, managed: bool, sanitize: bool) -> None:
    label = 'managed' if managed else 'scalar'
    with tempfile.TemporaryDirectory(prefix='sirius-union-fusion-') as tmp:
        d = Path(tmp)
        paths = {}
        for n, audit in ((min(128, count), True), (count, False)):
            sr = d / ('audit.sirius' if audit else 'bench.sirius')
            orig = d / ('audit.c' if audit else 'bench.c')
            new = d / ('fused_audit.c' if audit else 'fused.c')
            sr.write_text(source(n, managed), encoding='utf-8')
            check([str(compiler), str(sr), str(orig)])
            modified, sites = transform(orig.read_text(encoding='utf-8'))
            if sites != 2:
                raise AssertionError(f'{label}: expected two direct construction sites, got {sites}')
            new.write_text(modified, encoding='utf-8')
            expected = (n * (n - 1) // 2) % 251
            for name, path in (('current', orig), ('fused', new)):
                exe = executable_path(d / (name + ('_audit' if audit else '')))
                build(cc, path, exe, audit, sanitize and audit)
                check([str(exe)], expected)
                if not audit:
                    paths[name] = exe
        samples = {'current': [], 'fused': []}
        for i in range(runs + 2):
            for name in (('current', 'fused') if i % 2 == 0 else ('fused', 'current')):
                start = time.perf_counter()
                check([str(paths[name])], (count * (count - 1) // 2) % 251)
                if i >= 2:
                    samples[name].append((time.perf_counter() - start) * 1000)
        med = {name: statistics.median(samples[name]) for name in samples}
        print(f'{label}: count={count:,} sites=2 runs={runs} current={med["current"]:.2f}ms fused={med["fused"]:.2f}ms ratio={med["fused"]/med["current"]:.3f}x')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, help='native compiler executable (default: build cached bootstrap seed)')
    parser.add_argument('--cc', default='clang')
    parser.add_argument('--count', type=int, default=100000)
    parser.add_argument('--runs', type=int, default=9)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--case', choices=('all', 'scalar', 'managed'), default='all')
    args = parser.parse_args()
    if not shutil.which(args.cc):
        parser.error('C compiler unavailable')
    args.compiler = args.compiler.resolve() if args.compiler else BuildCache(args.cc).seed()
    if not args.compiler.is_file():
        parser.error('compiler executable unavailable')
    if args.count < 1 or args.runs < 1:
        parser.error('count and runs must be positive')
    cow_correctness(args.compiler, args.cc, args.sanitize)
    for managed in (False, True) if args.case == 'all' else (args.case == 'managed',):
        evaluate(args.compiler, args.cc, args.count, args.runs, managed, args.sanitize)
    print('Experimental C rewrite only; production emitter/runtime unchanged.')


if __name__ == '__main__':
    main()
