

| 文档名称      | 修订记录 [创建\|更新\|归档] | 备注         | 作者/记录人 |
| :-------- | ----------------- | ---------- | ------ |
| 同步日志库使用说明 | 2018-06-29 创建     | 同步日志库使用者阅读 | lc     |



<div align="center"> Copyright&copy; 2015-2018 77element Co,Ltd. All Rights Reserved </div>

<div STYLE="page-break-after: always;"></div>

## 概述

同步日志库，即日志的格式化与写入会阻塞当前线程，在性能要求不高的情况下提供方便，可配的日志功能。如果要在对写入性能要求高的场景下使用，请使用异步日志库。

本文档仅适用于版本5.3及5.3版本以上的同步日志库。

## 获取安装包

同步日志的安装包采用rpm包的形式发布，在svn上的获取路径为：

http://192.168.77.90:8080/tfs/DefaultCollection/APM/_git/common_bin

请在该目录中的APM基础库开发库目录，包名称为(版本号为举例)

irisyslog-5.3.0.rpm

## 安装

将rpm包拷贝到目标主机上，运行如下命令安装

```
rpm -ivh irisyslog-5.3.0.rpm
```

安装后头文件路径如下

```
/iridium/include/irisyslog/
```

库文件路径如下

```
/iridium/libs
```

库名称如下:

```
libiridiumlog.so -> libiridiumlog.so.1
libiridiumlog.so.1 -> libiridiumlog.so.5.3.0
```

安装配置文件如下:

```
/iridium/config/iridiumlog_muduo.conf.common	#该配置文件不生效，具体见后面配置文件的说明
```



## 日志文件命名规则

### 日志文件路径

```
/var/log/iridium
```

改路径硬编码在日志模块中，自定义目录也是追加在该目录之下，比如建立了名为“drc”的子目录，则日志文件的绝对路径为/var/log/iridium/drc

### 日志文件名

初次查看/var/log/iridium目录下的日志文件时，可能会对长长的文件名感到诧异，会发现文件名里除了在调用iridium_write_log接口时传入的名称时，还多了其他的内容。

具体的组成如下所示

```
进程名.用户自定义文件名.创建时间.主机名.进程号.log
```

只有"__用户自定义文件名__"是使用者可以控制的，其他的内容都是日志库自己加上去的。可以方便的看到是哪个进程写入的这个日志和文件的创建以及进程的启动，所以在自定义日志文件名时最好不要传入".log"的后缀。

## 日志配置文件

配置文件位于/iridium/config目录下。

首次安装配置文件如下所示：

```
/iridium/config/iridiumlog_muduo.conf.common
```

名称为iridiumlog_muduo.conf.common的配置文件是不生效的，需要将该配置文件重命名为iridiumlog_muduo.conf，路径不变，操作如下：

```
[root@misslio config]# pwd
/iridium/config
[root@misslio config]# cp iridiumlog_muduo.conf.common iridiumlog_muduo.conf
```

### 日志开关

```
@LOG_SWITCH=1		#日志开关，为0关闭，为1打开。
```

### 以模块开始和结束

```
-example
key1 = value1
key2 = value2
key3 = value3
-end
```

### 默认的内置模块

```
-TOTAL_SWITCH		#日志总开关，如果该模块有效，则后面对特定文件的配置不再生效
IS_VALID = 0		#本模块的开关
TIMMER = 10			#DEBUG级别输出计时，不需要时写0，大于0，则DEBUG模块在该数值(秒)之后会自动关闭
LOGINFO_MAX_LEN = 10240
DEBUG.INFO.ERR.FATAL = 0.1.1.1
LOG_FILE.FILE.FUNC.LINE.DATE = 1.0.1.1.1
LOG_TERMINAL.FILE.FUNC.LINE.DATE = 0.1.1.1.1
SINGLE_MAX_SIZE = 1048576	#1024*1024
IS_PROCESS_LOCK = 0
-end
```

### 单独日志文件的例子

```
-iridiumlog_example.log
DEBUG.INFO.ERR.FATAL = 0.1.1.1
LOG_FILE.FILE.FUNC.LINE.DATE = 1.1.1.1.1
LOG_TERMINAL.FILE.FUNC.LINE.DATE = 0.1.0.1.0
SINGLE_MAX_SIZE = 1048576	#1024*1024
IS_PROCESS_LOCK = 0
-end
```



## 接口使用说明

### 日志级别

```
IRILOG_DEBUG
IRILOG_INFO
IRILOG_ERR
IRILOG_FATAL
```

* IRILOG_DEBUG 调试代码，生成环境中该级别输出关闭，默认不打开该配置，需要在配置文件中显式打开。
* IRILOG_INFO  信息代码，从这个级别开始，默认均是打开的，用于记录代码运行过程中的信息，了解代码的运行情况，但不可大量输出该级别信息。
* IRILOG_ERR 错误信息，系统出错时记录。
* IRILOG_FATAL  严重错误信息，一般该信息会导致代码无法继续运行。为了方便追踪错误，建议该级别错误让程序主动退出并做日志记录，但日志模块对此级别信息不做这样的特殊处理。

函数接口

__Interfaces__

<style> 
A {text-decoration: NONE} 
</style>

<a href="#iridium_init_log" >int iridium_init_log(char *logpath)</a>

<a href="#iridium_write_log" >int iridium_write_log(const char* fileName,  int level,  const char* fmt, ...)</a>

<a href="#iridium_destroy_log" >void iridium_destroy_log(void)</a>

<a href="#iridium_log_version" >const char * iridium_log_version()</a>

__Description__

iridium_init_log函数用于初始化日志模块，但用户可以不调用该接口，日志模块会自动触发该接口；同时该接口也提供了用户自定义日志目录的功能。

iridium_write_log其实是一个宏定义，但在这里隐藏了这种细节，这个接口是直接的日志写入接口。

iridium_destroy_log用来销毁日志模块，做内存测试时必须调用。

iridium_log_version会返回日志模块的版本号

__Functions__

<p id ="iridium_init_log" >int iridium_init_log(char *logpath)</p>

```
int iridium_init_log(char *logpath)
```

日志初始化函数

__*Parameters*__

logpath  本进程的子目录，不需要时可传入NULL，目录字符串以"/"开始，结束字符可以有"/"，也可没有，代码会自动处理结尾

__*Returns*__

0 : 初始化成功

-1 : 初始化失败



<p id ="iridium_write_log" >iridium_write_log(const char* fileName,int level,  const char* fmt, ...)</p>

```
int iridium_write_log(const char* fileName, int level,  const char fmt, ...)
```

日志写入接口

__*Parameters*__

fileName	日志文件名，可以为NULL，此时采用默认的日志文件名“iridium_default”，注意文件名不要有.log后缀，会影响日志命名规范

level	日志级别，参考本节开头的说明

fmt, ...	具体的日志内容，格式化字符串的变参形式

__*Returns*__

0 : 写入成功

-1 : 写入失败



<p id ="iridium_destroy_log" >iridium_destroy_log(void)</p>

```
void iridium_destroy_log(void)
```

日志销毁函数

__*Parameters*__

None

__*Returns*__

None



<p id ="iridium_log_version" >iridium_log_version()</p>

```
const char * iridium_log_version()
```

获取日志版本号字符串

__*Parameters*__

None

__*Returns*__

返回一个字符串



## 测试代码：

### 代码

显式初始化，且建立子目录，日志路径为/var/log/iridium/directory

```
/****
 * Copyright 2015‐2018 77Element Co,Ltd. All Rights Reserved
 * @file ‐ test.c
 * @brief ‐ 同步日志示例函数1
 * @author ‐ lc
 * @date ‐ 2018‐06-29
 ****/
#include <irisyslog/iridiumlog.h>	/* 需要编译时指定路径  */

int main()
{
	iridium_init_log("directory");	/* 初始化，建立名为directory的子目录 */
	
	/* 文件名传入NULL，则建立默认的日志，iridium_default，不建议使用 */
  	iridium_write_log(NULL, IRILOG_DEBUG, "Test the default filename log");
  	iridium_write_log("iridium_test1", IRILOG_DEBUG, "Test the filename debug log");
  	/* 注意以上DEBUG级别的日志，默认不打开输出，需要打开，请参考日志配置文件一节 */
  	
  	iridium_write_log("iridium_test1", IRILOG_INFO, "Test the filename info log");
    iridium_write_log("iridium_test1", IRILOG_ERR, "Test the filename err log");
	iridium_write_log("iridium_test1", IRILOG_FATAL, "Test the filename fatal log");
	
	iridium_destroy_log();	/* 销毁日志 */
	
  	return 0;
}
```

### 编译

以下提供CMakeLists.txt的编译方法，头文件和链接路径可以参考

```
cmake_minimum_required(VERSION 2.4)

#项目名称
project(no_project)

#避免警告
if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#设置版本号
set(no_project_VERSION_MAJOR	1)
set(no_project_VERSION_MINOR	0)
set(no_project_VERSION_PATCH	0)
set(no_project_VERSION_REVISION	0000)

find_package(PkgConfig REQUIRED)

#设置选项
option(example "default OFF" OFF)
option(example2 "default ON" ON)

#寻找失败则cmake退出
pkg_search_module(GLIB2 glib-2.0 REQUIRED)

#头文件包含
include_directories(${PROJECT_SOURCE_DIR}
					${PROJECT_BINARY_DIR}

					${GLIB2_INCLUDE_DIRS}
					/iridium/include
)

#链接路径设置
link_directories(/usr/local/lib/mariadb)

#设置编译选项
add_compile_options(-Wall -Werror -O3)

#自定义库路径查找
find_library(LIBIRIDIUMLOG_PATH iridiumlog /iridium/libs)
if(NOT LIBIRIDIUMLOG_PATH)
MESSAGE(FATAL_ERROR "libiridiumlog not found")
endif(NOT LIBIRIDIUMLOG_PATH)

#设置链接库
set(link_libs ${LIBIRIDIUMLOG_PATH} ${GLIB2_LIBRARIES})

#设置可执行程序名称
set(target_name test)
set(target_name2 test2)
set(target_name3 test3)

#生成可执行文件
add_executable(${target_name} irisyslog_test.c)
target_link_libraries(${target_name} ${link_libs})
```

以上CMakeLists.txt可直接使用，请按照以下目录结构建立文件,带"[]"的表示为目录：

```
--[irisyslog_test]
  |-CMakeLists.txt
  |-irisyslog_test.c
  |-[build]
```

将示例1，示例2，示例3代码分别拷入irisyslog_test.c, irisyslog_test2.c, irisyslog_test3.c, 

将上述cmake的示例代码拷入CMakeLists.txt，然后按如下示例操作

```
[root@misslio irisyslog_test]# ls
build  CMakeLists.txt  irisyslog_test.c
[root@misslio irisyslog_test]# cd build/
[root@misslio build]# ls
[root@misslio build]# cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Found PkgConfig: /usr/bin/pkg-config (found version "0.27.1") 
-- checking for one of the modules 'glib-2.0'
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/hgfs/A-LICAN-SVN/code/C/irisyslog_test/build
[root@misslio build]# make
Scanning dependencies of target test
[ 33%] Building C object CMakeFiles/test.dir/irisyslog_test.o
Linking C executable test
[root@misslio build]# ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile  test
```

最后一步的test即为可以测试的可执行代码。

