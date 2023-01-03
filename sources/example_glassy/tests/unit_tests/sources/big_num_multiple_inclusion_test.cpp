// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <doctest/doctest.h>

#include <big_num.h>

namespace glassy::test {

SCENARIO("BigNum can be included into multiple translations units and used") {
  GIVEN("default created BigNum") {
    const auto result = glassy::BigNum::New();

    THEN("result has value") { CHECK(result.has_value()); }
  }
}

}  // namespace glassy::test
