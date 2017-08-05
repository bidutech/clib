# cmake_minimum_required(VERSION <specify CMake version here>)
project(clib)

cmake_minimum_required(VERSION 2.8)

##set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=11")
set(CMAKE_BUILD_TYPE "Debug")
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp")
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )
message("Build Type:" ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

INCLUDE_DIRECTORIES(
common
common/uuid
test
)

set(SOURCE_FILES
        main.c
        common/clib_string.h
        common/clib_string.c
        common/clib_time.h
        common/clib_time.c
        common/clib_file.h
        common/clib_file.c
        common/clib_conf.h
        common/clib_conf.c
        common/clib_http.h
        common/clib_http.c
        common/clib_http_url.h
        common/clib_http_url.c
        common/clib_btype.h
        common/clib_des.c
        common/clib_des.h
        common/clib_des1.c
        common/clib_des1.h

        common/clib_system.h
        common/clib_system.c
        common/uuid/clib_uuid_wraper.h
        common/uuid/clib_uuid_wraper.c
        common/uuid/clib_uuid_md5.h
        common/uuid/clib_uuid_md5.c
        common/uuid/clib_uuid_datatype.h
        common/uuid/clib_uuid32.h
        common/uuid/clib_uuid32.c
        common/uuid/clib_uuid_wraper.c
        common/uuid/clib_uuid_wraper.h 
        
        test/stringtest.h
        test/stringtest.c

        )

add_executable(clib ${SOURCE_FILES})
