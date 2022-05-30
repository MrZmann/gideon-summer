cmake_minimum_required(VERSION 3.22)
project(gideon_summer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(gideon_summer
        chained.h
        product.h
        #stdmap.h
        test.cc
        #stdmap.cpp
        chained.cpp)
