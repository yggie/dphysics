cmake_minimum_required(VERSION 2.6)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runtests tests.cpp)
target_link_libraries(runtests ${GTEST_LIBRARIES} pthread react)

set_target_properties(runtests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_BINARY_DIR}
)
