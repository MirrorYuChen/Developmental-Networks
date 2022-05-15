cmake_minimum_required(VERSION 3.1)
project(IHDR)

option(USE_CPP11 "Enable C++ 11 feature" ON)
option(BUILD_TEST "Build test examples" ON)

if(USE_CPP11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else(WIN32)
    add_definitions(-fPIC)
endif(WIN32)

set(LIBS "")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/win/debug/IHDR.lib)
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/win/release/IHDR.lib)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/libIHDR.so)
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")

add_executable(Example ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cc)
target_link_libraries(Example ${LIBS})
