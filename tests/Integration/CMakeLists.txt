cmake_minimum_required(VERSION 2.6)

add_executable(integration_tests integration_tests.cpp)
target_link_libraries(integration_tests ${GTEST_LIBRARIES} pthread react tests_lib)

add_custom_target(
  run_integration_tests ALL
)

add_custom_command(
  TARGET run_integration_tests
  COMMENT "Running all integration tests"
  POST_BUILD COMMAND integration_tests
)

