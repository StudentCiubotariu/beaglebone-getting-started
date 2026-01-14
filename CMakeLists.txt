cmake_minimum_required(VERSION 3.16)

project(project_beagleplay LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prefer -pthread when available
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Modules
add_subdirectory(logger)
add_subdirectory(mutex)
add_subdirectory(App/appTypes)
add_subdirectory(App/flexibleQueue)

# App
add_executable(project_beagleplay
  main.cpp
)

# Link modules (this is what propagates include dirs to main.cpp)
target_link_libraries(project_beagleplay PRIVATE
  logger
  mutex
  appTypes
  flexibleQueue
  Threads::Threads
)