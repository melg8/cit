// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_int.h>

#include <doctest/doctest.h>

namespace glassy {
namespace test {

SCENARIO("Asn1Int can be included into multiple translations units and used") {
  GIVEN("default created Asn1Int") {
    const auto result = glassy::Asn1Int::New();

    THEN("result has value") { CHECK(result.has_value()); }
  }
}

}  // namespace test
}  // namespace glassy
