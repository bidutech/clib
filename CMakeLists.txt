# cmake_minimum_required(VERSION <specify CMake version here>)
project(clib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

INCLUDE_DIRECTORIES(
common
test
)

set(SOURCE_FILES
        main.c
        common/clib_string.h
        common/clib_string.c
        common/clib_time.h
        common/clib_time.c
        test/stringtest.h
        test/stringtest.c)
add_executable(clib ${SOURCE_FILES})