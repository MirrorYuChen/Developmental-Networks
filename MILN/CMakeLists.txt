cmake_minimum_required(VERSION 3.1)
project(MILN)

option(USE_CPP11 "Enable C++ 11 feature" ON)
option(BUILD_TEST "Build test examples" ON)
option(BUILD_SHARED_LIBS "Enable to build shared library" ON)

if(USE_CPP11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
else (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
endif(BUILD_SHARED_LIBS)

if (BUILD_TEST)
    add_executable(Example ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cc )
    target_link_libraries(Example ${LIBS} ${PROJECT_NAME})
endif(BUILD_TEST)
