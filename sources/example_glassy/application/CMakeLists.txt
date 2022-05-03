# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

file(GLOB_RECURSE ALL_GLASSY_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_GLASSY_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(glassy_application "${ALL_GLASSY_APPLICATION_HEADERS}"
                                  "${ALL_GLASSY_APPLICATION_SOURCES}")

target_link_libraries(glassy_application glassy)

add_test(glassy_application glassy_application)
