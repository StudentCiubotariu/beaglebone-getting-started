# 🐶 BeagleBone Getting Started (C++ / Embedded Linux)

A hands-on starter project for developing applications on **BeagleBone / BeaglePlay** boards using modern C++ and CMake.

This repository demonstrates a modular embedded application architecture, including logging, threading, communication flow control, and Protocol Buffers integration.

---

## 🚀 Overview

This project provides:

- ✅ Clean CMake-based build system  
- ✅ Modular architecture (logger, mutex, flow control, etc.)  
- ✅ Protobuf-based messaging  
- ✅ Cross-compilation support (WSL → ARM)  
- ✅ Native build support (WSL / Linux)

---

## 📁 Project Structure
├── App/
│ ├── proto/ # Protobuf definitions and generated code
│ ├── app_types/ # Core data structures
│ ├── connection_hub/ # Communication layer
│ ├── flow_control/ # Message flow handling
│ └── runnables/ # Execution logic
├── logger/ # Logging utilities
├── mutex/ # Thread synchronization
├── main.cpp # Entry point
├── CMakeLists.txt # Build configuration
├── toolchain-aarch64.cmake
└── build.sh # Build script


---

## ⚙️ Requirements

### 🖥️ For WSL / Linux (host build)