# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(glaze REQUIRED)

file(GLOB_RECURSE ALL_COAL_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE ALL_COAL_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(coal STATIC "${ALL_COAL_LIB_HEADERS}"
                          "${ALL_COAL_LIB_SOURCES}")

header_directories(ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories(coal PUBLIC "${ALL_LIB_INCLUDE_DIRECTORIES}")

target_link_libraries(coal PRIVATE glaze::glaze)