// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_int.h>
#include <numeric_conversions.h>

#include <doctest/doctest.h>
#include <outcome.hpp>

namespace glassy::test {

SCENARIO("Asn1Int conversion to BigNum") {
  const auto value = Asn1Int::New(32);
  const auto converted = convert::FromAsn1Int(value.value());
  CHECK(converted.has_value());
}

}  // namespace glassy::test
