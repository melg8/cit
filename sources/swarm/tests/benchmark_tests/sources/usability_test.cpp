// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <placeholder_function.h>

namespace swarm::test {

[[nodiscard]] inline std::vector<std::byte> DataForBenchmark() noexcept {
  std::vector<std::byte> data(1024);
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = static_cast<std::byte>(i % 256);
  }
  return data;
}

SCENARIO("Basic computaion") {
  BENCHMARK("2 + 2 == 4") {
    const auto value = 2 + 2;
    if (value != 4) {
      exit(-1);
    }
    return value;
  };


  const auto test_data = DataForBenchmark();

  BENCHMARK("hex and ascii view") {
    std::string hex{};
    HexAsciiViewFrom(test_data, hex);
    return hex;
  };


  BENCHMARK("string push back value") {
    std::string hex{};
    hex.push_back(' ');
    return hex;
  };

  BENCHMARK("string hex[0] = ' ' value") {
    std::string hex{};
    hex.reserve(47 * 5 + 20);
    hex[0] = ' ';
    hex[1] = ' ';
    hex[2] = ' ';
    return hex;
  };

  BENCHMARK("string hex += \"   \" value") {
    std::string hex{};
    hex.reserve(47 * 5 + 20);
    hex += "   ";
    return hex;
  };

}

}  // namespace swarm::test
