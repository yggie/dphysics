cmake_minimum_required(VERSION 2.6)

add_executable(shapes_tests shapes_tests.cpp)
target_link_libraries(shapes_tests ${GTEST_LIBRARIES} pthread react)

add_custom_target(
  run_shapes_tests ALL
)

add_custom_command(
  TARGET run_shapes_tests
  COMMENT "Running tests in the Shapes module"
  POST_BUILD COMMAND shapes_tests
)

