cmake_minimum_required(VERSION 2.6)

add_executable(math_tests math_tests.cpp)
target_link_libraries(math_tests ${GTEST_LIBRARIES} pthread react tests_lib)

add_custom_target(
  run_math_tests ALL
)

add_custom_command(
  TARGET run_math_tests
  COMMENT "Running tests in the Math module"
  POST_BUILD COMMAND math_tests
)

