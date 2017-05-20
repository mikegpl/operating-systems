cmake_minimum_required(VERSION 3.6)
project(zada)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/zada.c src/lib/utils.c src/lib/utils.h)
add_executable(zada ${SOURCE_FILES})
target_link_libraries(zada pthread rt)