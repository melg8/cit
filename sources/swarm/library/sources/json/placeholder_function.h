// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef PLACEHOLDER_FUNCTION_H
#define PLACEHOLDER_FUNCTION_H

#include <array>
#include <span>
#include <string>

namespace swarm {

template <typename T>
[[nodiscard]] auto PlaceholderSumm(T a, T b) -> T {
  return a + b;
}

void HexAsciiViewFrom(std::span<const std::byte> data,
                      std::string& hex_view) noexcept;

}  // namespace swarm

#endif  // PLACEHOLDER_FUNCTION_H
