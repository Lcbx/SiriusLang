# Reusing the original compiler

Inspected 2026-11-02 at repository revision `33542df`. Paths below are relative
to this directory; line ranges describe that snapshot. Function names are the
more durable search keys. This is a source inspection and migration guide, not
a claim that the listed components compile unchanged with the rewrite.

The original compiler is the Sirius implementation in `../src`, supported by
`../runtime`, `../stdlib`, and `../tools`. Its MIR-to-C backend is the active old
backend; there is no separate legacy AST-to-C emitter to recover.

The best return is to reuse the C runtime and test infrastructure, adapt small
semantic helpers, and port selected algorithms. Importing the whole old HIR/MIR
would also import its representation and dependency costs.

## Current rewrite boundary

- The rewrite already has canonical composite types, recursive aggregate
  layouts, CoW updates, dictionaries, managed optionals, and scalar enums.
  These are existing assets, not reasons to replace its implementation.
- The next self-compilation gaps in [TODO.md](TODO.md) are payload-bearing
  unions/matching/equality, generic specialization, source modules, compiler
  I/O, and a separate reproducible seed.
- Current source is native-only: there is no `interpreter.sirius` here, and
  [main.sirius](main.sirius) invokes C emission. Historical references to a
  rewrite interpreter should not be used as evidence of current coverage.
- Type IDs, token codes, parser nodes and instructions differ between the two
  compilers. Sharing an integer representation does not make them compatible.

## Highest-value reuse for self-compilation

### 1. Labeled unions and composable type identity

| Original source | What to reuse |
| --- | --- |
| [types.sirius:128–241](../src/types.sirius#L128-L241) | Length-delimited descriptor keys, canonicality checks, child-before-parent acyclicity, and interning through one lookup. |
| [types.sirius:287–353](../src/types.sirius#L287-L353) | Recursive structural signatures independent of local TypeIds. Useful for module interfaces and specialization names. |
| [types.sirius:382–454](../src/types.sirius#L382-L454) | Ordered `(label, payload type)` union identity, explicit label selection, rejection of ambiguous implicit injection, and recursive collection/record compatibility. |
| [hir_declarations.sirius:55–331](../src/hir_declarations.sirius#L55-L331) | Collect declarations before resolving union aliases; preserve constructor identity separately from structural record shape. Adapt the pass ordering, not the entire declaration walker. |

Adapt to `IR.TypeTable` and its existing `TypeKind`. Old primitive IDs include
invalid/void/f64 entries; the rewrite has a different numbering and `NO_TYPE`
convention. Do not copy the numeric tags or retain the old unnamed-union path
unless a concrete rewrite requirement needs it.

The essential invariant is that two alternatives with the same record shape
remain distinguishable by label, while the records themselves remain
structural. The rewrite currently gives enums their own descriptor; decide how
that relates to empty-record unions before implementing payload-bearing types.

Suggested improvement: share one canonical descriptor key/index across
composite kinds instead of adding more linear `find_array`/`find_dict`-style
scans. Measure compiler workloads before changing existing caches.

### 2. Checked conversion plans and contextual constructor selection

| Original source | What to reuse |
| --- | --- |
| [hir_resolve.sirius:43–94](../src/hir_resolve.sirius#L43-L94) | A postorder conversion plan for a source/target type pair, with children pointing to earlier steps. |
| [hir_analyze.sirius:1906–1978](../src/hir_analyze.sirius#L1906-L1978) | Select a direct constructor's union label during checking; propagate expected array element types and cache structural conversion plans. |
| [mir_lower.sirius:211–373](../src/mir_lower.sirius#L211-L373) | Consume checked plans to rebuild records/arrays/optionals/dictionaries with converted children. Reference algorithm; CFG/register code needs replacement. |

Keep label decisions and conversion requirements in the rewrite's checked
representation. The C emitter should consume resolved alternatives and typed
operations, not infer constructor identity from a structural payload.
Conversions inside optional defaults must stay in the lazy fallback branch.
This avoids adding unions separately to every aggregate and then discovering
that they fail when nested.

### 3. Exhaustive matching and payload bindings

| Original source | What to reuse |
| --- | --- |
| [parser.sirius:700–786](../src/parser.sirius#L700-L786) | Grammar for payload patterns, bare tags, grouped alternatives and shared binders. Translate to the rewrite's parser representation. |
| [hir_resolve.sirius:284–342](../src/hir_resolve.sirius#L284-L342) | Constructor-position to canonical-field mapping; exhaustiveness and duplicate detection by alternative index. These are small, largely independent algorithms. |
| [hir_analyze.sirius:1999–2130](../src/hir_analyze.sirius#L1999-L2130) | Resolve tags, scope immutable arm bindings, require compatible types for shared binders, and reject missing/repeated arms. |
| [mir_lower.sirius:1333–1490](../src/mir_lower.sirius#L1333-L1490) | Evaluate the subject once, test tags, extract payloads only on matching edges, and join surviving arms. Port control-flow behavior to flat IR. |

Use explicit checked arm metadata: alternative index, payload type, field
indices, and binding slots. The old lowering still searches constructor
declarations for some binding mappings; resolve those once during checking.
Preserve the rewrite verifier's definite-initialization guarantees at arm joins.

### 4. Generic specialization without the callback subsystem

| Original source | What to reuse |
| --- | --- |
| [hir.sirius:141–180](../src/hir.sirius#L141-L180) | Registry keyed by definition identity plus concrete argument types; reserve stable IDs before analyzing recursive bodies. |
| [hir_specialize.sirius:23–71](../src/hir_specialize.sirius#L23-L71) | Check a concrete body with argument bindings and its expected return type. Adapt to `Semantic.Checked`. |
| [hir_specialize.sirius:81–197](../src/hir_specialize.sirius#L81-L197) | Worklist expansion, deduplicated callee requests, resolved specialization call targets, and annotated recursive-result requirements. |
| [hir_parametric.sirius:14–132](../src/hir_parametric.sirius#L14-L132) | Optional later reference for checking generic bodies before all concrete types are known. Not required for the first concrete-specialization path. |

Start with omitted parameter types and concrete call sites needed by compiler
sources. Do not import `hir_analyze`'s callback probing, first-class function
inference, overloads, or lambdas as prerequisites. None is required by the
current self-compilation plan.

Suggested improvement: use a structured specialization state and a stable
module/declaration ID, not a body-node ID outside its owning module. Cache
inferred results as well as emitted bodies; the old orchestration sometimes
calls concrete-body analysis again to discover a callee result. Diagnose
recursive inference cycles explicitly instead of repeatedly expanding them.

### 5. Source imports, symbol identity and type remapping

| Original source | What to reuse |
| --- | --- |
| [module.sirius:4–67](../src/module.sirius#L4-L67) | Unambiguous length-prefixed module symbols, validated member decoding, and dotted-name to source-path conversion. Small extraction candidate. |
| [module.sirius:70–90](../src/module.sirius#L70-L90) | Structural signatures and identifier-safe specialization/overload suffixes. Relevant when symbols cross compilation units. |
| [module_scan.sirius:608–626](../src/module_scan.sirius#L608-L626) | Validate module path components before constructing filenames. |
| [module_scan.sirius:679–766](../src/module_scan.sirius#L679-L766) | Parse-once cache, visiting/completed sets, cycle diagnostics, and dependency-before-importer traversal. Drop package fallback initially. |
| [module_scan.sirius:32–96](../src/module_scan.sirius#L32-L96) | Import exported declarations while keeping private names private. Uses old HIR structures, so port the rules. |
| [module_resolve.sirius:32–138](../src/module_resolve.sirius#L32-L138) | Resolve qualified calls, types, patterns and values against exports. The old AST/token rewriting is representation-specific. |
| [module_interface.sirius:7–74](../src/module_interface.sirius#L7-L74) | Recursively re-intern foreign types; module-local TypeIds cannot be copied directly. |
| [module_interface.sirius:78–97](../src/module_interface.sirius#L78-L97) | Remap constant value graphs along with their types when constants cross module boundaries. |

For the first whole-source build, a single shared type table can avoid repeated
foreign-type imports. Keep module namespaces and declaration identities
explicit even if all modules eventually emit into one C translation unit.
Traverse imports in source order; never derive symbol IDs from hash iteration.

Suggested improvements: preserve actual source spans rather than appending
synthetic symbol text to the source string, and keep missing-file outcomes
distinct from empty source. The old cache uses `read_text(path) or ""`, then
treats an empty string as a missing module. Memoize foreign TypeId remapping if
independent module tables are retained.

### 6. Compiler I/O and platform support

| Original source | What to reuse |
| --- | --- |
| [sirius_runtime.h:47–60](../runtime/sirius_runtime.h#L47-L60), [sirius_runtime.c:409–499](../runtime/sirius_runtime.c#L409-L499) | Existing filesystem ABI, read/write results and host error handling. |
| [sirius_runtime.c:923–1052](../runtime/sirius_runtime.c#L923-L1052) | Windows UTF-8/wide-path handling, platform directory operations and owned command-line argument arrays. |
| [hir_resolve.sirius:105–164](../src/hir_resolve.sirius#L105-L164) | Builtin argument/result contracts, including `arguments`, `read_text`, and `write_text`. |
| [mir_emit_c.sirius:868–1458](../src/mir_emit_c.sirius#L868-L1458) | Search `MIR_ARGUMENTS`, `MIR_READ_TEXT`, `MIR_WRITE_TEXT` for lowering to runtime calls and owned results. Extract individual cases only. |

The rewrite CLI already uses these operations when built by the original
compiler; the missing work is making the rewrite compile those operations
itself. Add typed IR cases and emit calls to the existing runtime. Its generated
`main(void)` must also initialize runtime arguments when programs use them.

## Runtime and backend work worth preserving

### 7. Existing CoW/ARC runtime: reuse directly

| Existing source | Reuse and constraints |
| --- | --- |
| [sirius_runtime.h:13–34](../runtime/sirius_runtime.h#L13-L34), [sirius_runtime.c:184–246](../runtime/sirius_runtime.c#L184-L246) | Specialized integer arrays and owned append/read helpers. Already used by rewrite output. |
| [sirius_runtime.h:127–153](../runtime/sirius_runtime.h#L127-L153), [sirius_runtime.c:516–608](../runtime/sirius_runtime.c#L516-L608) | Generic arrays, element copy/drop callbacks, uniqueness checks, growth and alias-safe replacement. Already reused. |
| [sirius_runtime.c:663–824](../runtime/sirius_runtime.c#L663-L824) | Generic dictionaries, hashing, cloning, overwrite handling and cursor iteration. Already reused. |
| [sirius_runtime.c:827–886](../runtime/sirius_runtime.c#L827-L886) | Byte-length strings, slicing, integer formatting and join. Already reused where exposed by the rewrite. |
| [sirius_runtime.h:85–125](../runtime/sirius_runtime.h#L85-L125), [sirius_runtime.c:20–176](../runtime/sirius_runtime.c#L20-L176) | ARC allocation/credit tracking, heap edges and frame-root auditing. Retain these diagnostics during migration. |

Important contracts: generic append consumes an owned array; set can replace
the caller's owner through a pointer; callback copies create owned elements.
The runtime snapshots borrowed replacement data before growth, CoW or dropping
an old value. Preserve that ordering for self-assignment and nested aliases.

Functional-but-in-place reuse depends on the generated ownership flow too:
an unnecessary retained temporary can make an otherwise unique array appear
shared. Reusing the runtime alone does not prove an update avoids copying.

### 8. Liveness, ownership and safe last-use moves

| Original source | What transfers |
| --- | --- |
| [mir_cfg.sirius:4–103](../src/mir_cfg.sirius#L4-L103) | Predecessors, reachability and explicit instruction-read classification. Map to flat instruction successors or derive temporary basic blocks. |
| [mir_lifetime.sirius:14–119](../src/mir_lifetime.sirius#L14-L119) | Packed live sets and predecessor worklist solving. A useful basis for releasing dead managed temporaries. |
| [mir_ownership.sirius:16–223](../src/mir_ownership.sirius#L16-L223) | Packed must/maybe ownership states and forward worklist analysis. |
| [mir_cleanup.sirius:16–138](../src/mir_cleanup.sirius#L16-L138) | Edge-specific cleanup using possible ownership and successor liveness, including conditionally initialized values. |
| [mir_opt.sirius:14–148](../src/mir_opt.sirius#L14-L148) | Turn proven last-use copy/retain/release sequences into moves; handle repeated operands and source/destination aliasing conservatively. |
| [mir_program.sirius:36–78](../src/mir_program.sirius#L36-L78) | Reference pass ordering: typed lowering, verification, guards, lifetimes, exit cleanup, optimization and ownership/failure audits. |

These are algorithm ports, not drop-in passes. Old MIR has explicit ownership
operations and block terminators; rewrite C currently owns values in typed
slots and cleans up on return. Add only the analysis needed for a measured
problem. Never transplant a local peephole into flat IR without proving that
no jump enters its middle.

Suggested improvement: maintain one instruction-effects interface for reads,
writes, moves and control-flow successors, so the verifier, liveness solver and
optimizer cannot silently disagree as new union/I/O instructions arrive.

### 9. Structural equality and storage decisions

| Original source | What transfers |
| --- | --- |
| [types.sirius:58–126](../src/types.sirius#L58-L126) | Enum recognition, equality eligibility and logical storage/ownership classification. Adapt policies; old records are generally object-backed. |
| [mir_interpreter.sirius:62–83](../src/mir_interpreter.sirius#L62-L83) | Recursive typed value equality as a semantic reference, including tagged values and the separate dictionary-key comparison mode. |
| [mir_emit_c.sirius:465–521](../src/mir_emit_c.sirius#L465-L521) | Generated recursive equality over records, arrays, optionals and unions. Re-express using the rewrite's C structs and existing element layouts. |
| [mir_emit_c.sirius:1487–1691](../src/mir_emit_c.sirius#L1487-L1691) | Liveness/interference-based scalar and managed slot compaction. Later optimization candidate, not a self-hosting prerequisite. |

Do not copy `S_MirRecord` boxing, union coallocation or inline-union thresholds
as the rewrite's default layout. Those helpers depend on the old backend's
heap ABI. Implement correct tag-plus-payload semantics first; measure physical
slot counts, stack use, retain/release traffic and allocation counts before
porting layout optimizations. Future floating equality needs explicit NaN and
signed-zero tests, especially when reused for dictionary keys.

## Useful later, or as a reference rather than a transplant

| Original source | Use and limitation |
| --- | --- |
| [tokenizer.sirius:149–216](../src/tokenizer.sirius#L149-L216), [parser.sirius:263–315](../src/parser.sirius#L263-L315) | Numeric token scanning and recursive type/parameter grammar. The rewrite already has its own lexer and annotation parser; port missing rules and fixtures, not old numeric token/node constants. |
| [hir_resolve.sirius:232–280](../src/hir_resolve.sirius#L232-L280), [mir_lower.sirius:144–209](../src/mir_lower.sirius#L144-L209) | Record spread/delete/edit rules and leaf-to-root nested update reconstruction. Nested updates already exist in the rewrite; use as a differential reference, not another implementation. |
| [hir_constants.sirius:19–48](../src/hir_constants.sirius#L19-L48), [hir_constants.sirius:53–245](../src/hir_constants.sirius#L53-L245) | String decoding and typed constant-value arenas for literals, expressions and aggregates. Adapt AST access and check integer boundaries; retain rewrite interpolation behavior rather than replacing its decoder. |
| [mir_guard.sirius:14–80](../src/mir_guard.sirius#L14-L80), [mir_verify.sirius:489–583](../src/mir_verify.sirius#L489-L583) | Explicit failure guards and auditing that unsafe arithmetic/index operations are dominated by checks. Keep failure semantics when adding optimization. |
| [mir_verify.sirius:57–487](../src/mir_verify.sirius#L57-L487), [mir_verify.sirius:600–689](../src/mir_verify.sirius#L600-L689) | Validation patterns: validate descriptors/signatures and operand bounds before reading types, then instruction and branch contracts. Adapt to the rewrite verifier, which already checks definite initialization. |
| [mir_float.sirius:1–125](../src/mir_float.sirius#L1-L125) | Isolated Sirius decimal parser for future `f64`. The file explicitly does not promise correctly rounded conversion or native/interpreter bit parity; not a finished parity solution. |
| [mir_wire.sirius:20–86](../src/mir_wire.sirius#L20-L86) | Small checked integer/length-prefixed string readers, including full signed-i64 boundaries. Suitable building blocks if rewrite artifacts are later needed. |
| [mir_wire.sirius:88–257](../src/mir_wire.sirius#L88-L257), [mir_link.sirius:13–97](../src/mir_link.sirius#L13-L97) | Serialization and type/function relocation algorithms. Old opcodes and object layouts prevent artifact compatibility by simple copying. |
| [hir_call_graph.sirius:143–241](../src/hir_call_graph.sirius#L143-L241), [mir_emit_c.sirius:1872–1936](../src/mir_emit_c.sirius#L1872-L1936) | Reachable-root discovery/emission and diagnostics. Later code-size work; preserve validation of unused rewrite functions rather than adopting a different checking policy accidentally. |
| [Std/Functional.sirius:20–137](../stdlib/Std/Functional.sirius#L20-L137), [Std/Numeric.sirius:20–92](../stdlib/Std/Numeric.sirius#L20-L92), [Std/Filesystem.sirius:59–103](../stdlib/Std/Filesystem.sirius#L59-L103) | Ordinary Sirius library algorithms, not compiler machinery. Reuse when modules, callbacks, floats and unions required by each function are supported. No capturing-closure implementation is needed. |

Defer the old artifact/package stack (`mir_library`, `mir_generic`,
`mir_template`, `mir_semantic`, `mir_bundle`, plus the package branches of
`module_build`) until the rewrite needs independent compilation or distribution.
Self-compilation from an acyclic source graph does not require those formats.
Likewise, `mir_interpreter` can remain a semantic oracle; importing its heap,
recursive execution and step/depth limits would be a separate feature decision.

## Tests and build tooling: reuse before rewriting infrastructure

| Existing source | Reuse |
| --- | --- |
| [build_support.py:20–34](../tools/build_support.py#L20-L34), [build_support.py:73–183](../tools/build_support.py#L73-L183) | Strict C11 flags, platform link handling, content-addressed builds, runtime compilation and native drivers. Already used by rewrite tests. |
| [bootstrap_support.py:11–33](../tools/bootstrap_support.py#L11-L33) | Generation-by-generation C reproduction check. Parameterize source root, entry, seed and output rather than using the old hard-coded `src/modular_cli` graph. |
| [test_mir_selfhost.py:49–77](../tools/test_mir_selfhost.py#L49-L77) | Build successive native compilers, compare generated C, and run smoke fixtures with a newly built compiler. Keep a rewrite-specific seed and test. |
| [test_match_tags.py:117–164](../tools/test_match_tags.py#L117-L164), [test_union_nested_coercions.py:65–95](../tools/test_union_nested_coercions.py#L65-L95) | Positive/negative label, grouped-match and nested-coercion cases; native/MIR comparisons and ARC patterns. Extract supported source cases and expected behavior. |
| [test_source_modules.py:13–44](../tools/test_source_modules.py#L13-L44), [test_mir_wire.py:44–84](../tools/test_mir_wire.py#L44-L84) | Temporary module trees, failure assertions, deterministic round trips and source-free execution checks. Artifact checks are for later. |

Also retain the source fixtures under `../tests/compiler/{fixtures,negative,
runtime_errors}`, `../tests/language`, and `../tests/mir`. In particular, use
`source_match_shared_bindings.sirius`, `source_union_same_shape.sirius`,
`source_union_nested_coercions.sirius`, `source_structural_equality.sirius`, and
the ownership/update drivers as behavioral specifications. Adapt harness entry
points; old serialized MIR and internal slot assertions are not rewrite tests.

Suggested improvement: allow rewrite test harnesses to select an externally
built compiler. Otherwise passing tests can continue exercising the original-
compiler-built rewrite instead of R2/R3, hiding a self-compilation defect.
Keep generated-program ARC checks distinct from auditing the compiler process;
the latter remains a documented blocker in `TODO.md`.

## Recommended order

1. Adapt labeled-union identity, checked alternatives and matching helpers;
   add structural equality needed by compiler sources. Test same-shaped labels,
   grouped binders, invalid payloads and managed snapshots from the start.
2. Add the minimal specialization registry/worklist. Prove repeated calls reuse
   one body and recursive result inference fails cleanly without an annotation.
3. Adapt source discovery, namespaces and runtime I/O. Compile the rewrite lexer
   with a driver first, then the full compiler graph, with deterministic order.
4. Reuse build helpers for a separate rewrite fixed point and run the existing
   rewrite suites through the newly built generations. Do not replace the old
   bootstrap until the rewrite seed is independently reproducible.
5. Profile real compiler workloads, then port only justified liveness/move/slot
   improvements. Preserve one value model and in-place reuse where ownership
   permits it; report copy/allocation counts alongside elapsed time.

Validation for this document: inspected implementation, current rewrite source,
milestones and representative test harnesses; checked referenced files and line
ranges. No compiler/runtime behavior was changed, and no execution or performance
claim was established by this inspection.
