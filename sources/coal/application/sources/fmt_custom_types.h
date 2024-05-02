// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef FMT_CUSTOM_TYPES_H
#define FMT_CUSTOM_TYPES_H

#include <fmt/core.h>
#include <fmt/format.h>
#include <boost/url.hpp>

template <>
struct fmt::formatter<boost::core::string_view>
    : fmt::formatter<std::string_view> {
  auto format(boost::core::string_view c, fmt::format_context& ctx) {
    return formatter<std::string_view>::format(std::string_view{c}, ctx);
  }

  
};

#endif  // FMT_CUSTOM_TYPES_H