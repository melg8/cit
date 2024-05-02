// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef FMT_CUSTOM_TYPES_H
#define FMT_CUSTOM_TYPES_H

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <boost/url.hpp>

template <>
struct fmt::formatter<boost::core::string_view>
    : fmt::formatter<std::string_view> {
  auto format(boost::core::string_view c, fmt::format_context& ctx) {
    return fmt::formatter<std::string_view>::format(std::string_view{c}, ctx);
  }
};

template <>
struct fmt::formatter<boost::urls::url> : fmt::formatter<std::string> {
  [[nodiscard]] static std::string Formatted(const boost::urls::url& url) {
    static constexpr auto kBlueLinkColorValue = 0x0645AD;
    const auto fg_blue = fg(fmt::rgb(kBlueLinkColorValue));
    return fmt::format(fg_blue, "{}",
                       static_cast<boost::core::string_view>(url));
  }

  auto format(const boost::urls::url& url, fmt::format_context& ctx) {
    return fmt::formatter<std::string>::format(Formatted(url), ctx);
  }
};

#endif  // FMT_CUSTOM_TYPES_H