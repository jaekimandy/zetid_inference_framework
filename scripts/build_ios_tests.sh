#!/bin/bash
# ZeticML iOS CMake Test Runner
# Uses existing CMakeLists.txt with iOS toolchain (similar to Android approach)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== ZeticML iOS CMake Test Runner ===${NC}"

# Configuration
IOS_PLATFORM=${IOS_PLATFORM:-"SIMULATOR"}  # SIMULATOR or OS
IOS_DEPLOYMENT_TARGET=${IOS_DEPLOYMENT_TARGET:-"12.0"}
BUILD_DIR="build_ios"
TEST_EXECUTABLE="neural_interface_tests"

# Function to check prerequisites
check_prerequisites() {
    echo -e "${YELLOW}Checking prerequisites...${NC}"

    # Check if running on macOS
    if [[ "$OSTYPE" != "darwin"* ]]; then
        echo -e "${RED}Error: iOS development requires macOS${NC}"
        exit 1
    fi

    # Check Xcode command line tools
    if ! command -v xcrun &> /dev/null; then
        echo -e "${RED}Error: Xcode command line tools not found${NC}"
        echo "Please install with: xcode-select --install"
        exit 1
    fi

    # Check iOS toolchain file
    TOOLCHAIN_FILE="ios.toolchain.cmake"
    if [ ! -f "$TOOLCHAIN_FILE" ]; then
        echo -e "${RED}Error: iOS CMake toolchain not found${NC}"
        echo "Expected: $TOOLCHAIN_FILE"
        exit 1
    fi

    # Check iOS SDK
    if [ "$IOS_PLATFORM" = "SIMULATOR" ]; then
        IOS_SDK_PATH=$(xcrun --sdk iphonesimulator --show-sdk-path 2>/dev/null || echo "")
    else
        IOS_SDK_PATH=$(xcrun --sdk iphoneos --show-sdk-path 2>/dev/null || echo "")
    fi

    if [ -z "$IOS_SDK_PATH" ]; then
        echo -e "${RED}Error: iOS SDK not found${NC}"
        echo "Please install Xcode from the Mac App Store"
        exit 1
    fi

    # Check CMake
    if ! command -v cmake &> /dev/null; then
        echo -e "${RED}Error: CMake not found${NC}"
        echo "Please install CMake: brew install cmake"
        exit 1
    fi

    echo -e "${GREEN}✓ Prerequisites check passed${NC}"
    echo "  iOS SDK: $IOS_SDK_PATH"
    echo "  Platform: $IOS_PLATFORM"
}

# Function to build using our existing CMake
build_with_cmake() {
    echo -e "${YELLOW}Building with CMake for iOS...${NC}"

    # Create build directory
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    # Configure with iOS toolchain (reusing our existing CMakeLists.txt!)
    echo "Configuring CMake with iOS toolchain..."
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE="../ios.toolchain.cmake" \
        -DPLATFORM=$IOS_PLATFORM \
        -DDEPLOYMENT_TARGET=$IOS_DEPLOYMENT_TARGET \
        -DCMAKE_BUILD_TYPE=Release

    # Build using our existing CMake targets
    echo "Building neural_interface_tests target..."
    make neural_interface_tests -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)

    if [ -f "$TEST_EXECUTABLE" ]; then
        echo -e "${GREEN}✓ iOS build successful${NC}"
        echo "Executable: $BUILD_DIR/$TEST_EXECUTABLE"
    else
        echo -e "${RED}✗ iOS build failed - executable not found${NC}"
        exit 1
    fi

    cd ..
}

# Function to prepare test data for iOS
prepare_test_data() {
    echo -e "${YELLOW}Preparing test data for iOS...${NC}"

    # Copy test data to build directory
    cp -r tests/data $BUILD_DIR/ 2>/dev/null || true

    echo -e "${GREEN}✓ Test data prepared${NC}"
}

# Function to run tests
run_tests() {
    echo -e "${YELLOW}Running C++ tests for iOS...${NC}"

    if [ "$IOS_PLATFORM" = "SIMULATOR" ]; then
        echo -e "${BLUE}Running on iOS Simulator (can execute directly)${NC}"
    else
        echo -e "${YELLOW}Built for iOS Device (would need device deployment)${NC}"
        echo -e "${YELLOW}For device testing, you would need to deploy via Xcode${NC}"
        return 0
    fi

    echo -e "${BLUE}=================== Test Output ===================${NC}"

    # Run tests (only works for iOS Simulator)
    if cd $BUILD_DIR && ./$TEST_EXECUTABLE; then
        echo -e "${BLUE}==================================================${NC}"
        echo -e "${GREEN}✅ All iOS C++ tests completed successfully!${NC}"
    else
        echo -e "${BLUE}==================================================${NC}"
        echo -e "${RED}❌ Some tests failed${NC}"
        exit 1
    fi

    cd ..
}

# Function to cleanup
cleanup() {
    echo -e "${YELLOW}Cleaning up...${NC}"
    rm -rf $BUILD_DIR 2>/dev/null || true
    echo -e "${GREEN}✓ Cleanup completed${NC}"
}

# Function to show system info
show_system_info() {
    echo -e "${BLUE}macOS and iOS development info:${NC}"
    echo "macOS: $(sw_vers -productVersion 2>/dev/null || echo 'Unknown')"
    echo "Xcode: $(xcodebuild -version 2>/dev/null | head -1 || echo 'Unknown')"
    if [ "$IOS_PLATFORM" = "SIMULATOR" ]; then
        echo "iOS Simulator SDK: $(xcrun --sdk iphonesimulator --show-sdk-path 2>/dev/null || echo 'Unknown')"
    else
        echo "iOS Device SDK: $(xcrun --sdk iphoneos --show-sdk-path 2>/dev/null || echo 'Unknown')"
    fi
    echo ""
}

# Main execution
main() {
    check_prerequisites
    show_system_info
    build_with_cmake
    prepare_test_data
    run_tests

    echo -e "${GREEN}🚀 ZeticML iOS C++ testing completed successfully!${NC}"
    echo -e "${YELLOW}Note: Same C++ codebase and test suite as desktop and Android${NC}"
}

# Handle script arguments
case "${1:-}" in
    "clean")
        echo "Cleaning up..."
        cleanup
        echo -e "${GREEN}✓ Cleaned up build files${NC}"
        ;;
    "build-only")
        check_prerequisites
        show_system_info
        build_with_cmake
        echo -e "${GREEN}✓ Build completed. Run without arguments to test.${NC}"
        ;;
    "device")
        export IOS_PLATFORM="OS"
        echo "Building for iOS Device..."
        main
        ;;
    "simulator")
        export IOS_PLATFORM="SIMULATOR"
        echo "Building for iOS Simulator..."
        main
        ;;
    "help"|"-h"|"--help")
        echo "ZeticML iOS CMake Test Runner"
        echo ""
        echo "Usage: $0 [COMMAND]"
        echo ""
        echo "Commands:"
        echo "  (no args)    Build and run C++ tests for iOS Simulator"
        echo "  simulator    Build and run for iOS Simulator (default)"
        echo "  device       Build for iOS Device (requires code signing for testing)"
        echo "  build-only   Build for iOS but don't run tests"
        echo "  clean        Clean up build files"
        echo "  help         Show this help message"
        echo ""
        echo "Environment Variables:"
        echo "  IOS_PLATFORM         SIMULATOR or OS (default: SIMULATOR)"
        echo "  IOS_DEPLOYMENT_TARGET iOS version (default: 12.0)"
        echo ""
        echo "Prerequisites:"
        echo "  - macOS with Xcode and command line tools"
        echo "  - CMake installed (brew install cmake)"
        ;;
    "")
        main
        ;;
    *)
        echo -e "${RED}Unknown command: $1${NC}"
        echo "Run '$0 help' for usage information"
        exit 1
        ;;
esac