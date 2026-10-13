"""Sirius compiler IR sketch derived from the bootstrap golden tests.

These are data-model definitions, not a parser or a complete lowering pass.
The intended pipeline is: tokens -> AST -> typed/specialized HIR -> CFG MIR -> C.
Both IR levels retain source locations for diagnostics.
"""
from __future__ import annotations
from dataclasses import dataclass, field
from enum import Enum, IntFlag, auto
from typing import TypeAlias

TypeId: TypeAlias = int
SymbolId: TypeAlias = int
LocalId: TypeAlias = int
BlockId: TypeAlias = int


@dataclass(frozen=True)
class Span:
    file: str
    line: int
    column: int
    end_line: int
    end_column: int


class TypeKind(Enum):
    PRIMITIVE = auto()
    STRING = auto()
    RECORD = auto()
    UNION = auto()
    OPTIONAL = auto()
    ARRAY = auto()
    DICT = auto()
    FUNCTION = auto()
    TYPE_VARIABLE = auto()  # pre-specialization only
    ANY = auto()  # later, not bootstrap


@dataclass(frozen=True)
class Field:
    name: str
    type_id: TypeId
    offset: int | None = None


@dataclass(frozen=True)
class Variant:
    # Constructor is used as pattern syntax; a variant's payload is a record.
    name: str
    payload_type: TypeId
    tag: int


@dataclass(frozen=True)
class TypeDescriptor:
    id: TypeId
    kind: TypeKind
    arguments: tuple[TypeId, ...] = ()
    fields: tuple[Field, ...] = ()
    variants: tuple[Variant, ...] = ()
    rank: int | None = None     # static for Array[T, N]; shape is dynamic
    size: int | None = None
    alignment: int | None = None
    # Canonical shape/field/type identity, NOT user-defined record names.
    # Type interning/layout calculates this from semantic type constituents.
    canonical_key: tuple[object, ...] = ()


class Effect(IntFlag):
    NONE = 0             # Pure computation and local mutation
    IO = auto()
    FFI = auto()         # Conservatively impure unless declared pure
    # Panic is permitted even in pure array callbacks.


@dataclass(frozen=True)
class Signature:
    parameters: tuple[TypeId, ...]
    returns: TypeId
    effects: Effect = Effect.NONE


@dataclass(frozen=True)
class Specialization:
    symbol: SymbolId
    signature: Signature
    # Monomorphization key: generic symbol + concrete parameter types.
    cache_key: tuple[SymbolId, tuple[TypeId, ...]]


# ---------- Typed high-level IR ----------
# Expr nodes retain surface structure needed for semantic checking.
# Literal types have already been resolved contextually.

@dataclass(frozen=True, kw_only=True)
class Expr:
    type_id: TypeId
    span: Span


@dataclass(frozen=True)
class Literal(Expr):
    value: bool | int | float | str


@dataclass(frozen=True)
class Read(Expr):
    symbol: SymbolId


@dataclass(frozen=True)
class FunctionValue(Expr):
    specialization: Specialization


@dataclass(frozen=True)
class Binary(Expr):
    operation: str
    lhs: Expr
    rhs: Expr
    # Resolved operator implementation, if not a builtin primitive.
    implementation: Specialization | None = None


@dataclass(frozen=True)
class Call(Expr):
    args: tuple[Expr, ...]
    direct: Specialization | None = None
    indirect: Expr | None = None
    # Exactly one of direct/indirect is used.
    # Dot-call methods become ordinary calls with a first receiver argument.


@dataclass(frozen=True)
class FieldRead(Expr):
    base: Expr
    field_index: int  # statically resolved


@dataclass(frozen=True)
class ArrayLiteral(Expr):
    elements: tuple[Expr, ...]


class Bounds(Enum):
    PROVEN = auto()    # a[i]
    OPTIONAL = auto()  # a[i?]
    PANIC = auto()     # a[i?]!


@dataclass(frozen=True)
class Index(Expr):
    base: Expr
    indices: tuple[Expr, ...]
    bounds: Bounds


@dataclass(frozen=True)
class RecordFieldSet:
    name: str
    value: Expr


@dataclass(frozen=True)
class RecordSpread:
    value: Expr


@dataclass(frozen=True)
class RecordDelete:
    name: str


RecordItem: TypeAlias = RecordFieldSet | RecordSpread | RecordDelete


@dataclass(frozen=True)
class RecordLiteral(Expr):
    # Preserve source order until evaluated, even when later items override.
    items: tuple[RecordItem, ...]


@dataclass(frozen=True)
class UnionInject(Expr):
    # Implicit, statically resolved injection from a record to a union type.
    tag: int
    payload: Expr


# Recursive statements are allowed; all local binding symbols are resolved.
@dataclass(frozen=True)
class Let:
    symbol: SymbolId
    mutable: bool
    value: Expr
    span: Span


@dataclass(frozen=True)
class FieldPath:
    indices: tuple[int, ...]


@dataclass(frozen=True)
class IndexPath:
    index: Expr
    bounds: Bounds


PathPart: TypeAlias = FieldPath | IndexPath


@dataclass(frozen=True)
class Assign:
    root_symbol: SymbolId  # compile-time check: mutable root
    path: tuple[PathPart, ...]
    value: Expr
    span: Span


@dataclass(frozen=True)
class If:
    condition: Expr
    then: tuple[Statement, ...]
    otherwise: tuple[Statement, ...]
    span: Span


@dataclass(frozen=True)
class While:
    condition: Expr
    body: tuple[Statement, ...]
    span: Span


@dataclass(frozen=True)
class For:
    symbol: SymbolId
    iterable: Expr
    body: tuple[Statement, ...]
    span: Span


@dataclass(frozen=True)
class Return:
    value: Expr | None
    span: Span


@dataclass(frozen=True)
class Recur:
    args: tuple[Expr, ...]
    span: Span


@dataclass(frozen=True)
class MatchArm:
    tag: int | None  # None for wildcard/default
    bind_symbols: tuple[SymbolId, ...]
    body: tuple[Statement, ...]


@dataclass(frozen=True)
class Match:
    value: Expr
    arms: tuple[MatchArm, ...]
    span: Span


@dataclass(frozen=True)
class Break:
    span: Span


@dataclass(frozen=True)
class Continue:
    span: Span


@dataclass(frozen=True)
class ExprStatement:
    value: Expr
    span: Span


Statement: TypeAlias = (Let | Assign | If | While | For | Return | Recur |
                        Match | Break | Continue | ExprStatement)


@dataclass(frozen=True)
class FunctionHIR:
    specialization: Specialization
    parameters: tuple[SymbolId, ...]
    body: tuple[Statement, ...]
    span: Span
    # Constructors are resolved here to functions returning record values.


# ---------- Lowered MIR (control-flow graph, C-oriented) ----------
# Instructions are explicit and non-nested. They run in listed order;
# branches/returns only occur in terminators. Locals are fixed typed slots.

@dataclass(frozen=True)
class Use:
    local: LocalId


@dataclass(frozen=True)
class Immediate:
    value: bool | int | float | str


Operand: TypeAlias = Use | Immediate


@dataclass(frozen=True)
class Op:
    destination: LocalId | None
    name: str
    args: tuple[Operand, ...] = ()
    # Examples: add_i64, lt_i64, call_direct, call_indirect,
    # make_record, record_field, array_get_optional, make_variant,
    # extract_variant, print_i64, map_get, read_text, write_text.
    # names are enumerated/validated by lowering in an implementation.
    metadata: tuple[object, ...] = ()
    span: Span | None = None


@dataclass(frozen=True)
class Retain:
    value: LocalId


@dataclass(frozen=True)
class Release:
    value: LocalId


@dataclass(frozen=True)
class MIRField:
    field_index: int


@dataclass(frozen=True)
class MIRIndex:
    index_local: LocalId
    bounds: Bounds


MIRPathPart: TypeAlias = MIRField | MIRIndex


@dataclass(frozen=True)
class CoWSet:
    root: LocalId
    path: tuple[MIRPathPart, ...]
    value: LocalId
    # Checks bounds, ensures uniqueness at affected levels, and replaces value.
    span: Span | None = None


Instruction: TypeAlias = Op | Retain | Release | CoWSet


@dataclass(frozen=True)
class Jump:
    target: BlockId


@dataclass(frozen=True)
class Branch:
    condition: LocalId
    if_true: BlockId
    if_false: BlockId


@dataclass(frozen=True)
class Switch:
    discriminant: LocalId
    cases: tuple[tuple[int, BlockId], ...]
    otherwise: BlockId


@dataclass(frozen=True)
class ReturnTerm:
    value: LocalId | None


@dataclass(frozen=True)
class PanicTerm:
    message: str


Terminator: TypeAlias = Jump | Branch | Switch | ReturnTerm | PanicTerm


@dataclass(frozen=True)
class Block:
    id: BlockId
    instructions: tuple[Instruction, ...]
    terminator: Terminator


@dataclass(frozen=True)
class Local:
    id: LocalId
    type_id: TypeId
    name_hint: str = ""
    # Borrow vs owned is determined by ARC/escape analysis, not syntax.


@dataclass(frozen=True)
class FunctionMIR:
    specialization: Specialization
    locals: tuple[Local, ...]
    arguments: tuple[LocalId, ...]
    blocks: tuple[Block, ...]
    entry: BlockId


@dataclass
class ModuleIR:
    types: dict[TypeId, TypeDescriptor] = field(default_factory=dict)
    functions: dict[tuple[SymbolId, tuple[TypeId, ...]], FunctionHIR] = field(default_factory=dict)
    lowered: list[FunctionMIR] = field(default_factory=list)
