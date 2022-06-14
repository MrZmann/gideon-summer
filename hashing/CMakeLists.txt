cmake_minimum_required(VERSION 3.22)
project(gideon_summer)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(gideon_summer
        Chained.h
        benchmark.cc
        Open.h
        #Product.h
        StdMap.h
        #test.cc
        stats.h
        #testing.cc
        StdUnorderedMap.h
        murmur3.c
        murmur3.h
        )
