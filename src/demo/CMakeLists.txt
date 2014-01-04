file(GLOB demo_SOURCE_FILES
  "*.cpp"
)

include_directories(
  ${react_SOURCE_DIR}/include
  ${react_SOURCE_DIR}/libs/glm
)

add_library(demo_lib ${demo_SOURCE_FILES})
target_link_libraries(demo_lib GL GLU glut)

add_executable(demo demo.cpp)
target_link_libraries(demo GL GLU glut react demo_lib)
set_target_properties(demo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${react_SOURCE_DIR}
)

