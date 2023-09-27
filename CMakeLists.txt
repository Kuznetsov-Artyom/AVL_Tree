CMAKE_MINIMUM_REQUIRED(VERSION 3.25.0)

project(avl_tree)

include_directories(./headers)
include_directories(./sources)

add_executable(avl_tree ./sources/main.cpp)