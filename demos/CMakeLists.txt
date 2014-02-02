cmake_minimum_required (VERSION 2.8)

include_directories(
  ${react_SOURCE_DIR}/libs/glm
)

file(GLOB demos_common_SOURCE_FILES "Common/*.cpp")

add_library(demos_common STATIC
  ${demos_common_SOURCE_FILES}
)

target_link_libraries(demos_common GL GLU glut)

set(demos_ROOT_DIR ${react_SOURCE_DIR}/demos)

add_subdirectory(RayTracing)
add_subdirectory(PlanetaryMotion)

file(GLOB demos_dashboard_SOURCE_FILES "*.cpp")
add_library(demos_dashboard STATIC
  ${demos_dashboard_SOURCE_FILES}
)

set(ALL_DEMOS ray_lib planetary_motion_lib demos_common)
target_link_libraries(demos_dashboard ${ALL_DEMOS})

add_executable(ray RayTracing/ray.cpp)
target_link_libraries(ray GL glut react ${ALL_DEMOS} demos_dashboard)
set_target_properties(ray PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_BINARY_DIR}
)

add_executable(planets PlanetaryMotion/planets.cpp)
target_link_libraries(planets GL glut react ${ALL_DEMOS} demos_dashboard)
set_target_properties(planets PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_BINARY_DIR}
)

