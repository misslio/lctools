
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

if __name__ == '__main__':
	func()
