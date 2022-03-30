add_subdirectory(application)
add_subdirectory(library)

if(ENABLE_TESTING)
  add_subdirectory(tests)
endif()
