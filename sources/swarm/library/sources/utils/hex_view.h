// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef HEX_VIEW_H
#define HEX_VIEW_H

#include <span>
#include <string>

namespace swarm {

void HexAsciiViewFrom(std::span<const std::byte> data,
                      std::string& hex_view) noexcept;

auto HexAsciiViewFrom(std::span<const std::byte> data) noexcept -> std::string;

}  // namespace swarm

#endif  // HEX_VIEW_H
