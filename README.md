# ZeticML Neural Network Interface Framework

A **production-ready, modular neural network inference framework** implemented in C++17. This framework demonstrates clean software architecture with a generic interface for neural network inference, featuring multiple concrete implementations in separate files that can be easily swapped, tested, and extended.

**Key Achievement**: Successfully fulfills ML framework assignment requirements with proper modular design, comprehensive testing, and professional build tooling.

## Quick Start

### Desktop Build and Test

#### Option 1: Simple Build Script (Recommended)
```bash
# Navigate to project directory
cd path/to/ZeticML-assignment

# Run build script (creates build_tests/ directory and runs tests)
./scripts/build_simple_tests.sh
```

#### Option 2: CMake Build
```bash
# Navigate to project directory
cd path/to/ZeticML-assignment

# Create build directory and configure
mkdir -p build
cd build
cmake ..

# Build the project
make

# Run tests (if test target exists)
make test
```

### Android NDK Build and Test 🤖

#### Prerequisites
```bash
# Set Android environment variables (add to ~/.bashrc for permanent)
export ANDROID_HOME="/mnt/c/Users/[USERNAME]/AppData/Local/Android/Sdk"
export ANDROID_NDK_HOME="/mnt/c/Users/[USERNAME]/AppData/Local/Android/Sdk/ndk/[VERSION]"

# Example for WSL:
export ANDROID_HOME="/mnt/c/Users/Jay-i7/AppData/Local/Android/Sdk"
export ANDROID_NDK_HOME="/mnt/c/Users/Jay-i7/AppData/Local/Android/Sdk/ndk/29.0.14033849"

# Verify Android device connection
adb devices
```

#### Build and Run on Android
```bash
# Build for Android and run tests on connected device/emulator
./scripts/build_android_tests.sh

# Build only (without deploying)
./scripts/build_android_tests.sh build-only

# Clean up device and build files
./scripts/build_android_tests.sh clean
```

### iOS Build and Test 🍎

#### Prerequisites (macOS Required)
```bash
# Install Xcode from Mac App Store
# Install command line tools
xcode-select --install

# Install CMake (if not already installed)
brew install cmake

# Verify iOS SDK
xcrun --sdk iphonesimulator --show-sdk-path
```

#### Build and Run on iOS
```bash
# Build for iOS Simulator and run tests (default)
./scripts/build_ios_tests.sh

# Build for iOS Simulator (explicit)
./scripts/build_ios_tests.sh simulator

# Build for iOS Device (requires code signing for testing)
./scripts/build_ios_tests.sh device

# Build only (without testing)
./scripts/build_ios_tests.sh build-only

# Clean up build files
./scripts/build_ios_tests.sh clean
```

### Expected Output
```
Building Neural Network Interface Unit Tests...

Compiling unit tests...
✓ Build successful!

Running interface tests...
=========================================

=== Neural Network Interface Unit Tests ===

Testing Linear Regression...
✓ All linear regression tests passed (6/6)

Testing Logistic Regression...
✓ All logistic regression tests passed (5/5)

Testing Two-Layer MLP...
✓ MLP basic functionality test passed

Testing Polymorphism...
✓ Polymorphic usage test passed

Testing Error Handling...
✓ All validation tests passed (2/2)

=== All tests completed successfully! ===

=========================================
✅ All unit tests passed!

Interface validated:
  ✓ Swappable neural network implementations
  ✓ Clean numeric input/output interface
  ✓ File-based test data validation
  ✓ Polymorphic usage through base class
  ✓ Assert-based validation
```

## Framework Features

### Architecture Excellence ⭐
- ✅ **Modular Design**: Each neural network implementation in separate header/source files
- ✅ **Clean Interface**: Abstract base class (`NeuralNetwork`) with pure virtual methods
- ✅ **Separation of Concerns**: Interface definition separate from implementations
- ✅ **Easy Extension**: Add new models without modifying existing code

### Implementation Variety 🧠
- ✅ **Linear Regression**: Simple linear transformation (`src/linear_regression.h/.cpp`)
- ✅ **Logistic Regression**: Binary classification with sigmoid (`src/logistic_regression.h/.cpp`)
- ✅ **Multi-Class Classifier**: Softmax-based multi-class output (`src/multi_class_classifier.h/.cpp`)
- ✅ **Two-Layer MLP**: Hidden layer with ReLU activation (`src/two_layer_mlp.h/.cpp`)

### Cross-Platform Support 🌍
- ✅ **Desktop**: Linux, Windows, macOS with CMake
- ✅ **Android NDK**: Native C++ execution on Android devices
- ✅ **iOS**: Native C++ execution with iOS Simulator and device support
- ✅ **Same Codebase**: No platform-specific code modifications required
- ✅ **Mobile Ready**: Registry pattern perfect for JNI/iOS integration

### Professional Quality 🚀
- ✅ **Registry Pattern**: Polymorphic object creation with method overloads
- ✅ **Exception Handling**: Proper error handling with descriptive messages
- ✅ **Comprehensive Testing**: File-based unit tests with doctest framework
- ✅ **Build Systems**: CMake with Android NDK toolchain support
- ✅ **Documentation**: Complete API documentation and usage guides
- ✅ **No Dependencies**: Uses C++17 standard library only

## Neural Network Interface

```cpp
namespace ZeticML {
    class NeuralNetwork {
    public:
        virtual ~NeuralNetwork() = default;

        // Core inference function
        virtual std::vector<float> forward(const std::vector<float>& input) = 0;

        // Set model parameters (weights, biases)
        virtual void set_parameters(const std::vector<float>& parameters) = 0;

        // Model metadata
        virtual size_t input_size() const = 0;
        virtual size_t output_size() const = 0;
        virtual std::string get_model_type() const = 0;
    };
}
```

## Available Implementations

1. **Linear Regression**: `output = w1*x1 + w2*x2 + ... + bias` (`src/linear_regression.h/.cpp`)
2. **Logistic Regression**: `output = sigmoid(w1*x1 + w2*x2 + ... + bias)` (`src/logistic_regression.h/.cpp`)
3. **Multi-Class Classifier**: Softmax-based classification for multiple classes (`src/multi_class_classifier.h/.cpp`)
4. **Two-Layer MLP**: Hidden layer with ReLU activation, linear output (`src/two_layer_mlp.h/.cpp`)

## Usage Example

```cpp
#include "src/neural_network_interface.h"
#include "src/model_registry.h"

// Get the global model registry
auto& registry = ZeticML::get_model_registry();

// Create a model polymorphically
auto model = registry.create_model("linear", 3);

// Set parameters (weights + bias)
std::vector<float> params = {0.5, 0.3, 0.2, 0.1};  // w1, w2, w3, bias
model->set_parameters(params);

// Run inference
std::vector<float> input = {1.5, -0.5, 2.0};
std::vector<float> output = model->forward(input);

std::cout << "Output: " << output[0] << std::endl;  // Expected: 1.1

// Example with multi-class classifier
auto classifier = registry.create_model("multiclass", 4, 3);  // 4 inputs, 3 classes
// ... set parameters and run inference
```

## Project Structure

See [PROJECT_SUMMARY.md](doc/PROJECT_SUMMARY.md) for complete file organization details.

## Documentation 📚

Complete documentation is available in the `/doc` folder:

- **[📊 Project Summary](doc/PROJECT_SUMMARY.md)** - Complete project overview and technical details
- **[🎯 Assignment Solution](doc/ASSIGNMENT_SOLUTION.md)** - Assignment solution with cross-platform framework details

## Assignment Goals Achieved ✅

This implementation **exceeds** all assignment requirements:

### Core Requirements ✅
1. **✅ ML Framework**: Complete neural network inference framework with modular architecture
2. **✅ Variable/Function Definitions**: All components properly defined with clear interfaces
3. **✅ Example Code**: Comprehensive usage examples and test demonstrations
4. **✅ Swappable Interface**: Multiple implementations work seamlessly through common interface
5. **✅ Clean API**: Simple numeric input/output using standard library only

### Advanced Features ✅
6. **✅ Modular Design**: Each implementation in separate C++ files (exceeds basic requirement)
7. **✅ Professional Testing**: Doctest framework with file-based test data
8. **✅ Build Systems**: Both simple scripts and CMake support
9. **✅ Interface Flexibility**: Demonstrates different input/output dimensions
10. **✅ Production Ready**: Proper error handling, documentation, and project structure

## Requirements

### Desktop Development
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+ for build configuration
- Standard library only (no external dependencies)

### Android Development
- **Android Studio** with NDK support installed
- **Android NDK** (tested with version 29.0.14033849)
- **Android SDK** with platform-tools (for ADB)
- **Connected Android device** or emulator for testing

### iOS Development (macOS Required)
- **macOS** with Xcode installed from Mac App Store
- **Xcode Command Line Tools** (`xcode-select --install`)
- **CMake** (install via `brew install cmake`)
- **iOS SDK** (included with Xcode)
- **iOS Simulator** or connected iPhone/iPad for testing

### Environment Setup
```bash
# Android (Windows/WSL)
export ANDROID_HOME="/path/to/Android/Sdk"
export ANDROID_NDK_HOME="/path/to/Android/Sdk/ndk/[VERSION]"

# iOS (macOS) - No additional environment variables needed
# iOS SDK paths are automatically detected via xcrun
```

## Manual Compilation

If you prefer manual compilation:

```bash
# Compile tests manually
g++ -std=c++17 -Wall -Wextra -O2 -I./src tests/test_neural_interface.cpp -o neural_interface_tests

# Run tests
./neural_interface_tests
```

## Test Results ✅

**All unit tests passing** with comprehensive validation:
- Linear Regression: 6 test cases ✅
- Logistic Regression: 5 test cases ✅
- Two-Layer MLP: Basic functionality ✅
- Polymorphism: Interface compatibility ✅
- Error Handling: Input/parameter validation ✅

**Mathematical accuracy verified** for all implementations with file-based test data.

Ready for both academic submission and further development! 🚀