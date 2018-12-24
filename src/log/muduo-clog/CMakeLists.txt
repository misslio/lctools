cmake_minimum_required(VERSION 2.6)

project(muduo_clog C CXX)

set(muduo_clog_VERSION_MAJOR	1)
set(muduo_clog_VERSION_MINOR	0)
set(muduo_clog_VERSION_PATCH	8)

include_directories(${PROJECT_SOURCE_DIR}
)

set(TARGET_NAME muduo_clog)

set(BUILD_CENTOS_INFO	"UNKNOWN")
execute_process(COMMAND cat /etc/redhat-release OUTPUT_VARIABLE BUILD_CENTOS_INFO)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -rdynamic
 )
 
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_CXX_FLAGS "-fPIC") 

set(base_SRCS
  muduo/base/AsyncLogging.cc
  muduo/base/Condition.cc
  muduo/base/CountDownLatch.cc
  muduo/base/Date.cc
  muduo/base/Exception.cc
  muduo/base/FileUtil.cc
  muduo/base/LogFile.cc
  muduo/base/Logging.cc
  muduo/base/LogStream.cc
  muduo/base/ProcessInfo.cc
  muduo/base/Timestamp.cc
  muduo/base/TimeZone.cc
  muduo/base/Thread.cc
  muduo/base/ThreadPool.cc
  )

set(clog_SRCS
	muduo/clog/muduo_clog.cc
)
  
set(utils_SRCS
	muduo/utils/ConfFileRead.cc
)

add_library(${TARGET_NAME} SHARED ${base_SRCS} ${utils_SRCS} ${clog_SRCS})
target_link_libraries(${TARGET_NAME} pthread rt)
set_target_properties(${TARGET_NAME}
	PROPERTIES VERSION ${muduo_clog_VERSION_MAJOR}.${muduo_clog_VERSION_MINOR}.${muduo_clog_VERSION_PATCH}
	SOVERSION 1
)

add_subdirectory(muduo/utils/tests)

#安装
#set(CMAKE_INSTALL_PREFIX /iridium)
set(DESTDIR /iridium)
file(GLOB HEADERS "muduo/clog/*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/clog)
install(FILES config/iridiumlog_muduo.conf.common.clog DESTINATION config)
install(TARGETS ${TARGET_NAME} DESTINATION libs)


# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGING_INSTALL_PREFIX /iridium)
set(CPACK_SET_DESTDIR false)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${BUILD_CENTOS_INFO}")
set(CPACK_PACKAGE_VENDOR "77Element Co,Ltd.")
set(CPACK_PACKAGE_VERSION "${muduo_clog_VERSION_MAJOR}.${muduo_clog_VERSION_MINOR}.${muduo_clog_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${muduo_clog_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${muduo_clog_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${muduo_clog_VERSION_PATCH}")
set(CPACK_RPM_PACKAGE_GROUP "${PROJECT_NAME}")
set(CPACK_RPM_PACKAGE_URL "http://${PROJECT_NAME}.com")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${PROJECT_NAME} rpm packet")
set(CPACK_PACKAGE_RELEASE "${muduo_clog_VERSION_MAJOR}")
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_NAME} Licence")

set(CPACK_GENERATOR "RPM")
include (CPack)





