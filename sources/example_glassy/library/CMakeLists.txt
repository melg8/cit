# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(OpenSSL 3.0.2 REQUIRED)
find_package(outcome 2.2.3 REQUIRED)
find_package(gsl-lite 0.40 REQUIRED)

file (GLOB_RECURSE ALL_KEY_NOTES_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file (GLOB_RECURSE ALL_KEY_NOTES_LIB_SOURCES
                   "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library (glassy STATIC
             "${ALL_KEY_NOTES_LIB_HEADERS}"
             "${ALL_KEY_NOTES_LIB_SOURCES}")

header_directories (ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories (glassy PUBLIC
                            "${ALL_LIB_INCLUDE_DIRECTORIES}")

target_link_libraries (glassy OpenSSL::Crypto outcome::outcome gsl::gsl-lite)
