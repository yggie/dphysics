cmake_minimum_required(VERSION 2.8)

file(GLOB ray_SOURCE_FILES "*.cpp")
file(GLOB ray_HEADER_FILES "*.h")

add_library(ray_lib
  ${ray_SOURCE_FILES}
)

#find_package(OpenMP REQUIRED)
#if (OpenMP_FOUND)
#message("FOUND ${OpenMP_C_FLAGS} and ${OpenMP_CXX_FLAGS} and ${OpenMP_EXE_LINKER_FLAGS}")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

target_link_libraries(ray_lib GL GLU glut react demos_common)

