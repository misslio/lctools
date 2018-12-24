cmake_minimum_required(VERSION 2.4)

#项目名称
project(project_name)

#避免警告
if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#设置版本号
set(project_name_VERSION_MAJOR	1)
set(project_name_VERSION_MINOR	0)
set(project_name_VERSION_PATCH	0)
set(project_name_VERSION_REVISION	0000)

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
					/xxx/include
)

#链接路径设置
link_directories(/usr/local/lib/mariadb ${GLIB2_LIBRARIES})

#设置编译选项
add_compile_options(-Wall -Werror -O3)

#自定义库路径查找
find_library(LIBXXXLOG_PATH xxxlog /xxx/libs)
if(NOT LIBXXXLOG_PATH)
MESSAGE(FATAL_ERROR "libxxxlog not found")
endif(NOT LIBXXXLOG_PATH)

#设置链接库
set(link_libs ${LIBXXXLOG_PATH})

#config.h生成
configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

#设置源文件
set(file_FILES

)


#设置可执行程序名称
set(target_name xxx)
#设置库名称
set(lib_name	yyy)

#生成可执行文件
add_executable(${target_name} ${file_FILES})
target_link_libraries(${target_name} ${link_libs})

#生成动态库
add_library(${lib_name} SHARED ${file_FILES})
target_link_libraries(${lib_name} ${link_libs})
set_target_properties(${lib_name} 
			PROPERTIES VERSION 	${project_name_VERSION_MAJOR}.${project_name_VERSION_MINOR}.${project_name_VERSION_PATCH}.${project_name_VERSION_REVISION}}
			SOVERSION 1
)

#打开测试，目前对cmake的ctest了解不多，暂不总结
#enable_testing()

#安装
#set(CMAKE_INSTALL_PREFIX /iridium)
set(DESTDIR	/iridium)
install(FILES ${PROJECT_SOURCE_DIR}/file.h DESTINATION include/xxxx)		#头文件
install(FILES ${PROJECT_SOURCE_DIR}/config/xxx.common DESTINATION config)	#配置文件

install(TARGETS ${target_name} DESTINATION bin)			#可执行程序
install(TARGETS ${lib_name} DESTINATION libs)		#库
install(FILES ${PROJECT_BINARY_DIR}/config.h DESTINATION include/config.h.xxx)	#项目配置文件，方便在安装环境中查看版本

# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGING_INSTALL_PREFIX /iridium)
set(CPACK_SET_DESTDIR false)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "description")
set(CPACK_PACKAGE_VENDOR "WeTest")
set(CPACK_PACKAGE_VERSION "${project_name_VERSION_MAJOR}.${project_name_VERSION_MINOR}.${project_name_VERSION_PATCH}.${project_name_VERSION_REVISION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${project_name_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${project_name_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${project_name_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_GROUP "${PROJECT_NAME}")
set(CPACK_RPM_PACKAGE_URL "http://${PROJECT_NAME}.com")
set(CPACK_RPM_PACKAGE_DESCRIPTION "component")
set(CPACK_PACKAGE_RELEASE "${project_name_VERSION_MAJOR}")
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_NAME} Licence")

set (CPACK_GENERATOR "RPM")
include (CPack)

