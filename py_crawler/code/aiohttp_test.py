import aiohttp
import asyncio

def func():
	with aiohttp.ClientSession() as session:
		with session.get('http://www.baidu.com') as resp:
			print(resp.status)
		#	print(await resp.text())

if __name__ == '__main__':
	func()
