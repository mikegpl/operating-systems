cmake_minimum_required(VERSION 3.6)
project(zad1)

set(CMAKE_C_STANDARD 11)

set(GENERATOR_SOURCES
        src/generator.c
        src/common.h src/utils.h)
add_executable(generator ${GENERATOR_SOURCES})

set(ZAD1A_SOURCES
        src/zad1a.c
        src/common.h
        src/utils.h)
add_executable(zad1a ${ZAD1A_SOURCES})
target_link_libraries(zad1a pthread)


set(ZAD1B_SOURCES
        src/zad1b.c
        src/common.h
        src/utils.h)
add_executable(zad1b ${ZAD1B_SOURCES})
target_link_libraries(zad1b pthread)

set(ZAD1C_SOURCES
        src/zad1c.c
        src/common.h
        src/utils.h)
add_executable(zad1c ${ZAD1C_SOURCES})
target_link_libraries(zad1c pthread)