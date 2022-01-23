cmake_minimum_required(VERSION 3.21.1)

# Project properties:
set(PROJECT_NAME Ku)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message(STATUS "${PROJECT_NAME} [C++${CMAKE_CXX_STANDARD}] ${CMAKE_CURRENT_SOURCE_DIR}")

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

# Project files:
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

include_directories(HEADER_FILES)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Uncomment to print all source and header files
# message(STATUS "Source Files: ${SOURCE_FILES}")
# message(STATUS "Header Files: ${HEADER_FILES}")


# Pass defines to the source code:
set(OUT_DIR "${PROJECT_SOURCE_DIR}/out")
add_definitions(-DOUT_DIR=\"${OUT_DIR}\")
message(STATUS "Defines:")
message(STATUS "   OUT_DIR: ${OUT_DIR}")

