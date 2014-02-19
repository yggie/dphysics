cmake_minimum_required(VERSION 2.6)

add_executable(utility_tests utility_tests.cpp)
target_link_libraries(utility_tests ${GTEST_LIBRARIES} pthread react)

add_custom_target(
  run_utility_tests ALL
)

add_custom_command(
  TARGET run_utility_tests
  COMMENT "Running all tests in the Utilities module"
  POST_BUILD COMMAND utility_tests 
)
