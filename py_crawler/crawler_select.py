import asyncio
from selectors import DefaultSelector, EVENT_WRITE, EVENT_READ
import socket

selector = DefaultSelector()
stopped = False

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

class Fetcher:
	def __init__(self, url):
		self.response = b''
		self.url = url
		self.sock = None



	def fetch(self):
		self.sock = socket.socket()
		self.sock.setblocking(False)
		
		request = 'GET {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format(self.url)

		try:
			self.sock.connect(('baidu.com', 80))
		except BlockingIOError:
			pass

		f = Future()

		def on_connected():
			f.set_result(None)

#		selector.register(self.sock.fileno(), EVENT_WRITE, self.connected)
		selector.register(self.sock.fileno(), EVENT_WRITE, on_connected)

		yield f

		selector.unregister(self.sock.fileno())
		print('connected!')

		self.sock.send(request.encode('ascii'))
		self.response = yield from read_all(self.sock)
		
		print(self.response)
		stopped = True

def read(sock):
	f = Future()

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

	return b''.join(response)

# ---------------- no use below -------------------------
	def connected(self, key, mask):
		print('connected')
		selector.unregister(key.fd)
		request = 'GET {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format(self.url)
		self.sock.send(request.encode('ascii'))
		
		selector.register(key.fd, EVENT_READ, self.read_response)

	def read_response(self, key, mask):
		global stopped

		chunk = self.sock.recv(4096)
		if chunk:
			self.response += chunk
		else:
			selector.unregister(key.fd)
		
		stopped = True
		print(self.response)

def loop():
		while not stopped:
			events = selector.select()
			for event_key, event_mask in events:
				callback = event_key.data
#				callback(event_key, event_mask)
				callback()

if __name__ == "__main__":
	fetcher = Fetcher("www.baidu.com")
	Task(fetcher.fetch())
	loop()





