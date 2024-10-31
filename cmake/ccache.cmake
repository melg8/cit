# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

if(${USE_CCACHE})
  find_program(CCACHE_FOUND NAMES ccache)
  if(CCACHE_FOUND)
    message("ccache found")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  else()
    message("ccache were enabled but not found in system")
  endif()
endif()
