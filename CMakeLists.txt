cmake_minimum_required(VERSION 3.6)
project(sxtwl_cpp)

IF(MSVC)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_compile_options(/source-charset:utf-8)
add_compile_options(/wd26495) # 初始化警告
add_compile_options(/wd6011) #可能为null指针警告
add_compile_options(/wd4244) #隐式转换可能丢失数据警告
add_compile_options(/wd4101) #未引用的局部变量警告
add_compile_options(/wd4267) #隐式转换可能丢失数据警告 size_t -> int
ENDIF()

#编译库的例子
add_subdirectory(src)

#编译例子
if(SXTWL_BUILD_EXAMPLES)
    add_subdirectory(example)
endif(SXTWL_BUILD_EXAMPLES)

#python的接口导出
# if(SXTWL_WRAPPER_PYTHON)
#     find_package(SWIG REQUIRED)
#     include(${SWIG_USE_FILE})

#     find_package(PythonLibs)
#     include_directories(${PYTHON_INCLUDE_DIR} "./src")
#     set(CMAKE_SWIG_FLAGS)
#     set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    
#     SWIG_ADD_MODULE(sxtwl_python  python swig/sxtwl.i)
#     #SWIG_ADD_LIBRARY(sxtwl_python MODULE LANGUAGE python  SOURCES swig/sxtwl.i)
#     SWIG_LINK_LIBRARIES(sxtwl_python ${PYTHON_LIBRARY} sxtwl)
# endif(SXTWL_WRAPPER_PYTHON)

#java的接口导出(适作于android, 在externalNativeBuild 下的cmake加上 arguments "-DSXTWL_WRAPPER_JAVA=1")
if(SXTWL_WRAPPER_JAVA)
    # find_package(SWIG REQUIRED)
   

    IF(ANDROID)
        message(STATUS "Android Jni")
    ELSE(ANDROID)
        find_package(JNI)
        if(${JNI_FOUND})
            message(STATUS "Jni Found")
        else(${JNI_FOUND})
            message(FATAL_ERROR "not found Jni")
        endif()
    endif(ANDROID)

    include_directories(${JNI_INCLUDE_DIRS} "./src")
    # #增加包名
    # IF(ANDROID)
    #     #参考:https://github.com/sureshjoshi/android-ndk-swig-example/blob/master/AS3/app/CMakeLists.txt
    #     set(JAVA_GEN_PACKAGE "com.seantone.sxtwl")
    #     string(REPLACE "." "/" JAVA_GEN_SUBDIR ${JAVA_GEN_PACKAGE})
    #     set(JAVA_GEN_DIR ${Project_SOURCE_DIR}/src/main/java/${JAVA_GEN_SUBDIR})

    #     # -s选项为strip，不strip生成的库文件会很大   
    #     set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

    #     set(CMAKE_SWIG_FLAGS -c++ -package ${JAVA_GEN_PACKAGE})
    #     if(CONSOLE)
    #     else(CONSOLE)
    #         set(CMAKE_SWIG_OUTDIR ${JAVA_GEN_DIR})
    #         set(SWIG_OUTFILE_DIR ${Project_SOURCE_DIR}/src/main/sxtwl_jni_cpp/sxtwlJAVA_wrap.cxx)
    #     endif(CONSOLE)
    # ELSE(ANDROID)
    #     set(CMAKE_SWIG_FLAGS -c++ -package com.huoyaojing.sxtwl)
    # ENDIF(ANDROID)

    # set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    # SWIG_ADD_MODULE(sxtwl_java  java com.seantone.sxtwl)
    # #SWIG_ADD_LIBRARY(sxtwl_java MODULE LANGUAGE java  SOURCES swig/sxtwl.i)
    # SWIG_LINK_LIBRARIES(sxtwl_java ${JNI_LIBRARIES} sxtwl)

    add_library(sxtwl_java SHARED "./export/java/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_java sxtwl ${JNI_LIBRARIES})


endif(SXTWL_WRAPPER_JAVA)

#lua接口的导出
if(SXTWL_WRAPPER_LUA)
    # find_package(SWIG REQUIRED)
    # include(${SWIG_USE_FILE})

    find_package(Lua)
    if(${LUA_FOUND})
        message(STATUS "lua Found")
    else(${LUA_FOUND})
        message(FATAL_ERROR "not found lua")
    endif()

    include_directories(${LUA_INCLUDE_DIR} "./src")


    add_library(sxtwl_lua SHARED "./export/lua/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_lua sxtwl ${LUA_LIBRARIES})

    # set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    # set(CMAKE_SWIG_FLAGS)
    
    # SWIG_ADD_MODULE(sxtwl_lua  lua swig/sxtwl.i)
    # #SWIG_ADD_LIBRARY(sxtwl_lua MODULE LANGUAGE lua SOURCES swig/sxtwl.i)
    # SWIG_LINK_LIBRARIES(sxtwl_lua ${LUA_LIBRARIES} sxtwl)
endif(SXTWL_WRAPPER_LUA)


#C#接口导出
if(SXTWL_WRAPPER_CSHARP)
    # find_package(SWIG REQUIRED)
    # include(${SWIG_USE_FILE})

    include_directories( "./src")

    add_library(sxtwl_csharp SHARED "./export/C#/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_csharp sxtwl)


    # set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    # set(CMAKE_SWIG_FLAGS)
    
    # SWIG_ADD_MODULE(sxtwl_csharp  csharp swig/sxtwl.i)
    # #SWIG_ADD_LIBRARY(sxtwl_csharp MODULE LANGUAGE csharp SOURCES swig/sxtwl.i)
    # SWIG_LINK_LIBRARIES(sxtwl_csharp  sxtwl)
endif(SXTWL_WRAPPER_CSHARP)


#php接口导出(仅支持php5和php7)
if(SXTWL_WRAPPER_PHP5)

    include(${CMAKE_SOURCE_DIR}/cmake/FindPHP.cmake)
    
    include_directories(${PHP_INCLUDE_DIRS} ${PHP_EXTENSIONS_INCLUDE_DIR} "./src")
   

    add_library(sxtwl_php5 SHARED "./export/php5/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_php5 sxtwl)



    # find_package(SWIG REQUIRED)
    # include(${SWIG_USE_FILE})
    # set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    # set(CMAKE_SWIG_FLAGS)
    
    # if(${PHP_VERSION_MAJOR} MATCHES 7)
    #     SWIG_ADD_MODULE(sxtwl_php  php7 swig/sxtwl.i)
    # else(${PHP_VERSION_MAJOR} MATCHES 7)
    #     SWIG_ADD_MODULE(sxtwl_php  php swig/sxtwl.i)
    # endif(${PHP_VERSION_MAJOR} MATCHES 7)
    # SWIG_LINK_LIBRARIES(sxtwl_php  sxtwl )
    
endif(SXTWL_WRAPPER_PHP5)


if(SXTWL_WRAPPER_PHP7)
    include(${CMAKE_SOURCE_DIR}/cmake/FindPHP.cmake)
    
    include_directories(${PHP_INCLUDE_DIRS} ${PHP_EXTENSIONS_INCLUDE_DIR} "./src")
   

    add_library(sxtwl_php7 SHARED "./export/php5/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_php7 sxtwl)



    # find_package(SWIG REQUIRED)
    # include(${SWIG_USE_FILE})
    # set_property(SOURCE swig/sxtwl.i PROPERTY CPLUSPLUS ON)
    # set(CMAKE_SWIG_FLAGS)
    
    # if(${PHP_VERSION_MAJOR} MATCHES 7)
    #     SWIG_ADD_MODULE(sxtwl_php  php7 swig/sxtwl.i)
    # else(${PHP_VERSION_MAJOR} MATCHES 7)
    #     SWIG_ADD_MODULE(sxtwl_php  php swig/sxtwl.i)
    # endif(${PHP_VERSION_MAJOR} MATCHES 7)
    # SWIG_LINK_LIBRARIES(sxtwl_php  sxtwl )
    
endif(SXTWL_WRAPPER_PHP7)

# for golang
if(SXTWL_WRAPPER_GO)
    include_directories("./src")
    add_library(sxtwl_go  STATIC  "./export/golang/sxtwl_wrap.cxx") 
    target_link_libraries(sxtwl_go sxtwl)
endif(SXTWL_WRAPPER_GO)

# for c
if(SXTWL_WRAPPER_C)
    add_subdirectory(c)
endif(SXTWL_WRAPPER_C)
