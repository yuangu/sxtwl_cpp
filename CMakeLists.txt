cmake_minimum_required(VERSION 2.6)
project(sxtml_cpp)

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()

add_subdirectory(src)
add_subdirectory(example)