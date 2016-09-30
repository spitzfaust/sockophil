cmake_minimum_required(VERSION 3.6)
project(sockophil)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include)
set(SOURCE_FILES
        ./src/sockophil.cpp include/sockophil/server.h src/server.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_executable(sockophil ${SOURCE_FILES})