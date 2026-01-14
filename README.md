# Hello C with CMake (AArch64 – First Step)

This README shows how to create a **minimal C project** using **CMake + Ninja** and **cross-compile it for AArch64**, then run it locally using **QEMU**.

This is the **first step only**.  
(We will later adapt this for ARMv7 / BeaglePlay.)

---

## Create the project

```bash
# from anywhere, but recommended from ~/beagleplay to keep things clean
mkdir -p ~/beagleplay
cd ~/beagleplay
```

---

## Minimal CMake project file (REQUIRED)

```bash
cat > CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.16)
project(project_beagleplay LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(project_beagleplay
    src/hello.cpp
)
EOF
```

---

## Tiny C program

```bash
mkdir -p src

cat > src/hello.cpp <<'EOF'
#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> v{1,2,3,4,5};
    int sum = std::accumulate(v.begin(), v.end(), 0);

    std::cout << "Hello from C++17 on BeaglePlay! Sum=" << sum << "\n";
    std::cout << "Imi iubesc sotia pentru ca m-a lasat sa lucrez cu placuta azi\n";

    return 0;
}
EOF
```

---

## Cross toolchain file (AArch64)

```bash
cat > toolchain-aarch64.cmake <<'EOF'
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
EOF
```

---

## Make sure dependencies exist

```bash
sudo apt update
sudo apt install -y \
  cmake \
  ninja-build \
  gcc-aarch64-linux-gnu \
  g++-aarch64-linux-gnu \
  qemu-user
```

---

## Configure and build

```bash
# Correct command (note the 'c' in cmake and the explicit source '-S .')
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake
cmake --build build -v
```

---

## Run locally using QEMU

```bash
qemu-aarch64 -L ./build/project_beagleplay
```
