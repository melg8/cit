# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(adventure_application "${ALL_ADVENTURE_LAND_APPLICATION_HEADERS}"
                                     "${ALL_ADVENTURE_LAND_APPLICATION_SOURCES}")

add_test(adventure_application adventure_application)
