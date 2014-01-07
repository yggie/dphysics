file(GLOB react_SOURCE_FILES
  "*.cpp"
  "Memory/*.cpp"
  "Collision/*.cpp"
  "Collision/Shapes/*.cpp"
  "Entities/*.cpp"
  "Utilities/*.cpp"
)

file(GLOB react_HEADER_FILES
  "${react_SOURCE_DIR}/include/react/*.h"
  "${react_SOURCE_DIR}/include/react/Memory/*.h"
  "${react_SOURCE_DIR}/include/react/Collision/*.h"
  "${react_SOURCE_DIR}/include/react/Collision/Shapes/*.h"
  "${react_SOURCE_DIR}/include/react/Entities/*.h"
  "${react_SOURCE_DIR}/include/react/Utilities/*.h"
)

set(MY_HEADERS ${react_HEADER_FILES})

add_library(react STATIC
  ${react_SOURCE_FILES}
)

set_target_properties(react PROPERTIES VERSION ${react_VERSION})

