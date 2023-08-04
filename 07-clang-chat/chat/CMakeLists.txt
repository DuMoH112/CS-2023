cmake_minimum_required(VERSION 3.25)
project(chat C)

set(CMAKE_C_STANDARD 99)

add_executable(chat server/main.c)
