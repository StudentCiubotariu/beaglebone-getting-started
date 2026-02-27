# toolchain-aarch64.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# --- Sysroot (RECOMMENDED) ---
# Pass this when configuring:
#   -DSYSROOT=/path/to/beagleplay-sysroot
if(DEFINED SYSROOT)
  set(CMAKE_SYSROOT ${SYSROOT})
  set(CMAKE_FIND_ROOT_PATH ${SYSROOT})
endif()

# Prevent CMake from mixing host and target paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  # programs (like protoc) come from host
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)   # libs from sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)   # headers from sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)   # packages from sysroot