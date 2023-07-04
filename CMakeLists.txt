cmake_minimum_required(VERSION 3.19)





set (CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")
project(termonte)

add_executable(
  termnote
  main.cpp
)