file(GLOB react_SOURCE_FILES
  "*.cpp"
  "Memory/*.cpp"
  "Collision/*.cpp"
  "Collision/Shapes/*.cpp"
  "Entities/*.cpp"
  "Utilities/*.cpp"
  "Dynamics/*.cpp"
)

add_library(react STATIC
  ${react_SOURCE_FILES}
)

set_target_properties(react PROPERTIES VERSION ${react_VERSION})

