cmake_minimum_required(VERSION 3.10)

project(CommonCppDartCode)

set(CMAKE_CXX_STANDARD 20)

# Add include directories. Adjust as needed for your project.
include_directories(
    .  # Include the current directory
    Messages
    cpp
    cpp/json
)

# Gather source files from the cpp subdirectory
file(GLOB_RECURSE SOURCES "cpp/*.cpp") # Recursively finds all .cpp files

set(LIB_NAME "${ROOT_PROJECT_NAME}CommonCppDartCode")
add_library(${LIB_NAME} STATIC ${SOURCES})

# Set the library output directory (relative to the top-level build directory)
set_target_properties(${LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
