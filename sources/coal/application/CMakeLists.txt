# SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(Boost "1.84" COMPONENTS cobalt json url REQUIRED)
find_package(OpenSSL 3.0.2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(libassert REQUIRED)
find_package(fmt REQUIRED)
find_package(ctre REQUIRED)
find_package(glaze REQUIRED)
find_package(magic_enum REQUIRED)

option(SPDLOG_NO_EXCEPTIONS "Compile with -fno-exceptions. Call abort() on any spdlog exceptions" ON)

file(GLOB_RECURSE ALL_COALA_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

message("adventure land headers:" "${ALL_COALA_APPLICATION_HEADERS}")

file(GLOB_RECURSE ALL_COALA_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(coala "${ALL_COALA_APPLICATION_HEADERS}"
                                     "${ALL_COALA_APPLICATION_SOURCES}")

header_directories(ALL_COALA_INCLUDE_DIRECTORIES)

target_include_directories(coala PUBLIC 
                           "${ALL_COALA_INCLUDE_DIRECTORIES}")


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compiler_flags(-frtti)
  add_compiler_flags(-fexceptions)
  add_compiler_flags(-Wno-zero-as-null-pointer-constant) # TODO(melg): fix this.

  # Due to https://github.com/boostorg/cobalt/issues/7
  add_compiler_flags(-Wno-mismatched-new-delete) 
endif()  

target_link_libraries(coala PRIVATE Boost::cobalt 
                                    Boost::json
                                    Boost::url
                                    ctre::ctre
                                    fmt::fmt
                                    glaze::glaze
                                    libassert::assert
                                    magic_enum::magic_enum
                                    OpenSSL::SSL
                                    spdlog::spdlog
                                    coal)
