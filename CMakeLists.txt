cmake_minimum_required(
  VERSION 3.30) # Kittens will die if you switch to an earlier version of CMake.

project(bobbxldr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-MMD -MP)

add_subdirectory(lib/Catch2)


FILE(GLOB_RECURSE SOURCES_LIB
    CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "src/lib/*.cpp")

FILE(GLOB_RECURSE SOURCES_LIB_TEMPLATES
    CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "include/*.hpp") # templates

FILE(GLOB SOURCES_MAIN
    CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "src/main.cpp")

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES_LIB})
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC include)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES_MAIN})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib)

# SOURCES
#   _MAIN           - contains main.cpp
#   _LIB            - contains all library code for the main app
#   _LIB_TEMPLATES  - contains all header files
#   _TEST           - contains test files

enable_testing()

FILE(GLOB_RECURSE SOURCES_TEST LIST_DIRECTORIES false "tests/*.cpp")

if(SOURCES_TEST)
  add_executable(${CMAKE_PROJECT_NAME}_test ${SOURCES_TEST})

  target_link_libraries(${CMAKE_PROJECT_NAME}_test
                        PRIVATE
                        ${CMAKE_PROJECT_NAME}_lib
                        Catch2::Catch2WithMain)

  target_include_directories(${CMAKE_PROJECT_NAME}_test PRIVATE include)

  include(Catch)
  catch_discover_tests(${CMAKE_PROJECT_NAME}_test)
endif()

