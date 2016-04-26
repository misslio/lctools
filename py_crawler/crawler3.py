import asyncio
import socket
from selectors import DefaultSelector, EVENT_WRITE

class Fetcher:

	def __init_(self, url):
		self.reponse = b''
		self.url = url
		self.sock = None

	def fetch(self):
		self.sock = socket.socket()
		self.sock.setblocking(False)
		try:
			self.sock.connect(('baidu.com', 80))
		except BlockingIOError:
			pass

		selector.register(self.sock.fileno(), EVENT_WRITE,self.connected)

	def connected(self, key, mask):
		print('connected')
		selector.unregister(key.fd)
		request = 'Get {} HTTP/1.0\r\nHost: baidu.com\r\n\r\n'.format(url)
		self.sock.send(request.encode('ascii'))

		selector.register(key.fd, EVENT_READ, self.read_response)
	
	def read_response(self, key, mask):
		global stopped

		chunk = self.sock.recv(4096)
		if chunk:
			self.response += chunk
		else
			selector.unregister(key.fd)
			links = self.parse_links()

		seen_urls.update(links)
		urls_todo.remove(self.url)
		if not urls_todo:
			stopped = True

	

	def parse_links(self, response):
        """Return a FetchStatistic and list of links."""
        links = set()
        content_type = None
        encoding = None
        body = yield from response.read()

        if response.status == 200:
            content_type = response.headers.get('content-type')
            pdict = {}

            if content_type:
                content_type, pdict = cgi.parse_header(content_type)

            encoding = pdict.get('charset', 'utf-8')
            if content_type in ('text/html', 'application/xml'):
                text = yield from response.text()

                # Replace href with (?:href|src) to follow image links.
                urls = set(re.findall(r'''(?i)href=["']([^\s"'<>]+)''',
                                      text))
                if urls:
                    LOGGER.info('got %r distinct urls from %r',
                                len(urls), response.url)
                for url in urls:
                    normalized = urllib.parse.urljoin(response.url, url)
                    defragmented, frag = urllib.parse.urldefrag(normalized)
                    if self.url_allowed(defragmented):
                        links.add(defragmented)

        stat = FetchStatistic(
            url=response.url,
            next_url=None,
            status=response.status,
            exception=None,
            size=len(body),
            content_type=content_type,
            encoding=encoding,
            num_urls=len(links),
            num_new_urls=len(links - self.seen_urls))

        return stat, links


