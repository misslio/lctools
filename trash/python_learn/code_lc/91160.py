from urllib import request

def func():
	url = 'http://sz.91160.com'
	headers = ('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36')

	opener = request.build_opener()
	opener.addheaders = [headers]
	f = opener.open(url)

	fp = open('91160.html', 'w+')
	fp.write(f.read().decode())
	fp.close()



if __name__ == '__main__':
	func()
