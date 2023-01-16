// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <asn_1_integer.h>
#include <big_num.h>
#include <numeric_conversions.h>

#include <testing_framework.h>
#include <outcome.hpp>

#define O_TRY OUTCOME_TRY

namespace glassy::test {

static Result<Asn1Integer> ProvideAsn1Value() noexcept {
  O_TRY(auto asn_1_integer, Asn1IntegerFrom(32));
  O_TRY(auto bignum, convert::FromAsn1Int(asn_1_integer));
  O_TRY(bignum += BigNum::New(1));
  O_TRY(convert::FromBigNum(bignum, asn_1_integer));
  return {std::move(asn_1_integer)};
}

SCENARIO("Asn1Integer and Bignum usability") {
  BENCHMARK("compare modernized") {
    const auto asn_1_integer = ProvideAsn1Value();
    CHECK(asn_1_integer.has_value());

    CHECK(std::is_eq(
        Asn1IntegerCmp(asn_1_integer.value(), Asn1IntegerFrom(33).value())));
  };
}

}  // namespace glassy::test
