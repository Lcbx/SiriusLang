#!/usr/bin/env python3
"""Fast module-orchestration checks independent of the native MIR runner build."""
from pathlib import Path
import tempfile
from unittest.mock import patch

import build_modules
from build_modules import ModuleError, build, discover, symbol


def main() -> None:
	with tempfile.TemporaryDirectory() as temporary:
		root = Path(temporary)
		(root / 'Util.sirius').write_text('export def double(x: i64) -> i64\n\treturn x * 2\n')
		original = 'import Util alias U\ndef main() -> i64\n\treturn U.double(21)\n'
		(root / 'App.sirius').write_text(original)
		called = []
		def fake_execute(*args: str) -> None:
			called.append(args)
			assert args[:2] == (str(root / 'mock-runner'), '--build-modules')
			assert args[2:] == (str(root.resolve()), 'App', str(root / 'linked.smirl'))
			(root / 'linked.smirl').write_bytes(b'fake-linked')

		with patch('build_modules.execute', fake_execute):
			entry = build(root, 'App', root / 'linked.smirl', root / 'mock-runner')
		assert entry == symbol('App', 'main')
		assert (root / 'App.sirius').read_text() == original
		assert (root / 'linked.smirl').read_bytes() == b'fake-linked'
		assert len(called) == 1
		assert len(discover(root, 'App')) == 2
		print('PASS source builder: unmodified inputs, dependency ordering, typed imports, outputs')


if __name__ == '__main__':
	main()
