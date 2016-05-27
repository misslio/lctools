import urllib.request

def func():
	with urllib.request.urlopen('http://www.baidu.com') as f:
		print (f.read(3000))


if __name__ == '__main__':
	func()
