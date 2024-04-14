cmake_minimum_required(VERSION 3.6)
project(sxtwl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES  "*.cpp" )

if(UNIX)
    add_compile_options(-fPIC)
elseif(MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
    #add_compile_options(/source-charset:utf-8)
endif()

add_library(sxtwl STATIC ${SRC_FILES})

# 安装库文件
install(TARGETS sxtwl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装头文件
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/const.h
    ${CMAKE_CURRENT_SOURCE_DIR}/day.h
    ${CMAKE_CURRENT_SOURCE_DIR}/eph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/JD.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SSQ.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sxtwl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/XL.h
    DESTINATION include
)
