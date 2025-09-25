cmake_minimum_required(VERSION 3.15)
project(ZeticNeuralNetwork VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fsanitize=address -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -march=native -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Wall /fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /arch:AVX2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /arch:AVX2 /DNDEBUG")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Core library sources - modular design
set(CORE_SOURCES
    src/linear_regression.cpp
    src/logistic_regression.cpp
    src/multi_class_classifier.cpp
    src/two_layer_mlp.cpp
    src/model_registry.cpp
)

set(CORE_HEADERS
    src/neural_network_interface.h
    src/model_registry.h
    src/linear_regression.h
    src/logistic_regression.h
    src/multi_class_classifier.h
    src/two_layer_mlp.h
    src/test_data_loader.h
)

# Create core library
add_library(zetic_core STATIC ${CORE_SOURCES})

# Example executable
add_executable(neural_network_example examples/neural_example.cpp)
target_link_libraries(neural_network_example zetic_core)

# Test executables
add_executable(neural_interface_tests
    tests/test_neural_interface.cpp
)
target_link_libraries(neural_interface_tests zetic_core)

# Custom targets
add_custom_target(run_example
    COMMAND neural_network_example
    DEPENDS neural_network_example
    COMMENT "Running neural network example"
)

add_custom_target(run_all_tests
    COMMAND neural_interface_tests
    DEPENDS neural_interface_tests
    COMMENT "Running all unit tests"
)

# Android-specific configuration
if(ANDROID)
    # Link Android NDK libraries
    target_link_libraries(neural_interface_tests log)

    # Set Android-specific properties
    message(STATUS "Building for Android:")
    message(STATUS "  ABI: ${ANDROID_ABI}")
    message(STATUS "  Platform: ${ANDROID_PLATFORM}")
endif()

# iOS-specific configuration
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Set iOS-specific properties
    message(STATUS "Building for iOS:")
    message(STATUS "  Platform: ${PLATFORM}")
    message(STATUS "  Architecture: ${CMAKE_OSX_ARCHITECTURES}")
    message(STATUS "  Deployment Target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")

    # iOS doesn't need additional libraries for our C++ tests
    # All required functionality is in the standard library
endif()

# Testing
enable_testing()

add_test(NAME neural_interface_tests COMMAND neural_interface_tests)

# Set test properties
set_tests_properties(neural_interface_tests PROPERTIES
    PASS_REGULAR_EXPRESSION "All tests passed"
)

# Print configuration summary
message(STATUS "")
message(STATUS "Zetic Neural Network Framework Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")