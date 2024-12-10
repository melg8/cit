# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

add_subdirectory(application)
add_subdirectory(library)

if(ENABLE_TESTING)
  add_subdirectory(tests)
endif()
