# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(StaticSPP)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable Debug Mode
set(CMAKE_BUILD_TYPE Debug)  # ✅ Ensures debugging symbols are included

# Add all source files
set(SOURCE_FILES
    main.cpp
    datetime.cpp
    NRinexUtils.cpp
    rinex.cpp
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/eigen-3.4.0
)

# Add an executable
add_executable(StaticSPP ${SOURCE_FILES})

# ✅ Ensure -g flag is added for debugging symbols
target_compile_options(StaticSPP PRIVATE -g)
