cmake_minimum_required(VERSION 3.6)
project(sxtwl_c)

include_directories("../src")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()

file(GLOB SRC_FILES *.cpp *.c )

IF (EMSCRIPTEN)
add_executable(sxtwl_c  ${SRC_FILES})
target_link_libraries(sxtwl_c
sxtwl)
ELSE(EMSCRIPTEN)
add_library(sxtwl_c STATIC  ${SRC_FILES})
target_link_libraries(sxtwl_c
sxtwl)
ENDIF(EMSCRIPTEN)


