cmake_minimum_required(VERSION 3.15.5)
project (cit)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccache.cmake")

add_executable(cit sources/main.cc)
