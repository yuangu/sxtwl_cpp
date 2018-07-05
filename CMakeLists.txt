cmake_minimum_required(VERSION 3.10)
project(sxtwl_cpp)

IF(MSVC)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()

#编译库的例子
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
   
    SWIG_ADD_LIBRARY(sxtwl_python MODULE LANGUAGE python  SOURCES swig/sxtwl.i)
    SWIG_LINK_LIBRARIES(sxtwl_python ${PYTHON_LIBRARY} sxtwl)
endif(SXTWL_WRAPPER_PYTHON)

#java的接口导出
if(SXTWL_WRAPPER_JAVA)
    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    find_package(JNI)
    if(${JNI_FOUND})
        message(STATUS "Jni Found")
    else(${JNI_FOUND})
        message(FATAL_ERROR "not found Jni")
    endif()

    include_directories(${JNI_INCLUDE_DIRS} "./src")
     #增加包名
    set(CMAKE_SWIG_FLAGS -package com.huoyaojing.sxtwl)
    set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
   
    SWIG_ADD_LIBRARY(sxtwl_java MODULE LANGUAGE java  SOURCES swig/sxtwl.i)
    SWIG_LINK_LIBRARIES(sxtwl_java ${JNI_LIBRARIES} sxtwl)
endif(SXTWL_WRAPPER_JAVA)