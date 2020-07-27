add_executable(cit main.cc)

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_options(
    cit
    PRIVATE -fno-omit-frame-pointer
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-padded
            -Wno-global-constructors
            -Werror)
endif()

set_property(TARGET cit PROPERTY CXX_STANDARD 17)
