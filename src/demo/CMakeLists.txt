cmake_minimum_required(VERSION 2.8)

file(GLOB demo_SOURCE_FILES "*.cpp")
file(GLOB demo_HEADER_FILES "${react_SOURCE_DIR}/include/demo/*.h")

add_library(demo_lib
  ${demo_SOURCE_FILES}
)
target_link_libraries(demo_lib GL GLU glut)

add_executable(demo demo.cpp)
target_link_libraries(demo GL GLU glut react demo_lib)
set_target_properties(demo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_BINARY_DIR}
)
