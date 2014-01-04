file(GLOB ray_SOURCE_FILES "*.cpp")

include_directories(
  ${react_SOURCE_DIR}/include
  ${react_SOURCE_DIR}/libs/glm
)

add_library(ray_lib ${ray_SOURCE_FILES})
target_link_libraries(ray_lib GL GLU glut react demo_lib)

add_executable(ray ray.cpp)
target_link_libraries(ray GL glut react demo_lib ray_lib)
set_target_properties(ray PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_SOURCE_DIR}
)
