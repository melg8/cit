// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>

// NOLINTNEXTLINE
static std::uint64_t Fibonacci(std::uint64_t number) noexcept {
  return number < 2 ? 1 : Fibonacci(number - 1) + Fibonacci(number - 2);
}

TEST_CASE("It works") {
  BENCHMARK("Fibonacci 20") { return Fibonacci(10); };
  BENCHMARK("Fibonacci 20") { return Fibonacci(20); };
}
