cmake_minimum_required(VERSION 2.8)

file(GLOB planetary_motion_SOURCE_FILES "*.cpp")
file(GLOB planetary_motion_HEADER_FILES "*.h")

add_library(planetary_motion_lib
  ${planetary_motion_SOURCE_FILES}
)

target_link_libraries(planetary_motion_lib GL GLU glut react demos_common)

