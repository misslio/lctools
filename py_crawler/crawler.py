import socket

def fetch(url):
	sock = socket.socket()
	sock.connect(('baidu.com', 80))
	request = 'GET {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format(url)
	sock.send(request.encode('ascii'))
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
