测试点，
1，测试内部模块的读入链表，链表由每一个节点控制一个日志节点，每个节点保存该日志的名称，权限位，备份个数及单个日志大小上限，
至少一个head节点，head节点名称为Year/mon/day.log,日志权限为1.1.1.1.1 打印权限为0.1.1.1.1备份个数为10个，单个文件大小为300M
其他节点从日志配置文件中读取，以头插法加入链表，初始化完成后，默认head节点和配置文件中的节点已经在链表中。
在日志中也可以直接写入一个文件名，这个新的日志文件将会以默认配置插入到节点链表中
可以调用go_through_iridiumlog函数将节点的使用情况遍历打印出来
2通过不同的控制位来查看是否日志的写入正确的受控制位控制
3通过日志备份配置项验证是否日志超过默认大小时会正确的备份，日志的滚动备份的机制：
	a日志超过单个文件配置大小，需要备份，备份当前a日志备份为a.0, 已经存在的a.0备份为a.1,
	已经存在的a.1备份为a.2以此类推
	保证当前在写日志为a,最近一份备份为a.0,然后是a.1，上限由配置项rollback_number决定
4日志模块会以固定写入次数来检测文件是否存在，日志文件被删除后会自动重建
5以valgrind等工具来验证是否会有内存部分的问题

没有测试过多线程的情况
具体见测试代码test.c注释