cmake_minimum_required(VERSION 3.6)
project(zad1)

set(CMAKE_C_STANDARD 11)

set(ZAD2A_SOURCES
        src/zad2a.c
        src/common.h
        src/utils.h)
add_executable(zad2a ${ZAD2A_SOURCES})
target_link_libraries(zad2a pthread)

set(ZAD2B_SOURCES
        src/zad2b.c
        src/common.h
        src/utils.h)
add_executable(zad2b ${ZAD2B_SOURCES})
target_link_libraries(zad2b pthread)
