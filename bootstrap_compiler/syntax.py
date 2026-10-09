"""Sirius lexer and syntax-only recursive-descent/Pratt parser.

AST is deliberately independent from the C backend and from semantic types.
"""
from __future__ import annotations
from dataclasses import dataclass
from sly import Lexer


class CompileError(Exception):
    def __init__(self, code: str, line: int, message: str):
        self.code, self.line, self.message = code, line, message
        super().__init__(f'{code} at line {line}: {message}')


class SiriusLexer(Lexer):
    tokens = {IDENT, FLOAT, INTEGER, STRING, OP}
    ignore = ' \r\t'
    ignore_comment = r'\#.*'
    OP = r'\.\.|->|==|!=|<=|>=|\+=|-=|\*=|/=|\*\*|[+\-*/%<>=!|&~?.:,()\[\]{}]'
    STRING = r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    FLOAT = r'(?:[0-9]+\.[0-9]+|\.[0-9]+)(?:[eE][+\-]?[0-9]+)?'
    INTEGER = r'[0-9]+'
    IDENT = r'[A-Za-z_][A-Za-z0-9_]*'

    def error(self, t):
        raise CompileError('E_LEX', self.lineno, f'unexpected character {t.value[0]!r}')


@dataclass(frozen=True)
class Tok:
    value: str
    line: int
    col: int


@dataclass(frozen=True)
class TypeExpr:
    name: str
    args: tuple['TypeExpr | int', ...] = ()


@dataclass(frozen=True)
class Expr:
    kind: str
    value: object = None
    args: tuple['Expr', ...] = ()
    line: int = 0


@dataclass(frozen=True)
class Statement:
    kind: str
    value: object = None
    args: tuple[object, ...] = ()
    line: int = 0


@dataclass(frozen=True)
class Definition:
    name: str
    params: tuple[tuple[str, TypeExpr | None], ...]
    returns: TypeExpr | None
    body: tuple[Statement, ...]
    line: int


def tokenize(source: str) -> list[Tok]:
    """Indentation requires literal tabs; newlines within brackets are whitespace."""
    out: list[Tok] = []
    levels = [0]
    nesting = 0
    for line_no, raw in enumerate(source.splitlines(), 1):
        indent = len(raw) - len(raw.lstrip('\t'))
        text = raw[indent:]
        if text.startswith(' '):
            if text.strip() and not text.lstrip().startswith('#'):
                raise CompileError('E_INDENT', line_no, 'indent blocks with tabs, not spaces')
            text = text.lstrip()
        lexer = SiriusLexer()
        lexer.lineno = line_no
        tokens = [Tok(str(t.value), line_no, indent + t.index + 1) for t in lexer.tokenize(text)]
        if not tokens:
            continue
        if nesting == 0:
            if indent > levels[-1]:
                if indent != levels[-1] + 1:
                    raise CompileError('E_INDENT', line_no, 'indent increases by exactly one tab')
                levels.append(indent)
                out.append(Tok('<INDENT>', line_no, 1))
            elif indent < levels[-1]:
                while len(levels) > 1 and indent < levels[-1]:
                    levels.pop()
                    out.append(Tok('<DEDENT>', line_no, 1))
                if levels[-1] != indent:
                    raise CompileError('E_INDENT', line_no, 'inconsistent indentation')
        out.extend(tokens)
        for t in tokens:
            if t.value in ('(', '[', '{'): nesting += 1
            elif t.value in (')', ']', '}'): nesting -= 1
            if nesting < 0:
                raise CompileError('E_SYNTAX', line_no, 'unmatched closing bracket')
        if nesting == 0:
            out.append(Tok('<NL>', line_no, len(raw) + 1))
    if nesting:
        raise CompileError('E_SYNTAX', len(source.splitlines()), 'unclosed bracket')
    while len(levels) > 1:
        levels.pop()
        out.append(Tok('<DEDENT>', line_no if source else 1, 1))
    out.append(Tok('<EOF>', line_no if source else 1, 1))
    return out


class Parser:
    precedence = {'or': 1, 'and': 2, '|': 3, '==': 4, '!=': 4, '<': 4,
                  '<=': 4, '>': 4, '>=': 4, '..': 5, '+': 6, '-': 6,
                  '*': 7, '/': 7, '%': 7}

    def __init__(self, source: str):
        self.tokens = tokenize(source)
        self.i = 0

    @property
    def current(self) -> Tok:
        return self.tokens[self.i]

    def at(self, value: str) -> bool:
        return self.current.value == value

    def pop(self, expected: str | None = None) -> Tok:
        t = self.current
        if expected is not None and t.value != expected:
            raise CompileError('E_SYNTAX', t.line, f'expected {expected!r}; got {t.value!r}')
        self.i += 1
        return t

    def accept(self, value: str) -> bool:
        if self.at(value):
            self.pop()
            return True
        return False

    def name(self) -> str:
        t = self.pop()
        if not (t.value and (t.value[0].isalpha() or t.value[0] == '_')):
            raise CompileError('E_SYNTAX', t.line, f'expected identifier, got {t.value!r}')
        return t.value

    def nl(self):
        self.pop('<NL>')

    def block(self) -> tuple[Statement, ...]:
        self.nl()
        self.pop('<INDENT>')
        body = []
        while not self.at('<DEDENT>'):
            if self.at('<EOF>'):
                raise CompileError('E_SYNTAX', self.current.line, 'unexpected end of block')
            body.append(self.statement())
        self.pop('<DEDENT>')
        return tuple(body)

    def parse(self) -> tuple[Definition | Statement, ...]:
        defs = []
        while not self.at('<EOF>'):
            if self.at('def'):
                start = self.pop().line
                name = self.name()
                self.pop('(')
                params = []
                while not self.at(')'):
                    key = self.name()
                    annotation = self.typeexpr() if self.accept(':') else None
                    params.append((key, annotation))
                    if not self.accept(','): break
                self.pop(')')
                returns = self.typeexpr() if self.accept('->') else None
                body = self.block()
                defs.append(Definition(name, tuple(params), returns, body, start))
            else:
                defs.append(self.statement())
        return tuple(defs)

    def typeexpr(self) -> TypeExpr:
        name = self.name()
        args = []
        if self.accept('['):
            while not self.at(']'):
                if self.current.value.isdecimal():
                    args.append(int(self.pop().value))
                else:
                    args.append(self.typeexpr())
                if not self.accept(','): break
            self.pop(']')
        return TypeExpr(name, tuple(args))

    def statement(self) -> Statement:
        start = self.current.line
        if self.at('let') or self.at('var'):
            kind = self.pop().value
            name = self.name()
            ann = self.typeexpr() if self.accept(':') else None
            self.pop('=')
            val = self.expr()
            opt_else = None
            if self.accept('else'):
                if self.accept('return'):
                    opt_else = Statement('return', self.expr(), (), self.current.line)
                else:
                    raise CompileError('E_SYNTAX', self.current.line, 'expected return after optional else')
            self.nl()
            return Statement(kind, (name, ann), (val, opt_else), start)
        if self.accept('pub'):
            name = self.name()
            if self.accept('.'):
                field = self.name()
                val = Expr('attr', field, (Expr('name', name, (), start),), start)
                name = field
            else:
                val = self.expr() if self.accept('=') else None
            self.nl()
            return Statement('pub', name, (val,), start)
        if self.accept('if'):
            arms = [(self.expr(), self.block())]
            while self.accept('elif'):
                arms.append((self.expr(), self.block()))
            otherwise = self.block() if self.accept('else') else ()
            return Statement('if', tuple(arms), (otherwise,), start)
        if self.accept('while'):
            cond = self.expr()
            return Statement('while', cond, (self.block(),), start)
        if self.accept('for'):
            name = self.name()
            self.pop('in')
            iterable = self.expr()
            return Statement('for', name, (iterable, self.block()), start)
        if self.accept('match'):
            value = self.expr()
            self.nl()
            self.pop('<INDENT>')
            arms = []
            while not self.at('<DEDENT>'):
                ctor = self.name()
                self.pop('(')
                fields = []
                while not self.at(')'):
                    fields.append(self.name())
                    if not self.accept(','): break
                self.pop(')')
                arms.append((ctor, tuple(fields), self.block()))
            self.pop('<DEDENT>')
            return Statement('match', value, (tuple(arms),), start)
        if self.accept('return'):
            expr = None if self.at('<NL>') else self.expr()
            self.nl()
            return Statement('return', expr, (), start)
        if self.accept('recur'):
            f = self.expr()
            self.nl()
            return Statement('recur', f, (), start)
        if self.accept('break'):
            self.nl(); return Statement('break', None, (), start)
        if self.accept('continue'):
            self.nl(); return Statement('continue', None, (), start)
        result = self.simple_statement()
        self.nl()
        return result

    def simple_statement(self) -> Statement:
        start = self.current.line
        expr = self.expr()
        if self.current.value in ('=', '+=', '-=', '*=', '/='):
            op = self.pop().value
            val = self.expr()
            result = Statement('assign', op, (expr, val), start)
        else:
            result = Statement('expr', expr, (), start)
        # In optional `else return false`, caller consumes newline.
        if not self.at('<NL>') and not self.at('<EOF>'):
            raise CompileError('E_SYNTAX', self.current.line, f'unexpected token {self.current.value!r}')
        return result

    def expr(self, min_bp: int = 0) -> Expr:
        t = self.pop()
        v = t.value
        if v in ('-', 'not', '~', '+'):
            right = self.expr(8)
            lhs = Expr('unary', v, (right,), t.line)
        elif v == '(':
            lhs = self.expr()
            self.pop(')')
        elif v == '[':
            items = []
            while not self.at(']'):
                items.append(self.expr())
                if not self.accept(','): break
            self.pop(']')
            lhs = Expr('array', None, tuple(items), t.line)
        elif v == '{':
            items = []
            while not self.at('}'):
                if self.accept('del'):
                    name = self.name()
                    items.append(('del', name, None))
                else:
                    name = self.name()
                    if self.accept('.'):
                        self.pop('*')
                        items.append(('spread', name, None))
                    else:
                        self.pop('=')
                        items.append(('field', name, self.expr()))
                if not self.accept(',') and not self.at('}'):
                    # Newlines inside record literals are ignored, so adjacent entries work.
                    pass
            self.pop('}')
            lhs = Expr('record', tuple(items), (), t.line)
        elif v == 'fn':
            self.pop('(')
            params = []
            while not self.at(')'):
                key = self.name()
                typ = self.typeexpr() if self.accept(':') else None
                params.append((key, typ))
                if not self.accept(','): break
            self.pop(')')
            lhs = Expr('lambda', tuple(params), (self.expr(),), t.line)
        elif v in ('true', 'false'):
            lhs = Expr('bool', v == 'true', (), t.line)
        elif v[0].isdigit():
            lhs = Expr('float' if '.' in v or 'e' in v.lower() else 'int', v, (), t.line)
        elif v[0] in ('"', "'"):
            import ast
            lhs = Expr('string', ast.literal_eval(v), (), t.line)
        elif v[0].isalpha() or v[0] == '_':
            if v == 'Array' and self.at('['):
                self.pop('[')
                elem = self.typeexpr(); self.pop(',')
                rank = self.pop().value; self.pop(']')
                if rank != '1': raise CompileError('E_UNKNOWN_TYPE', t.line, 'only rank-1 arrays available')
                lhs = Expr('array_type', (elem, 1), (), t.line)
            elif v == 'Dict' and self.at('['):
                self.pop('[')
                keytype = self.typeexpr(); self.pop(',')
                valtype = self.typeexpr(); self.pop(']')
                lhs = Expr('dict_type', (keytype, valtype), (), t.line)
            else:
                lhs = Expr('name', v, (), t.line)
        else:
            raise CompileError('E_SYNTAX', t.line, f'unexpected expression token {v!r}')
        while True:
            op = self.current.value
            if op == '(' and 10 >= min_bp:
                self.pop('(')
                args = []
                while not self.at(')'):
                    args.append(self.expr())
                    if not self.accept(','): break
                self.pop(')')
                lhs = Expr('call', None, (lhs, *args), t.line)
                continue
            if op == '.' and 10 >= min_bp:
                self.pop('.')
                lhs = Expr('attr', self.name(), (lhs,), t.line)
                continue
            if op == '[' and 10 >= min_bp:
                self.pop('[')
                index = self.expr()
                optional = self.accept('?')
                self.pop(']')
                panic = self.accept('!')
                if panic and not optional:
                    raise CompileError('E_SYNTAX', t.line, 'checked indexing requires [index?]!')
                lhs = Expr('index', 'panic' if panic else 'optional' if optional else 'proven', (lhs, index), t.line)
                continue
            bp = self.precedence.get(op, -1)
            if bp < min_bp:
                break
            self.pop()
            rhs = self.expr(bp + 1)
            lhs = Expr('binary', op, (lhs, rhs), t.line)
        return lhs
