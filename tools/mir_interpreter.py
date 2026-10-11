"""Small executable core for Sirius CFG MIR.

This intentionally interprets only scalar instructions and explicit control flow.
The production Sirius MIR and source-to-MIR lowering do not exist yet; this
module is an executable prototype, not an alternative compiler frontend.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Callable, TypeAlias


Scalar: TypeAlias = int | bool | str | None


class MIRExecutionError(Exception):
	"""The MIR program cannot be executed or explicitly panicked."""


@dataclass(frozen=True)
class Const:
	dst: str
	value: Scalar


@dataclass(frozen=True)
class Copy:
	dst: str
	src: str


@dataclass(frozen=True)
class Binary:
	dst: str
	op: str
	left: str
	right: str


@dataclass(frozen=True)
class Call:
	dst: str | None
	function: str
	args: tuple[str, ...] = ()


Instruction: TypeAlias = Const | Copy | Binary | Call


@dataclass(frozen=True)
class Jump:
	target: str


@dataclass(frozen=True)
class Branch:
	condition: str
	if_true: str
	if_false: str


@dataclass(frozen=True)
class Return:
	value: str | None = None


@dataclass(frozen=True)
class Panic:
	message: str


Terminator: TypeAlias = Jump | Branch | Return | Panic


@dataclass(frozen=True)
class Block:
	instructions: tuple[Instruction, ...]
	terminator: Terminator


@dataclass(frozen=True)
class Function:
	parameters: tuple[str, ...]
	blocks: dict[str, Block]
	entry: str = "entry"


@dataclass(frozen=True)
class Program:
	functions: dict[str, Function]


def verify(program: Program) -> None:
	"""Check basic CFG structure. Typed MIR validation belongs to the compiler."""
	for name, fn in program.functions.items():
		if len(set(fn.parameters)) != len(fn.parameters):
			raise MIRExecutionError(f"{name}: repeated parameter")
		if fn.entry not in fn.blocks:
			raise MIRExecutionError(f"{name}: entry block {fn.entry!r} not found")
		for label, block in fn.blocks.items():
			term = block.terminator
			if isinstance(term, Jump):
				targets = (term.target,)
			elif isinstance(term, Branch):
				targets = (term.if_true, term.if_false)
			elif isinstance(term, (Return, Panic)):
				targets = ()
			else:
				raise MIRExecutionError(f"{name}.{label}: unknown terminator")
			for target in targets:
				if target not in fn.blocks:
					raise MIRExecutionError(f"{name}.{label}: missing block {target!r}")


@dataclass
class Interpreter:
	program: Program
	externals: dict[str, Callable[..., Scalar]] = field(default_factory=dict)
	max_steps: int = 100_000
	max_call_depth: int = 256
	_steps: int = field(init=False, default=0)

	def run(self, function: str, *args: Scalar) -> Scalar:
		verify(self.program)
		self._steps = 0
		return self._invoke(function, args, 0)

	def _step(self) -> None:
		self._steps += 1
		if self._steps > self.max_steps:
			raise MIRExecutionError("MIR instruction limit exceeded")

	def _invoke(self, name: str, args: tuple[Scalar, ...], depth: int) -> Scalar:
		if depth >= self.max_call_depth:
			raise MIRExecutionError("MIR call depth limit exceeded")
		if name not in self.program.functions:
			if name not in self.externals:
				raise MIRExecutionError(f"unknown function {name!r}")
			return self.externals[name](*args)

		fn = self.program.functions[name]
		if len(args) != len(fn.parameters):
			raise MIRExecutionError(f"{name}: expected {len(fn.parameters)} arguments, got {len(args)}")
		locals_ = dict(zip(fn.parameters, args))
		label = fn.entry

		def read(reg: str) -> Scalar:
			if reg not in locals_:
				raise MIRExecutionError(f"{name}.{label}: uninitialized local {reg!r}")
			return locals_[reg]

		while True:
			block = fn.blocks[label]
			for instr in block.instructions:
				self._step()
				if isinstance(instr, Const):
					locals_[instr.dst] = instr.value
				elif isinstance(instr, Copy):
					locals_[instr.dst] = read(instr.src)
				elif isinstance(instr, Binary):
					left, right = read(instr.left), read(instr.right)
					locals_[instr.dst] = self._binary(instr.op, left, right)
				elif isinstance(instr, Call):
					values = tuple(read(arg) for arg in instr.args)
					result = self._invoke(instr.function, values, depth + 1)
					if instr.dst is not None:
						locals_[instr.dst] = result
				else:
					raise MIRExecutionError(f"{name}.{label}: unknown instruction")

			self._step()
			term = block.terminator
			if isinstance(term, Jump):
				label = term.target
			elif isinstance(term, Branch):
				cond = read(term.condition)
				if type(cond) is not bool:
					raise MIRExecutionError("branch condition must be bool")
				label = term.if_true if cond else term.if_false
			elif isinstance(term, Return):
				return read(term.value) if term.value is not None else None
			elif isinstance(term, Panic):
				raise MIRExecutionError(term.message)
			else:
				raise MIRExecutionError(f"{name}.{label}: unknown terminator")

	@staticmethod
	def _binary(op: str, left: Scalar, right: Scalar) -> Scalar:
		if op == "eq":
			return type(left) is type(right) and left == right
		if op in ("add", "sub", "mul", "lt", "le", "gt", "ge"):
			if type(left) is not int or type(right) is not int:
				raise MIRExecutionError(f"{op} expects i64 operands")
			if op == "add":
				return left + right
			if op == "sub":
				return left - right
			if op == "mul":
				return left * right
			if op == "lt":
				return left < right
			if op == "le":
				return left <= right
			if op == "gt":
				return left > right
			return left >= right
		raise MIRExecutionError(f"unsupported MIR binary operation {op!r}")
