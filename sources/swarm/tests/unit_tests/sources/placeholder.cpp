// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <placeholder_function.h>

#include <span>
#include <vector>

namespace swarm::test {

// Intentionally one byte short of 48,
// 16 bytes per line, 3 lines total.
[[nodiscard]] inline std::vector<std::byte> TestData() noexcept {
  const auto size = size_t{47};

  std::vector<std::byte> result(size);
  for (size_t i = 0; i < size; ++i) {
    result[i] = static_cast<std::byte>(i);
  }
  return result;
}

[[nodiscard]] inline std::string ExpectedOuput() noexcept {
  std::string e{};
  e += "0000: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f  "
       "................\n";
  e += "0010: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f  "
       "................\n";
  e += "0020: 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e     "
       " !\"#$%&'()*+,-. \n";
  e += "Size: 47 bytes\n";
  return e;
}

SCENARIO("placeholder") {
  SECTION("can compare two values") { CHECK(2 * 2 == 4); }

  SECTION("hex to ascii view conversion") {
    const auto data = TestData();
    std::string result{};
    HexAsciiViewFrom(std::span{data}, result);
    const auto expected = ExpectedOuput();
    CHECK(result == expected);
    CHECK(result.size() == 231);
  }
}

}  // namespace swarm::test
