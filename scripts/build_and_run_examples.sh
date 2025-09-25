#!/bin/bash
# Build examples for ZeticML assignment

echo "Building ZeticML Examples..."

# Create build directory
mkdir -p build_examples
cd build_examples

echo "Compiling neural example with modular framework..."
g++ -std=c++17 -Wall -Wextra -O2 \
    -I../src \
    ../examples/neural_example.cpp \
    ../src/linear_regression.cpp \
    ../src/logistic_regression.cpp \
    ../src/multi_class_classifier.cpp \
    ../src/two_layer_mlp.cpp \
    ../src/model_registry.cpp \
    -o neural_example

echo "✓ Examples built successfully!"
echo ""
echo "Running example to show results..."
echo "================================="
./neural_example
echo "================================="
echo ""
echo "This example demonstrates:"
echo "  ✓ Modular neural network interface framework"
echo "  ✓ Four different implementation types"
echo "  ✓ Polymorphic usage through common interface"
echo "  ✓ Factory pattern for object creation"