# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(OpenSSL 3.0.2 REQUIRED)
find_package(outcome 2.2.3 REQUIRED)
find_package(gsl-lite 0.40 REQUIRED)

file(GLOB_RECURSE ALL_GLASSY_TESTS_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_GLASSY_TESTS_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(benchmark_tests "${ALL_GLASSY_TESTS_APPLICATION_HEADERS}"
                               "${ALL_GLASSY_TESTS_APPLICATION_SOURCES}")

target_link_libraries(
  benchmark_tests PUBLIC glassy testing_framework gsl::gsl-lite OpenSSL::Crypto
                         outcome::outcome)

target_include_directories(benchmark_tests PUBLIC "${OUTCOME_INCLUDE_DIR}")

add_test(benchmark_tests benchmark_tests)
