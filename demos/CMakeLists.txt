cmake_minimum_required (VERSION 2.8)

file(GLOB demos_common_SOURCE_FILES
  "Common/*.cpp"
)

add_library(demos_common STATIC
  ${demos_common_SOURCE_FILES}
)

target_link_libraries(demos_common  GL GLU glut)

set(demos_ROOT_DIR ${react_SOURCE_DIR}/demos)

add_subdirectory(RayTracing)

