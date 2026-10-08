cmake_minimum_required(VERSION 3.28)

project(CraftingTable VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

option(CT_BUILD_TESTS "Build CraftingTable tests" ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CtTargetOptions)

add_subdirectory(third_party)
add_subdirectory(engine)
add_subdirectory(tools)

if(CT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
