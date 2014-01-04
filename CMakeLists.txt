cmake_minimum_required (VERSION 2.8)

project(react)

set(react_VERSION 0.0.0)

option(DEBUG "Enables assertions and logging" ON)

if(DEBUG)
  add_definitions(-g -DNDEBUG)
endif(DEBUG)
add_definitions(-std=c++11 -Wall -Wextra)

add_subdirectory(src/react)
add_subdirectory(src/demo)
add_subdirectory(src/ray)

