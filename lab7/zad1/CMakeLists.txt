cmake_minimum_required(VERSION 3.6)
project(zad1)

set(CMAKE_C_STANDARD 11)

set(BARBER_SOURCES
        src/lib/pidQueue.c
        src/lib/pidQueue.h
        src/barber.c src/lib/utils.h src/lib/utils.c src/common.h)
add_executable(barber ${BARBER_SOURCES})

set(KAMINO_SOURCES
        src/lib/pidQueue.c
        src/lib/pidQueue.h
        src/kamino.c src/lib/utils.h src/lib/utils.c src/common.h)
add_executable(kamino ${KAMINO_SOURCES})

