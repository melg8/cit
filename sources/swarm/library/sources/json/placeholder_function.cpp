// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <placeholder_function.h>

#include <array>
#include <cstring>

namespace swarm {

class StringAppend {
 public:
  explicit StringAppend(std::string& string) : string_{string}, pos_{0} {}

  inline void Append(const char value) noexcept { string_[pos_++] = value; }

  inline void FinalizeSize() noexcept { string_.resize(pos_, '\0'); }

 private:
  std::string& string_;
  size_t pos_ = 0;
};

inline void WriteLineNumber(size_t line_number,
                            StringAppend& hex_vew) noexcept {
  hex_vew.Append((line_number >> 12) + '0');
  hex_vew.Append(((line_number >> 8) & 0x0f) + '0');
  hex_vew.Append(((line_number >> 4) & 0x0f) + '0');
  hex_vew.Append((line_number & 0x0f) + '0');
  hex_vew.Append(':');
  hex_vew.Append(' ');
}

template <typename T>
[[nodiscard]] inline constexpr std::span<const char> SpanFrom(
    T& text) noexcept {
  return std::span(text, strlen(text));
}

inline void WriteSizeOfData(size_t size, StringAppend& hex_view) noexcept {
  constexpr auto size_text = "Size: ";
  for (const auto c : SpanFrom(size_text)) {
    hex_view.Append(c);
  }
  for (const auto c : std::to_string(size)) {
    hex_view.Append(c);
  }
  for (const auto c : SpanFrom(" bytes\n")) {
    hex_view.Append(c);
  }
}

[[nodiscard]] inline bool IsPrintableAscii(std::byte byte) noexcept {
  return 32 <= static_cast<size_t>(byte) && static_cast<size_t>(byte) <= 126;
}

void HexAsciiViewFrom(std::span<const std::byte> data,
                      std::string& hex_view) noexcept {
  constexpr const std::array<char, 16> kHex = {'0', '1', '2', '3', '4', '5',
                                               '6', '7', '8', '9', 'a', 'b',
                                               'c', 'd', 'e', 'f'};
  const auto length = data.size();
  hex_view.reserve(length * 5 + 20);
  hex_view.resize(length * 5 + 20, '\0');
  const auto bytes_per_row = size_t{16};
  const auto rows = length / bytes_per_row;
  StringAppend str{hex_view};

  for (size_t i = 0; i < rows; ++i) {
    const auto total_bytes = i * bytes_per_row;
    WriteLineNumber(total_bytes, str);

    for (size_t j = 0; j < bytes_per_row; ++j) {
      const auto byte = data[total_bytes + j];
      str.Append(kHex[static_cast<size_t>(byte >> 4)]);
      str.Append(kHex[static_cast<size_t>(byte & std::byte{0x0f})]);
      str.Append(' ');
    }
    str.Append(' ');
    for (size_t j = 0; j < bytes_per_row; ++j) {
      const auto byte = data[total_bytes + j];
      if (IsPrintableAscii(byte)) {
        str.Append(static_cast<char>(byte));
      } else {
        str.Append('.');
      }
    }
    str.Append('\n');
  }

  const auto rest_of_bytes = length % bytes_per_row;
  WriteLineNumber(rows * bytes_per_row, str);

  // Fill hex for last row values.
  for (size_t j = 0; j < rest_of_bytes; ++j) {
    const auto byte = data[rows * bytes_per_row + j];
    str.Append(kHex[static_cast<size_t>(byte >> 4)]);
    str.Append(kHex[static_cast<size_t>(byte & std::byte{0x0f})]);
    str.Append(' ');
  }

  // Fill hex with spaces at last row if it is not full.
  for (size_t j = 0; j < bytes_per_row - rest_of_bytes; ++j) {
    for (const char c : SpanFrom("   ")) {
      str.Append(c);
    }
  }

  str.Append(' ');

  // Fill ascii last row values.
  for (size_t j = 0; j < rest_of_bytes; ++j) {
    const auto byte = data[rows * bytes_per_row + j];
    if (IsPrintableAscii(byte)) {
      str.Append(static_cast<char>(byte));
    } else {
      str.Append('.');
    }
  }

  // Fill empty spaces on ascii last row.
  for (size_t j = 0; j < bytes_per_row - rest_of_bytes; ++j) {
    str.Append(' ');
  }
  str.Append('\n');
  WriteSizeOfData(data.size(), str);
  str.FinalizeSize();
}

}  // namespace swarm
