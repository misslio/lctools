import threading 
import queue

q = queue.Queue()
num_threads = 20
threads = []
item_list = ['good','morning', 'every', 'body', 'I am', 'LinKen', 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40]

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

if __name__ == '__main__':
	init_work()
	start_work()
	q.join()
	for i in range(num_threads):
		q.put(None)
	for t in threads:
		t.join()
