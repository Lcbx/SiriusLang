The biggest opportunity is reducing unnecessary CoW copies in generated code, followed by allocation improvements in the runtime. These are inspection findings; speedups need measurement.

| Priority | Location | Finding and suggested change |
|---|---|---|
| High | rewrite emitter:385 (/C:/Users/User/Desktop/tests/SiriusLang/rewrite/emit_c.sirius:385), array append:467 (/C:/Users/User/Desktop/tests/SiriusLang/rewrite/emit_c.sirius:467) | Insert/append retain their input before calling the consuming runtime operation. That makes a live input appear shared—even when its value is about to be replaced. Introduce proven last-use ownership transfers. Repeated accumulation can otherwise incur repeated full-container copies. |
| High | generic array growth:562 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:562) | Shared append clones into an exactly sized buffer, then immediately grows it. Combine uniqueness and capacity preparation so cloning allocates sufficient capacity once. Preserve callbacks and alias-safe item copying. |
| Medium | array construction:541 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:541) | Construction zeroes storage and then overwrites every element. For elements without copy callbacks, use one bulk memcpy; consider uninitialized allocation where complete initialization is guaranteed. |
| Medium | string concatenation:280 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:280) | s_string_concat_take always allocates and copies both strings. Consuming ownership currently provides no storage reuse. A unique-buffer path could help, but repeated concatenation needs spare capacity or a builder to avoid quadratic copying. |
| Medium | dictionary construction:676 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:676) | Every empty dictionary allocates an eight-slot table. Lazy allocation could benefit empty or unused dictionaries. Use the existing dictionary profiler’s empty_release and size counters before changing this. |
| Later | string search:850 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:850) | Search performs memcmp at every position, with quadratic worst cases. Start with a first-byte search fast path; consider a linear-time algorithm only if workloads justify it. |

Naming: the useful cleanup is ownership consistency:

- s_array_append → s_array_append_take
- s_dict_insert → s_dict_insert_take
- Document that _take consumes the container, while element/key/value pointers remain borrowed.
- Document that s_array_at returns a borrowed pointer invalidated by relevant mutation or release.

I would avoid broad cosmetic renaming. Any actual rename must update both emitters, runtime-internal callers, tests, and regenerated bootstrap C together.

Useful additions:

- Capacity-aware array/dictionary constructors and reserve operations.
- Bulk array extension, with explicit ownership and self-aliasing behavior.
- Array profiling counters for clones, copied elements and growth, complementing S_DICT_PROFILE.
- Shared checked allocation-size helpers. For example, string join:879 (/C:/Users/User/Desktop/tests/SiriusLang/runtime/sirius_runtime.c:879) checks accumulated content length but does not separately check overflow when adding the header and terminating byte.

I’d start with ownership-transfer measurement and combined clone/growth allocation. Validate unique and shared containers, self-append/self-assignment, managed elements, snapshots and ARC balance.

