cmake_minimum_required(VERSION 2.6)

add_executable(entities_tests entities_tests.cpp)
target_link_libraries(entities_tests ${GTEST_LIBRARIES} pthread react)

add_custom_target(
  run_entities_tests ALL
)

add_custom_command(
  TARGET run_entities_tests
  COMMENT "Running tests in the Shapes module"
  POST_BUILD COMMAND entities_tests
)
