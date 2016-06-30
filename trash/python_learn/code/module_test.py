import aiohttp
import asyncio
import threading 
import queue

q = queue.Queue()
num_threads = 20
threads = []
item_list = ['good','morning', 'every', 'body', 'I am', 'LinKen', 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40]


#test asyncio start
@asyncio.coroutine
def func():
	with aiohttp.ClientSession() as session:
			resp = yield from session.get('http://www.baidu.com')
			print(resp.status)
			print(resp.text)
			session.close()
#test asyncio end



#test queue start
def worker():
	while True:
		item = q.get()
		if item is None:
			break
		do_work(item)
		q.task_done()

def start_work():
	for i in range(num_threads):
		t = threading.Thread(target=worker)
		t.start()
		threads.append(t)

def init_work():
	for item in item_list:
		q.put(item)

def do_work(item):
	print (item)
#test queue end

#test re start	
def test():
	host = 'www.baidu.com'
	if re.match(r'\A[\d\.]*\Z', host):
		print ('match good')
	else:
		print ('no match')

	parts = host.split('.')[-2:]
	print (parts)
	p_join = ''.join(parts)
	print (p_join)
#test re end


#test start
class Sample:
	def __enter__(self):
		print ("in enter")
		return self

	def __exit__(self, type, value, trace):
		print ("type:{}".format(type))
		print ("value:{}".format(value))
		print ("trace:{}".format(trace))

	def do_something(self):
		bar = 1/0
		return bar + 10

def get_sample():
	return Sample()

def func():
	with Sample() as sample:
		print ("sample:{}".format(sample))
		print('good')
		sample.do_something()
#test end




if __name__ == '__main__':
	if False: #test asyncio
		loop = asyncio.get_event_loop()
		loop.run_until_complete(func());

	if False: #test queue
		init_work()
		start_work()
		q.join()
	
		print ('name++++++++++++++++++++++')
	
		for i in range(num_threads):
		q.put(None)

		for t in threads:
			t.join()
	
	if False:	#test re
		test()

	if False:	#test
		func()
