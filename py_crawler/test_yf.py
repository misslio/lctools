import asyncio

def gen_fn():
	result = yield 1
	print('result of yield: {}'.format(result))
	result2 = yield 2
	print('result of 2nd yield {}'.format(result2))
	return 'done'

def caller_fn():
	gen = gen_fn()
	rv = yield from gen
	print('return value of yield-from: {}'.format(rv))


