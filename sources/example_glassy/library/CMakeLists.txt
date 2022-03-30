file (GLOB_RECURSE ALL_KEY_NOTES_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file (GLOB_RECURSE ALL_KEY_NOTES_LIB_SOURCES
                   "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library (glassy STATIC
             "${ALL_KEY_NOTES_LIB_HEADERS}"
             "${ALL_KEY_NOTES_LIB_SOURCES}")

header_directories (ALL_LIB_INCLUDE_DIRECTORIES)

target_include_directories (glassy PUBLIC
                            "${ALL_LIB_INCLUDE_DIRECTORIES}")