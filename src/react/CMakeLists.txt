file(GLOB react_SOURCE_FILES
  "*.cpp"
  "memory/*.cpp"
)

file(GLOB react_HEADERS
  "${react_SOURCE_DIR}/include/react/*.h"
  "${react_SOURCE_DIR}/include/react/memory/*.h"
)

include_directories(${react_SOURCE_DIR}/include)

add_library(react STATIC
  ${react_SOURCE_FILES}
)

set_target_properties(react PROPERTIES VERSION ${react_VERSION})

