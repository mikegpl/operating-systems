cmake_minimum_required(VERSION 3.6)
project(zad1)

set(CMAKE_C_STANDARD 11)

set(GENERATOR_SOURCES
        src/generator.c
        src/common.h src/utils.h)
add_executable(generator ${GENERATOR_SOURCES})

set(ZAD1A_SOURCES
        src/zad1a.c
        src/common.h)
add_executable(zad1a ${ZAD1A_SOURCES})
target_link_libraries(zad1a pthread rt)