file(GLOB react_SOURCE_FILES
  "*.cpp"
  "memory/*.cpp"
)

file(GLOB react_HEADER_FILES
  "${react_SOURCE_DIR}/include/react/*.h"
  "${react_SOURCE_DIR}/include/react/memory/*.h"
)

set(MY_HEADERS ${react_HEADER_FILES})

add_library(react STATIC
  ${react_SOURCE_FILES}
)

set_target_properties(react PROPERTIES VERSION ${react_VERSION})

