cmake_minimum_required(VERSION 2.8)
project(network)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(echoclient echoclient.c csapp.c)
add_executable(echoserver echoserver.c csapp.c)
add_executable(echoserverp echoserverp.c csapp.c)
add_executable(echoservert echoservert.c csapp.c)
add_executable(handle_deadlock handle_deadlock.c csapp.c)
add_executable(race race.c csapp.c)
add_executable(no_race no_race.c csapp.c)