cmake_minimum_required(VERSION 2.6)
project(sxwnl)

 set(CMAKE_CXX_STANDARD 11)
 set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES  "*.cpp" )

 add_executable(sxwnl ${SRC_FILES})