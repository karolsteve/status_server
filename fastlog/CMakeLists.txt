cmake_minimum_required(VERSION 3.10.2)
project(fastlog)

set(CMAKE_CXX_STANDARD 17)
add_library(${PROJECT_NAME} STATIC src/FastLog.cpp include/${PROJECT_NAME}/FastLog.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/${PROJECT_NAME})