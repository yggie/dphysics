file(GLOB react_SOURCE_FILES
  "*.cpp"
  "Memory/*.cpp"
  "Math/*.cpp"
  "Collision/Shapes/*.cpp"
  "Entities/*.cpp"
  "Utilities/*.cpp"
  "Collision/*.cpp"
  "Dynamics/*.cpp"
)

add_library(react STATIC
  ${react_SOURCE_FILES}
)

set_target_properties(react PROPERTIES VERSION ${react_VERSION})

