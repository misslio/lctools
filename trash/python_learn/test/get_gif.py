import re
import os
import time
from urllib import request 
def get_urls(url):
	html = request.urlopen(url).read()
	pic = re.compile(r'<img alt=".+?" src="(.+?)" /><br />')
	pics = pic.findall(html)
	for urls in pics:
		try:
			name = re.sub(':','_',time.ctime()[10:19])+'.jpg'
			print (name)
			urllib.urlretrieve(urls, '/repo/lctools/python_learn/test/'+name)
		except:
			print ('{} fail'.form(name))
			continue

def get_href(page):
	s = request.urlopen(page) 
	hrefs = re.compile(r'<h2><a href="(.+?)".+?</a></h2>')
	s = s.read()
#	s = s.decode('GBK')
	links = hrefs.findall(s)
	return links

def main():
	for i in range(67):
		a = 'http://www.baidu.com'
		href = get_href(a)
		time.sleep(1)
		for j in href:
			get_urls(j)


if __name__ == '__main__':
	main()
