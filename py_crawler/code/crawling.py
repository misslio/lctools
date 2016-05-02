
import asyncio
import cgi
from collections import namedtuple
import logging
import re 		#regular expression module
import time
import urllib.parse

try:
	from asyncio import JoniableQueue as Queue
except:
	from asyncio import Queue

import aiohttp #install with "pip install aiohttp"

LOGGER = logging.getlogger(__name__)

# in: www.baidu.com out: baiducom
def lenient_host(host):
	parts=  host.split('.')[-2:] #what's split doing
	return ''.join(parts)

# In http ,300~399 means redirection. 
# see if status is in the tuple
def is_redirect(response):
	return response.status in (300, 301, 302, 303, 307)


FetchStatistic = namedtuple('FetchStatistic', ['url', 'next_url', 'status', 'exception', 'size', 'content_type', 'encoding', 'num_urls', 'num_new_urls'])

class Crawler:
	"""crawl a set of urls.

	This manage two sets of URLs: 'urls' and 'done'. 'urls' is a set of
	URLs seen, and 'done' is a list of FetchStatistics.
	"""
	def __init__(self, roots, exclude = None, 
				strict = True, max_redirect = 10, 
				max_tries = 4, max_tasks = 10, 
				loop = None):
		self.loop = loop or asyncio.get_event_loop()
		self.roots = roots
		self.exclude = exclude
		self.strict = strict
		self.max_redirect = max_redirect
		self.max_tries = max_tries
		self.max_tasks = max_tasks
		self.q = Queue(loop = self.loop)
		self.seen_urls = set()
		self.done = []
		self.session = aiohttp.ClientSession(loop=self.loop)
		self.root_domains = set()
		for root in roots:
			parts = urllib.parse.urlparse(root)
			host, port = urllib.parse.splitport(parts.netloc)
			if not host:
				continue
			if re.match(r'\A[\d\.]*\Z', host):	#about regular expression, not understand for now 
				self.root_domains.add(host)
			else:
				host = host.lower()
				if self.strict:
					self.root_domains.add(host)
				else:
					self.root_domains.add(lenient_host(host))

		for root in roots:
			self.add_url(root)
		self.t0 = time.time()
		self.t1 = None

	def close(self):
		"""Close resources"""
		self.session.close()
	
	def host_okay(self, host):
		"""Check if a host should be crawled.

		A literal match (after lowercasing) is always good. For hosts
		that don't look like IP addresses, some approximate matches 
		are okay depending on the strict flag.
		"""

		host = host.lower()
		if host in self.root_domains:
			return True
		if re.match(r'\A[\d\.]*\Z', host):
			return False
		if self.strict:
			return self._host_okay_strictish(host)
		else:
			return self._host_okay_lenient(host)

	def _host_okay_strictish(self, host):
		"""Check if a host should be crawled, strict-ish version.

		This check for equality modulo an initial 'www.' component.
		"""

		host = host[4:] 
		if host.startswith('www.') else 'www.' + host
		return host in self.root_domains

	def _host_okay_lenient(self, host):
		"""Check fi a host should be crawler, lenient version.

		This compares the last two components of the host.
		"""

		return lenient_host(host) in self.root_domains

	def record_statistic(self, fetch_statistic):
		"""Record the FetchStatistic for completed / failer URL."""

		self.done.append(fetch_statistic)

	@asyncio.coroutine
	def parse_link(self, response)
		"""Return a FetchStatistic and list of links."""
		links = set()
		content_type = None
		encoding = None
		body = yield from response.read()

		if response.status == 200:
			content_type = response.headers.get('content_type')
			pdict = {}

			if content_type:
				content_type, pdict = cgi.parse_header(content_type)

			encoding = pdict.get('charset', 'utf-8')
			if content_type in ('text/html','application/xml')
				text = yield from response.text()

				urls = set(re.findall(r'''(?i)href=["']([^\s"'<>]+)''', text)

				if urls:
					LOGGER.info('got %r distinct urls from %r', len(urls), response.url)

				for url in urls:
					normalized = urllib.parse.urljoin(response.url, url)
					defragmented, frag = urllib.parse.urldefrag(normalized)
					if self.url_allowed(defragmented):
							links.add(defragmented)

			stat = FetchStatistic(
				url = response.url,
				next_url = None,
				status = response.status,
				exception = None,
				size = len(body),
				content_type = content_type,
				encoding = encoding,
				num_urls = len(links),
				num_new_urls = len(links - self.seen_urls)

			return stat, links




	def add_url(self, url, max_redirect = None):
		"""Add a URL to the queue if not seen before."""
		if max_redirect is None:
			max_redirect = self.max_redirect
		LOGGER.debuf('adding %r %r', url, max_redirect)
		self.seen_urls.add(url)
		self.q.put_nowait(url, max_redirect)







loop = asyncio.get_event_loop()

crawler = crawling.Crawler('http://www.baidu.com', max_redirect = 10)

loop.run_until_complete(crawler.crawl())




