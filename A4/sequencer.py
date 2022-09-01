#!/usr/bin/env python3

import random
class sequencer:
	def __init__(self):
		self.happened = False
		self.ready_func = lambda: True
	def ready(self):
		return not self.happened and self.ready_func()
	def after(self, other):
		old_ready = self.ready_func
		self.ready_func = lambda: False if not other.happened else old_ready()

class open_op:
	def __init__(self, file):
		self.file = file
		self.seq = sequencer()
	def sequence(self, num):
		self.file.num = num
		self.seq.happened = True
		return f'Character Driver Module {num}: open (number {self.file.num})'
class release_op:
	def __init__(self, file):
		self.file = file
		self.seq = sequencer()
	def sequence(self, num):
		self.seq.happened = True
		return f'Character Driver Module {num}: release (number {self.file.num})'

class read_op:
	def __init__(self, file):
		self.file = file
		self.seq = sequencer()
	def sequence(self, num):
		self.seq.happened = True
		return f'Character Driver Module {num}: read (number {self.file.num})'

class write_op:
	def __init__(self, file):
		self.file = file
		self.seq = sequencer()
	def sequence(self, num):
		self.seq.happened = True
		return f'Character Driver Module {num}: write (number {self.file.num})'

class init_op:
	def __init__(self):
		self.seq = sequencer()
	def sequence(self, num):
		self.seq.happened = True
		return f'Character Driver Module {num}: init'

class exit_op:
	def __init__(self):
		self.seq = sequencer()
	def sequence(self, num):
		self.seq.happened = True
		return f'Character Driver Module {num}: exit'

class fake_file:
	def __init__(self):
		self.num = 0
		self.m_open = None
		self.m_close = release_op(self)
	def open(self):
		assert self.m_open is None
		assert self.m_close is not None
		op = open_op(self)
		self.m_close.seq.after(op.seq)
		self.m_open = op
		return op
	def release(self):
		assert self.m_open is not None
		assert self.m_close is not None
		op = self.m_close
		self.m_close = None
		return op
	def read(self):
		assert self.m_open is not None
		assert self.m_close is not None
		op = read_op(self)
		op.seq.after(self.m_open.seq)
		self.m_close.seq.after(op.seq)
		return op
	def write(self):
		assert self.m_open is not None
		assert self.m_close is not None
		op = write_op(self)
		op.seq.after(self.m_open.seq)
		self.m_close.seq.after(op.seq)
		return op


def main():
	file1 = fake_file()
	file2 = fake_file()
	file3 = fake_file()
	module_init = init_op()
	module_exit = exit_op()

	file1_open = None
	file2_open = None
	file3_open = None
	file1_release = None
	file2_release = None
	file3_release = None

	operations = [
		file1_open := file1.open(),
		file1.read(),
		file1.read(),
		file1.write(),
		file1.write(),
		file1_release := file1.release(),
		file2_open := file2.open(),
		file2.read(),
		file2.read(),
		file2.write(),
		file2_release := file2.release(),
		file3_open := file3.open(),
		file3.read(),
		file3.write(),
		file3.write(),
		file3.write(),
		file3_release := file3.release(),
	]
	for op in operations:
		op.seq.after(module_init.seq)
		module_exit.seq.after(op.seq)
	operations.append(module_init)
	operations.append(module_exit)

	file2_open.seq.after(file1_release.seq)
	
	for i in range(len(operations)):
		ready = [op for op in operations if op.seq.ready()]
		op = random.choice(ready)
		print(op.sequence(i))

if __name__ == '__main__':
	main()
