import asyncio

@asyncio.coroutine
def hello():
	print('hello world')
	r = yield from asyncio.sleep(10)
	print('end')


@asyncio.coroutine
def hello2():
	print('good morning')
	print('good end')

if __name__ == "__main__":
	loop = asyncio.get_event_loop()
	tasks = [hello(), hello2()]
	loop.run_until_complete(asyncio.wait(tasks))
	loop.close()
