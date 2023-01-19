# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

file(GLOB_RECURSE ALL_THIRD_PARTY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE ALL_THIRD_PARTY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(third_party INTERFACE "${ALL_THIRD_PARTY_HEADERS}"
            "${ALL_THIRD_PARTY_SOURCES}")

header_directories(ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories(third_party SYSTEM
                           INTERFACE "${ALL_LIB_INCLUDE_DIRECTORIES}")

target_link_libraries(third_party)
