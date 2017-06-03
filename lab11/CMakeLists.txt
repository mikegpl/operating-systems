cmake_minimum_required(VERSION 3.7)
project(lab11)

set(CMAKE_C_STANDARD 11)

set(SERVER_FILES server.c)
add_executable(server ${SERVER_FILES})


set(CLIENT_FILES client.c)
add_executable(client ${CLIENT_FILES})