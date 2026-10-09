"""First executable Sirius backend: specialization + typed, ownership-aware C lowering.

The syntax AST stays separate; this backend is deliberately small and is NOT yet
an explicit CFG MIR. All managed expression results are scoped owner temporaries.
"""
from __future__ import annotations
from dataclasses import dataclass
from contextlib import contextmanager
from syntax import Definition, Statement, Expr, TypeExpr, CompileError


@dataclass(frozen=True)
class T:
    kind: str
    name: str
    args: tuple['T', ...] = ()
    fields: tuple[tuple[str, 'T'], ...] = ()
    variants: tuple[tuple[str, 'T'], ...] = ()

    @property
    def c(self):
        return {'i64': 'int64_t', 'f64': 'double', 'bool': 'bool', 'void': 'void',
                'string': 'S_String *', 'array': 'S_Array_i64 *',
                'dict': 'S_Dict_i64_i64 *', 'opt_i64': 'S_Optional_i64',
                'opt_string': 'S_Optional_string'}.get(self.kind, self.name)

    @property
    def managed(self):
        return (self.kind in ('string', 'array', 'dict', 'opt_string')
                or (self.kind == 'record' and any(t.managed for _, t in self.fields))
                or (self.kind == 'union' and any(t.managed for _, t in self.variants)))


I64 = T('i64', 'i64')
F64 = T('f64', 'f64')
BOOL = T('bool', 'bool')
VOID = T('void', 'void')
STRING = T('string', 'string')
ARRAY = T('array', 'Array[i64,1]', (I64,))
DICT = T('dict', 'Dict[i64,i64]', (I64, I64))
OPT_I64 = T('opt_i64', 'Optional[i64]', (I64,))
OPT_STRING = T('opt_string', 'Optional[string]', (STRING,))


@dataclass
class Value:
    code: str
    typ: T


@dataclass
class Binding:
    typ: T
    mutable: bool
    cname: str
    immutable_reason: str = 'E_IMMUTABLE_ASSIGN'


@dataclass
class Instance:
    name: str
    args: tuple[T, ...]
    ret: T
    c_name: str
    params: tuple[str, ...]
    code: str = ''
    building: bool = False


class Context:
    def __init__(self, compiler: 'Compiler', inst: Instance, defn: Definition,
                 outer_names: set[str] | None = None):
        self.compiler = compiler
        self.inst = inst
        self.defn = defn
        self.outer_names = outer_names or set()
        self.lines: list[str] = []
        self.indent = 1
        self.bindings: dict[str, Binding] = {}
        self.scopes: list[list[tuple[str | None, Binding | None, str | None, T | None]]] = [[]]
        self.counter = 0
        self.loop_scope_depths: list[int] = []
        self.published: dict[str, Value] = {}
        for (name, _), typ in zip(defn.params, inst.args):
            self.bindings[name] = Binding(typ, False, f'arg_{name}', 'E_IMMUTABLE_PARAMETER')

    def error(self, code: str, line: int, message: str):
        raise CompileError(code, line, message)

    def emit(self, statement: str):
        self.lines.append('    ' * self.indent + statement)

    def fresh(self, label='v'):
        self.counter += 1
        return f'_s_{label}_{self.counter}'

    @contextmanager
    def block(self, header: str):
        self.emit(header + ' {')
        self.indent += 1
        self.scopes.append([])
        try:
            yield
        finally:
            self.cleanup(len(self.scopes) - 1)
            self.pop_scope()
            self.indent -= 1
            self.emit('}')

    def pop_scope(self):
        for name, previous, _, _ in reversed(self.scopes.pop()):
            if name is not None:
                if previous is None:
                    self.bindings.pop(name, None)
                else:
                    self.bindings[name] = previous

    def cleanup(self, depth: int):
        for scope in reversed(self.scopes[depth:]):
            for _, _, cname, typ in reversed(scope):
                if typ is not None and typ.managed:
                    self.emit(f'{self.compiler.drop_code(typ, cname)};')

    def temp(self, expr: str, typ: T, prefix='tmp') -> Value:
        cname = self.fresh(prefix)
        self.emit(f'{self.compiler.declaration(typ, cname)} = {expr};')
        self.scopes[-1].append((None, None, cname, typ))
        return Value(cname, typ)

    def snapshot(self, v: Value) -> Value:
        # Evaluate now, preserving Python-like left-to-right operand/argument order.
        # A managed snapshot owns an additional reference, even if v is borrowed.
        return self.temp(self.compiler.copy_code(v.typ, v.code), v.typ, 'eval')

    def bind(self, name: str, typ: T, val: Value, mutable: bool, reason='E_IMMUTABLE_ASSIGN'):
        previous = self.bindings.get(name)
        if previous is not None:
            # Shadowing works by assigning a fresh C symbol and restoring previous binding.
            pass
        cname = self.fresh(name)
        self.emit(f'{self.compiler.declaration(typ, cname)} = {self.compiler.copy_code(typ, val.code)};')
        self.emit(f'(void){cname};')  # Valid for unused bindings in generated C11.
        self.scopes[-1].append((name, previous, cname, typ))
        self.bindings[name] = Binding(typ, mutable, cname, reason)

    def lookup(self, name: str, line: int) -> Binding:
        if name not in self.bindings:
            code = 'E_CAPTURE_NOT_SUPPORTED' if name in self.outer_names else 'E_UNKNOWN_SYMBOL'
            self.error(code, line, f'unknown variable {name!r}')
        return self.bindings[name]


class Compiler:
    def __init__(self, program: tuple[Definition | Statement, ...]):
        self.defs: dict[str, Definition] = {d.name: d for d in program if isinstance(d, Definition)}
        self.aliases: dict[str, T] = {}
        self.record_cache: dict[tuple[tuple[str, T], ...], T] = {}
        self.instances: dict[tuple[str, tuple[T, ...]], Instance] = {}
        self.extra_functions: dict[str, Definition] = {}
        self.num_lambda = 0
        self.module_lets = [d for d in program if isinstance(d, Statement)]
        for d in self.module_lets:
            if d.kind != 'let' or d.args[0].kind != 'binary' or d.args[0].value != '|':
                raise CompileError('E_MODULE', d.line, 'only union type aliases are supported at module scope')
        self.prep_aliases()

    def error(self, code, line, message):
        raise CompileError(code, line, message)

    def record(self, fields: dict[str, T]) -> T:
        canonical = tuple(sorted(fields.items()))
        if canonical not in self.record_cache:
            self.record_cache[canonical] = T('record', f'S_Record_{len(self.record_cache)}', fields=canonical)
        return self.record_cache[canonical]

    def resolve(self, typ: TypeExpr | None) -> T | None:
        if typ is None: return None
        table = {'i64': I64, 'f64': F64, 'bool': BOOL, 'string': STRING,
                 'void': VOID}
        if typ.name in table and not typ.args: return table[typ.name]
        if typ.name == 'Array' and typ.args == (TypeExpr('i64'), 1): return ARRAY
        if typ.name == 'Dict' and typ.args == (TypeExpr('i64'), TypeExpr('i64')): return DICT
        if typ.name in self.aliases and not typ.args: return self.aliases[typ.name]
        self.error('E_UNKNOWN_TYPE', 0, f'unsupported type {typ!r}')

    def prep_aliases(self):
        # Union aliases here are built from annotated constructor signatures.
        # This bootstrap phase intentionally excludes recursive union definitions.
        for statement in self.module_lets:
            name, annotation = statement.value
            if annotation is not None:
                self.error('E_MODULE', statement.line, 'union aliases cannot have value annotations')
            def alternatives(e):
                if e.kind == 'binary' and e.value == '|':
                    return alternatives(e.args[0]) + alternatives(e.args[1])
                if e.kind == 'name' and e.value in self.defs:
                    return [e.value]
                self.error('E_UNION_TYPE', e.line, 'union alternatives must be constructors')
            variant_names = alternatives(statement.args[0])
            variants = []
            for ctor in variant_names:
                d = self.defs[ctor]
                params = tuple(self.resolve(ann) for _, ann in d.params)
                if any(t is None for t in params):
                    self.error('E_UNION_TYPE', d.line, 'union constructors need annotated parameters')
                inst = self.specialize(ctor, params)
                if inst.ret.kind != 'record':
                    self.error('E_UNION_TYPE', d.line, 'union constructor must produce a record')
                if inst.ret in [typ for _, typ in variants]:
                    self.error('E_DUPLICATE_UNION_VARIANT', d.line, 'structurally identical variants cannot be distinguished')
                variants.append((ctor, inst.ret))
            self.aliases[name] = T('union', f'S_Union_{name}', variants=tuple(variants))

    def declaration(self, typ: T, name: str) -> str:
        if typ.kind == 'function':
            args = ', '.join(a.c for a in typ.args[:-1]) or 'void'
            return f'{typ.args[-1].c} (*{name})({args})'
        return f'{typ.c} {name}'

    def copy_code(self, typ: T, code: str) -> str:
        if typ.kind == 'string': return f's_string_retain({code})'
        if typ.kind == 'opt_string': return f's_opt_string_retain({code})'
        if typ.kind == 'union' and typ.managed: return f'{typ.name}_retain({code})'
        if typ.kind == 'array': return f's_array_i64_retain({code})'
        if typ.kind == 'dict': return f's_dict_i64_i64_retain({code})'
        if typ.kind == 'record' and typ.managed: return f'{typ.name}_retain({code})'
        return code

    def drop_code(self, typ: T, code: str) -> str:
        if typ.kind == 'string': return f's_string_release({code})'
        if typ.kind == 'opt_string': return f's_opt_string_drop({code})'
        if typ.kind == 'union' and typ.managed: return f'{typ.name}_drop({code})'
        if typ.kind == 'array': return f's_array_i64_release({code})'
        if typ.kind == 'dict': return f's_dict_i64_i64_release({code})'
        if typ.kind == 'record' and typ.managed: return f'{typ.name}_drop({code})'
        return f'(void)({code})'

    def cast(self, ctx: Context, v: Value, dest: T, line: int) -> Value:
        if v.typ == dest: return v
        if dest.kind == 'union':
            for n, (ctor, variant) in enumerate(dest.variants):
                if variant == v.typ:
                    return ctx.temp(f'({dest.c}){{ .tag = {n}, .data.v{n} = {self.copy_code(variant, v.code)} }}', dest)
        self.error('E_NUMERIC_TYPE_MISMATCH' if v.typ in (I64,F64) and dest in (I64,F64)
                   else 'E_TYPE_MISMATCH', line, f'{v.typ.name} is not assignable to {dest.name}')

    def specialize(self, name: str, args: tuple[T, ...]) -> Instance:
        key = (name, args)
        if key in self.instances:
            return self.instances[key]
        d = self.defs.get(name) or self.extra_functions.get(name)
        if d is None:
            self.error('E_UNKNOWN_SYMBOL', 0, f'unknown function {name}')
        if len(args) != len(d.params):
            self.error('E_ARGUMENT_COUNT', d.line, f'{name} expects {len(d.params)} arguments')
        for (param, ann), typ in zip(d.params, args):
            expected = self.resolve(ann)
            if expected is not None and expected != typ:
                self.error('E_TYPE_MISMATCH', d.line, f'parameter {param} expects {expected.name}, got {typ.name}')
        ret = self.resolve(d.returns) or VOID
        c_name = 'main' if name == 'main' else f's_{name}_{len(self.instances)}'
        inst = Instance(name, args, ret, c_name, tuple(p for p, _ in d.params), building=True)
        self.instances[key] = inst
        ctx = Context(self, inst, d)
        for s in d.body:
            self.statement(ctx, s)
        if ctx.published:
            if any(s.kind == 'return' for s in d.body):
                self.error('E_CONSTRUCTOR_RETURN', d.line, 'constructor cannot return an expression')
            ret = self.record({n: v.typ for n, v in ctx.published.items()})
            inst.ret = ret
            designated = ', '.join(f'.{n} = {self.copy_code(v.typ, v.code)}' for n, v in ctx.published.items())
            returned = ctx.fresh('constructed')
            ctx.emit(f'{ret.c} {returned} = ({ret.c}){{{designated}}};')
            ctx.cleanup(0)
            ctx.emit(f'return {returned};')
        elif ret == VOID and d.returns is None and inst.ret == VOID:
            # An inferred-return evaluator has to have encountered a return expression.
            self.error('E_NO_RETURN', d.line, f'cannot infer the return type of {name}')
        if inst.ret != VOID:
            ctx.emit('s_panic("missing return");')
        else:
            ctx.cleanup(0)
        # Keep a statement-level cleanup for normal exits as well; returns add their own.
        params = ', '.join(self.declaration(a, f'arg_{p}') for a, p in zip(args, inst.params)) or 'void'
        result = 'int' if name == 'main' else inst.ret.c
        def contains_recur(statements):
            for stmt in statements:
                if stmt.kind == 'recur': return True
                if stmt.kind == 'if':
                    if any(contains_recur(body) for _, body in stmt.value): return True
                    if contains_recur(stmt.args[0]): return True
                if stmt.kind in ('while','for') and contains_recur(stmt.args[-1]): return True
            return False
        if contains_recur(d.body):
            # Direct recur is a real loop rather than a recursive C call.
            lines = ['    for (;;) {', *('    ' + line for line in ctx.lines), '    }']
        else:
            lines = ctx.lines
        inst.code = f'{result} {c_name}({params}) {{\n' + '\n'.join(lines) + '\n}'
        inst.building = False
        return inst

    def typed(self, ctx: Context, e: Expr, expected: T | None = None) -> Value:
        v = self.expr(ctx, e)
        if expected is not None:
            v = self.cast(ctx, v, expected, e.line)
        return v

    def expr(self, ctx: Context, e: Expr) -> Value:
        k = e.kind
        if k == 'int': return Value(f'INT64_C({e.value})', I64)
        if k == 'float': return Value(e.value, F64)
        if k == 'bool': return Value('true' if e.value else 'false', BOOL)
        if k == 'string':
            import json
            return ctx.temp(f's_string_from_utf8({json.dumps(e.value)})', STRING)
        if k == 'name':
            if e.value in ctx.bindings:
                b = ctx.lookup(e.value, e.line)
                return Value(b.cname, b.typ)
            if e.value in self.defs or e.value in self.extra_functions:
                d = self.defs.get(e.value) or self.extra_functions[e.value]
                if any(a is None for _, a in d.params) or d.returns is None:
                    self.error('E_FUNCTION_SIGNATURE', e.line, 'function values need concrete annotated signatures')
                args = tuple(self.resolve(t) for _, t in d.params)
                inst = self.specialize(e.value, args)
                return Value(inst.c_name, T('function', 'fn', (*args, inst.ret)))
            return Value(ctx.lookup(e.value, e.line).cname, VOID)
        if k == 'unary':
            x = self.expr(ctx, e.args[0])
            if e.value == 'not':
                if x.typ != BOOL: self.error('E_TYPE_MISMATCH', e.line, 'not requires bool')
                return Value(f'(!{x.code})', BOOL)
            if x.typ not in (I64, F64): self.error('E_TYPE_MISMATCH', e.line, 'unary numeric operator requires number')
            return Value(f'({e.value}{x.code})', x.typ)
        if k == 'binary':
            op = e.value
            lhs = ctx.snapshot(self.expr(ctx, e.args[0]))
            if op in ('and', 'or') and lhs.typ == BOOL:
                out = ctx.fresh('logic')
                ctx.emit(f'bool {out};')
                ctx.emit(f'{out} = {lhs.code};')
                with ctx.block(f'if ({lhs.code if op == "and" else "!" + lhs.code})'):
                    rhs = self.typed(ctx, e.args[1], BOOL)
                    ctx.emit(f'{out} = {rhs.code};')
                return Value(out, BOOL)
            if op == 'or' and lhs.typ.kind.startswith('opt_'):
                target = lhs.typ.args[0]
                result = ctx.fresh('fallback')
                ctx.emit(f'{self.declaration(target, result)};')
                ctx.scopes[-1].append((None, None, result, target))
                with ctx.block(f'if ({lhs.code}.has_value)'):
                    ctx.emit(f'{result} = {self.copy_code(target, f"{lhs.code}.value")};')
                with ctx.block('else'):
                    rhs = self.typed(ctx, e.args[1], target)
                    ctx.emit(f'{result} = {self.copy_code(target, rhs.code)};')
                return Value(result, target)
            rhs = ctx.snapshot(self.expr(ctx, e.args[1]))
            if op == '..': return Value('', T('range', 'range', (lhs.typ, rhs.typ)))
            if op == '+' and lhs.typ == rhs.typ == STRING:
                return ctx.temp(f's_string_concat({lhs.code}, {rhs.code})', STRING)
            if lhs.typ != rhs.typ:
                self.error('E_NUMERIC_TYPE_MISMATCH' if lhs.typ in (I64,F64) and rhs.typ in (I64,F64)
                           else 'E_TYPE_MISMATCH', e.line, f'operator {op}: {lhs.typ.name} vs {rhs.typ.name}')
            if op in ('+', '-', '*', '/', '%'):
                if lhs.typ not in (I64, F64): self.error('E_TYPE_MISMATCH', e.line, 'arithmetic needs scalars')
                if op == '%' and lhs.typ != I64: self.error('E_TYPE_MISMATCH', e.line, '% needs integers')
                return Value(f'({lhs.code} {op} {rhs.code})', lhs.typ)
            if op in ('==', '!=', '<', '<=', '>', '>='):
                return Value(f'({lhs.code} {op} {rhs.code})', BOOL)
            self.error('E_UNSUPPORTED', e.line, f'operator {op} unsupported at runtime')
        if k == 'array':
            items = [ctx.snapshot(self.typed(ctx, x, I64)) for x in e.args]
            return ctx.temp(f's_array_i64_make({len(items)}, (int64_t[]){{{", ".join(x.code for x in items)}}})', ARRAY)
        if k == 'record':
            fields: dict[str, Value] = {}
            for kind, name, expr in e.value:
                if kind == 'spread':
                    val = ctx.lookup(name, e.line)
                    if val.typ.kind != 'record': self.error('E_TYPE_MISMATCH', e.line, 'spread needs a record')
                    for field, typ in val.typ.fields:
                        fields[field] = Value(f'{val.cname}.{field}', typ)
                elif kind == 'del':
                    if name not in fields: self.error('E_UNKNOWN_FIELD', e.line, f'cannot delete absent field {name}')
                    del fields[name]
                else:
                    fields[name] = ctx.snapshot(self.expr(ctx, expr))
            typ = self.record({name: v.typ for name, v in fields.items()})
            vals = ', '.join(f'.{name} = {self.copy_code(v.typ, v.code)}' for name, v in fields.items())
            return ctx.temp(f'({typ.c}){{{vals}}}', typ)
        if k == 'attr':
            b = self.expr(ctx, e.args[0])
            if b.typ.kind == 'record':
                fields = dict(b.typ.fields)
                if e.value not in fields:
                    self.error('E_UNKNOWN_FIELD', e.line, f'{b.typ.name} has no field {e.value}')
                return Value(f'{b.code}.{e.value}', fields[e.value])
            self.error('E_UNKNOWN_FIELD', e.line, f'{b.typ.name} has no field {e.value}')
        if k == 'index':
            arr = ctx.snapshot(self.expr(ctx, e.args[0]))
            ix = ctx.snapshot(self.typed(ctx, e.args[1], I64))
            if arr.typ != ARRAY: self.error('E_TYPE_MISMATCH', e.line, 'index expects Array[i64,1]')
            if e.value == 'proven':
                self.error('E_INDEX_NOT_PROVEN_SAFE', e.line, 'bounds not statically proven')
            if e.value == 'panic':
                return Value(f's_array_i64_get_checked({arr.code}, {ix.code})', I64)
            return ctx.temp(f's_array_i64_get({arr.code}, {ix.code})', OPT_I64)
        if k == 'lambda':
            for _, annotation in e.value:
                if annotation is None: self.error('E_FUNCTION_SIGNATURE', e.line, 'lambda arguments need types')
            self.num_lambda += 1
            name = f'lambda_{self.num_lambda}'
            params = e.value
            outer = set(ctx.bindings)
            defn = Definition(name, params, None, (Statement('return', e.args[0], (), e.line),), e.line)
            self.extra_functions[name] = defn
            args = tuple(self.resolve(t) for _, t in params)
            key = (name, args)
            inst = Instance(name, args, VOID, f's_{name}', tuple(n for n, _ in params))
            self.instances[key] = inst
            sub = Context(self, inst, defn, outer_names=outer)
            self.statement(sub, defn.body[0])
            sub.emit('s_panic("missing return");')
            params_c = ', '.join(self.declaration(t, f'arg_{n}') for (n, _), t in zip(params, args))
            inst.code = f'{inst.ret.c} {inst.c_name}({params_c}) {{\n' + '\n'.join(sub.lines) + '\n}'
            return Value(inst.c_name, T('function', 'fn', (*args, inst.ret)))
        if k == 'dict_type':
            if e.value != (TypeExpr('i64'), TypeExpr('i64')):
                self.error('E_UNKNOWN_TYPE', e.line, 'only Dict[i64,i64] available initially')
            return Value('s_dict_i64_i64_new()', DICT)
        if k == 'call':
            callee = e.args[0]
            if callee.kind == 'name' and callee.value in ('print','sqrt','write_text','read_text'):
                return self.builtin(ctx, callee.value, e.args[1:], e.line)
            if callee.kind == 'name' and callee.value in self.defs:
                args = [ctx.snapshot(self.expr(ctx, a)) for a in e.args[1:]]
                return self.call(ctx, callee.value, args, e.line)
            if callee.kind == 'dict_type':
                if len(e.args) != 1: self.error('E_ARGUMENT_COUNT', e.line, 'Dict() takes no arguments')
                return ctx.temp(self.expr(ctx, callee).code, DICT)
            if callee.kind == 'attr':
                receiver = ctx.snapshot(self.expr(ctx, callee.args[0]))
                method = callee.value
                args = [ctx.snapshot(self.expr(ctx, a)) for a in e.args[1:]]
                if receiver.typ == DICT:
                    if method == 'insert' and len(args) == 2:
                        key, val = (self.cast(ctx, a, I64, e.line) for a in args)
                        return ctx.temp(f's_dict_i64_i64_insert(s_dict_i64_i64_retain({receiver.code}), {key.code}, {val.code})', DICT)
                    if method == 'get' and len(args) == 1:
                        key = self.cast(ctx, args[0], I64, e.line)
                        return ctx.temp(f's_dict_i64_i64_get({receiver.code}, {key.code})', OPT_I64)
                if method == 'map' and receiver.typ == ARRAY:
                    # Until map is implemented, reject it honestly as unsupported.
                    # A known directly/transitively impure named callback still gets
                    # the more specific purity diagnostic.
                    callback = e.args[1] if len(e.args) == 2 else None
                    if callback and callback.kind == 'name' and self.is_impure(callback.value):
                        self.error('E_PURITY_VIOLATION', e.line, 'map callback has observable effects')
                    self.error('E_UNSUPPORTED', e.line, 'array.map has not been implemented')
                if method in self.defs:
                    return self.call(ctx, method, [receiver, *args], e.line)
                self.error('E_UNKNOWN_METHOD', e.line, f'{receiver.typ.name} has no method {method}')
            fn = self.expr(ctx, callee)
            if fn.typ.kind == 'function':
                args = [ctx.snapshot(self.cast(ctx, self.expr(ctx, a), t, e.line)) for a, t in zip(e.args[1:], fn.typ.args[:-1])]
                if len(args) != len(fn.typ.args)-1: self.error('E_ARGUMENT_COUNT', e.line, 'wrong function pointer arity')
                ret = fn.typ.args[-1]
                raw = f'{fn.code}({", ".join(a.code for a in args)})'
                return ctx.temp(raw, ret) if ret.managed else Value(raw, ret)
            self.error('E_NOT_CALLABLE', e.line, f'{fn.typ.name} not callable')
        self.error('E_UNSUPPORTED', e.line, f'expression kind {k} unsupported')

    def is_impure(self, name: str, visiting: frozenset[str] = frozenset()) -> bool:
        if name in visiting: return False
        definition = self.defs.get(name)
        if definition is None: return True
        def expression_impure(expr: Expr) -> bool:
            if expr.kind == 'call':
                fn = expr.args[0]
                if fn.kind == 'name':
                    if fn.value in ('print', 'write_text', 'read_text'): return True
                    if fn.value in self.defs and self.is_impure(fn.value, visiting | {name}): return True
            return any(expression_impure(a) for a in expr.args)
        def statement_impure(stmt: Statement) -> bool:
            if isinstance(stmt.value, Expr) and expression_impure(stmt.value): return True
            if stmt.kind in ('let', 'var', 'assign'):
                return any(isinstance(a, Expr) and expression_impure(a) for a in stmt.args)
            if stmt.kind == 'if':
                return (any(expression_impure(test) or any(statement_impure(x) for x in body)
                            for test, body in stmt.value)
                        or any(statement_impure(x) for x in stmt.args[0]))
            if stmt.kind in ('while','for'):
                return any(isinstance(a, Expr) and expression_impure(a) or
                           isinstance(a, tuple) and any(statement_impure(x) for x in a)
                           for a in stmt.args) or (
                           stmt.kind == 'while' and expression_impure(stmt.value))
            return False
        return any(statement_impure(s) for s in definition.body)

    def builtin(self, ctx: Context, name: str, args: tuple[Expr, ...], line: int) -> Value:
        vals = [ctx.snapshot(self.expr(ctx, e)) for e in args]
        if name == 'print' and len(vals) == 1:
            typ = vals[0].typ
            printers = {I64: 's_print_i64', F64: 's_print_f64', BOOL: 's_print_bool', STRING: 's_print_string'}
            if typ not in printers: self.error('E_TYPE_MISMATCH', line, 'cannot print this type yet')
            return Value(f'{printers[typ]}({vals[0].code})', VOID)
        if name == 'sqrt' and len(vals) == 1:
            v = self.cast(ctx, vals[0], F64, line)
            return Value(f'sqrt({v.code})', F64)
        if name == 'write_text' and len(vals) == 2:
            for val in vals: self.cast(ctx, val, STRING, line)
            return Value(f's_write_text({vals[0].code}, {vals[1].code})', BOOL)
        if name == 'read_text' and len(vals) == 1:
            self.cast(ctx, vals[0], STRING, line)
            return ctx.temp(f's_read_text({vals[0].code})', OPT_STRING)
        self.error('E_ARGUMENT_COUNT', line, f'invalid arguments to {name}')

    def call(self, ctx: Context, name: str, args: list[Value], line: int) -> Value:
        definition = self.defs.get(name) or self.extra_functions.get(name)
        if len(args) != len(definition.params):
            self.error('E_ARGUMENT_COUNT', line, f'{name} expects {len(definition.params)} arguments')
        args = [self.cast(ctx, v, target, line) if target is not None else v
                for v, (_, annotation) in zip(args, definition.params)
                for target in (self.resolve(annotation),)]
        inst = self.specialize(name, tuple(v.typ for v in args))
        expr = f'{inst.c_name}({", ".join(v.code for v in args)})'
        if inst.ret == VOID: return Value(expr, VOID)
        return ctx.temp(expr, inst.ret) if inst.ret.managed else Value(expr, inst.ret)

    def stmt_block(self, ctx: Context, body: tuple[Statement, ...], header: str):
        with ctx.block(header):
            for s in body: self.statement(ctx, s)

    def statement(self, ctx: Context, s: Statement):
        kind = s.kind
        if kind in ('let', 'var'):
            name, ann = s.value
            val = self.expr(ctx, s.args[0])
            if ann is not None: val = self.cast(ctx, val, self.resolve(ann), s.line)
            if s.args[1] is not None:
                if val.typ.kind not in ('opt_i64', 'opt_string'):
                    self.error('E_TYPE_MISMATCH', s.line, 'else return requires Optional')
                ctx.emit(f'if (!{val.code}.has_value) {{')
                ctx.indent += 1
                self.statement(ctx, s.args[1])
                ctx.indent -= 1
                ctx.emit('}')
                val = Value(f'{val.code}.value', val.typ.args[0])
            ctx.bind(name, val.typ, val, mutable=(kind == 'var'))
            return
        if kind == 'pub':
            val = self.expr(ctx, s.args[0]) if s.args[0] is not None else self.expr(ctx, Expr('name', s.value, (), s.line))
            if s.value not in ctx.published:
                if len(ctx.scopes) != 1:
                    self.error('E_UNCONDITIONAL_FIELD_REQUIRED', s.line, 'conditional branches cannot introduce published fields')
                # The published field is an owned, mutable *compiler-internal* slot.
                ctx.published[s.value] = ctx.snapshot(val)
            else:
                prev = ctx.published[s.value]
                val = self.cast(ctx, val, prev.typ, s.line)
                saved = ctx.snapshot(val)
                if prev.typ.managed:
                    ctx.emit(f'{self.drop_code(prev.typ, prev.code)};')
                ctx.emit(f'{prev.code} = {self.copy_code(prev.typ, saved.code)};')
            # `pub x` freezes subsequent ordinary assignment to the source binding.
            if s.args[0] is None and s.value in ctx.bindings:
                ctx.bindings[s.value].mutable = False
            return
        if kind == 'return':
            value = self.expr(ctx, s.value) if s.value is not None else Value('', VOID)
            if ctx.inst.ret == VOID and ctx.defn.returns is None:
                ctx.inst.ret = value.typ
            else:
                value = self.cast(ctx, value, ctx.inst.ret, s.line)
            if value.typ.managed:
                owned = ctx.fresh('result')
                ctx.emit(f'{self.declaration(value.typ, owned)} = {self.copy_code(value.typ, value.code)};')
                ctx.cleanup(0)
                ctx.emit(f'return {owned};')
            else:
                if value.typ != VOID:
                    cname = ctx.fresh('result')
                    ctx.emit(f'{self.declaration(value.typ, cname)} = {value.code};')
                    ctx.cleanup(0)
                    ctx.emit(f'return {cname};')
                else:
                    ctx.cleanup(0)
                    ctx.emit('return;')
            return
        if kind == 'expr':
            val = self.expr(ctx, s.value)
            if val.typ == VOID: ctx.emit(f'{val.code};')
            return
        if kind == 'assign':
            target, rhs = s.args
            node = target
            while node.kind in ('attr', 'index'):
                node = node.args[0]
            if node.kind != 'name':
                self.error('E_ASSIGN_TARGET', s.line, 'assignment requires a variable root')
            b = ctx.lookup(node.value, s.line)
            if not b.mutable:
                self.error(b.immutable_reason, s.line, f'{node.value} is immutable')
            if target.kind == 'index':
                # Native CoW for array field or root array; writes require checked index.
                arr = self.expr(ctx, target.args[0]); idx = self.typed(ctx, target.args[1], I64)
                if target.value != 'panic':
                    self.error('E_INDEX_NOT_PROVEN_SAFE', s.line, 'use [index?]! for checked update')
                val = self.typed(ctx, rhs, I64)
                if arr.typ != ARRAY: self.error('E_TYPE_MISMATCH', s.line, 'indexed assignment requires Array')
                if s.value != '=': self.error('E_UNSUPPORTED', s.line, 'compound indexed assignment deferred')
                ctx.emit(f's_array_i64_set(&{arr.code}, {idx.code}, {val.code});')
                return
            if target.kind == 'attr':
                dest = self.expr(ctx, target)
                if s.value == '=':
                    rhsval = self.typed(ctx, rhs, dest.typ)
                    if dest.typ.managed:
                        copied = ctx.temp(self.copy_code(dest.typ, rhsval.code), dest.typ)
                        ctx.emit(f'{self.drop_code(dest.typ, dest.code)};')
                        ctx.emit(f'{dest.code} = {self.copy_code(dest.typ, copied.code)};')
                    else: ctx.emit(f'{dest.code} = {rhsval.code};')
                else:
                    val = self.typed(ctx, rhs, dest.typ)
                    ctx.emit(f'{dest.code} {s.value} {val.code};')
                return
            if target.kind != 'name': self.error('E_ASSIGN_TARGET', s.line, 'unsupported lvalue')
            val = self.typed(ctx, rhs, b.typ)
            if s.value == '=':
                if b.typ.managed:
                    cp = ctx.temp(self.copy_code(b.typ, val.code), b.typ)
                    ctx.emit(f'{self.drop_code(b.typ, b.cname)};')
                    ctx.emit(f'{b.cname} = {self.copy_code(b.typ, cp.code)};')
                else: ctx.emit(f'{b.cname} = {val.code};')
            else:
                if b.typ not in (I64, F64): self.error('E_TYPE_MISMATCH', s.line, 'compound assignments require scalars')
                ctx.emit(f'{b.cname} {s.value} {val.code};')
            return
        if kind == 'if':
            # A nested else keeps later condition preludes lazy and legal C11.
            def arms(index):
                if index == len(s.value):
                    for stmt in s.args[0]: self.statement(ctx, stmt)
                    return
                test, body = s.value[index]
                val = self.typed(ctx, test, BOOL)
                self.stmt_block(ctx, body, f'if ({val.code})')
                if index + 1 < len(s.value) or s.args[0]:
                    with ctx.block('else'): arms(index+1)
            arms(0)
            return
        if kind == 'while':
            # Re-evaluate the entire test, including its statement preludes.
            ctx.loop_scope_depths.append(len(ctx.scopes))
            with ctx.block('while (true)'):
                test = self.typed(ctx, s.value, BOOL)
                ctx.emit(f'if (!({test.code})) {{')
                ctx.indent += 1
                ctx.cleanup(len(ctx.scopes)-1)
                ctx.emit('break;')
                ctx.indent -= 1
                ctx.emit('}')
                for stmt in s.args[0]: self.statement(ctx, stmt)
            ctx.loop_scope_depths.pop()
            return
        if kind == 'for':
            iterable = s.args[0]
            if iterable.kind != 'binary' or iterable.value != '..':
                self.error('E_UNSUPPORTED', s.line, 'initial for supports integer range')
            lo = self.typed(ctx, iterable.args[0], I64)
            hi = self.typed(ctx, iterable.args[1], I64)
            end = ctx.fresh('end')
            ctx.emit(f'int64_t {end} = {hi.code};')
            counter = ctx.fresh('for')
            ctx.loop_scope_depths.append(len(ctx.scopes))
            with ctx.block(f'for (int64_t {counter} = {lo.code}; {counter} < {end}; ++{counter})'):
                old = ctx.bindings.get(s.value)
                ctx.bindings[s.value] = Binding(I64, False, counter)
                ctx.scopes[-1].append((s.value, old, None, None))
                for stmt in s.args[1]: self.statement(ctx, stmt)
            ctx.loop_scope_depths.pop()
            return
        if kind in ('break', 'continue'):
            if not ctx.loop_scope_depths: self.error('E_LOOP_CONTROL', s.line, f'{kind} outside loop')
            ctx.cleanup(ctx.loop_scope_depths[-1])
            ctx.emit(f'{kind};')
            return
        if kind == 'recur':
            call = s.value
            if call.kind != 'call' or call.args[0].kind != 'name' or call.args[0].value != ctx.inst.name:
                self.error('E_RECUR', s.line, 'recur must be direct self call')
            vals = [self.expr(ctx, e) for e in call.args[1:]]
            if len(vals) != len(ctx.inst.args): self.error('E_ARGUMENT_COUNT', s.line, 'bad recur count')
            names = []
            for val, typ in zip(vals, ctx.inst.args):
                val = self.cast(ctx, val, typ, s.line)
                new = ctx.fresh('next')
                ctx.emit(f'{self.declaration(typ, new)} = {val.code};')
                names.append(new)
            # Only scalar recur initially, as the bootstrap golden fixture uses.
            if any(t.managed for t in ctx.inst.args):
                self.error('E_UNSUPPORTED', s.line, 'managed recur awaits ownership-aware MIR')
            ctx.cleanup(0)
            for param, n in zip(ctx.inst.params, names): ctx.emit(f'arg_{param} = {n};')
            ctx.emit('continue;')
            return
        if kind == 'match':
            subj = self.expr(ctx, s.value)
            if subj.typ.kind != 'union': self.error('E_TYPE_MISMATCH', s.line, 'match requires union')
            arm_names = {name for name, _, _ in s.args[0]}
            if arm_names != {name for name, _ in subj.typ.variants}:
                self.error('E_NON_EXHAUSTIVE_MATCH', s.line, 'match arms must cover each alternative')
            ctx.emit(f'switch ({subj.code}.tag) {{')
            ctx.indent += 1
            for name, binders, body in s.args[0]:
                num = next(i for i, (nm, _) in enumerate(subj.typ.variants) if nm == name)
                fields = subj.typ.variants[num][1].fields
                if len(binders) != len(fields): self.error('E_PATTERN_ARITY', s.line, 'pattern field count mismatch')
                ctx.emit(f'case {num}: {{')
                ctx.indent += 1; ctx.scopes.append([])
                for binder, (field, typ) in zip(binders, fields):
                    ctx.bind(binder, typ, Value(f'{subj.code}.data.v{num}.{field}', typ), False)
                for stmt in body: self.statement(ctx, stmt)
                ctx.cleanup(len(ctx.scopes)-1); ctx.pop_scope()
                ctx.emit('break;')
                ctx.indent -= 1; ctx.emit('}')
            ctx.emit('default: s_panic("invalid union tag");')
            ctx.indent -= 1; ctx.emit('}')
            return
        self.error('E_UNSUPPORTED', s.line, f'statement {kind} unsupported')

    def render_types(self) -> str:
        out = []
        for rec in self.record_cache.values():
            out.append(f'typedef struct {{ {" ".join(self.declaration(t, f) + ";" for f,t in rec.fields)} }} {rec.name};')
            if rec.managed:
                retain = '\n'.join(f'    value.{n} = {self.copy_code(t, f"value.{n}")};' for n,t in rec.fields if t.managed)
                drop = '\n'.join(f'    {self.drop_code(t, f"value.{n}")};' for n,t in rec.fields if t.managed)
                out.append(f'static {rec.c} {rec.name}_retain({rec.c} value) {{\n{retain}\n    return value;\n}}')
                out.append(f'static void {rec.name}_drop({rec.c} value) {{\n{drop}\n}}')
        for u in self.aliases.values():
            if u.kind == 'union':
                inner = ' '.join(f'{v.c} v{i};' for i, (_, v) in enumerate(u.variants))
                out.append(f'typedef struct {{ int tag; union {{ {inner} }} data; }} {u.name};')
                if u.managed:
                    rt = '\n'.join(f'    case {i}: value.data.v{i} = {self.copy_code(v, f"value.data.v{i}")}; break;'
                                   for i,(_,v) in enumerate(u.variants))
                    dt = '\n'.join(f'    case {i}: {self.drop_code(v, f"value.data.v{i}")}; break;'
                                   for i,(_,v) in enumerate(u.variants))
                    out.append(f'static inline {u.c} {u.name}_retain({u.c} value) {{\n    switch (value.tag) {{\n{rt}\n    default: s_panic("invalid union tag");\n    }}\n    return value;\n}}')
                    out.append(f'static inline void {u.name}_drop({u.c} value) {{\n    switch (value.tag) {{\n{dt}\n    default: s_panic("invalid union tag");\n    }}\n}}')
        return '\n'.join(out)

    def compile(self) -> str:
        # Analyze all fully annotated definitions, even in programs without main.
        # Uncalled generic definitions are checked when specialized.
        for name, d in self.defs.items():
            if name != 'main' and all(ann is not None for _, ann in d.params):
                self.specialize(name, tuple(self.resolve(ann) for _, ann in d.params))
        if 'main' not in self.defs: self.error('E_NO_MAIN', 0, 'program has no main function')
        self.specialize('main', ())
        all_instances = list(self.instances.values())
        prototypes = []
        for inst in all_instances:
            decl = ', '.join(self.declaration(t, f'arg_{p}') for t, p in zip(inst.args, inst.params)) or 'void'
            prototypes.append(f'{"int" if inst.name == "main" else inst.ret.c} {inst.c_name}({decl});')
        return ('/* Generated by the Sirius Python bootstrap compiler. */\n'
                '#include "sirius_runtime.h"\n#include <math.h>\n\n'
                'static inline S_Optional_string s_opt_string_retain(S_Optional_string v) { if (v.has_value) v.value = s_string_retain(v.value); return v; }\n'
                'static inline void s_opt_string_drop(S_Optional_string v) { if (v.has_value) s_string_release(v.value); }\n\n'
                + self.render_types() + '\n\n' + '\n'.join(prototypes) + '\n\n'
                + '\n\n'.join(i.code for i in all_instances) + '\n')
