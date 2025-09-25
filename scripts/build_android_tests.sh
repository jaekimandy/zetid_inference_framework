#!/bin/bash
# ZeticML Android NDK Test Runner using existing CMake
# Leverages our existing CMakeLists.txt with Android NDK toolchain

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== ZeticML Android NDK Test Runner (CMake) ===${NC}"

# Configuration
ANDROID_NDK_HOME=${ANDROID_NDK_HOME:-"$ANDROID_HOME/ndk-bundle"}
ANDROID_ABI=${ANDROID_ABI:-"arm64-v8a"}
ANDROID_PLATFORM=${ANDROID_PLATFORM:-"android-21"}
BUILD_DIR="build_android"
TEST_EXECUTABLE="neural_interface_tests"
DEVICE_PATH="/data/local/tmp/$TEST_EXECUTABLE"

# Function to check prerequisites
check_prerequisites() {
    echo -e "${YELLOW}Checking prerequisites...${NC}"

    # Check Android NDK
    if [ ! -d "$ANDROID_NDK_HOME" ]; then
        echo -e "${RED}Error: Android NDK not found at $ANDROID_NDK_HOME${NC}"
        echo "Please set ANDROID_NDK_HOME environment variable or install Android NDK"
        exit 1
    fi

    # Check CMake toolchain file
    TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake"
    if [ ! -f "$TOOLCHAIN_FILE" ]; then
        echo -e "${RED}Error: Android NDK CMake toolchain not found${NC}"
        echo "Expected: $TOOLCHAIN_FILE"
        exit 1
    fi

    # Check ADB
    if ! command -v adb &> /dev/null; then
        echo -e "${RED}Error: ADB not found. Please install Android SDK platform-tools${NC}"
        exit 1
    fi

    # Check device connection
    DEVICE_COUNT=$(adb devices | grep -c "device$" || true)
    if [ "$DEVICE_COUNT" -eq 0 ]; then
        echo -e "${RED}Error: No Android device connected${NC}"
        echo "Please connect an Android device or start an emulator"
        echo "Run 'adb devices' to check connected devices"
        exit 1
    fi

    # Check CMake
    if ! command -v cmake &> /dev/null; then
        echo -e "${RED}Error: CMake not found${NC}"
        exit 1
    fi

    echo -e "${GREEN}✓ Prerequisites check passed${NC}"
    echo "  NDK: $ANDROID_NDK_HOME"
    echo "  Target: $ANDROID_ABI ($ANDROID_PLATFORM)"
}

# Function to build using our existing CMake
build_with_cmake() {
    echo -e "${YELLOW}Building with CMake for Android...${NC}"

    # Create build directory
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    # Configure with Android NDK toolchain (reusing our existing CMakeLists.txt!)
    echo "Configuring CMake with Android NDK toolchain..."
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI=$ANDROID_ABI \
        -DANDROID_PLATFORM=$ANDROID_PLATFORM \
        -DCMAKE_BUILD_TYPE=Release

    # Build using our existing CMake targets
    echo "Building neural_interface_tests target..."
    make neural_interface_tests -j$(nproc 2>/dev/null || echo 4)

    if [ -f "$TEST_EXECUTABLE" ]; then
        echo -e "${GREEN}✓ Build successful${NC}"
        echo "Executable: $BUILD_DIR/$TEST_EXECUTABLE"
    else
        echo -e "${RED}✗ Build failed - executable not found${NC}"
        exit 1
    fi

    cd ..
}

# Function to copy test data to Android
copy_test_data() {
    echo -e "${YELLOW}Copying test data to Android device...${NC}"

    # Create test data directory on device
    adb shell "mkdir -p /data/local/tmp/tests/data" 2>/dev/null || true

    # Copy test data files
    local files_copied=0
    for test_file in tests/data/*.txt; do
        if [ -f "$test_file" ]; then
            filename=$(basename "$test_file")
            echo "Copying $filename..."
            adb push "$test_file" "/data/local/tmp/tests/data/$filename"
            files_copied=$((files_copied + 1))
        fi
    done

    echo -e "${GREEN}✓ Copied $files_copied test data files${NC}"
}

# Function to deploy and run tests
deploy_and_run() {
    echo -e "${YELLOW}Deploying and running tests on Android device...${NC}"

    # Push executable
    echo "Pushing test executable..."
    adb push "$BUILD_DIR/$TEST_EXECUTABLE" "$DEVICE_PATH"

    # Make executable
    adb shell "chmod 755 $DEVICE_PATH"

    # Copy test data
    copy_test_data

    # Show device info
    echo -e "${BLUE}Device: $(adb shell getprop ro.product.model 2>/dev/null || echo 'Unknown')${NC}"
    echo -e "${BLUE}Android: $(adb shell getprop ro.build.version.release 2>/dev/null || echo 'Unknown')${NC}"

    echo -e "${BLUE}=================== Test Output ===================${NC}"

    # Run tests and capture output
    if adb shell "cd /data/local/tmp && ./$TEST_EXECUTABLE"; then
        echo -e "${BLUE}==================================================${NC}"
        echo -e "${GREEN}✅ All Android tests completed successfully!${NC}"
    else
        echo -e "${BLUE}==================================================${NC}"
        echo -e "${RED}❌ Some tests failed${NC}"
        exit 1
    fi
}

# Function to cleanup
cleanup() {
    echo -e "${YELLOW}Cleaning up...${NC}"
    adb shell "rm -f $DEVICE_PATH" 2>/dev/null || true
    adb shell "rm -rf /data/local/tmp/tests" 2>/dev/null || true
    rm -rf $BUILD_DIR 2>/dev/null || true
    echo -e "${GREEN}✓ Cleanup completed${NC}"
}

# Main execution
main() {
    check_prerequisites
    build_with_cmake
    deploy_and_run

    echo -e "${GREEN}🚀 ZeticML Android testing completed successfully!${NC}"
    echo -e "${YELLOW}Note: Test files remain on device. Run '$0 clean' to remove them.${NC}"
}

# Handle script arguments
case "${1:-}" in
    "clean")
        echo "Cleaning up..."
        cleanup
        echo -e "${GREEN}✓ Cleaned up build files and device${NC}"
        ;;
    "build-only")
        check_prerequisites
        build_with_cmake
        echo -e "${GREEN}✓ Build completed. Run without arguments to deploy and test.${NC}"
        ;;
    "help"|"-h"|"--help")
        echo "ZeticML Android NDK Test Runner (using existing CMake)"
        echo ""
        echo "Usage: $0 [COMMAND]"
        echo ""
        echo "Commands:"
        echo "  (no args)    Build and run tests on Android device"
        echo "  build-only   Build for Android but don't deploy"
        echo "  clean        Clean up build files and device"
        echo "  help         Show this help message"
        echo ""
        echo "Environment Variables:"
        echo "  ANDROID_NDK_HOME    Path to Android NDK (required)"
        echo "  ANDROID_ABI         Target ABI (default: arm64-v8a)"
        echo "  ANDROID_PLATFORM    Target platform (default: android-21)"
        echo ""
        echo "Prerequisites:"
        echo "  - Android NDK installed with CMake support"
        echo "  - Android device connected via ADB"
        echo "  - CMake installed"
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