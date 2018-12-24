cmake_minimum_required(VERSION 2.6)

project(clog_test)

include_directories(${PROJECT_SOURCE_DIR}

					/iridium/include
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_library(LIBMUDUO_CLOG_PATH muduo_clog /iridium/libs)
if(NOT LIBMUDUO_CLOG_PATH)
MESSAGE(FATAL_ERROR "libmuduo_clog not found")
endif(NOT LIBMUDUO_CLOG_PATH)

add_executable(test_clog simple_test.c)
target_link_libraries(test_clog ${LIBMUDUO_CLOG_PATH})
