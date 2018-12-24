cmake_minimum_required(VERSION 2.4)

project(irisyslog)
set(irisyslog_VERSION_MAJOR	5)
set(irisyslog_VERSION_MINOR	3)
set(irisyslog_VERSION_PATCH	3)

set(TARGET_NAME iridiumlog)

#close an unneccesary warning
cmake_policy(SET CMP0003 NEW)

option(OPEN_TEST "open compile test mode" OFF)
option(LOG_DEBUG "log printf debug info" OFF)

set(BUILD_CENTOS_INFO	"UNKNOWN")
execute_process(COMMAND cat /etc/redhat-release OUTPUT_VARIABLE BUILD_CENTOS_INFO)

add_definitions(-Wall -g -Werror)


#寻找失败则cmake退出
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 glib-2.0 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}
		${PROJECT_BINARY_DIR}
		
		${GLIB2_INCLUDE_DIRS}
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
add_definitions(-Wall -g)

configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

set(iridiumlog_SRCS
	iridiumlog.c
	wrapfcntl.c
	get_localtime/get_localtime.c
	iridiumlog_conf/iridiumlog_conf.c
	iridiumlog_conf/iridium_conf_core.c
	get_proc_name/get_proc_name.c
)

add_library(${TARGET_NAME} SHARED ${iridiumlog_SRCS})
target_link_libraries(${TARGET_NAME} glib-2.0 pthread)
set_target_properties(${TARGET_NAME} 
			PROPERTIES VERSION 	${irisyslog_VERSION_MAJOR}.${irisyslog_VERSION_MINOR}.${irisyslog_VERSION_PATCH}
			SOVERSION 1
)

if (OPEN_TEST)
add_executable(test_multithread logtest/multiThreadTest.c ${iridiumlog_SRCS})
target_link_libraries(test_multithread glib-2.0 pthread)

add_executable(test_simple logtest/test.c ${iridiumlog_SRCS})
target_link_libraries(test_simple glib-2.0 pthread)

add_executable(test_iridiumlog_conf logtest/test_iridiumlog_conf.c ${iridiumlog_SRCS})
target_link_libraries(test_iridiumlog_conf glib-2.0 pthread)
endif()

#安装
#set(CMAKE_INSTALL_PREFIX /iridium)
set(DESTDIR /iridium)
install(FILES ${PROJECT_SOURCE_DIR}/com_def.h DESTINATION include/irisyslog)
install(FILES ${PROJECT_SOURCE_DIR}/iridiumlog.h DESTINATION include/irisyslog)
install(FILES ${PROJECT_SOURCE_DIR}/wrapfcntl.h DESTINATION include/irisyslog)

install(FILES ${PROJECT_SOURCE_DIR}/config/iridiumlog_muduo.conf.common DESTINATION config)

install(TARGETS iridiumlog DESTINATION libs)

# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGING_INSTALL_PREFIX /iridium)
set(CPACK_SET_DESTDIR false)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${BUILD_CENTOS_INFO}")
set(CPACK_PACKAGE_VENDOR "WeTest")
set(CPACK_PACKAGE_VERSION "${irisyslog_VERSION_MAJOR}.${irisyslog_VERSION_MINOR}.${irisyslog_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${irisyslog_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${irisyslog_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${irisyslog_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_GROUP "${PROJECT_NAME}")
set(CPACK_RPM_PACKAGE_URL "http://${PROJECT_NAME}.com")
set(CPACK_RPM_PACKAGE_DESCRIPTION "iridium System log component")
set(CPACK_PACKAGE_RELEASE "${irisyslog_VERSION_MAJOR}")
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_NAME} Licence")

set(CPACK_GENERATOR "RPM")

#set (CPACK_RESOURCE_FILE_LICENSE
#  "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${irisyslog_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${irisyslog_VERSION_MINOR}")
include (CPack)