cmake_minimum_required(VERSION 3.10)
project(sxtwl_cpp)

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()
  

add_subdirectory(src)

#编译例子
if(SXTWL_BUILD_EXAMPLES)
    add_subdirectory(example)
endif(SXTWL_BUILD_EXAMPLES)

#python的接口导出
if(SXTWL_WRAPPER_PYTHON)
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    find_package(PythonLibs)
    include_directories(${PYTHON_INCLUDE_DIR} "./src")

    set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
   
    SWIG_ADD_LIBRARY(sxtwl MODULE LANGUAGE python  SOURCES swig/sxtwl.i)
    SWIG_LINK_LIBRARIES(sxtwl ${PYTHON_LIBRARY} sxtwl)
endif(SXTWL_WRAPPER_PYTHON)