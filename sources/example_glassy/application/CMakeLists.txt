# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(outcome 2.2.8 REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE ALL_GLASSY_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_GLASSY_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")


add_executable(glassy_application "${ALL_GLASSY_APPLICATION_HEADERS}"
                                  "${ALL_GLASSY_APPLICATION_SOURCES}")

target_include_directories(glassy_application PUBLIC "${OUTCOME_INCLUDE_DIR}")
target_link_libraries(glassy_application PUBLIC glassy outcome::outcome Qt${QT_VERSION_MAJOR}::Widgets)


add_test(glassy_application glassy_application outcome::outcome)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(glassy_application)
endif()
