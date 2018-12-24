### lctools
代码仓库，主要为C语言。

### 目录结构

* src 

所有的源码存放目录

* docs

文档存放目录

* other

其他类型文件存放目录

* trash

垃圾桶，删除的文件会临时存放在本目录下

### src目录

#### csort 

各种排序算法

#### log

linux系统下的日志库代码

#### network

关于网络的一些小函数代码

#### template

存放模板文件

目前收录了makefile和cmake

#### utils

小工具集代码

工具组件位于根目录下的utils目录中，每个模块以独立的目录形式存在，此处仅描述各个组件提供的功能，具体请参考响应源码。

NOTE : 给出的目录名称可能有变。

##### 配置文件

目录 : config

读取配置文件，一个配置文件中可以配置多个模块，配置文件的格式如下

```
#disable or enable configure file.
# 0 - disable
# 1 - enable
@CONF_SWITCH=1

#MODE_START: This is a example. A mode should start with a '-', then the remaining line
#	         on this line will be considered a mode name, no matter what it is.
#KEY_VALUES: The next lines are your key-value pairs.
#END : "-end" is needed by every single mode. If not, this will be a confused configure.
-example
key1 = value1
key2 = value2
key3 = value3
-end
```

配置选项说明

- “@CONF_SWITCH” 启用或停用
- “#” 注释，支持行尾注释
- “-example” 模块配置开始，“-end”模块配置结束

配置项支持 int、double、gint8、gint16、gint32、gint64、guint8、guint16、guint32、guint64和字符串等类型。

**模块线程不安全。**

##### 守护进程

目录 : single_instance

将当前进程以守护进程方式运行。使用该接口时，需注意文件路径，文件描述符等信息。

##### 获取进程名

目录 : get_proc_name

获取当前进程的目录和进程名称。

##### 获取函数调用栈信息

目录 : get_trace

获取函数调用栈信息主要是代调试或代码异常退出时使用，追踪函数调用关系。

该代码目前功能并不完善，解析代码未调用。

##### 选项参数

目录 : opt_entry

深入解析与过滤库模块共用的入参处理代码，如有修改，需要注意与过滤库模块保持一致。

读取进程运行携带的参数，针对特定的选项参数，目前只适配了深入解析模块。

##### Redis功能封装

目录 : redis

对Redis的频道订阅、向Redis中写键值对进行二次封装，统一初始化和销毁，线程安全。

支持订阅多个频道，当频道收到消息后会触发回调函数。

##### 单实例进程

目录 : single_instance

采用建议性锁实现的但实例函数，确保系统中只有一个进程运行，禁止进程重复运行。

##### 写PCAP文件

目录 : write_packet

把帧数据流写到文件中，标准的pcap文件，文件路径为"/var/log/iridium/"，文件名为“iridium_analysis_fatal.pcap”。

该模块目前作为调试辅助代码，当解析某个数据帧出异常时，可以调用该模块提供的接口，把数据写到文件中，一个进程只能写一个数据帧，重复调用数据包会被复写。

##### 文件后缀匹配

目录：file_ext

文件类型后缀定义与匹配模块。