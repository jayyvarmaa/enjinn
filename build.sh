#!/bin/bash

echo "========================================"
echo "  EnJinn Build Script (Linux/macOS)"
echo "========================================"
echo ""

# Check if vcpkg is installed
if [ -z "$VCPKG_ROOT" ]; then
    echo "[ERROR] VCPKG_ROOT environment variable not set"
    echo ""
    echo "Please install vcpkg and set VCPKG_ROOT:"
    echo "  1. git clone https://github.com/microsoft/vcpkg.git ~/vcpkg"
    echo "  2. cd ~/vcpkg"
    echo "  3. ./bootstrap-vcpkg.sh"
    echo "  4. export VCPKG_ROOT=~/vcpkg (add to ~/.bashrc or ~/.zshrc)"
    echo ""
    exit 1
fi

echo "[INFO] vcpkg found at: $VCPKG_ROOT"
echo ""

# Navigate to EnJinn directory
cd "$(dirname "$0")/EnJinn" || exit

# Create build directory
if [ ! -d "build" ]; then
    echo "[INFO] Creating build directory..."
    mkdir build
fi

cd build || exit

# Configure with CMake
echo "[INFO] Configuring project with CMake..."
echo "[INFO] Dependencies will be automatically installed from vcpkg.json"
echo ""

cmake .. -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if [ $? -ne 0 ]; then
    echo ""
    echo "[ERROR] CMake configuration failed!"
    echo "Please check the error messages above."
    exit 1
fi

echo ""
echo "[INFO] Configuration successful!"
echo ""

# Build the project
echo "[INFO] Building project (Debug configuration)..."
echo ""

cmake --build . --config Debug

if [ $? -ne 0 ]; then
    echo ""
    echo "[ERROR] Build failed!"
    echo "Please check the error messages above."
    exit 1
fi

echo ""
echo "========================================"
echo "  Build Complete!"
echo "========================================"
echo ""
echo "Executable location:"
echo "  $(pwd)/Debug/EnJinnCore"
echo ""
echo "To run the engine:"
echo "  cd build/Debug"
echo "  ./EnJinnCore"
echo ""
