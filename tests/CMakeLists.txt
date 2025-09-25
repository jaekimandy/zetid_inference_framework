cmake_minimum_required(VERSION 3.10)
project(ZeticML_Tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source files - separate implementation files
set(FRAMEWORK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/linear_regression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/logistic_regression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/multi_class_classifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/two_layer_mlp.cpp
)

set(TEST_SOURCES
    test_neural_interface.cpp
)

# Create test executable
add_executable(neural_interface_tests
    ${FRAMEWORK_SOURCES}
    ${TEST_SOURCES}
)

# Enable testing
enable_testing()

# Add test
add_test(NAME interface_tests COMMAND neural_interface_tests)

# Custom target to run tests
add_custom_target(run_tests
    COMMAND neural_interface_tests
    DEPENDS neural_interface_tests
    COMMENT "Running unit tests"
)