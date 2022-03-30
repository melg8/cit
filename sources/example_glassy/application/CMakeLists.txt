file (GLOB_RECURSE ALL_GLASSY_APPLICATION_HEADERS
                   "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

file (GLOB_RECURSE ALL_GLASSY_APPLICATION_SOURCES
                   "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable (glassy_application
                "${ALL_GLASSY_APPLICATION_HEADERS}"
                "${ALL_GLASSY_APPLICATION_SOURCES}")

target_link_libraries (glassy_application glassy)
