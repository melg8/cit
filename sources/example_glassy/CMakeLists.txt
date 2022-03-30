add_subdirectory(application)
add_subdirectory(library)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
