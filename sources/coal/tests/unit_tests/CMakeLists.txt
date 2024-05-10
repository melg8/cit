# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(glaze REQUIRED)

file(GLOB_RECURSE ALL_COAL_TESTS_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_COAL_TESTS_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(coal_tests "${ALL_COAL_TESTS_APPLICATION_HEADERS}"
                           "${ALL_COAL_TESTS_APPLICATION_SOURCES}")

target_link_libraries(
  coal_tests PUBLIC coal testing_framework glaze::glaze)

add_test(coal_tests coal_tests)

header_directories(ALL_COAL_TESTS_INCLUDE_DIRECTORIES)

target_include_directories(coal_tests PUBLIC 
                           "${ALL_COAL_TESTS_INCLUDE_DIRECTORIES}")