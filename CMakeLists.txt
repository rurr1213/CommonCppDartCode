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

# create an OBJECT library
set(LIB_NAME "${ROOT_PROJECT_NAME}CommonCppDartCodeObjects")
add_library(${LIB_NAME} OBJECT ${SOURCES})
