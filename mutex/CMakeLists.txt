# Mutex module

#add_library(mutex
#  src/mutex.cpp
#)

add_library(mutex INTERFACE)

target_include_directories(mutex INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
