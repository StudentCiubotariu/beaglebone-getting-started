cmake_minimum_required(VERSION 3.16)
project(project_beagleplay LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(project_beagleplay src/hello.cpp)
# target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/include)
