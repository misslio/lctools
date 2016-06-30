import asyncio
import threading

@asyncio.coroutine
def func():
	print ('first hello %s' % threading.currentThread())

	r = yield from asyncio.sleep(5);

	print ('after hello %s' % threading.currentThread())

@asyncio.coroutine
def func2():
	print ('first hello ------------')

	r = yield from asyncio.sleep(2);

	print ('after hello-----------++++-----')


if __name__ == '__main__':
	loop = asyncio.get_event_loop()
	tasks = [func(), func()]
	loop.run_until_complete(asyncio.wait(tasks))
	loop.close()

