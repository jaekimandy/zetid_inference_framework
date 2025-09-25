#!/bin/bash
# Build and run neural network interface unit tests

echo "Building Neural Network Interface Unit Tests..."

# Create build directory
mkdir -p build_tests
cd build_tests

echo "Compiling unit tests..."

# Compile the unit tests with separate implementation files
g++ -std=c++17 -Wall -Wextra -O2 \
    -I../src \
    ../tests/test_neural_interface.cpp \
    ../src/linear_regression.cpp \
    ../src/logistic_regression.cpp \
    ../src/multi_class_classifier.cpp \
    ../src/two_layer_mlp.cpp \
    -o neural_interface_tests

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo ""
    echo "Running interface tests..."
    echo "========================================="

    # Run the tests
    ./neural_interface_tests

    test_result=$?
    echo "========================================="

    if [ $test_result -eq 0 ]; then
        echo "✅ All unit tests passed!"
        echo ""
        echo "Interface validated:"
        echo "  ✓ Swappable neural network implementations"
        echo "  ✓ Clean numeric input/output interface"
        echo "  ✓ File-based test data validation"
        echo "  ✓ Polymorphic usage through base class"
        echo "  ✓ Assert-based validation"
    else
        echo "❌ Some unit tests failed!"
    fi

    echo ""
    echo "Test data files used:"
    echo "  - tests/data/linear_regression_demo.txt"
    echo "  - tests/data/logistic_regression_demo.txt"
    echo ""
    echo "To add more tests:"
    echo "  1. Create new .txt files in tests/data/"
    echo "  2. Follow format: input1,input2,... -> expected_output"
    echo "  3. Rebuild and run!"

else
    echo "❌ Build failed!"
    exit 1
fi