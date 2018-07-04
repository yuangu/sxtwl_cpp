cmake_minimum_required(VERSION 3.10)
project(sxwnl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES  "*.cpp" )

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()

add_library(sxwnl STATIC ${SRC_FILES})