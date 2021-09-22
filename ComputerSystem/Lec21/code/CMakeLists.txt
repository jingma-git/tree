cmake_minimum_required(VERSION 3.10)
project(network)

add_executable(echoclient echoclient.c csapp.c)
target_link_libraries(echoclient pthread)

add_executable(echoserveri echoserveri.c csapp.c echo.c)
target_link_libraries(echoserveri pthread)