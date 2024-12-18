// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <hex_view.h>

#include <array>
#include <cstdint>
#include <cstring>

namespace swarm {

class StringAppend {
 public:
  explicit StringAppend(size_t length, std::string& string)
      : string_{string}, pos_{0} {
    string_.resize(length, '\0');
  }

  inline constexpr void Append(const char value) noexcept {
    string_[pos_++] = value;
  }

  inline constexpr void AppendDigit(const uint8_t value) noexcept {
    Append('0' + (value & 0x0f));
  }

  inline constexpr void Append(std::span<const char> chars) noexcept {
    for (const auto c : chars) {
      Append(c);
    }
  }

  using HexArray = const std::array<char, 16>;
  static constexpr HexArray kHex = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

  inline void AppendAsHex(std::byte value) noexcept {
    Append(kHex[static_cast<size_t>(value >> 4)]);
    Append(kHex[static_cast<size_t>(value & std::byte{0x0f})]);
    Append(' ');
  }

  inline void FinalizeSize() noexcept { string_.resize(pos_, '\0'); }

 private:
  std::string& string_;
  size_t pos_ = 0;
};

inline void WriteLineNumber(size_t line_number,
                            StringAppend& hex_view) noexcept {
  hex_view.AppendDigit(line_number >> 12);
  hex_view.AppendDigit(line_number >> 8);
  hex_view.AppendDigit(line_number >> 4);
  hex_view.AppendDigit(line_number);
  hex_view.Append(':');
  hex_view.Append(' ');
}

[[nodiscard]] inline std::span<const char> SpanFrom(
    const char* const text) noexcept {
  return std::span(text, std::strlen(text));
}

inline void WriteSizeOfData(size_t size, StringAppend& hex_view) noexcept {
  hex_view.Append(SpanFrom("Size: "));
  hex_view.Append(std::to_string(size));
  hex_view.Append(SpanFrom(" bytes\n"));
}

[[nodiscard]] inline bool IsPrintableAscii(std::byte byte) noexcept {
  return 32 <= static_cast<size_t>(byte) && static_cast<size_t>(byte) <= 126;
}

void HexAsciiViewFrom(std::span<const std::byte> data,
                      std::string& hex_view) noexcept {
  const auto length = data.size();
  const auto bytes_per_row = size_t{16};
  const auto rows = length / bytes_per_row;
  StringAppend str{length * 5 + 20, hex_view};

  for (size_t i = 0; i < rows; ++i) {
    const auto total_bytes = i * bytes_per_row;
    WriteLineNumber(total_bytes, str);

    for (size_t j = 0; j < bytes_per_row; ++j) {
      str.AppendAsHex(data[total_bytes + j]);
    }
    str.Append(' ');
    for (size_t j = 0; j < bytes_per_row; ++j) {
      const auto byte = data[total_bytes + j];
      if (IsPrintableAscii(byte)) [[unlikely]] {
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
    str.AppendAsHex(data[rows * bytes_per_row + j]);
  }

  // Fill hex with spaces at last row if it is not full.
  for (size_t j = 0; j < bytes_per_row - rest_of_bytes; ++j) {
    str.Append(SpanFrom("   "));
  }

  // Fill ascii last row values.
  for (size_t j = 0; j < rest_of_bytes; ++j) {
    const auto byte = data[rows * bytes_per_row + j];
    if (IsPrintableAscii(byte)) [[unlikely]] {
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

auto HexAsciiViewFrom(std::span<const std::byte> data) noexcept -> std::string {
  std::string hex{};
  HexAsciiViewFrom(data, hex);
  return hex;
}

}  // namespace swarm
