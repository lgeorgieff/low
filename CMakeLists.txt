cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(low CXX)

set(low_MAJOR_VERSION 0)
set(low_MINOR_VERSION 0)
set(low_PATCH_VERSION 1)
set(low_VERSION ${low_MAJOR_VERSION}.${low_MINOR_VERSION}.${low_PATCH_VERSION})

message("-- Project: ${PROJECT_NAME}@${low_VERSION}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CXX_WARNINGS "-Wall -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CXX_WARNINGS "-Wpedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CXX_WARNINGS "/W4 /WX /EHsc")
endif()

message("-- Warnings: ${CXX_WARNINGS}")

if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${CXX_WARNINGS}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
  set(CMAKE_C_FLAGS   "${CXX_WARNINGS}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(DOCUMENTATION_DIR ${CMAKE_SOURCE_DIR}/doc/)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(backend)
