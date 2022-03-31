cmake_minimum_required(VERSION 3.15.5)
project(cit)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TESTING "Enable the tests" ON)
option(USE_CCACHE "Enable use of ccache" ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccache.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/recursive_directories_macro.cmake")

if(ENABLE_TESTING)
  enable_testing()
endif()

add_subdirectory(sources)
