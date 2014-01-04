file(GLOB ray_SOURCE_FILES "*.cpp")
file(GLOB ray_HEADER_FILES "${react_SOURCE_DIR}/include/ray/*.h")

add_library(ray_lib
  ${ray_SOURCE_FILES}
)

target_link_libraries(ray_lib GL GLU glut react demo_lib)

add_executable(ray ray.cpp)
target_link_libraries(ray GL glut react demo_lib ray_lib)
set_target_properties(ray PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_BINARY_DIR}
)
