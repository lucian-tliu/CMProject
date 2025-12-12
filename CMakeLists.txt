cmake_minimum_required(VERSION 3.15)

# Project Settings
project(MyProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories
set(SRCDIR ${CMAKE_SOURCE_DIR}/src)
set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/output)

# Source Files
file(GLOB_RECURSE SRC_FILES
    ${SRCDIR}/*.cpp
)

# Output Path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Executable
add_executable(main ${SRC_FILES})

# Include directories
target_include_directories(main PRIVATE ${INCDIR})

# Preprocessor Definitions
target_compile_definitions(main PRIVATE PROJECT_OUTPUT_DIR="${OUTPUT_DIR}")

# Compiler Options
target_compile_options(main PRIVATE -O3 -fopenmp -march=native -ftree-vectorize)

# OpenMP linking (gomp for gcc/MPICH)
target_link_libraries(main PRIVATE gomp)