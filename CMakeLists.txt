# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.24.3)
project(cit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TESTING "Enable the tests" ON)
option(USE_CCACHE "Enable use of ccache" ON)
option(ENABLE_COVERAGE "Enable profiling and coverage report analysis" ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccache.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/compiler_flags.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/recursive_directories_macro.cmake")

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

if(ENABLE_TESTING)
  enable_testing()
endif()

add_subdirectory(sources)
