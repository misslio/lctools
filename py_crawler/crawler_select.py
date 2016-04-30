import asyncio
from selectors import DefaultSelector, EVENT_WRITE, EVENT_READ
import socket

selector = DefaultSelector()
stopped = False

class Fetcher:
	def __init__(self, url):
		self.response = b''
		self.url = url
		self.sock = None



	def fetch(self):
		self.sock = socket.socket()
		self.sock.setblocking(False)

		try:
			self.sock.connect(('baidu.com', 80))
		except BlockingIOError:
			pass

		selector.register(self.sock.fileno(), EVENT_WRITE, self.connected)


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
				callback(event_key, event_mask)


if __name__ == "__main__":
	fetcher = Fetcher("www.baidu.com")
	fetcher.fetch()
	loop()





