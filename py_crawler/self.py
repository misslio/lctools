from collections import namedtuple

def test():
	friend = namedtuple("Friend", ['name', 'age','email'])

	f = friend('xiaowang', 33, 'xiaowang@163.com')

	print(f)
	print(f.age)
	print(f.email)

	f2 = friend(name='xiaozhang', email='xiaozhang@sina.com',age=30)
	print(f2)

	name,age,email=f2
	print(name,age,email)

if __name__ == "__main__":
	test()
