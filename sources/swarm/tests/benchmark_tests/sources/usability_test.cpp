// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

namespace swarm::test {

SCENARIO("Basic computaion") {
  BENCHMARK("2 + 2 == 4") {
    const auto value = 2 + 2;
    CHECK(value == 4);
  };
}
}  // namespace swarm::test
