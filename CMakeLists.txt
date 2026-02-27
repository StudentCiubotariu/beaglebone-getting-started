cmake_minimum_required(VERSION 3.16)

project(project_beagleplay LANGUAGES CXX)

# ---------- Global language settings ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prefer -pthread when available
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# If you want Protobuf available globally (fine), keep this.
# Otherwise, you can move this find_package into App/proto instead.
find_package(Protobuf REQUIRED)

# ---------- Modules ----------
add_subdirectory(logger)
add_subdirectory(mutex)

add_subdirectory(App/proto)         # <-- NEW: owns protobuf generation + exposes a target
add_subdirectory(App/app_types)
add_subdirectory(App/connection_hub)
add_subdirectory(App/flow_control)
add_subdirectory(App/runnables)

# ---------- App ----------
add_executable(project_beagleplay
  main.cpp
)

target_link_libraries(project_beagleplay PRIVATE
  logger
  mutex
  runnables
  flow_control
  proto
  Threads::Threads
)
