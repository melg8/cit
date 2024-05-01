# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT


find_package(Boost "1.84" COMPONENTS cobalt json url REQUIRED)
find_package(OpenSSL 3.0.2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(libassert REQUIRED)


option(SPDLOG_NO_EXCEPTIONS "Compile with -fno-exceptions. Call abort() on any spdlog exceptions" ON)

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

message("adventure land headers:" "${ALL_ADVENTURE_LAND_APPLICATION_HEADERS}")

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(adventure_application "${ALL_ADVENTURE_LAND_APPLICATION_HEADERS}"
                                     "${ALL_ADVENTURE_LAND_APPLICATION_SOURCES}")

header_directories(ALL_ADVENTURE_APPLICATION_INCLUDE_DIRECTORIES)

target_include_directories(adventure_application PUBLIC 
                           "${ALL_ADVENTURE_APPLICATION_INCLUDE_DIRECTORIES}")


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compiler_flags(-frtti)
  add_compiler_flags(-fexceptions)
  add_compiler_flags(-Wno-zero-as-null-pointer-constant) # TODO(melg): fix this.
endif()  

target_link_libraries(adventure_application PUBLIC Boost::cobalt 
                                                   Boost::json
                                                   Boost::url
                                                   libassert::assert
                                                   OpenSSL::SSL
                                                   spdlog::spdlog)
