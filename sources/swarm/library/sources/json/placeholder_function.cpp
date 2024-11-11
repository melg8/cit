// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <placeholder_function.h>

#include <array>

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
  hex_view += std::to_string(size);
  hex_view += " bytes\n";
}

[[nodiscard]] inline bool IsPrintableAscii(std::byte byte) noexcept {
  return 32 <= static_cast<size_t>(byte) && static_cast<size_t>(byte) <= 126;
}

inline void WriteHexPart(std::span<const std::byte> line, std::string &hex_view) noexcept {
  constexpr std::array<char, 16> kHex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                         'a', 'b', 'c', 'd', 'e', 'f'};
  for (const auto byte : line) {
    hex_view.push_back(kHex[static_cast<size_t>(byte >> 4)]);
    hex_view.push_back(kHex[static_cast<size_t>(byte & std::byte{0x0f})]);
    hex_view.push_back(' ');
  }
  const auto bytes_per_row = 16;
  const auto delta = bytes_per_row - line.size();

  for (size_t i = 0; i < delta; ++i) {
    hex_view += "   ";
  }
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
  for (size_t i = 0; i < delta; ++i) {
    hex_view.push_back(' ');
  }
}


void HexAsciiViewFromOld(std::span<const std::byte> data, std::string &hex_view) noexcept {
  const auto length = data.size();
  hex_view.reserve(length * 5 + 20);
  const auto bytes_per_row = size_t{16};

  for (size_t i = 0; i < length; i += bytes_per_row) {
    WriteLineNumber(i, hex_view);
    const auto row_size = std::min(bytes_per_row, length - i);
    WriteHexPart(data.subspan(i, row_size), hex_view);
    hex_view.push_back(' ');
    WriteAsciiPart(data.subspan(i, row_size), hex_view);
    hex_view.push_back('\n');
  }
  WriteSizeOfData(data.size(), hex_view);
}

void HexAsciiViewFrom(std::span<const std::byte> data, std::string &hex_view) noexcept {
  constexpr const std::array<char, 16> kHex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                               'a', 'b', 'c', 'd', 'e', 'f'};
  const auto length = data.size();
  hex_view.reserve(length * 5 + 20);
  const auto bytes_per_row = size_t{16};
  const auto rows = length / bytes_per_row;

  for (size_t i = 0; i < rows; ++i) {
    WriteLineNumber(i * bytes_per_row, hex_view);

    for (size_t j = 0; j < bytes_per_row; ++j) {
      const auto byte = data[i * bytes_per_row + j];
      hex_view.push_back(kHex[static_cast<size_t>(byte >> 4)]);
      hex_view.push_back(kHex[static_cast<size_t>(byte & std::byte{0x0f})]);
      hex_view.push_back(' ');
    }
    hex_view.push_back(' ');
    for (size_t j = 0; j < bytes_per_row; ++j) {
      const auto byte = data[i * bytes_per_row + j];
      if (IsPrintableAscii(byte)) {
        hex_view.push_back(static_cast<char>(byte));
      } else {
        hex_view.push_back('.');
      }
    }
    hex_view.push_back('\n');
  }

  const auto rest_of_bytes = length % bytes_per_row;

  WriteLineNumber(rows * bytes_per_row, hex_view);
  // Fill hex for last row values.
  for (size_t j = 0; j < rest_of_bytes; ++j) {
    const auto byte = data[rows * bytes_per_row + j];
    hex_view.push_back(kHex[static_cast<size_t>(byte >> 4)]);
    hex_view.push_back(kHex[static_cast<size_t>(byte & std::byte{0x0f})]);
    hex_view.push_back(' ');
  }

  // Fill hex with spaces at last row if it is not full.
  for (size_t j = 0; j < bytes_per_row - rest_of_bytes; ++j) {
    hex_view.push_back(' ');
    hex_view.push_back(' ');
    hex_view.push_back(' ');
  }

  hex_view.push_back(' ');

  // Fill ascii last row values.
  for (size_t j = 0; j < rest_of_bytes; ++j) {
    const auto byte = data[rows * bytes_per_row + j];
    if (IsPrintableAscii(byte)) {
      hex_view.push_back(static_cast<char>(byte));
    } else {
      hex_view.push_back('.');
    }
  }

  for (size_t j = 0; j < bytes_per_row - rest_of_bytes; ++j) {
    hex_view.push_back(' ');
  }
  hex_view.push_back('\n');
  WriteSizeOfData(data.size(), hex_view);
}

} // namespace swarm
