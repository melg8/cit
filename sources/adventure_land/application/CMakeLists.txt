# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT


find_package(Boost "1.84" COMPONENTS cobalt json url REQUIRED)
find_package(OpenSSL 3.0.2 REQUIRED)    

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file(GLOB_RECURSE ALL_ADVENTURE_LAND_APPLICATION_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(adventure_application "${ALL_ADVENTURE_LAND_APPLICATION_HEADERS}"
                                     "${ALL_ADVENTURE_LAND_APPLICATION_SOURCES}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compiler_flags(-frtti)
  add_compiler_flags(-fexceptions)
  add_compiler_flags(-Wno-zero-as-null-pointer-constant) # TODO(melg): fix this.
endif()  

target_link_libraries(adventure_application PUBLIC Boost::cobalt Boost::json Boost::url OpenSSL::SSL)
