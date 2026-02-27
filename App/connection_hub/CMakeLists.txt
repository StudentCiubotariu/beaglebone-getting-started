# streams (header-only)
add_library(connection_hub_streams INTERFACE)
add_library(connection_hub::streams ALIAS connection_hub_streams)

target_include_directories(connection_hub_streams INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# connection_hub (header-only)
add_library(connection_hub INTERFACE)
add_library(connection_hub::core ALIAS connection_hub)

target_include_directories(connection_hub INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(connection_hub INTERFACE
  connection_hub::streams
  logger
  app_types
)
