cmake_minimum_required(VERSION 3.6)
project(zadb)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/zadb.c src/lib/utils.c src/lib/utils.h)
add_executable(zadb ${SOURCE_FILES})
target_link_libraries(zadb pthread rt)