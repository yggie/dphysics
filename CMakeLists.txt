cmake_minimum_required (VERSION 2.8)

project(react)

set(react_VERSION 0.0.0)

option(DEBUG "Enables assertions and logging" ON)
option(DEMO "Run demo application" ON)
option(TESTS "Compile tests specs (requires gtest library)" ON)

if(DEBUG)
  add_definitions(-g -DNDEBUG)
endif(DEBUG)
add_definitions(-std=c++11 -Wall -Wextra)

include_directories(
  ${react_SOURCE_DIR}
  ${react_SOURCE_DIR}/include
  ${react_SOURCE_DIR}/libs/glm
)

add_subdirectory(src/react)
add_subdirectory(src/demo)
add_subdirectory(demos)
if(TESTS)
  add_subdirectory(tests)
endif(TESTS)
