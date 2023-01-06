// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_integer.h>

#include <doctest/doctest.h>

namespace glassy::test {

SCENARIO(
    "Asn1Integer can be included into multiple translations units and used") {
  GIVEN("default created Asn1Int") {
    const auto result = Asn1IntegerFrom(0);

    THEN("result has value") { CHECK(result.has_value()); }
  }
}

}  // namespace glassy::test
