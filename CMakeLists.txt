cmake_minimum_required(VERSION 3.8) # Kittens will die if you switch to an earlier version of CMake.

project(bobbxldr)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MMD -MP -O0 -g")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable("${CMAKE_PROJECT_NAME}" src/main.cpp src/INotifyWrapper.cpp)
