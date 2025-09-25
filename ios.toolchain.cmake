# iOS CMake Toolchain File
# Based on https://github.com/leetal/ios-cmake
# Simplified version for ZeticML project

set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_SYSTEM_PROCESSOR arm64)

# Deployment target
set(DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum iOS deployment version")
set(CMAKE_OSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})

# Platform selection
set(PLATFORM "SIMULATOR" CACHE STRING "Type of iOS platform (OS, SIMULATOR)")

if(PLATFORM STREQUAL "OS")
    # iOS Device (iPhone/iPad)
    set(CMAKE_OSX_SYSROOT iphoneos)
    set(CMAKE_OSX_ARCHITECTURES arm64)
    set(IOS_PLATFORM_LOCATION "iPhoneOS.platform")
elseif(PLATFORM STREQUAL "SIMULATOR")
    # iOS Simulator
    set(CMAKE_OSX_SYSROOT iphonesimulator)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    set(IOS_PLATFORM_LOCATION "iPhoneSimulator.platform")
else()
    message(FATAL_ERROR "Unsupported PLATFORM: ${PLATFORM}")
endif()

# Find the iOS SDK path
execute_process(
    COMMAND xcrun --sdk ${CMAKE_OSX_SYSROOT} --show-sdk-path
    OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT EXISTS ${CMAKE_OSX_SYSROOT})
    message(FATAL_ERROR "iOS SDK not found at ${CMAKE_OSX_SYSROOT}")
endif()

message(STATUS "iOS SDK: ${CMAKE_OSX_SYSROOT}")
message(STATUS "iOS Platform: ${PLATFORM}")
message(STATUS "iOS Architecture: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "iOS Deployment Target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")

# Set the find root path mode
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# This is a cross compilation
set(CMAKE_CROSSCOMPILING TRUE)

# Standard settings
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_CXX_FLAGS_INIT "")

# iOS specific flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot ${CMAKE_OSX_SYSROOT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot ${CMAKE_OSX_SYSROOT}")

# Bitcode is deprecated and no longer needed in modern iOS versions