#!/bin/bash
# CMake-based build script for Zetic Neural Network Framework

set -e

echo "Building Zetic Neural Network Framework with CMake..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring project..."
cmake ..

# Build all targets
echo "Building all targets..."
make -j$(nproc 2>/dev/null || echo 4)

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Build successful!"
    echo ""
    echo "Built executables:"
    echo "  - neural_interface_tests (unit tests with doctest)"
    echo ""
    echo "Run commands:"
    echo "  ./neural_interface_tests"
    echo "  make test"
    echo ""
    echo "Running tests automatically..."
    echo "========================================="
    ./neural_interface_tests
    echo "========================================="
else
    echo "❌ Build failed!"
    exit 1
fi