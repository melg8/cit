// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <placeholder_function.h>

#include <cmath>
#include <array>
#include <algorithm>

namespace swarm {

inline void WriteLineNumber(size_t line_number, std::string &hex_vew) noexcept {
  hex_vew.push_back((line_number >> 12) + '0');
  hex_vew.push_back(((line_number >> 8) & 0x0f) + '0');
  hex_vew.push_back(((line_number >> 4) & 0x0f) + '0');
  hex_vew.push_back((line_number & 0x0f) + '0');
  hex_vew.push_back(':');
  hex_vew.push_back(' ');
}


inline void WriteSizeOfData(size_t size, std::string &hex_view) noexcept {
  hex_view += "Size: ";
  const auto max_log = log10(size);
  const auto max_power = static_cast<size_t>(ceil(max_log));

  for (size_t i = max_power; i > 0; --i) {
    const auto power = static_cast<size_t>(pow(10, i));
    const auto value = (size/power) % 10;
    if (value != 0) {
      const char text_value = value + '0';
      hex_view.push_back(text_value);
    }
  }
  hex_view.push_back((size % 10) + '0');
  hex_view += " bytes\n";
}

[[nodiscard]] inline bool IsPrintableAscii(std::byte byte) noexcept {
  return 32 <= static_cast<size_t>(byte) && static_cast<size_t>(byte) <= 126;
}

inline void WriteDeltaPart(size_t delta, const char* filler, std::string &hex_view) noexcept {
  for (size_t i = 0; i < delta; ++i) {
    hex_view += filler;
  }
}

inline void WriteHexPart(std::span<const std::byte> line, std::string &hex_view) noexcept {
  std::array<char, 16> hex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                              'a', 'b', 'c', 'd', 'e', 'f'};

  for (const auto byte : line) {
    hex_view.push_back(hex.at(static_cast<size_t>(byte >> 4)));
    hex_view.push_back(hex.at(static_cast<size_t>(byte & std::byte{0x0f})));
    hex_view.push_back(' ');
  }
  const auto bytes_per_row = 16;
  const auto delta = bytes_per_row - line.size();
  WriteDeltaPart(delta, "   ", hex_view);
}

inline void WriteAsciiPart(std::span<const std::byte> line, std::string &hex_view) noexcept {
  for (const auto byte : line) {
    if (IsPrintableAscii(byte)) {
      hex_view.push_back(static_cast<char>(byte));
    } else {
      hex_view.push_back('.');
    }
  }
  const auto bytes_per_row = 16;
  const auto delta = bytes_per_row - line.size();
  WriteDeltaPart(delta, " ", hex_view);
}


void HexAsciiViewFrom(std::span<const std::byte> data, std::string &hex_view) noexcept {
  hex_view.clear();

  const auto length = data.size();
  hex_view.reserve(length * 5 + 20);
  const auto bytes_per_row = size_t{16};

  for (size_t i = 0; i < length; i += bytes_per_row) {
    WriteLineNumber(i, hex_view);
    const auto row_size = std::min(bytes_per_row, length - i);
    const auto line = data.subspan(i, row_size);
    WriteHexPart(line, hex_view);
    hex_view.push_back(' ');
    WriteAsciiPart(line, hex_view);
    hex_view.push_back('\n');
  }
  WriteSizeOfData(data.size(), hex_view);
}

} // namespace swarm
