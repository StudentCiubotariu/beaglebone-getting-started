#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

TARGET="${1:-aarch64}"   # DEFAULT = aarch64
GENERATOR=(-G Ninja)

log() {
  echo
  echo "==== $1 ===="
}

mkdir -p logs
LOGFILE="logs/build-$(date +'%Y%m%d-%H%M%S').log"
exec > >(tee -a "$LOGFILE") 2>&1

echo "== Build started: $(date) =="
echo "Target: $TARGET"
echo "Log file: $LOGFILE"

# -------------------------
# Build
# -------------------------
case "$TARGET" in
  aarch64)
    log "Configure aarch64"
    cmake -S . -B build-aarch64 "${GENERATOR[@]}" \
      -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake

    log "Build aarch64"
    cmake --build build-aarch64 -v

    echo
    echo "AArch64 binary:"
    echo "  build-aarch64/project_beagleplay"
    echo "Copy to target device and run there."
    ;;
  host)
    log "Configure host"
    cmake -S . -B build-host "${GENERATOR[@]}"

    log "Build host"
    cmake --build build-host -v

    echo
    echo "Run locally:"
    echo "  ./build-host/project_beagleplay"
    ;;
  clean)
    rm -rf build-aarch64 build-host logs
    echo "Cleaned build directories + logs"

    # Optional: if you generate protobuf into the SOURCE tree:
    # rm -rf App/proto/generated
    # echo "Removed App/proto/generated"
    ;;
  *)
    echo "Usage:"
    echo "  ./build.sh        # aarch64 (target deploy)"
    echo "  ./build.sh host   # host build"
    echo "  ./build.sh clean"
    exit 1
    ;;
esac

echo
echo "== Build finished: $(date) =="
