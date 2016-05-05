import aiohttp
import asyncio

@asyncio.coroutine
def func():
	with aiohttp.ClientSession() as session:
			resp = yield from session.get('http://www.baidu.com')
			print(resp.status)
			#print(resp.text)
			session.close()

if __name__ == '__main__':
	loop = asyncio.get_event_loop()
	loop.run_until_complete(func());
