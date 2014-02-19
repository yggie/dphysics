cmake_minimum_required(VERSION 2.6)

add_executable(collision_tests collision_tests.cpp)
target_link_libraries(collision_tests ${GTEST_LIBRARIES} pthread react)

add_custom_target(
  run_collision_tests ALL
)

add_custom_command(
  TARGET run_collision_tests
  COMMENT "Running tests in the Collisions module"
  POST_BUILD COMMAND collision_tests
)

