# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(OpenSSL 3.0.2 REQUIRED)

file(GLOB_RECURSE ALL_SWARM_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE ALL_SWARM_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(swarm STATIC "${ALL_SWARM_LIB_HEADERS}"
                          "${ALL_SWARM_LIB_SOURCES}")

header_directories(ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories(swarm PUBLIC "${ALL_LIB_INCLUDE_DIRECTORIES}")

target_link_libraries(swarm PRIVATE OpenSSL::SSL)
