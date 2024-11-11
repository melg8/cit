# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

file(GLOB_RECURSE ALL_SWARM_TESTS_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_SWARM_TESTS_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(swarm_bench_tests "${ALL_SWARM_TESTS_APPLICATION_HEADERS}"
                               "${ALL_SWARM_TESTS_APPLICATION_SOURCES}")

target_link_libraries(
  swarm_bench_tests PUBLIC swarm testing_framework)

target_include_directories(swarm_bench_tests PUBLIC "${OUTCOME_INCLUDE_DIR}")

add_test(swarm_bench_tests swarm_bench_tests)
