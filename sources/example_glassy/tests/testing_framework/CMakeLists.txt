# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(Catch2 3.2.1 REQUIRED)

file(GLOB_RECURSE ALL_TESTING_FRAMEWORK_LIB_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE ALL_TESTING_FRAMEWORK_LIB_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(testing_framework STATIC "${ALL_TESTING_FRAMEWORK_LIB_HEADERS}"
                                     "${ALL_TESTING_FRAMEWORK_LIB_SOURCES}")

header_directories(ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories(testing_framework
                           PUBLIC "${ALL_LIB_INCLUDE_DIRECTORIES}")

target_link_libraries(testing_framework PUBLIC Catch2::Catch2WithMain)
