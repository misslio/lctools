cmake_minimum_required(VERSION 2.4)

project(csort)

if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_compile_options(-Wall -g -O0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(insertsort_test
	Insertsort.c
	Insertsort_test.c
)
