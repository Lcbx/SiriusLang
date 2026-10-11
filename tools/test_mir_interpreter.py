"""Run with: python -m unittest discover -s tools -p test_mir_interpreter.py"""

import unittest

from mir_interpreter import (
	Binary, Block, Branch, Call, Const, Copy, Function, Interpreter,
	Jump, MIRExecutionError, Panic, Program, Return,
)


class MIRInterpreterTests(unittest.TestCase):
	def test_arithmetic(self):
		fn = Function((), {"entry": Block((Const("x", 7), Const("y", 5), Binary("result", "mul", "x", "y")), Return("result"))})
		self.assertEqual(Interpreter(Program({"main": fn})).run("main"), 35)

	def test_loop_and_branch(self):
		fn = Function(("n",), {
			"entry": Block((Const("acc", 1), Const("one", 1)), Jump("check")),
			"check": Block((Binary("continue", "gt", "n", "one"),), Branch("continue", "body", "done")),
			"body": Block((Binary("acc", "mul", "acc", "n"), Binary("n", "sub", "n", "one")), Jump("check")),
			"done": Block((), Return("acc")),
		})
		self.assertEqual(Interpreter(Program({"factorial": fn})).run("factorial", 5), 120)

	def test_calls(self):
		inc = Function(("n",), {"entry": Block((Const("one", 1), Binary("result", "add", "n", "one")), Return("result"))})
		main = Function((), {"entry": Block((Const("x", 4), Call("y", "inc", ("x",))), Return("y"))})
		self.assertEqual(Interpreter(Program({"inc": inc, "main": main})).run("main"), 5)

	def test_externals_and_output(self):
		output = []
		fn = Function((), {"entry": Block((Const("x", "hello"), Call(None, "print", ("x",))), Return())})
		interp = Interpreter(Program({"main": fn}), externals={"print": lambda x: output.append(x)})
		self.assertIsNone(interp.run("main"))
		self.assertEqual(output, ["hello"])

	def test_invalid_cfg(self):
		fn = Function((), {"entry": Block((), Jump("missing"))})
		with self.assertRaisesRegex(MIRExecutionError, "missing block"):
			Interpreter(Program({"main": fn})).run("main")

	def test_uninitialized_register(self):
		fn = Function((), {"entry": Block((Copy("x", "missing"),), Return("x"))})
		with self.assertRaisesRegex(MIRExecutionError, "uninitialized"):
			Interpreter(Program({"main": fn})).run("main")

	def test_panics(self):
		fn = Function((), {"entry": Block((), Panic("expected panic"))})
		with self.assertRaisesRegex(MIRExecutionError, "expected panic"):
			Interpreter(Program({"main": fn})).run("main")

	def test_instruction_budget(self):
		fn = Function((), {"entry": Block((), Jump("entry"))})
		with self.assertRaisesRegex(MIRExecutionError, "instruction limit"):
			Interpreter(Program({"main": fn}), max_steps=10).run("main")

	def test_reject_non_bool_branch(self):
		fn = Function((), {"entry": Block((Const("x", 1),), Branch("x", "ok", "ok")), "ok": Block((), Return())})
		with self.assertRaisesRegex(MIRExecutionError, "must be bool"):
			Interpreter(Program({"main": fn})).run("main")

	def test_unsupported_operation(self):
		fn = Function((), {"entry": Block((Const("x", 1), Binary("y", "div", "x", "x")), Return("y"))})
		with self.assertRaisesRegex(MIRExecutionError, "unsupported"):
			Interpreter(Program({"main": fn})).run("main")

	def test_call_depth_budget(self):
		fn = Function((), {"entry": Block((Call(None, "main"),), Return())})
		with self.assertRaisesRegex(MIRExecutionError, "call depth"):
			Interpreter(Program({"main": fn}), max_call_depth=5).run("main")


if __name__ == "__main__":
	unittest.main()
