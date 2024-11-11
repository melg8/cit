# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(spdlog REQUIRED)

file(GLOB_RECURSE ALL_SWARM_TESTS_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_SWARM_TESTS_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(swarm_tests "${ALL_SWARM_TESTS_APPLICATION_HEADERS}"
                           "${ALL_SWARM_TESTS_APPLICATION_SOURCES}")

target_link_libraries(
  swarm_tests PUBLIC swarm testing_framework spdlog::spdlog)

add_test(swarm_tests swarm_tests)

header_directories(ALL_SWARM_TESTS_INCLUDE_DIRECTORIES)

target_include_directories(swarm_tests PUBLIC 
                           "${ALL_SWARM_TESTS_INCLUDE_DIRECTORIES}")
