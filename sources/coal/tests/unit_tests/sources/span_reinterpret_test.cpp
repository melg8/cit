// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

namespace coal::test {

template <class T>
[[nodiscard]] std::span<T> ReinterpretAsSpanOf(
    std::span<std::byte> s) noexcept {
  auto data = s.data();
  if (!data || s.empty()) {
    return {};
  }
  const auto bytes = s.size_bytes();
  const auto count = bytes / static_cast<int>(sizeof(T));
  return std::span<T>{reinterpret_cast<T*>(data), count};
}

SCENARIO("reinterpretation of span of bytes into span of uint8_t values") {
  std::vector<std::byte> data{std::byte(0), std::byte(1), std::byte(2),
                              std::byte(3), std::byte(4)};

  const std::span<uint8_t> span = ReinterpretAsSpanOf<uint8_t>(data);

  CHECK(span.size() == 5);
  CHECK(span[0] == uint8_t{0});
  CHECK(span[1] == uint8_t{1});
  CHECK(span[2] == uint8_t{2});
  CHECK(span[3] == uint8_t{3});
  CHECK(span[4] == uint8_t{4});
}

}  // namespace coal::test
