cmake_minimum_required(VERSION 3.1)
project(CCIPCA)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdLibs/eigen339
)

set(LIBS "")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdLibs/opencv/include)
    set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/3rdLibs/opencv/lib/opencv_world3416.lib)
else(CMAKE_SYSTEM_NAME MATCHES "Windows")
    find_package(OpenCV3 REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    set(LIBS ${OpenCV_LIBS})
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/CCIPCA.cc)

add_executable(Example ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cc )
target_link_libraries(Example ${LIBS} ${PROJECT_NAME})
