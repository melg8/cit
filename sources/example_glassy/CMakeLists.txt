# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

add_subdirectory(application)
add_subdirectory(library)
add_subdirectory(third_party)

if(ENABLE_TESTING)
  add_subdirectory(tests)
endif()
