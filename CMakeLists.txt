# cmake_minimum_required(VERSION <specify CMake version here>)
project(clib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(
common
test
)

set(SOURCE_FILES
        main.cpp
        common/clib_string.h
        common/clib_string.cpp
        common/clib_time.h
        common/clib_time.cpp
        test/stringtest.h
        test/stringtest.cpp)
add_executable(clib ${SOURCE_FILES})