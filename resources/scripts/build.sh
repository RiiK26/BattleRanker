#!/usr/bin/env bash

# Exit on any error
set -e

# Get the absolute path to the project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

cd "$PROJECT_ROOT"

echo "=== Building BattleRanker Internal Cheat ==="

# Configure CMake if the build directory hasn't been configured yet
if [ ! -f "build/CMakeCache.txt" ]; then
    echo "[*] Configuring CMake with MinGW64 toolchain..."
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-mingw64.cmake
fi

# Build the project using all available CPU cores
echo "[*] Compiling..."
cmake --build build -j$(nproc)

echo "[+] Build completed successfully!"
