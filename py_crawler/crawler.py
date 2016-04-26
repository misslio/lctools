import socket
import asyncio
from selectors import DefaultSelector, EVENT_WRITE



def fetch(url):
	selector = DefaultSelector()
	sock = socket.socket()
	sock.setblocking(False)
	
	try:
		sock.connect(('baidu.com', 80))
	except BlockingIOError:
		pass
	def connected():
		selector.unregister(sock.fileno())
		print('connected')


	select.register(sock.fileno(), EVENT_WRITE, connected)
	request = 'GET {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format(url)
	
	while True:
		try:
			sock.send(request.encode('ascii'))
			break
		except OSError as e:
			pass
	
	response = b''

	chunk = sock.recv(4096)

	while chunk:
		response += chunk
		chunk = sock.recv(4096)
	response.encode('utf-8')
	print("%s\n"%(response))
	# Page is now downloaded.
	links = parse_links(response)
	q.add(links)

if __name__ == "__main__":
	fetch("baidu.com")
