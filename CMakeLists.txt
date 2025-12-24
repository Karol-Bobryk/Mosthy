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


FILE(GLOB_RECURSE SOURCES 
     CONFIGURE_DEPENDS
     LIST_DIRECTORIES false 
     "src/*.cpp")

FILE(GLOB_RECURSE SOURCES_TEMPLATES 
     CONFIGURE_DEPENDS
     LIST_DIRECTORIES false 
     "include/*.hpp") # templates

add_executable(${CMAKE_PROJECT_NAME} 
  ${SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

enable_testing()

FILE(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false "tests/*.cpp")

if(TEST_SOURCES)
  add_executable(${CMAKE_PROJECT_NAME}_test
    ${TEST_SOURCES}
  )
  
  target_link_libraries(${CMAKE_PROJECT_NAME}_test 
                        PRIVATE 
                        Catch2::Catch2WithMain)

  target_include_directories(${CMAKE_PROJECT_NAME}_test PRIVATE include)

  include(Catch)
  catch_discover_tests(${CMAKE_PROJECT_NAME}_test)
endif()




