cmake_minimum_required(VERSION 3.0)
project(CicmWrapper)

message("Configuring CicmWrapper")

add_definitions(-O2 -fPIC)

file(GLOB_RECURSE PROJECT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

file(GLOB_RECURSE PROJECT_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(${PROJECT_NAME} ${PROJECT_SRCS})
