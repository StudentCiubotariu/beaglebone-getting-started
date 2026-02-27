add_library(runnables STATIC
  src/runnables.cpp
  src/appRunnables/runnable_app_one.cpp
  src/appRunnables/runnable_app_two.cpp
  src/appRunnables/runnable_app_three.cpp
  src/appRunnables/runnable_app_four.cpp
)

# Public include: exposes runnables.hpp to main
target_include_directories(runnables PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private include: lets runnables.cpp include runnables_internal.hpp in src/
target_include_directories(runnables PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(runnables PRIVATE
  flow_control
  connection_hub
  logger
  proto
  Threads::Threads
)
