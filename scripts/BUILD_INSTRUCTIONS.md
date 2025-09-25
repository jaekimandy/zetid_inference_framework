# ZeticML Neural Network Framework - Build Instructions

This directory contains build scripts for the ZeticML neural network inference framework.

## Available Build Scripts

### For WSL/Linux Users (Recommended)

**🎯 Main Build Scripts:**
- `./scripts/build_simple_tests.sh` - **Primary** build script with comprehensive testing
- `./scripts/build_cmake.sh` - CMake-based build with automatic test execution

**📋 Usage:**
```bash
# Make scripts executable (first time only)
chmod +x scripts/*.sh

# Option 1: Simple build script (recommended)
./scripts/build_simple_tests.sh

# Option 2: CMake build with professional tooling
./scripts/build_cmake.sh
```

### For Windows Users

Use WSL (Windows Subsystem for Linux) for the best experience with the scripts above. All scripts are optimized for WSL/Linux environments.

## What Each Script Does

### `build_simple_tests.sh` ⭐ **Recommended**
- Compiles all modular neural network implementations
- Builds comprehensive test suite with doctest framework
- Runs all unit tests automatically
- Creates `build_tests/` directory with executable
- **Files compiled**: All separate `.cpp` implementation files
- **Output**: `neural_interface_tests` executable

### `build_cmake.sh` 🔧 **Professional Build**
- Uses CMake for professional build management
- Compiles modular source files into static library
- Builds both tests and examples
- Runs tests automatically after successful build
- Creates `build/` directory with multiple executables
- **Output**: `neural_interface_tests` and `neural_network_example`

## Build Outputs

**Simple Build:**
```
build_tests/
└── neural_interface_tests    # Main test executable
```

**CMake Build:**
```
build/
├── neural_interface_tests    # Test executable
├── neural_network_example    # Usage example
└── libzetic_core.a          # Static library
```

## Framework Architecture

Our modular framework includes:
- **Linear Regression** (`src/linear_regression.h/.cpp`)
- **Logistic Regression** (`src/logistic_regression.h/.cpp`)
- **Multi-Class Classifier** (`src/multi_class_classifier.h/.cpp`)
- **Two-Layer MLP** (`src/two_layer_mlp.h/.cpp`)

## Troubleshooting

### Common Issues:

**"Permission denied":**
```bash
chmod +x scripts/*.sh
```

**"CMake not found":**
```bash
sudo apt install cmake
```

**Build fails with "file not found":**
- Make sure you're running from project root
- Check that all modular source files exist in `src/`

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+)
- Standard library only (no external dependencies except doctest for testing)
- CMake 3.10+ (for CMake build option)

## For Interview/Demo

**Recommended approach:**
1. **Start with simple build**: `./scripts/build_simple_tests.sh`
2. **Show test results**: Demonstrates comprehensive validation
3. **Try CMake build**: `./scripts/build_cmake.sh`
4. **Discuss architecture**: Modular design with separate implementation files