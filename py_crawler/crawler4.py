import asyncio
import socket
from selectors import DefaultSelector, EVENT_WRITE, EVENT_READ

selector = DefaultSelector()
stopped = False
sock = socket.socket()

class Future:
	def __init__(self):
		self.result = None
		self._callbacks = []

	def add_done_callback(self, fn):
		self._callbacks.append(fn)

	def set_result(self, result):
		self.result = result
		for fn in self._callbacks:
			fn(self)


class Task:
	def __init__(self, coro):
		self.coro = coro
		f = Future()
		f.set_result(None)
		self.step(f)

	def step(self, future):
		try:
			next_future = self.coro.send(future.result)
		except StopIteration:
			return

		next_future.add_done_callback(self.step)

def loop():
	while not stopped:
		events = selector.select()
		for event_key, event_mask in events:
			callback = event_key.data
			callback()


def read(sock):
	f = Future()
	print('in read')	
	def on_readable():
		f.set_result(sock.recv(4096))
	
	selector.register(sock.fileno(), EVENT_READ, on_readable)
	chunk = yield f
	selector.unregister(sock.fileno())
	return chunk

def read_all(sock):
	response = []

	chunk = yield from read(sock)
	while chunk:
		response.append(chunk)
		chunk = yield from read(sock)
	print(b''.join(response))
	return b''.join(response)


def fetch():
	sock.setblocking(False)
	request = 'Get {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format("baidu.com")
	
	try:
		sock.connect(('baidu.com', 80))
	except BlockingIOError:
		pass

	f = Future()

	def on_connected():
		f.set_result(None)
	
	selector.register(sock.fileno(), EVENT_WRITE, on_connected)

	yield f

	selector.unregister(sock.fileno())
	print('connected!')

	sock.send(request.encode('ascii'))
	response = yield from read_all(sock)


if __name__ == "__main__":
	Task(fetch())
#	gen = fetch()
#	gen.send(None)
	loop()
#	while True:
#		pass
#	next(fetch())
