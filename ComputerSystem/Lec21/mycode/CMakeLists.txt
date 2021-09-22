cmake_minimum_required(VERSION 2.8)
project(network)

set(CMAKE_BUILD_TYPE, DEBUG)
add_executable(echoclient echoclient.c csapp.c)
target_link_libraries(echoclient pthread)

add_executable(echoserver echoserver.c csapp.c)
target_link_libraries(echoserver pthread)