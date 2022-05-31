cmake_minimum_required(VERSION 3.22)
project(gideon_summer)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(gideon_summer
        chained.h
        product.h
        #stdmap.h
        #test.cc
        #stdmap.cpp
        chained.cpp
        stats.h
        stats.cpp
        filetest.cpp
        )
