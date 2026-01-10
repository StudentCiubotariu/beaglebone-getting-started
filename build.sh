#!/usr/bin/env bash
set -e

# Clean logs folder each run
rm -rf logs
mkdir -p logs
LOGFILE="logs/build-$(date +'%Y%m%d-%H%M%S').log"

# Mirror all output to console AND log file
exec > >(tee -a "$LOGFILE") 2>&1

echo "== Build started: $(date) =="
echo "Log file: $LOGFILE"
echo

# Your exact commands
rm -rf build
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake
cmake --build build -v

echo
echo "== Build finished: $(date) =="
