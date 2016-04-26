import asyncio
from selectors import DefaultSelector, EVENT_WRITE
import socket


selector = DefaultSelector()
sock = socket.socket()


def con():

	sock.setblocking(False)
	
	try:
		sock.connect(('baidu.com', 80))
	except BlockingIOError:
		pass
	
	selector.register(sock.fileno(), EVENT_WRITE, connected)


def connected():
	selector.unregister(sock.fileno())
	print('connected')


def loop():
	while True:
		events = selector.select()
		for event_key, event_mask in events:
			callback = event_key.data
			callback()


if __name__ == "__main__":
	con()
	loop()





