# MIR compiler profiling notes

These are **historical local measurements**, not current performance promises.
Compiler source, generated C, instrumentation, hardware and C optimization
level all affect the result. Do not add inclusive times together when one
phase calls another. Use [ROADMAP.md](ROADMAP.md) for priorities and
[RUNTIME.md](RUNTIME.md) for collection implementation details.

## Reproduce and interpret

```sh
python tools/test_mir_selfhost.py --workdir build/mir-selfhost
python tools/profile_mir.py --generated-c build/mir-selfhost/generation_2.c --opt 0
python tools/profile_mir.py --generated-c build/mir-selfhost/generation_2.c --opt 1
python tools/profile_ir_storage.py
python tools/profile_runtime_dict.py --runs 3
```

`profile_mir.py` instruments a generated compiler C translation unit and
reports phase calls plus inclusive/exclusive time. Its phase IDs depend on
the concrete specialization ordering: recheck the table after compiler source
changes. It never edits the committed C. `--keep-dir PATH` preserves
instrumented output. `profile_ir_storage.py` instruments a **disposable** C
compiler and reports initial MIR record/array allocations and CoW clones by
runtime type ID; it does not measure reallocations or peak resident memory.
`profile_runtime_dict.py` measures actual `S_Dict` operations while compiling
Sirius, rather than isolated synthetic dictionary timings.

## Selected optimization history (October 2026)

| Change | Observed local result | Caution |
| --- | --- | --- |
| Entry-seeded ownership worklist and may-only exit cleanup | Clang `-O0` self-compile 22.05 → 16.75 s; `-O1` 10.67 → 8.52 s | Single instrumented runs; changes combined |
| Compact `S_MirField` union and parser token-width guard | `-O0` 16.75 → 11.83 s | Combined, single runs; generated C size similar |
| Consuming `MIR_RECORD_SET` | Generated C 3,454,644 → 3,169,724 bytes | No demonstrated end-to-end speedup |
| Fused direct record-array append | 100,000 appends 6.35 s → <0.01 s in a targeted `-O1` microbenchmark | Self-compilation roughly unchanged |
| Last-use managed call transfer | `-O0` self-compilation ~13.28 → 7.65–7.96 s | Before/after comparison on one environment; dramatic for certain CoW loops |
| Multiple distinct call argument transfers | Generated C 3,248,899 → 3,235,475 bytes; fewer retain sites | No convincing whole-program speedup beyond single-transfer variant |
| Shared initial liveness register state | Median `-O0` 6.89 → 5.63 s; median `-O1` 2.22 → 1.92 s | 3–4 runs per variant; cumulative requested bytes *rose* |

The parser and CFG ownership analyses—not C text emission—were the dominant
measured costs in early profiling. C emission itself was well below 1% in the
initial instrumented run. An attempted per-literal string cache, a naive
combined ownership pass, and several dictionary layout tweaks were reverted
when profiling did not support them.

## Allocation attribution (snapshot, 2026-10-14)

After sharing empty ownership/liveness states between CFG blocks, an
instrumented self-compilation captured:

| Allocation category | CoW clone calls | Cumulative requested bytes |
| --- | ---: | ---: |
| All generated records/arrays | 641,421 | 3.33 GB |
| `Array[i64]` ownership dataflow (runtime ID 13) | 473,741 | 2.84 GB |
| Parser `Array[Node]` arena (runtime ID 12) | 2,781 | 0.42 GB |

Runtime type IDs **depend on the exact bootstrap** and should not be copied
into assertions about future versions. Clone requests are included in overall
allocation counters; they are cumulative requested bytes, not peak RSS or
allocation time. Tracing suggested `mir_ownership_join` /
`mir_ownership_apply_effects` and parser `append_node` as expensive CoW
sites. Packing register masks or adding a private mutable parser builder may
help, but both require ownership/representation changes and validation.

## Profiling policy

- Separate **instrumented phase timings**, **uninstrumented wall time**,
  **generated-C size**, and **microbenchmarks**. They answer different questions.
- Confirm self-hosting produces byte-identical C after changes; run positive,
  negative, native/interpreter and ownership tests before crediting an
  optimization.
- Compare repeat runs on the same C compiler and optimization flags. Do not
  interpret changes in cumulative allocations as proven speed improvements.
- Prefer targeted simplifications supported by a measurement to broad MIR
  packing or runtime ABI rewrites.
