cmake_minimum_required(VERSION 3.0)

project(traingame)

add_executable(traingame main.cpp)

install(TARGETS traingame RUNTIME DESTINATION bin)
